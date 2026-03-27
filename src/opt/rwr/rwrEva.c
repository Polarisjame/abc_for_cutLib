/**CFile****************************************************************

  FileName    [rwrDec.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [DAG-aware AIG rewriting package.]

  Synopsis    [Evaluation and decomposition procedures.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: rwrDec.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "base/abc/abc.h"
#include "base/main/main.h"
#include "rwr.h"
#include "rwrStudent.h"
#include "bool/dec/dec.h"
#include "aig/ivy/ivy.h"
#include <math.h>

ABC_NAMESPACE_IMPL_START

#define MY_HASH_TABLE_SIZE 10007  // 选择一个大素数作为表大小

// P1 optimization: reusable buffers for Rwr_StudentExtractFeatures
static unsigned char * s_pFeatMark  = NULL;
static int *           s_pFeatIndex = NULL;
static int             s_nFeatAlloc = 0;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

static Dec_Graph_t * Rwr_CutEvaluate( Rwr_Man_t * p, Abc_Obj_t * pRoot, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, int LevelMax, int * pGainBest, int fPlaceEnable );
static Dec_Graph_t * Rwr_CutEvaluate_v2( Rwr_Man_t * p, Abc_Obj_t * pRoot, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, int LevelMax, int * pGainBest, int fPlaceEnable, double OriDelay, int fComplRoot, double * pDelayGainBest, double * pNewDelayBest, abctime * pCandPredTotalTime, abctime * pBestPredTime, int * pSubgraphsEvald, abctime * pEvalTotalTime );
Dec_Graph_t * Rwr_CutEvaluate_mine(int sock, Rwr_Man_t * p, Rwr_Man_t * pCopy, Abc_Obj_t * pRoot, Abc_Ntk_t * pNtk, unsigned uPhase, char * pPerm, int index, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, float oriDelay, int LevelMax, int * pGainBest, int fPlaceEnable );
static int Rwr_CutIsBoolean( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves );
static int Rwr_CutCountNumNodes( Abc_Obj_t * pObj, Cut_Cut_t * pCut );
static int Rwr_NodeGetDepth_rec( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves );
static int Rwr_StudentEnsureReverseLevels( Abc_Ntk_t * pNtk );
static void Rwr_StudentReleaseReverseLevels( Abc_Ntk_t * pNtk, int fOwned );
static int Rwr_StudentIsLeaf( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves );
static void Rwr_StudentCollectCone_rec( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves, unsigned char * pMark, Vec_Ptr_t * vNodes );
static int Rwr_StudentExtractFeaturesEx( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves, Abc_Obj_t * pContextRoot, double * pFeatures );
static int Rwr_StudentExtractFeatures( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves, double * pFeatures );
static int Rwr_StudentEnsureModelLoaded( void );
static double Rwr_StudentPredictDelay( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves );
static double Rwr_StudentPredictDelayTimed( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves, abctime * pTime );
static double Rwr_StudentPredictDelayInPlace( Abc_Ntk_t * pNtk, Abc_Obj_t * pNewRoot, Vec_Ptr_t * vLeaves, Abc_Obj_t * pOrigRoot, abctime * pTime );
static double Rwr_StudentTimeToMicroseconds( abctime Time );
static void Rwr_StudentAccumulatePrediction( Rwr_Man_t * p, abctime Time );
static int Rwr_StudentShouldPreferCandidate( int fHasBest, double BestDelayGain, int BestGain, double CurDelayGain, int CurGain, double DelayMargin );
static void Rwr_StudentLogChoice( Abc_Obj_t * pRoot, Cut_Cut_t * pCut, int GainBest, double OriDelay, double NewDelay, double DelayGainBest, abctime OriPredTime, abctime CandPredTotalTime, abctime BestPredTime, int nSubgraphsEvald, abctime EvalTotalTime );

static int g_RwrStudentQuiet = 0;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Performs rewriting for one node.]

  Description [This procedure considers all the cuts computed for the node
  and tries to rewrite each of them using the "forest" of different AIG
  structures precomputed and stored in the RWR manager. 
  Determines the best rewriting and computes the gain in the number of AIG
  nodes in the final network. In the end, p->vFanins contains information 
  about the best cut that can be used for rewriting, while p->pGraph gives 
  the decomposition dag (represented using decomposition graph data structure).
  Returns gain in the number of nodes or -1 if node cannot be rewritten.]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int Rwr_NodeRewrite( Rwr_Man_t * p, Cut_Man_t * pManCut, Abc_Obj_t * pNode, int fUpdateLevel, int fUseZeros, int fPlaceEnable )
{
    int fVeryVerbose = 0;
    Dec_Graph_t * pGraph;
    Cut_Cut_t * pCut;//, * pTemp;
    Abc_Obj_t * pFanin;
    unsigned uPhase;
    unsigned uTruthBest = 0; // Suppress "might be used uninitialized"
    unsigned uTruth;
    char * pPerm;
    int Required, nNodesSaved;
    int nNodesSaveCur = -1; // Suppress "might be used uninitialized"
    int i, GainCur = -1, GainBest = -1;
    abctime clk, clk2;//, Counter;

    p->nNodesConsidered++;
    // get the required times
    Required = fUpdateLevel? Abc_ObjRequiredLevel(pNode) : ABC_INFINITY;

    // get the node's cuts
clk = Abc_Clock();
    pCut = (Cut_Cut_t *)Abc_NodeGetCutsRecursive( pManCut, pNode, 0, 0 );
    assert( pCut != NULL );
p->timeCut += Abc_Clock() - clk;

//printf( " %d", Rwr_CutCountNumNodes(pNode, pCut) );
/*
    Counter = 0;
    for ( pTemp = pCut->pNext; pTemp; pTemp = pTemp->pNext )
        Counter++;
    printf( "%d ", Counter );
*/
    // go through the cuts
clk = Abc_Clock();
    for ( pCut = pCut->pNext; pCut; pCut = pCut->pNext )
    {
        // consider only 4-input cuts
        if ( pCut->nLeaves < 4 )
            continue;
//            Cut_CutPrint( pCut, 0 ), printf( "\n" );

        // get the fanin permutation
        uTruth = 0xFFFF & *Cut_CutReadTruth(pCut);
        pPerm = p->pPerms4[ (int)p->pPerms[uTruth] ];
        uPhase = p->pPhases[uTruth];
        // collect fanins with the corresponding permutation/phase
        Vec_PtrClear( p->vFaninsCur );
        Vec_PtrFill( p->vFaninsCur, (int)pCut->nLeaves, 0 );
        for ( i = 0; i < (int)pCut->nLeaves; i++ )
        {
            pFanin = Abc_NtkObj( pNode->pNtk, pCut->pLeaves[(int)pPerm[i]] );
            if ( pFanin == NULL )
                break;
            pFanin = Abc_ObjNotCond(pFanin, ((uPhase & (1<<i)) > 0) );
            Vec_PtrWriteEntry( p->vFaninsCur, i, pFanin );
        }
        if ( i != (int)pCut->nLeaves )
        {
            p->nCutsBad++;
            continue;
        }
        p->nCutsGood++;

        {
            int Counter = 0;
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
                if ( Abc_ObjFanoutNum(Abc_ObjRegular(pFanin)) == 1 )
                    Counter++;
            if ( Counter > 2 )
                continue;
        }

clk2 = Abc_Clock();
/*
        printf( "Considering: (" );
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            printf( "%d ", Abc_ObjFanoutNum(Abc_ObjRegular(pFanin)) );
        printf( ")\n" );
*/
        // mark the fanin boundary 
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            Abc_ObjRegular(pFanin)->vFanouts.nSize++;

        // label MFFC with current ID
        Abc_NtkIncrementTravId( pNode->pNtk );
        nNodesSaved = Abc_NodeMffcLabelAig( pNode );
        // unmark the fanin boundary
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            Abc_ObjRegular(pFanin)->vFanouts.nSize--;
p->timeMffc += Abc_Clock() - clk2;

        // evaluate the cut
clk2 = Abc_Clock();
        pGraph = Rwr_CutEvaluate( p, pNode, pCut, p->vFaninsCur, nNodesSaved, Required, &GainCur, fPlaceEnable );
p->timeEval += Abc_Clock() - clk2;

        // check if the cut is better than the current best one
        if ( pGraph != NULL && GainBest < GainCur )
        {
            // save this form
            nNodesSaveCur = nNodesSaved;
            GainBest  = GainCur;
            p->pGraph  = pGraph;
            p->fCompl = ((uPhase & (1<<4)) > 0);
            uTruthBest = 0xFFFF & *Cut_CutReadTruth(pCut);
            // collect fanins in the
            Vec_PtrClear( p->vFanins );
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
                Vec_PtrPush( p->vFanins, pFanin );
        }
    }
p->timeRes += Abc_Clock() - clk;

    if ( GainBest == -1 )
        return -1;
/*
    if ( GainBest > 0 )
    {
        printf( "Class %d  ", p->pMap[uTruthBest] );
        printf( "Gain = %d. Node %d : ", GainBest, pNode->Id );
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
            printf( "%d ", Abc_ObjRegular(pFanin)->Id );
        Dec_GraphPrint( stdout, p->pGraph, NULL, NULL );
        printf( "\n" );
    }
*/

//    printf( "%d", nNodesSaveCur - GainBest );
/*
    if ( GainBest > 0 )
    {
        if ( Rwr_CutIsBoolean( pNode, p->vFanins ) )
            printf( "b" );
        else
        {
            printf( "Node %d : ", pNode->Id );
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
                printf( "%d ", Abc_ObjRegular(pFanin)->Id );
            printf( "a" );
        }
    }
*/
/*
    if ( GainBest > 0 )
        if ( p->fCompl )
            printf( "c" );
        else
            printf( "." );
*/

    // copy the leaves
    Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
        Dec_GraphNode((Dec_Graph_t *)p->pGraph, i)->pFunc = pFanin;
/*
    printf( "(" );
    Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
        printf( " %d", Abc_ObjRegular(pFanin)->vFanouts.nSize - 1 );
    printf( " )  " );
*/
//    printf( "%d ", Rwr_NodeGetDepth_rec( pNode, p->vFanins ) );

    p->nScores[p->pMap[uTruthBest]]++;
    p->nNodesGained += GainBest;
    if ( fUseZeros || GainBest > 0 )
    {
        p->nNodesRewritten++;
    }

    // report the progress
    if ( fVeryVerbose && GainBest > 0 )
    {
        printf( "Node %6s :   ", Abc_ObjName(pNode) );
        printf( "Fanins = %d. ", p->vFanins->nSize );
        printf( "Save = %d.  ", nNodesSaveCur );
        printf( "Add = %d.  ",  nNodesSaveCur-GainBest );
        printf( "GAIN = %d.  ", GainBest );
        printf( "Cone = %d.  ", p->pGraph? Dec_GraphNodeNum((Dec_Graph_t *)p->pGraph) : 0 );
        printf( "Class = %d.  ", p->pMap[uTruthBest] );
        printf( "\n" );
    }
    return GainBest;
}

static int Rwr_StudentEnsureReverseLevels( Abc_Ntk_t * pNtk )
{
    if ( pNtk->vLevelsR )
        return 0;
    Abc_NtkLevel( pNtk );
    Abc_NtkStartReverseLevels( pNtk, 0 );
    return 1;
}

static void Rwr_StudentReleaseReverseLevels( Abc_Ntk_t * pNtk, int fOwned )
{
    if ( fOwned )
        Abc_NtkStopReverseLevels( pNtk );
}

static int Rwr_StudentIsLeaf( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves )
{
    Abc_Obj_t * pLeaf;
    int i;
    pObj = Abc_ObjRegular( pObj );
    Vec_PtrForEachEntry( Abc_Obj_t *, vLeaves, pLeaf, i )
        if ( pObj == Abc_ObjRegular(pLeaf) )
            return 1;
    return 0;
}

static void Rwr_StudentCollectCone_rec( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves, unsigned char * pMark, Vec_Ptr_t * vNodes )
{
    pObj = Abc_ObjRegular( pObj );
    if ( pObj == NULL || pMark[pObj->Id] )
        return;
    pMark[pObj->Id] = 1;
    Vec_PtrPush( vNodes, pObj );
    if ( Abc_ObjIsCi(pObj) || Rwr_StudentIsLeaf( pObj, vLeaves ) )
        return;
    assert( Abc_ObjIsNode(pObj) );
    Rwr_StudentCollectCone_rec( Abc_ObjFanin0(pObj), vLeaves, pMark, vNodes );
    Rwr_StudentCollectCone_rec( Abc_ObjFanin1(pObj), vLeaves, pMark, vNodes );
}

static int Rwr_StudentExtractFeaturesEx( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves, Abc_Obj_t * pContextRoot, double * pFeatures )
{
    enum { EDGE_WEIGHT_PLAIN = 0, EDGE_WEIGHT_INV = 1 };
    Vec_Ptr_t * vNodes;
    unsigned char * pMark = NULL;
    int * pIndex = NULL, * pInDeg = NULL, * pOutDeg = NULL, * pInvIn = NULL, * pInvOut = NULL;
    int * pEdgeSrc = NULL, * pEdgeDst = NULL, * pEdgeType = NULL, * pOrder = NULL;
    double * pDepthW = NULL, * pDepthT = NULL;
    Abc_Obj_t * pObj, * pRootChoice = NULL;
    int nObjsAlloc, nNodes, nEdges, i, k = 0, e, fOwnReverse = 0;
    int nLeaves = 0, nInvEdges = 0, RootInvCount = 0;
    double WeightedDepth = 0.0, TopoDepth = 0.0;
    double AvgWidth, MaxFanout = 0.0, InvRatio, ChainDensity, ForkDensity, AvgCurvature;
    double RRoot, SlackRoot, RootFanout, MaxLLeaf = 0.0;
    double MaxLeafFanout = 0.0, MeanLeafFanout = 0.0, StdLeafFanout = 0.0, FracLeafPi = 0.0;
    double ChainCount = 0.0, ForkCount = 0.0, CurvatureSum = 0.0;
    double LeafFanoutSum = 0.0, LeafFanoutSqSum = 0.0, LeafPiCount = 0.0;
    int GlobalDepth;

    if ( pNtk == NULL || pRoot == NULL || pFeatures == NULL )
        return 0;

    fOwnReverse = Rwr_StudentEnsureReverseLevels( pNtk );
    nObjsAlloc = Abc_NtkObjNumMax( pNtk ) + 1;
    vNodes = Vec_PtrAlloc( 32 );
    // P1: reuse static buffers for pMark and pIndex
    if ( s_nFeatAlloc < nObjsAlloc )
    {
        ABC_FREE( s_pFeatMark );
        ABC_FREE( s_pFeatIndex );
        s_pFeatMark  = ABC_CALLOC( unsigned char, nObjsAlloc );
        s_pFeatIndex = ABC_ALLOC( int, nObjsAlloc );
        s_nFeatAlloc = nObjsAlloc;
        for ( i = 0; i < nObjsAlloc; ++i )
            s_pFeatIndex[i] = -1;
    }
    pMark  = s_pFeatMark;
    pIndex = s_pFeatIndex;

    Rwr_StudentCollectCone_rec( pRoot, vLeaves, pMark, vNodes );
    nNodes = Vec_PtrSize( vNodes );
    if ( nNodes == 0 )
        goto cleanup;

    pInDeg   = ABC_CALLOC( int, nNodes );
    pOutDeg  = ABC_CALLOC( int, nNodes );
    pInvIn   = ABC_CALLOC( int, nNodes );
    pInvOut  = ABC_CALLOC( int, nNodes );
    pEdgeSrc = ABC_ALLOC( int, 2 * nNodes + 2 );
    pEdgeDst = ABC_ALLOC( int, 2 * nNodes + 2 );
    pEdgeType= ABC_ALLOC( int, 2 * nNodes + 2 );
    pOrder   = ABC_ALLOC( int, nNodes );
    pDepthW  = ABC_CALLOC( double, nNodes );
    pDepthT  = ABC_CALLOC( double, nNodes );

    Vec_PtrForEachEntry( Abc_Obj_t *, vNodes, pObj, i )
    {
        pIndex[pObj->Id] = i;
        pOrder[i] = i;
    }

    nEdges = 0;
    Vec_PtrForEachEntry( Abc_Obj_t *, vNodes, pObj, i )
    {
        Abc_Obj_t * pFaninR;
        int IndexFanin;

        if ( Abc_ObjIsCi(pObj) || Rwr_StudentIsLeaf( pObj, vLeaves ) )
            continue;

        pFaninR = Abc_ObjRegular( Abc_ObjFanin0(pObj) );
        IndexFanin = pFaninR ? pIndex[pFaninR->Id] : -1;
        if ( IndexFanin >= 0 )
        {
            pEdgeSrc[nEdges] = IndexFanin;
            pEdgeDst[nEdges] = i;
            pEdgeType[nEdges] = Abc_ObjFaninC0(pObj) ? EDGE_WEIGHT_INV : EDGE_WEIGHT_PLAIN;
            pOutDeg[IndexFanin]++;
            pInDeg[i]++;
            if ( pEdgeType[nEdges] == EDGE_WEIGHT_INV )
            {
                pInvOut[IndexFanin]++;
                pInvIn[i]++;
                nInvEdges++;
            }
            nEdges++;
        }

        pFaninR = Abc_ObjRegular( Abc_ObjFanin1(pObj) );
        IndexFanin = pFaninR ? pIndex[pFaninR->Id] : -1;
        if ( IndexFanin >= 0 )
        {
            pEdgeSrc[nEdges] = IndexFanin;
            pEdgeDst[nEdges] = i;
            pEdgeType[nEdges] = Abc_ObjFaninC1(pObj) ? EDGE_WEIGHT_INV : EDGE_WEIGHT_PLAIN;
            pOutDeg[IndexFanin]++;
            pInDeg[i]++;
            if ( pEdgeType[nEdges] == EDGE_WEIGHT_INV )
            {
                pInvOut[IndexFanin]++;
                pInvIn[i]++;
                nInvEdges++;
            }
            nEdges++;
        }
    }

    for ( i = 1; i < nNodes; ++i )
    {
        int Cur = pOrder[i];
        int j = i - 1;
        while ( j >= 0 )
        {
            Abc_Obj_t * pCurObj = (Abc_Obj_t *)Vec_PtrEntry( vNodes, Cur );
            Abc_Obj_t * pPrevObj = (Abc_Obj_t *)Vec_PtrEntry( vNodes, pOrder[j] );
            int CurLevel = Abc_ObjLevel( pCurObj );
            int PrevLevel = Abc_ObjLevel( pPrevObj );
            if ( PrevLevel < CurLevel || (PrevLevel == CurLevel && pPrevObj->Id < pCurObj->Id) )
                break;
            pOrder[j + 1] = pOrder[j];
            j--;
        }
        pOrder[j + 1] = Cur;
    }

    Vec_PtrForEachEntry( Abc_Obj_t *, vNodes, pObj, i )
    {
        if ( pOutDeg[i] == 0 )
        {
            if ( pRootChoice == NULL || Abc_ObjLevel(pRootChoice) < Abc_ObjLevel(pObj) )
                pRootChoice = pObj;
        }
        if ( pInDeg[i] == 0 )
        {
            double LeafFanout = (double)Abc_ObjFanoutNum( pObj );
            nLeaves++;
            LeafFanoutSum += LeafFanout;
            LeafFanoutSqSum += LeafFanout * LeafFanout;
            if ( MaxLeafFanout < LeafFanout )
                MaxLeafFanout = LeafFanout;
            if ( MaxLLeaf < (double)Abc_ObjLevel(pObj) )
                MaxLLeaf = (double)Abc_ObjLevel(pObj);
            if ( Abc_ObjIsCi(pObj) )
                LeafPiCount += 1.0;
        }
        if ( MaxFanout < (double)pOutDeg[i] )
            MaxFanout = (double)pOutDeg[i];
        ChainCount += (double)pInDeg[i] * (double)pOutDeg[i];
        if ( pOutDeg[i] > 1 )
            ForkCount += 0.5 * (double)pOutDeg[i] * (double)(pOutDeg[i] - 1);
    }
    if ( pRootChoice == NULL )
        pRootChoice = Abc_ObjRegular( pRoot );

    for ( i = 0; i < nNodes; ++i )
    {
        int u = pOrder[i];
        for ( e = 0; e < nEdges; ++e )
        {
            double EdgeWeight;
            int v;
            if ( pEdgeSrc[e] != u )
                continue;
            v = pEdgeDst[e];
            EdgeWeight = pEdgeType[e] == EDGE_WEIGHT_INV ? 1.5 : 1.0;
            if ( pDepthW[v] < pDepthW[u] + EdgeWeight )
                pDepthW[v] = pDepthW[u] + EdgeWeight;
            if ( pDepthT[v] < pDepthT[u] + 1.0 )
                pDepthT[v] = pDepthT[u] + 1.0;
        }
    }
    for ( i = 0; i < nNodes; ++i )
    {
        if ( WeightedDepth < pDepthW[i] )
            WeightedDepth = pDepthW[i];
        if ( TopoDepth < pDepthT[i] )
            TopoDepth = pDepthT[i];
    }

    for ( e = 0; e < nEdges; ++e )
    {
        double DegSum = (double)(pInDeg[pEdgeSrc[e]] + pOutDeg[pEdgeSrc[e]] + pInDeg[pEdgeDst[e]] + pOutDeg[pEdgeDst[e]]);
        if ( DegSum > 0.0 )
            CurvatureSum += (pEdgeType[e] == EDGE_WEIGHT_INV ? 1.5 : 1.0) / DegSum;
        if ( pRootChoice && pEdgeDst[e] == pIndex[pRootChoice->Id] && pEdgeType[e] == EDGE_WEIGHT_INV )
            RootInvCount++;
    }

    GlobalDepth = pNtk->LevelMax;
    if ( pContextRoot )
    {
        // use original root's context for features that depend on global position
        RRoot = (double)Abc_ObjReverseLevel(pContextRoot);
        SlackRoot = (double)(GlobalDepth - (Abc_ObjLevel(pContextRoot) + Abc_ObjReverseLevel(pContextRoot)));
        RootFanout = (double)Abc_ObjFanoutNum(pContextRoot);
    }
    else
    {
        RRoot = pRootChoice ? (double)Abc_ObjReverseLevel(pRootChoice) : 0.0;
        SlackRoot = pRootChoice ? (double)(GlobalDepth - (Abc_ObjLevel(pRootChoice) + Abc_ObjReverseLevel(pRootChoice))) : 0.0;
        RootFanout = pRootChoice ? (double)Abc_ObjFanoutNum(pRootChoice) : 0.0;
    }
    if ( nLeaves > 0 )
    {
        MeanLeafFanout = LeafFanoutSum / (double)nLeaves;
        StdLeafFanout = LeafFanoutSqSum / (double)nLeaves - MeanLeafFanout * MeanLeafFanout;
        StdLeafFanout = StdLeafFanout > 0.0 ? sqrt( StdLeafFanout ) : 0.0;
        FracLeafPi = LeafPiCount / (double)nLeaves;
    }

    AvgWidth = nNodes > 0 ? (double)nNodes / (TopoDepth + 1.0) : 0.0;
    InvRatio = nEdges > 0 ? (double)nInvEdges / (double)nEdges : 0.0;
    ChainDensity = nNodes > 1 ? ChainCount / ((double)nNodes * (double)nNodes) : 0.0;
    ForkDensity = nNodes > 1 ? ForkCount / ((double)nNodes * (double)nNodes) : 0.0;
    AvgCurvature = nEdges > 0 ? CurvatureSum / (double)nEdges : 0.0;

    pFeatures[0]  = RRoot;
    pFeatures[1]  = SlackRoot;
    pFeatures[2]  = RootFanout;
    pFeatures[3]  = MaxLLeaf;
    pFeatures[4]  = MaxLeafFanout;
    pFeatures[5]  = MeanLeafFanout;
    pFeatures[6]  = StdLeafFanout;
    pFeatures[7]  = FracLeafPi;
    pFeatures[8]  = WeightedDepth;
    pFeatures[9]  = TopoDepth;
    pFeatures[10] = WeightedDepth - TopoDepth;
    pFeatures[11] = (double)nNodes;
    pFeatures[12] = AvgWidth;
    pFeatures[13] = MaxFanout;
    pFeatures[14] = InvRatio;
    pFeatures[15] = (double)RootInvCount;
    pFeatures[16] = ChainDensity;
    pFeatures[17] = ForkDensity;
    pFeatures[18] = AvgCurvature;
    k = 1;

cleanup:
    Rwr_StudentReleaseReverseLevels( pNtk, fOwnReverse );
    // P1: reset only the touched entries in the reusable buffers
    if ( vNodes )
    {
        Vec_PtrForEachEntry( Abc_Obj_t *, vNodes, pObj, i )
        {
            pMark[pObj->Id] = 0;
            pIndex[pObj->Id] = -1;
        }
        Vec_PtrFree( vNodes );
    }
    // pMark and pIndex are static buffers — do NOT free them
    ABC_FREE( pInDeg );
    ABC_FREE( pOutDeg );
    ABC_FREE( pInvIn );
    ABC_FREE( pInvOut );
    ABC_FREE( pEdgeSrc );
    ABC_FREE( pEdgeDst );
    ABC_FREE( pEdgeType );
    ABC_FREE( pOrder );
    ABC_FREE( pDepthW );
    ABC_FREE( pDepthT );
    return k;
}

static int Rwr_StudentExtractFeatures( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves, double * pFeatures )
{
    return Rwr_StudentExtractFeaturesEx( pNtk, pRoot, vLeaves, NULL, pFeatures );
}

static int Rwr_StudentEnsureModelLoaded( void )
{
    if ( Rwr_StudentHasModel() )
        return 1;
    if ( !Rwr_StudentLoadModel( Rwr_StudentDefaultModelPath() ) )
    {
        Abc_Print( -1, "Student runtime model load failed: %s\n", Rwr_StudentDefaultModelPath() );
        return 0;
    }
    return 1;
}

static double Rwr_StudentPredictDelay( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves )
{
    double Features[RWR_STUDENT_INPUT_DIM];
    if ( !Rwr_StudentEnsureModelLoaded() )
        return 1.0e100;
    if ( !Rwr_StudentExtractFeatures( pNtk, pRoot, vLeaves, Features ) )
    {
        Abc_Print( -1, "Student feature extraction failed for root %d.\n", pRoot ? pRoot->Id : -1 );
        return 1.0e100;
    }
    return Rwr_StudentInferDelay( Features );
}

static double Rwr_StudentPredictDelayTimed( Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, Vec_Ptr_t * vLeaves, abctime * pTime )
{
    abctime clk = Abc_Clock();
    double Delay = Rwr_StudentPredictDelay( pNtk, pRoot, vLeaves );
    if ( pTime )
        *pTime = Abc_Clock() - clk;
    return Delay;
}

static double Rwr_StudentPredictDelayInPlace( Abc_Ntk_t * pNtk, Abc_Obj_t * pNewRoot, Vec_Ptr_t * vLeaves, Abc_Obj_t * pOrigRoot, abctime * pTime )
{
    double Features[RWR_STUDENT_INPUT_DIM];
    abctime clk = Abc_Clock();
    if ( !Rwr_StudentEnsureModelLoaded() )
    {
        if ( pTime ) *pTime = Abc_Clock() - clk;
        return 1.0e100;
    }
    if ( !Rwr_StudentExtractFeaturesEx( pNtk, pNewRoot, vLeaves, pOrigRoot, Features ) )
    {
        Abc_Print( -1, "Student in-place feature extraction failed for root %d.\n", pNewRoot ? Abc_ObjRegular(pNewRoot)->Id : -1 );
        if ( pTime ) *pTime = Abc_Clock() - clk;
        return 1.0e100;
    }
    if ( pTime ) *pTime = Abc_Clock() - clk;
    return Rwr_StudentInferDelay( Features );
}

static double Rwr_StudentTimeToMicroseconds( abctime Time )
{
    return 1000000.0 * (double)Time / (double)CLOCKS_PER_SEC;
}

static void Rwr_StudentAccumulatePrediction( Rwr_Man_t * p, abctime Time )
{
    if ( p == NULL )
        return;
    p->nStudentPredictions++;
    p->timeStudentPredict += Time;
}

static int Rwr_StudentShouldPreferCandidate( int fHasBest, double BestDelayGain, int BestGain, double CurDelayGain, int CurGain, double DelayMargin )
{
    if ( !fHasBest )
        return 1;
    if ( CurDelayGain > BestDelayGain + DelayMargin )
        return 1;
    if ( BestDelayGain > CurDelayGain + DelayMargin )
        return 0;
    if ( CurGain > BestGain )
        return 1;
    return 0;
}

void Rwr_StudentSetQuiet( int fQuiet )
{
    g_RwrStudentQuiet = fQuiet;
}

void Rwr_StudentLogInit( int fQuiet, int nProgressStep )
{
    if ( fQuiet )
        return;
    FILE * pFile = Abc_FrameOpenLogFile( "rwr_student_eval.log", "w" );
    time_t TimeNow;
    struct tm * pTm;
    char Buffer[64];
    if ( pFile == NULL )
        return;
    TimeNow = time( NULL );
    pTm = localtime( &TimeNow );
    if ( pTm && strftime( Buffer, sizeof(Buffer), "%Y-%m-%d %H:%M:%S", pTm ) )
        fprintf( pFile, "run_start=%s quiet=%d progress_step=%d\n", Buffer, fQuiet, nProgressStep );
    else
        fprintf( pFile, "run_start=unknown quiet=%d progress_step=%d\n", fQuiet, nProgressStep );
    fprintf( pFile, "model_src=%s\n", Rwr_StudentModelPath() ? Rwr_StudentModelPath() : Rwr_StudentDefaultModelPath() );
    fclose( pFile );
}

void Rwr_StudentLogProgress( Rwr_Man_t * p, int fForce )
{
    FILE * pFile;
    double PredTotalUs, PredAvgUs, ElapsedSec;
    if ( p == NULL )
        return;
    if ( p->fStudentQuiet )
        return;
    if ( !fForce )
    {
        if ( p->nStudentProgressStep <= 0 )
            return;
        if ( p->nStudentCutsEvaluated == 0 || (p->nStudentCutsEvaluated % p->nStudentProgressStep) != 0 )
            return;
    }
    pFile = Abc_FrameOpenLogFile( "rwr_student_eval.log", "a" );
    if ( pFile == NULL )
        return;
    PredTotalUs = Rwr_StudentTimeToMicroseconds( p->timeStudentPredict );
    PredAvgUs = p->nStudentPredictions ? PredTotalUs / (double)p->nStudentPredictions : 0.0;
    ElapsedSec = (double)(Abc_Clock() - p->timeStudentWallStart) / (double)CLOCKS_PER_SEC;
    fprintf( pFile, "progress nodes=%d cuts=%d preds=%d pred_total_us=%.3f pred_avg_us=%.3f elapsed_s=%.3f final=%d\n",
        p->nStudentNodesProcessed, p->nStudentCutsEvaluated, p->nStudentPredictions, PredTotalUs, PredAvgUs, ElapsedSec, fForce ? 1 : 0 );
    fclose( pFile );
    if ( !p->fStudentQuiet )
    {
        printf( "student progress: nodes=%d cuts=%d preds=%d pred_avg_us=%.3f elapsed_s=%.3f%s\n",
            p->nStudentNodesProcessed, p->nStudentCutsEvaluated, p->nStudentPredictions, PredAvgUs, ElapsedSec, fForce ? " final" : "" );
    }
}

static void Rwr_StudentLogChoice( Abc_Obj_t * pRoot, Cut_Cut_t * pCut, int GainBest, double OriDelay, double NewDelay, double DelayGainBest, abctime OriPredTime, abctime CandPredTotalTime, abctime BestPredTime, int nSubgraphsEvald, abctime EvalTotalTime )
{
    if ( g_RwrStudentQuiet )
        return;
    FILE * pFile = Abc_FrameOpenLogFile( "rwr_student_eval.log", "a" );
    int i;
    double CandPredTotalUs = Rwr_StudentTimeToMicroseconds( CandPredTotalTime );
    double CandPredAvgUs = nSubgraphsEvald ? CandPredTotalUs / (double)nSubgraphsEvald : 0.0;
    if ( pFile == NULL )
        return;
    fprintf( pFile, "root=%d truth=%u leaves=", pRoot->Id, 0xFFFF & *Cut_CutReadTruth(pCut) );
    for ( i = 0; i < (int)pCut->nLeaves; ++i )
        fprintf( pFile, "%s%d", i ? "," : "", pCut->pLeaves[i] );
    fprintf( pFile, " area_gain=%d ori_delay=%.6f new_delay=%.6f delay_gain=%.6f", GainBest, OriDelay, NewDelay, DelayGainBest );
    fprintf( pFile, " ori_pred_us=%.3f cand_pred_total_us=%.3f cand_pred_avg_us=%.3f", Rwr_StudentTimeToMicroseconds( OriPredTime ), CandPredTotalUs, CandPredAvgUs );
    fprintf( pFile, " best_pred_us=%.3f subgraphs_evald=%d eval_total_us=%.3f\n", Rwr_StudentTimeToMicroseconds( BestPredTime ), nSubgraphsEvald, Rwr_StudentTimeToMicroseconds( EvalTotalTime ) );
    fclose( pFile );
}

int Rwr_NodeRewrite_v2( Rwr_Man_t * p, Cut_Man_t * pManCut, Abc_Obj_t * pNode, int fUpdateLevel, int fUseZeros, int fPlaceEnable, int fQuiet )
{
    int fVeryVerbose = 0;
    double DelayMargin = 20.0;
    Dec_Graph_t * pGraph;
    Cut_Cut_t * pCut, * pCutBest = NULL;
    Abc_Obj_t * pFanin;
    unsigned uPhase;
    unsigned uTruthBest = 0;
    unsigned uTruth;
    char * pPerm;
    int Required, nNodesSaved;
    int nNodesSaveCur = -1, nSubgraphsEvaldCur = 0, nSubgraphsEvaldBest = 0;
    int i, GainCur = -1, GainBest = -1;
    double DelayGainCur = -1.0e100, DelayGainBest = -1.0e100, OriDelay, OriDelayBest = 1.0e100, NewDelayCur = 1.0e100, NewDelayBest = 1.0e100;
    abctime clk, clk2, OriPredTime, OriPredTimeBest = 0, CandPredTotalTimeCur = 0, CandPredTotalTimeBest = 0, BestPredTimeCur = 0, BestPredTimeBest = 0, EvalTotalTimeCur = 0, EvalTotalTimeBest = 0;

    p->fStudentQuiet = fQuiet;
    p->nNodesConsidered++;
    p->nStudentNodesProcessed++;
    Required = fUpdateLevel ? Abc_ObjRequiredLevel(pNode) : ABC_INFINITY;

clk = Abc_Clock();
    pCut = (Cut_Cut_t *)Abc_NodeGetCutsRecursive( pManCut, pNode, 0, 0 );
    assert( pCut != NULL );
p->timeCut += Abc_Clock() - clk;

clk = Abc_Clock();
    for ( pCut = pCut->pNext; pCut; pCut = pCut->pNext )
    {
        if ( pCut->nLeaves < 4 )
            continue;

        uTruth = 0xFFFF & *Cut_CutReadTruth(pCut);
        pPerm = p->pPerms4[ (int)p->pPerms[uTruth] ];
        uPhase = p->pPhases[uTruth];
        Vec_PtrClear( p->vFaninsCur );
        Vec_PtrFill( p->vFaninsCur, (int)pCut->nLeaves, 0 );
        for ( i = 0; i < (int)pCut->nLeaves; i++ )
        {
            pFanin = Abc_NtkObj( pNode->pNtk, pCut->pLeaves[(int)pPerm[i]] );
            if ( pFanin == NULL )
                break;
            pFanin = Abc_ObjNotCond( pFanin, ((uPhase & (1<<i)) > 0) );
            Vec_PtrWriteEntry( p->vFaninsCur, i, pFanin );
        }
        if ( i != (int)pCut->nLeaves )
        {
            p->nCutsBad++;
            continue;
        }
        p->nCutsGood++;

        {
            int Counter = 0;
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
                if ( Abc_ObjFanoutNum(Abc_ObjRegular(pFanin)) == 1 )
                    Counter++;
            if ( Counter > 2 )
                continue;
        }

        p->nStudentCutsEvaluated++;

clk2 = Abc_Clock();
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            Abc_ObjRegular(pFanin)->vFanouts.nSize++;
        Abc_NtkIncrementTravId( pNode->pNtk );
        nNodesSaved = Abc_NodeMffcLabelAig( pNode );
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            Abc_ObjRegular(pFanin)->vFanouts.nSize--;
p->timeMffc += Abc_Clock() - clk2;

        OriDelay = Rwr_StudentPredictDelayTimed( pNode->pNtk, pNode, p->vFaninsCur, &OriPredTime );
        Rwr_StudentAccumulatePrediction( p, OriPredTime );

clk2 = Abc_Clock();
        pGraph = Rwr_CutEvaluate_v2( p, pNode, pCut, p->vFaninsCur, nNodesSaved, Required, &GainCur, fPlaceEnable, OriDelay, ((uPhase & (1<<4)) > 0), &DelayGainCur, &NewDelayCur, &CandPredTotalTimeCur, &BestPredTimeCur, &nSubgraphsEvaldCur, &EvalTotalTimeCur );
p->timeEval += Abc_Clock() - clk2;
        Rwr_StudentLogProgress( p, 0 );

        if ( pGraph != NULL && Rwr_StudentShouldPreferCandidate( GainBest != -1, DelayGainBest, GainBest, DelayGainCur, GainCur, DelayMargin ) )
        {
            pCutBest = pCut;
            nNodesSaveCur = nNodesSaved;
            GainBest = GainCur;
            DelayGainBest = DelayGainCur;
            OriDelayBest = OriDelay;
            NewDelayBest = NewDelayCur;
            OriPredTimeBest = OriPredTime;
            CandPredTotalTimeBest = CandPredTotalTimeCur;
            BestPredTimeBest = BestPredTimeCur;
            nSubgraphsEvaldBest = nSubgraphsEvaldCur;
            EvalTotalTimeBest = EvalTotalTimeCur;
            p->pGraph = pGraph;
            p->fCompl = ((uPhase & (1<<4)) > 0);
            uTruthBest = 0xFFFF & *Cut_CutReadTruth(pCut);
            Vec_PtrClear( p->vFanins );
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
                Vec_PtrPush( p->vFanins, pFanin );
        }
    }
p->timeRes += Abc_Clock() - clk;

    if ( GainBest == -1 )
        return -1;

    Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
        Dec_GraphNode((Dec_Graph_t *)p->pGraph, i)->pFunc = pFanin;

    if ( pCutBest != NULL )
        Rwr_StudentLogChoice( pNode, pCutBest, GainBest, OriDelayBest, NewDelayBest, DelayGainBest, OriPredTimeBest, CandPredTotalTimeBest, BestPredTimeBest, nSubgraphsEvaldBest, EvalTotalTimeBest );

    p->nScores[p->pMap[uTruthBest]]++;
    p->nNodesGained += GainBest;
    if ( fUseZeros || GainBest > 0 )
        p->nNodesRewritten++;

    if ( fVeryVerbose && GainBest > 0 )
    {
        printf( "Node %6s :   ", Abc_ObjName(pNode) );
        printf( "Fanins = %d. ", p->vFanins->nSize );
        printf( "Save = %d.  ", nNodesSaveCur );
        printf( "Add = %d.  ",  nNodesSaveCur-GainBest );
        printf( "GAIN = %d.  ", GainBest );
        printf( "Class = %d.  ", p->pMap[uTruthBest] );
        printf( "DelayGain = %.4f.  ", DelayGainBest );
        printf( "\n" );
    }
    return GainBest;
}

int Rwr_NodeRewrite_mine(int sock, Rwr_Man_t * p, Rwr_Man_t * pCopy, Cut_Man_t * pManCut, Abc_Obj_t * pNode, Abc_Ntk_t * pNtk, int index, int fUpdateLevel, int fUseZeros, int fPlaceEnable )
{
    extern int * myHashInit();
    extern void myHashDestroy(int * hash_nodes);

    int fVeryVerbose = 0;
    Dec_Graph_t * pGraph;
    Cut_Cut_t * pCut;//, * pTemp;
    Abc_Obj_t * pFanin;
    unsigned uPhase;
    unsigned uTruthBest = 0; // Suppress "might be used uninitialized"
    unsigned uTruth;
    char * pPerm;
    int Required, nNodesSaved;
    int nNodesSaveCur = -1; // Suppress "might be used uninitialized"
    int i, GainCur = -1, GainBest = -1;
    abctime clk, clk2;//, Counter;

    p->nNodesConsidered++;
    pCopy->nNodesConsidered++;
    // get the required times
    Required = fUpdateLevel? Abc_ObjRequiredLevel(pNode) : ABC_INFINITY;

    // get the node's cuts
clk = Abc_Clock();
    pCut = (Cut_Cut_t *)Abc_NodeGetCutsRecursive( pManCut, pNode, 0, 0 );
    assert( pCut != NULL );
p->timeCut += Abc_Clock() - clk;

//printf( " %d", Rwr_CutCountNumNodes(pNode, pCut) );
/*
    Counter = 0;
    for ( pTemp = pCut->pNext; pTemp; pTemp = pTemp->pNext )
        Counter++;
    printf( "%d ", Counter );
*/
    Rwr_print_nodeStructure( pNtk, "RwrStrucutOri.log" );
    Cut_Man_t * pManCutTrav = Abc_NtkStartCutManForRewrite_mine( pNtk );
    Rwr_printCut( pManCutTrav, pNtk, "cut_trave.log" );
    Cut_ManStop(pManCutTrav);
    // {"stage", "root", "Leaves"}

    // go through the cuts
clk = Abc_Clock();
    for ( pCut = pCut->pNext; pCut; pCut = pCut->pNext )
    {
        // consider only 4-input cuts
        if ( pCut->nLeaves < 4 )
            continue;
//            Cut_CutPrint( pCut, 0 ), printf( "\n" );

        // get the fanin permutation
        uTruth = 0xFFFF & *Cut_CutReadTruth(pCut);
        pPerm = p->pPerms4[ (int)p->pPerms[uTruth] ];
        uPhase = p->pPhases[uTruth];
        // collect fanins with the corresponding permutation/phase
        Vec_PtrClear( p->vFaninsCur );
        Vec_PtrFill( p->vFaninsCur, (int)pCut->nLeaves, 0 );
        for ( i = 0; i < (int)pCut->nLeaves; i++ )
        {
            pFanin = Abc_NtkObj( pNode->pNtk, pCut->pLeaves[(int)pPerm[i]] );
            if ( pFanin == NULL )
                break;
            pFanin = Abc_ObjNotCond(pFanin, ((uPhase & (1<<i)) > 0) );
            Vec_PtrWriteEntry( p->vFaninsCur, i, pFanin );
        }
        if ( i != (int)pCut->nLeaves )
        {
            p->nCutsBad++;
            continue;
        }
        p->nCutsGood++;

        {
            int Counter = 0;
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
                if ( Abc_ObjFanoutNum(Abc_ObjRegular(pFanin)) == 1 )
                    Counter++;
            if ( Counter > 2 )
                continue;
        }

clk2 = Abc_Clock();
/*
        printf( "Considering: (" );
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            printf( "%d ", Abc_ObjFanoutNum(Abc_ObjRegular(pFanin)) );
        printf( ")\n" );
*/
        // mark the fanin boundary 
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            Abc_ObjRegular(pFanin)->vFanouts.nSize++;

        // label MFFC with current ID
        Abc_NtkIncrementTravId( pNode->pNtk );
        nNodesSaved = Abc_NodeMffcLabelAig( pNode );
        // unmark the fanin boundary
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
            Abc_ObjRegular(pFanin)->vFanouts.nSize--;
p->timeMffc += Abc_Clock() - clk2;

        char json_data[100];
        int offset = 0;
        offset += snprintf(json_data + offset, sizeof(json_data) - offset, "{\"stage\": \"ori\", \"root\": %d, \"Leaves\": [", pNode->Id);
        for ( int j = 0; j < (int)pCut->nLeaves; j++ ){
            pFanin = Abc_NtkObj( pNode->pNtk, pCut->pLeaves[j] );
            if ( pFanin == NULL )
                break;
            offset += snprintf(json_data + offset, sizeof(json_data) - offset, "%d", pFanin->Id);
            if ((j+1) < (int)pCut->nLeaves)
                offset += snprintf(json_data + offset, sizeof(json_data) - offset, ", ");
        }
        offset += snprintf(json_data + offset, sizeof(json_data) - offset, "]}");
        Rwr_print("Send Ori Message:   ");
        Rwr_print(json_data);
        Rwr_print("\n");
        send(sock, json_data, strlen(json_data), 0);
        char buffer[1024] = {0};
        recv(sock, buffer, sizeof(buffer), 0);
        Rwr_print("Recieve Ori Message:   ");
        Rwr_print(buffer);
        Rwr_print("\n");
        if (strcmp(buffer, "finish") == 0) {
            if ( !g_RwrStudentQuiet )
                printf("检测到 finish,退出\n");
            close(sock);
            exit(0);
        }
        float oriDelay = atof(buffer);
        if(oriDelay < -1000) continue;


        // evaluate the cut
clk2 = Abc_Clock();
        pGraph = Rwr_CutEvaluate_mine(sock, p, pCopy, pNode, pNtk, uPhase, pPerm, index, pCut, p->vFaninsCur, nNodesSaved, oriDelay, Required, &GainCur, fPlaceEnable );
p->timeEval += Abc_Clock() - clk2;

        // check if the cut is better than the current best one
        if ( pGraph != NULL && GainBest < GainCur )
        {
            // save this form
            nNodesSaveCur = nNodesSaved;
            GainBest  = GainCur;
            p->pGraph  = pGraph;
            p->fCompl = ((uPhase & (1<<4)) > 0);
            uTruthBest = 0xFFFF & *Cut_CutReadTruth(pCut);
            // collect fanins in the
            Vec_PtrClear( p->vFanins );
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFaninsCur, pFanin, i )
                Vec_PtrPush( p->vFanins, pFanin );
        }
        Rwr_print("Done One Cut\n");
    }
p->timeRes += Abc_Clock() - clk;

    if ( GainBest == -1 )
        return -1;
/*
    if ( GainBest > 0 )
    {
        printf( "Class %d  ", p->pMap[uTruthBest] );
        printf( "Gain = %d. Node %d : ", GainBest, pNode->Id );
        Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
            printf( "%d ", Abc_ObjRegular(pFanin)->Id );
        Dec_GraphPrint( stdout, p->pGraph, NULL, NULL );
        printf( "\n" );
    }
*/

//    printf( "%d", nNodesSaveCur - GainBest );
/*
    if ( GainBest > 0 )
    {
        if ( Rwr_CutIsBoolean( pNode, p->vFanins ) )
            printf( "b" );
        else
        {
            printf( "Node %d : ", pNode->Id );
            Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
                printf( "%d ", Abc_ObjRegular(pFanin)->Id );
            printf( "a" );
        }
    }
*/
/*
    if ( GainBest > 0 )
        if ( p->fCompl )
            printf( "c" );
        else
            printf( "." );
*/

    // copy the leaves
    Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
        Dec_GraphNode((Dec_Graph_t *)p->pGraph, i)->pFunc = pFanin;
/*
    printf( "(" );
    Vec_PtrForEachEntry( Abc_Obj_t *, p->vFanins, pFanin, i )
        printf( " %d", Abc_ObjRegular(pFanin)->vFanouts.nSize - 1 );
    printf( " )  " );
*/
//    printf( "%d ", Rwr_NodeGetDepth_rec( pNode, p->vFanins ) );

    p->nScores[p->pMap[uTruthBest]]++;
    p->nNodesGained += GainBest;
    if ( fUseZeros || GainBest > 0 )
    {
        p->nNodesRewritten++;
    }

    // report the progress
    if ( fVeryVerbose && GainBest > 0 )
    {
        printf( "Node %6s :   ", Abc_ObjName(pNode) );
        printf( "Fanins = %d. ", p->vFanins->nSize );
        printf( "Save = %d.  ", nNodesSaveCur );
        printf( "Add = %d.  ",  nNodesSaveCur-GainBest );
        printf( "GAIN = %d.  ", GainBest );
        printf( "Cone = %d.  ", p->pGraph? Dec_GraphNodeNum((Dec_Graph_t *)p->pGraph) : 0 );
        printf( "Class = %d.  ", p->pMap[uTruthBest] );
        printf( "\n" );
    }
    return GainBest;
}

/**Function*************************************************************

  Synopsis    [Evaluates the cut.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Dec_Graph_t * Rwr_CutEvaluate( Rwr_Man_t * p, Abc_Obj_t * pRoot, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, int LevelMax, int * pGainBest, int fPlaceEnable )
{
    extern int            Dec_GraphToNetworkCount( Abc_Obj_t * pRoot, Dec_Graph_t * pGraph, int NodeMax, int LevelMax );
    Vec_Ptr_t * vSubgraphs;
    Dec_Graph_t * pGraphBest = NULL; // Suppress "might be used uninitialized"
    Dec_Graph_t * pGraphCur;
    Rwr_Node_t * pNode, * pFanin;
    int nNodesAdded, GainBest, i, k;
    unsigned uTruth;
    float CostBest;//, CostCur;
    // find the matching class of subgraphs
    uTruth = 0xFFFF & *Cut_CutReadTruth(pCut);
    vSubgraphs = Vec_VecEntry( p->vClasses, p->pMap[uTruth] );
    p->nSubgraphs += vSubgraphs->nSize;
    // determine the best subgraph
    GainBest = -1;
    CostBest = ABC_INFINITY;
    Vec_PtrForEachEntry( Rwr_Node_t *, vSubgraphs, pNode, i )
    {
        // get the current graph
        pGraphCur = (Dec_Graph_t *)pNode->pNext;
        // copy the leaves
        Vec_PtrForEachEntry( Rwr_Node_t *, vFaninsCur, pFanin, k )
            Dec_GraphNode(pGraphCur, k)->pFunc = pFanin;

        // detect how many unlabeled nodes will be reused
        nNodesAdded = Dec_GraphToNetworkCount( pRoot, pGraphCur, nNodesSaved, LevelMax );
        if ( nNodesAdded == -1 )
            continue;
        assert( nNodesSaved >= nNodesAdded );
/*
        // evaluate the cut
        if ( fPlaceEnable )
        {
            extern float Abc_PlaceEvaluateCut( Abc_Obj_t * pRoot, Vec_Ptr_t * vFanins );

            float Alpha = 0.5; // ???
            float PlaceCost;

            // get the placement cost of the cut
            PlaceCost = Abc_PlaceEvaluateCut( pRoot, vFaninsCur );

            // get the weigted cost of the cut
            CostCur = nNodesSaved - nNodesAdded + Alpha * PlaceCost;

            // do not allow uphill moves
            if ( nNodesSaved - nNodesAdded < 0 )
                continue;

            // decide what cut to use
            if ( CostBest > CostCur )
            {
                GainBest   = nNodesSaved - nNodesAdded; // pure node cost
                CostBest   = CostCur;                   // cost with placement
                pGraphBest = pGraphCur;                 // subgraph to be used for rewriting

                // score the graph
                if ( nNodesSaved - nNodesAdded > 0 )
                {
                    pNode->nScore++;
                    pNode->nGain += GainBest;
                    pNode->nAdded += nNodesAdded;
                }
            }
        }
        else
*/
        {
            // count the gain at this node
            if ( GainBest < nNodesSaved - nNodesAdded )
            {
                GainBest   = nNodesSaved - nNodesAdded;
                pGraphBest = pGraphCur;

                // score the graph
                if ( nNodesSaved - nNodesAdded > 0 )
                {
                    pNode->nScore++;
                    pNode->nGain += GainBest;
                    pNode->nAdded += nNodesAdded;
                }
            }
        }
    }
    if ( GainBest == -1 )
        return NULL;
    *pGainBest = GainBest;
    return pGraphBest;
}

static Dec_Graph_t * Rwr_CutEvaluate_v2( Rwr_Man_t * p, Abc_Obj_t * pRoot, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, int LevelMax, int * pGainBest, int fPlaceEnable, double OriDelay, int fComplRoot, double * pDelayGainBest, double * pNewDelayBest, abctime * pCandPredTotalTime, abctime * pBestPredTime, int * pSubgraphsEvald, abctime * pEvalTotalTime )
{
    extern int            Dec_GraphToNetworkCount( Abc_Obj_t * pRoot, Dec_Graph_t * pGraph, int NodeMax, int LevelMax );
    extern Abc_Obj_t *    Dec_GraphToNetwork( Abc_Ntk_t * pNtk, Dec_Graph_t * pGraph );
    Vec_Ptr_t * vSubgraphs;
    Dec_Graph_t * pGraphBest = NULL;
    Dec_Graph_t * pGraphCur;
    Rwr_Node_t * pNode, * pFanin;
    int nNodesAdded, GainBest, GainCur, i, k;
    double DelayGainBestLocal = -1.0e100, DelayGainCur;
    double NewDelayBest = 1.0e100, NewDelayCur;
    unsigned uTruth;
    int nSubgraphsEvald = 0;
    abctime EvalStart = Abc_Clock();
    abctime CandPredTotalTime = 0, BestPredTime = 0;
    double DelayMargin = 10.0;

    (void)fPlaceEnable;

    uTruth = 0xFFFF & *Cut_CutReadTruth(pCut);
    vSubgraphs = Vec_VecEntry( p->vClasses, p->pMap[uTruth] );
    p->nSubgraphs += vSubgraphs->nSize;
    GainBest = -1;
    Vec_PtrForEachEntry( Rwr_Node_t *, vSubgraphs, pNode, i )
    {
        Abc_Obj_t * pRootNew = NULL;
        Abc_Obj_t * pRootNewR = NULL;
        int nObjsBefore;
        abctime CandPredTime = 0;

        pGraphCur = (Dec_Graph_t *)pNode->pNext;
        Vec_PtrForEachEntry( Rwr_Node_t *, vFaninsCur, pFanin, k )
            Dec_GraphNode( pGraphCur, k )->pFunc = pFanin;

        nNodesAdded = Dec_GraphToNetworkCount( pRoot, pGraphCur, nNodesSaved, LevelMax );
        if ( nNodesAdded == -1 )
            continue;
        assert( nNodesSaved >= nNodesAdded );
        GainCur = nNodesSaved - nNodesAdded;
        DelayGainCur = -1.0e100;
        NewDelayCur = 1.0e100;
        nSubgraphsEvald++;

        nObjsBefore = Abc_NtkObjNumMax( pRoot->pNtk );
        if ( fComplRoot )
            Dec_GraphComplement( pGraphCur );
        pRootNew = Dec_GraphToNetwork( pRoot->pNtk, pGraphCur );
        if ( fComplRoot )
            Dec_GraphComplement( pGraphCur );

        if ( pRootNew != NULL && Abc_ObjRegular(pRootNew) != Abc_ObjRegular(pRoot) )
        {
            NewDelayCur = Rwr_StudentPredictDelayInPlace( pRoot->pNtk, pRootNew, vFaninsCur, pRoot, &CandPredTime );
            CandPredTotalTime += CandPredTime;
            Rwr_StudentAccumulatePrediction( p, CandPredTime );
            DelayGainCur = OriDelay - NewDelayCur;
        }

        if ( pRootNew != NULL )
        {
            pRootNewR = Abc_ObjRegular( pRootNew );
            if ( pRootNewR->Id >= nObjsBefore && Abc_ObjFanoutNum(pRootNewR) == 0 )
                Abc_AigDeleteNode( (Abc_Aig_t *)pRoot->pNtk->pManFunc, pRootNewR );
        }

        Vec_PtrForEachEntry( Rwr_Node_t *, vFaninsCur, pFanin, k )
            Dec_GraphNode( pGraphCur, k )->pFunc = pFanin;

        if ( pRootNew == NULL || Abc_ObjRegular(pRootNew) == Abc_ObjRegular(pRoot) )
            continue;

        if ( Rwr_StudentShouldPreferCandidate( GainBest != -1, DelayGainBestLocal, GainBest, DelayGainCur, GainCur, DelayMargin ) )
        {
            GainBest = GainCur;
            DelayGainBestLocal = DelayGainCur;
            NewDelayBest = NewDelayCur;
            BestPredTime = CandPredTime;
            pGraphBest = pGraphCur;
            if ( GainCur > 0 )
            {
                pNode->nScore++;
                pNode->nGain += GainCur;
                pNode->nAdded += nNodesAdded;
            }
        }
    }
    if ( GainBest == -1 )
        return NULL;
    if ( pDelayGainBest )
        *pDelayGainBest = DelayGainBestLocal;
    if ( pNewDelayBest )
        *pNewDelayBest = NewDelayBest;
    if ( pCandPredTotalTime )
        *pCandPredTotalTime = CandPredTotalTime;
    if ( pBestPredTime )
        *pBestPredTime = BestPredTime;
    if ( pSubgraphsEvald )
        *pSubgraphsEvald = nSubgraphsEvald;
    if ( pEvalTotalTime )
        *pEvalTotalTime = Abc_Clock() - EvalStart;
    *pGainBest = GainBest;
    return pGraphBest;
}

Dec_Graph_t * Rwr_CutEvaluate_mine(int sock, Rwr_Man_t * p, Rwr_Man_t * pCopy, Abc_Obj_t * pRoot, Abc_Ntk_t * pNtk, unsigned uPhase, char * pPerm, int index, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, float oriDelay, int LevelMax, int * pGainBest, int fPlaceEnable )
{
    extern int            Dec_GraphToNetworkCount( Abc_Obj_t * pRoot, Dec_Graph_t * pGraph, int NodeMax, int LevelMax );
    extern int           Dec_GraphUpdateNetwork_mine( Abc_Obj_t * pRoot, Dec_Graph_t * pGraph, int fUpdateLevel, int nGain );
    Vec_Ptr_t * vSubgraphs;
    Dec_Graph_t * pGraphBest = NULL; // Suppress "might be used uninitialized"
    Dec_Graph_t * pGraphCur;
    Rwr_Node_t * pNode, * pFanin;
    Abc_Obj_t * pFaninCopy;
    Cut_Man_t * pCutManSub;

    int nNodesAdded, GainBest, i, j, k, realId;
    float delayBest;
    unsigned uTruth;
    float CostBest;//, CostCur;
    // find the matching class of subgraphs
    uTruth = 0xFFFF & *Cut_CutReadTruth(pCut);
    vSubgraphs = Vec_VecEntry( p->vClasses, p->pMap[uTruth] );
    p->nSubgraphs += vSubgraphs->nSize;
    // determine the best subgraph
    GainBest = 20;
    delayBest = -1;
    CostBest = ABC_INFINITY;
    Vec_PtrForEachEntry( Rwr_Node_t *, vSubgraphs, pNode, i )
    {

        FILE * fp = Abc_FrameOpenLogFile( "cut_trave_update.log", "w" );
        if ( fp )
            fclose(fp);
        // printf("Root ID: %d\n", pRoot->Id);
        
        int fakeId2Id[Abc_NtkObjNumMax(pNtk)+3000];
        Abc_Ntk_t * pNtkCopy = Abc_NtkDup_mine(pNtk, fakeId2Id);
        Rwr_print_nodeStructure( pNtkCopy, "RwrStrucutUpdatedBeforeRep.log" );
        realId = fakeId2Id[index];
        Abc_Obj_t * pNodeCopy = Abc_NtkObj( pNtkCopy, realId ); 
        // printf("%d %d %d \n", index, realId, pNodeCopy->fakeId);
        assert(pNodeCopy->fakeId == index);
        int ori_rootId = pNodeCopy->Id;
        // printf("%d %d %d %d\n", (pNodeCopy->pNtk), pNtkCopy, (pRoot->pNtk), pNtk);
        Vec_PtrClear( pCopy->vFaninsCur );
        Vec_PtrFill( pCopy->vFaninsCur, (int)pCut->nLeaves, 0 );
        int CutIds[(int)pCut->nLeaves],cutCap = 0;
        for (int j = 0; j < (int)pCut->nLeaves; j++ )
        {
            if(pCut->pLeaves[(int)pPerm[j]] <= Abc_NtkPiNum(pNtk)){
                realId = pCut->pLeaves[(int)pPerm[j]];
                pFaninCopy = Abc_NtkObj( pNtkCopy, realId);}
            else{
                realId = fakeId2Id[pCut->pLeaves[(int)pPerm[j]]];
                pFaninCopy = Abc_NtkObj( pNtkCopy, realId);
                // printf("%d %d %d \n", pCut->pLeaves[(int)pPerm[j]], realId, pFaninCopy->fakeId);
                assert(pFaninCopy->fakeId == pCut->pLeaves[(int)pPerm[j]]);}
            if ( pFaninCopy == NULL )
                break;
            CutIds[cutCap] = pFaninCopy->Id;
            cutCap += 1;
            pFaninCopy = Abc_ObjNotCond(pFaninCopy, ((uPhase & (1<<j)) > 0) );
            Vec_PtrWriteEntry( pCopy->vFaninsCur, j, pFaninCopy );
        }

        // get the current graph
        pGraphCur = (Dec_Graph_t *)pNode->pNext;
        Vec_PtrForEachEntry( Rwr_Node_t *, pCopy->vFaninsCur, pFanin, k ){
            // printf("Fanin: %d ", k);
            Dec_GraphNode(pGraphCur, k)->pFunc = pFanin;
        }
        // printf("\n");

        // replace Subgraph
        int fCompl = ((uPhase & (1<<4)) > 0);
        if(fCompl) Dec_GraphComplement(pGraphCur);
        int rootId = Dec_GraphUpdateNetwork_mine(pNodeCopy, pGraphCur, 1, 0);
        if (rootId == -1){
            Abc_NtkDelete( pNtkCopy );
            continue;
        }
        else if(rootId <= Abc_NtkPiNum(pNtk)){
            pGraphCur = (Dec_Graph_t *)pNode->pNext;
            if(fCompl) Dec_GraphComplement(pGraphCur);
            Vec_PtrForEachEntry( Rwr_Node_t *, vFaninsCur, pFanin, k )
                Dec_GraphNode(pGraphCur, k)->pFunc = pFanin;
    
            // detect how many unlabeled nodes will be reused
            nNodesAdded = Dec_GraphToNetworkCount( pRoot, pGraphCur, nNodesSaved, LevelMax );

            GainBest   = 100000;
            pGraphBest = pGraphCur;
            pNode->nScore++;
            pNode->nGain += GainBest;
            pNode->nAdded += nNodesAdded;
            *pGainBest = GainBest;
            return pGraphBest;
        }
        
        pCutManSub = Abc_NtkStartCutManForRewrite_mine( pNtkCopy );
        Rwr_printCut( pCutManSub, pNtkCopy, "cut_trave_update.log" );
        Cut_ManStop(pCutManSub);
        Rwr_print_nodeStructure( pNtkCopy, "RwrStrucutUpdated.log" );

        char json_data[100];
        int offset = 0;
        offset += snprintf(json_data + offset, sizeof(json_data) - offset, "{\"stage\": \"update\", \"root\": %d, \"Leaves\": [", rootId);
        for(j = 0; j < cutCap; j++)
        {
            offset += snprintf(json_data + offset, sizeof(json_data) - offset, "%d", CutIds[j]);
            if ((j+1) < (int)pCut->nLeaves)
                offset += snprintf(json_data + offset, sizeof(json_data) - offset, ", ");
            
        }
        offset += snprintf(json_data + offset, sizeof(json_data) - offset, "], \"ori_root\": %d}", ori_rootId);
        Rwr_print("Send Update Message:   ");
        Rwr_print(json_data);
        Rwr_print("\n");
        send(sock, json_data, strlen(json_data), 0);
        char buffer[1024] = {0};
        recv(sock, buffer, sizeof(buffer), 0);
        Rwr_print("Recieve Update Message:   ");
        Rwr_print(buffer);
        Rwr_print("\n");
        if (strcmp(buffer, "finish") == 0) {
            if ( !g_RwrStudentQuiet )
                printf("检测到 finish,退出\n");
            close(sock);
            exit(0);
        }
        float updateDelay = atof(buffer);
        Abc_NtkDelete( pNtkCopy );
        if(updateDelay < -1000) continue;
        Rwr_print("Done One Subgraph\n");

        // copy the leaves
        pGraphCur = (Dec_Graph_t *)pNode->pNext;
        if(fCompl) Dec_GraphComplement(pGraphCur);
        Vec_PtrForEachEntry( Rwr_Node_t *, vFaninsCur, pFanin, k )
            Dec_GraphNode(pGraphCur, k)->pFunc = pFanin;

        // detect how many unlabeled nodes will be reused
        nNodesAdded = Dec_GraphToNetworkCount( pRoot, pGraphCur, nNodesSaved, LevelMax );
        if ( nNodesAdded == -1 )
            continue;
        assert( nNodesSaved >= nNodesAdded );
/*
        // evaluate the cut
        if ( fPlaceEnable )
        {
            extern float Abc_PlaceEvaluateCut( Abc_Obj_t * pRoot, Vec_Ptr_t * vFanins );

            float Alpha = 0.5; // ???
            float PlaceCost;

            // get the placement cost of the cut
            PlaceCost = Abc_PlaceEvaluateCut( pRoot, vFaninsCur );

            // get the weigted cost of the cut
            CostCur = nNodesSaved - nNodesAdded + Alpha * PlaceCost;

            // do not allow uphill moves
            if ( nNodesSaved - nNodesAdded < 0 )
                continue;

            // decide what cut to use
            if ( CostBest > CostCur )
            {
                GainBest   = nNodesSaved - nNodesAdded; // pure node cost
                CostBest   = CostCur;                   // cost with placement
                pGraphBest = pGraphCur;                 // subgraph to be used for rewriting

                // score the graph
                if ( nNodesSaved - nNodesAdded > 0 )
                {
                    pNode->nScore++;
                    pNode->nGain += GainBest;
                    pNode->nAdded += nNodesAdded;
                }
            }
        }
        else
*/
        {
            // count the gain at this node
            if ( GainBest < oriDelay - updateDelay )
            {
                GainBest   = oriDelay - updateDelay;
                pGraphBest = pGraphCur;

                // score the graph
                if ( oriDelay - updateDelay > 0 )
                {
                    pNode->nScore++;
                    pNode->nGain += GainBest;
                    pNode->nAdded += nNodesAdded;
                }
            }
        }
        // {
        //     // count the gain at this node
        //     if ( GainBest < nNodesSaved - nNodesAdded )
        //     {
        //         GainBest   = nNodesSaved - nNodesAdded;
        //         pGraphBest = pGraphCur;

        //         // score the graph
        //         if ( nNodesSaved - nNodesAdded > 0 )
        //         {
        //             pNode->nScore++;
        //             pNode->nGain += GainBest;
        //             pNode->nAdded += nNodesAdded;
        //         }
        //     }
        // }
    }
    if ( GainBest == -1 )
        return NULL;
    *pGainBest = GainBest;
    return pGraphBest;
}

/**Function*************************************************************

  Synopsis    [Checks the type of the cut.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Rwr_CutIsBoolean_rec( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves, int fMarkA )
{
    if ( Vec_PtrFind(vLeaves, pObj) >= 0 || Vec_PtrFind(vLeaves, Abc_ObjNot(pObj)) >= 0 )
    {
        if ( fMarkA )
            pObj->fMarkA = 1;
        else
            pObj->fMarkB = 1;
        return;
    }
    assert( !Abc_ObjIsCi(pObj) );
    Rwr_CutIsBoolean_rec( Abc_ObjFanin0(pObj), vLeaves, fMarkA );
    Rwr_CutIsBoolean_rec( Abc_ObjFanin1(pObj), vLeaves, fMarkA );
}

/**Function*************************************************************

  Synopsis    [Checks the type of the cut.]

  Description [Returns 1(0) if the cut is Boolean (algebraic).]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int Rwr_CutIsBoolean( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves )
{
    Abc_Obj_t * pTemp;
    int i, RetValue;
    Vec_PtrForEachEntry( Abc_Obj_t *, vLeaves, pTemp, i )
    {
        pTemp = Abc_ObjRegular(pTemp);
        assert( !pTemp->fMarkA && !pTemp->fMarkB );
    }
    Rwr_CutIsBoolean_rec( Abc_ObjFanin0(pObj), vLeaves, 1 );
    Rwr_CutIsBoolean_rec( Abc_ObjFanin1(pObj), vLeaves, 0 );
    RetValue = 0;
    Vec_PtrForEachEntry( Abc_Obj_t *, vLeaves, pTemp, i )
    {
        pTemp = Abc_ObjRegular(pTemp);
        RetValue |= pTemp->fMarkA && pTemp->fMarkB;
        pTemp->fMarkA = pTemp->fMarkB = 0;
    }
    return RetValue;
}


/**Function*************************************************************

  Synopsis    [Count the nodes in the cut space of a node.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Rwr_CutCountNumNodes_rec( Abc_Obj_t * pObj, Cut_Cut_t * pCut, Vec_Ptr_t * vNodes )
{
    int i;
    for ( i = 0; i < (int)pCut->nLeaves; i++ )
        if ( pCut->pLeaves[i] == pObj->Id )
        {
            // check if the node is collected
            if ( pObj->fMarkC == 0 )
            {
                pObj->fMarkC = 1;
                Vec_PtrPush( vNodes, pObj );
            }
            return;
        }
    assert( Abc_ObjIsNode(pObj) );
    // check if the node is collected
    if ( pObj->fMarkC == 0 )
    {
        pObj->fMarkC = 1;
        Vec_PtrPush( vNodes, pObj );
    }
    // traverse the fanins
    Rwr_CutCountNumNodes_rec( Abc_ObjFanin0(pObj), pCut, vNodes );
    Rwr_CutCountNumNodes_rec( Abc_ObjFanin1(pObj), pCut, vNodes );
}

/**Function*************************************************************

  Synopsis    [Count the nodes in the cut space of a node.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int Rwr_CutCountNumNodes( Abc_Obj_t * pObj, Cut_Cut_t * pCut )
{
    Vec_Ptr_t * vNodes;
    int i, Counter;
    // collect all nodes
    vNodes = Vec_PtrAlloc( 100 );
    for ( pCut = pCut->pNext; pCut; pCut = pCut->pNext )
        Rwr_CutCountNumNodes_rec( pObj, pCut, vNodes );
    // clean all nodes
    Vec_PtrForEachEntry( Abc_Obj_t *, vNodes, pObj, i )
        pObj->fMarkC = 0;
    // delete and return
    Counter = Vec_PtrSize(vNodes);
    Vec_PtrFree( vNodes );
    return Counter;
}


/**Function*************************************************************

  Synopsis    [Returns depth of the cut.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int Rwr_NodeGetDepth_rec( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves )
{
    Abc_Obj_t * pLeaf;
    int i, Depth0, Depth1;
    if ( Abc_ObjIsCi(pObj) )
        return 0;
    Vec_PtrForEachEntry( Abc_Obj_t *, vLeaves, pLeaf, i )
        if ( pObj == Abc_ObjRegular(pLeaf) )
            return 0;
    Depth0 = Rwr_NodeGetDepth_rec( Abc_ObjFanin0(pObj), vLeaves );
    Depth1 = Rwr_NodeGetDepth_rec( Abc_ObjFanin1(pObj), vLeaves );
    return 1 + Abc_MaxInt( Depth0, Depth1 );
}


/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Rwr_ScoresClean( Rwr_Man_t * p )
{
    Vec_Ptr_t * vSubgraphs;
    Rwr_Node_t * pNode;
    int i, k;
    for ( i = 0; i < p->vClasses->nSize; i++ )
    {
        vSubgraphs = Vec_VecEntry( p->vClasses, i );
        Vec_PtrForEachEntry( Rwr_Node_t *, vSubgraphs, pNode, k )
            pNode->nScore = pNode->nGain = pNode->nAdded = 0;
    }
}

static int Gains[222];

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int Rwr_ScoresCompare( int * pNum1, int * pNum2 )
{
    if ( Gains[*pNum1] > Gains[*pNum2] )
        return -1;
    if ( Gains[*pNum1] < Gains[*pNum2] )
        return 1;
    return 0;
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Rwr_ScoresReport( Rwr_Man_t * p )
{
    extern void Ivy_TruthDsdComputePrint( unsigned uTruth );
    int Perm[222];
    Vec_Ptr_t * vSubgraphs;
    Rwr_Node_t * pNode;
    int i, iNew, k;
    unsigned uTruth;
    // collect total gains
    assert( p->vClasses->nSize == 222 );
    for ( i = 0; i < p->vClasses->nSize; i++ )
    {
        Perm[i] = i;
        Gains[i] = 0;
        vSubgraphs = Vec_VecEntry( p->vClasses, i );
        Vec_PtrForEachEntry( Rwr_Node_t *, vSubgraphs, pNode, k )
            Gains[i] += pNode->nGain;
    }
    // sort the gains
    qsort( Perm, (size_t)222, sizeof(int), (int (*)(const void *, const void *))Rwr_ScoresCompare );

    // print classes
    for ( i = 0; i < p->vClasses->nSize; i++ )
    {
        iNew = Perm[i];
        if ( Gains[iNew] == 0 )
            break;
        vSubgraphs = Vec_VecEntry( p->vClasses, iNew );
        printf( "CLASS %3d: Subgr = %3d. Total gain = %6d.  ", iNew, Vec_PtrSize(vSubgraphs), Gains[iNew] );
        uTruth = (unsigned)p->pMapInv[iNew];
        Extra_PrintBinary( stdout, &uTruth, 16 );
        printf( "  " );
        Ivy_TruthDsdComputePrint( (unsigned)p->pMapInv[iNew] | ((unsigned)p->pMapInv[iNew] << 16) );
        Vec_PtrForEachEntry( Rwr_Node_t *, vSubgraphs, pNode, k )
        {
            if ( pNode->nScore == 0 )
                continue;
            printf( "    %2d: S=%5d. A=%5d. G=%6d. ", k, pNode->nScore, pNode->nAdded, pNode->nGain );
            Dec_GraphPrint( stdout, (Dec_Graph_t *)pNode->pNext, NULL, NULL );
        }
    }
}



void Rwr_printNode_recur(Abc_Ntk_t * pNtk, Abc_Obj_t * pRoot, int * vVisited){
    Abc_Obj_t * pFanin;
    int i;
    if (Abc_ObjIsCi(pRoot) || vVisited[pRoot->Id] == 1){
        return;
    }
    Abc_ObjForEachFanin(pRoot, pFanin, i){
        printf("%d ", Abc_ObjId(pFanin));
        if(i == 0)
            printf("%d ", pRoot->fCompl0);
        else if(i == 1)
            printf("%d ", pRoot->fCompl1);
    }
    printf("%d \n", pRoot->Id);
    vVisited[pRoot->Id] = 1;
    Abc_ObjForEachFanin(pRoot, pFanin, i){
        Rwr_printNode_recur(pNtk, pFanin, vVisited);
    }
}

void Rwr_print_nodeStructure( Abc_Ntk_t * pNtk, const char * pFileName ){
    Abc_Obj_t * pObj;
    Abc_Obj_t * pFanin;
    int nNodes = Abc_NtkObjNumMax(pNtk);
    int vVisited[nNodes+1000];
    int j;
    int i;

    for (i = 0; i < nNodes+1000; i++){
        vVisited[i] = 0;
    }
    if ( !Abc_FrameRedirectStdoutToLog( pFileName, "w" ) )
        return;
    Abc_NtkForEachPo( pNtk, pObj, i )
    {
        Abc_ObjForEachFanin(pObj, pFanin, j){
            assert (j < 1);
            printf("%d ", Abc_ObjId(pFanin));
            printf("%d ", pObj->fCompl0);
            printf("%d\n", pObj->Id);
            Rwr_printNode_recur(pNtk, pFanin, vVisited);
        }
    }
    
    Abc_FrameRestoreStdout();
}

void Rwr_print(char* info){
    if ( g_RwrStudentQuiet )
        return;
    if ( !Abc_FrameRedirectStdoutToLog( "rw_log.log", "a" ) )
        return;
    fputs( info, stdout );
    Abc_FrameRestoreStdout();
}

void Rwr_printCut( Cut_Man_t * pManCut, Abc_Ntk_t * pNtk, const char * pFileName ){
    if ( !Abc_FrameRedirectStdoutToLog( pFileName, "w" ) )
        return;
    Abc_Obj_t * pObj, *pPo;
    int nNodes = Abc_NtkObjNumMax(pNtk);
    int vVisited[nNodes+3000];
    int i;
    
    for(i = 0; i < nNodes+3000; i++){
        vVisited[i] = 0;
    }
    Abc_NtkForEachPo( pNtk, pPo, i )
    {
        int j;
        Abc_ObjForEachFanin(pPo, pObj, j){
            assert (j < 1);
            Rwr_printCut_recur(pManCut, pObj, vVisited);
        }
    }
    Abc_FrameRestoreStdout();

}

void Rwr_printCut_recur(Cut_Man_t * pManCut, Abc_Obj_t * pRoot, int * vVisited){
    Abc_Obj_t * pFanin;
    Cut_Cut_t * pCut;
    int j;

    if (Abc_ObjIsPi(pRoot) || Abc_AigNodeIsConst(pRoot) || vVisited[pRoot->Id] == 1)
        return;
        
    if (( Abc_NodeIsPersistant(pRoot) ) || ( Abc_ObjFanoutNum(pRoot) > 1000 )){
        vVisited[pRoot->Id] = 1;
        Abc_ObjForEachFanin(pRoot, pFanin, j){
            Rwr_printCut_recur(pManCut, pFanin, vVisited);
        }
    }

    printf("Node %d\n", pRoot->Id);
    vVisited[pRoot->Id] = 1;
    pCut = (Cut_Cut_t *)Abc_NodeGetCutsRecursive( pManCut, pRoot, 0, 0 );
    // printf(" Start Leaves\n");
    unsigned pTruth;
    for ( pCut = pCut->pNext; pCut; pCut = pCut->pNext ){
        // if ( pCut->nLeaves < 4 )
        //     continue;

        pTruth =  0xFFFF & *Cut_CutReadTruth(pCut);
        printf(" Truth : %u Leaves: ", pTruth);

        for ( j = 0; j < (int)pCut->nLeaves; j++ ){
            pFanin = Abc_NtkObj( pRoot->pNtk, pCut->pLeaves[j] );
            if ( pFanin == NULL )
                break;
            printf("%d ", pFanin->Id);
        }
        printf("\n");
    }
    Abc_ObjForEachFanin(pRoot, pFanin, j){
        Rwr_printCut_recur(pManCut, pFanin, vVisited);
    }
}

Cut_Man_t * Abc_NtkStartCutManForRewrite_mine( Abc_Ntk_t * pNtk )
{
    static Cut_Params_t Params, * pParams = &Params;
    Cut_Man_t * pManCut;
    Abc_Obj_t * pObj;
    int i;
    // start the cut manager
    memset( pParams, 0, sizeof(Cut_Params_t) );
    pParams->nVarsMax  = 4;     // the max cut size ("k" of the k-feasible cuts)
    pParams->nKeepMax  = 250;   // the max number of cuts kept at a node
    pParams->fTruth    = 1;     // compute truth tables
    pParams->fFilter   = 1;     // filter dominated cuts
    pParams->fSeq      = 0;     // compute sequential cuts
    pParams->fDrop     = 0;     // drop cuts on the fly
    pParams->fVerbose  = 0;     // the verbosiness flag
    pParams->nIdsMax   = Abc_NtkObjNumMax( pNtk );
    pManCut = Cut_ManStart( pParams );
    if ( pParams->fDrop )
        Cut_ManSetFanoutCounts( pManCut, Abc_NtkFanoutCounts(pNtk) );
    // set cuts for PIs
    Abc_NtkForEachCi( pNtk, pObj, i )
        if ( Abc_ObjFanoutNum(pObj) > 0 )
            Cut_NodeSetTriv( pManCut, pObj->Id );
    return pManCut;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END
