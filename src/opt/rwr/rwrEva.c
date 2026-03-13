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
#include "bool/dec/dec.h"
#include "aig/ivy/ivy.h"

ABC_NAMESPACE_IMPL_START

#define MY_HASH_TABLE_SIZE 10007  // 选择一个大素数作为表大小


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

static Dec_Graph_t * Rwr_CutEvaluate( Rwr_Man_t * p, Abc_Obj_t * pRoot, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, int LevelMax, int * pGainBest, int fPlaceEnable );
Dec_Graph_t * Rwr_CutEvaluate_mine(int sock, Rwr_Man_t * p, Rwr_Man_t * pCopy, Abc_Obj_t * pRoot, Abc_Ntk_t * pNtk, unsigned uPhase, char * pPerm, int index, Cut_Cut_t * pCut, Vec_Ptr_t * vFaninsCur, int nNodesSaved, float oriDelay, int LevelMax, int * pGainBest, int fPlaceEnable );
static int Rwr_CutIsBoolean( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves );
static int Rwr_CutCountNumNodes( Abc_Obj_t * pObj, Cut_Cut_t * pCut );
static int Rwr_NodeGetDepth_rec( Abc_Obj_t * pObj, Vec_Ptr_t * vLeaves );

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
    if ( !Abc_FrameRedirectStdoutToLog( "rw_log.log", "a" ) )
        return;
    printf(info);
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
