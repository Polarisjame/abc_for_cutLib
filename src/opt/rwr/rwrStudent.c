#include "rwrStudent.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

ABC_NAMESPACE_IMPL_START

static const char * s_RwrStudentDefaultModelPath = "checkpoints/extract_cutlib_student_group_fusion/abc_student_model2.txt";
static Rwr_StudentModel_t s_RwrStudentModel;
static char s_RwrStudentModelPath[1024];

static void Rwr_StudentLinearFree( Rwr_StudentLinear_t * pLinear )
{
    ABC_FREE( pLinear->pWeight );
    ABC_FREE( pLinear->pBias );
    memset( pLinear, 0, sizeof(Rwr_StudentLinear_t) );
}

static void Rwr_StudentLayerNormFree( Rwr_StudentLayerNorm_t * pNorm )
{
    ABC_FREE( pNorm->pWeight );
    ABC_FREE( pNorm->pBias );
    memset( pNorm, 0, sizeof(Rwr_StudentLayerNorm_t) );
}

static void Rwr_StudentEncoderFree( Rwr_StudentResidualEncoder_t * pEncoder )
{
    Rwr_StudentLinearFree( &pEncoder->Main0 );
    Rwr_StudentLayerNormFree( &pEncoder->Norm );
    Rwr_StudentLinearFree( &pEncoder->Main1 );
    Rwr_StudentLinearFree( &pEncoder->Skip );
}

static void Rwr_StudentExpertFree( Rwr_StudentExpert_t * pExpert )
{
    Rwr_StudentLinearFree( &pExpert->Fc0 );
    Rwr_StudentLinearFree( &pExpert->Fc1 );
}

static void Rwr_StudentModelReset( Rwr_StudentModel_t * pModel )
{
    int i;
    if ( pModel == NULL )
        return;
    ABC_FREE( pModel->pInputMu );
    ABC_FREE( pModel->pInputInvSigma );
    for ( i = 0; i < RWR_STUDENT_GROUP_COUNT; ++i )
        Rwr_StudentEncoderFree( &pModel->Encoders[i] );
    Rwr_StudentLinearFree( &pModel->GroupScore0 );
    Rwr_StudentLinearFree( &pModel->GroupScore1 );
    Rwr_StudentLayerNormFree( &pModel->PostNorm );
    Rwr_StudentLinearFree( &pModel->ExpertGate );
    if ( pModel->pExperts )
    {
        for ( i = 0; i < pModel->num_experts; ++i )
            Rwr_StudentExpertFree( &pModel->pExperts[i] );
    }
    ABC_FREE( pModel->pExperts );
    memset( pModel, 0, sizeof(Rwr_StudentModel_t) );
}

void Rwr_StudentFreeModel( void )
{
    Rwr_StudentModelReset( &s_RwrStudentModel );
    s_RwrStudentModelPath[0] = '\0';
}

int Rwr_StudentHasModel( void )
{
    return s_RwrStudentModel.fLoaded;
}

const char * Rwr_StudentModelPath( void )
{
    return s_RwrStudentModelPath[0] ? s_RwrStudentModelPath : NULL;
}

const char * Rwr_StudentDefaultModelPath( void )
{
    return s_RwrStudentDefaultModelPath;
}

static char * Rwr_StudentTrim( char * pStr )
{
    char * pEnd;
    while ( *pStr && isspace((unsigned char)*pStr) )
        ++pStr;
    pEnd = pStr + strlen(pStr);
    while ( pEnd > pStr && isspace((unsigned char)pEnd[-1]) )
        *--pEnd = '\0';
    return pStr;
}

static int Rwr_StudentParseDoubleArray( const char * pValue, double ** ppValues, int * pnValues )
{
    int nCap, nSize;
    double * pNums;
    const char * p;
    char * pEnd;
    nCap = 16;
    nSize = 0;
    pNums = ABC_ALLOC( double, nCap );
    for ( p = pValue; *p; )
    {
        while ( *p && !(isdigit((unsigned char)*p) || *p == '-' || *p == '+' || *p == '.') )
            ++p;
        if ( !*p )
            break;
        if ( nSize == nCap )
        {
            double * pNew;
            nCap *= 2;
            pNew = ABC_ALLOC( double, nCap );
            memcpy( pNew, pNums, sizeof(double) * nSize );
            ABC_FREE( pNums );
            pNums = pNew;
        }
        pNums[nSize] = strtod( p, &pEnd );
        if ( p == pEnd )
        {
            ABC_FREE( pNums );
            return 0;
        }
        ++nSize;
        p = pEnd;
    }
    *ppValues = pNums;
    *pnValues = nSize;
    return 1;
}

static int Rwr_StudentAssignNumbers( const char * pKey, const char * pValue, double ** ppDest, int nExpected )
{
    double * pNums;
    int nNums;
    if ( !Rwr_StudentParseDoubleArray( pValue, &pNums, &nNums ) )
        return 0;
    if ( nNums != nExpected )
    {
        Abc_Print( -1, "Student model parse error: key %s expected %d numbers, got %d.\n", pKey, nExpected, nNums );
        ABC_FREE( pNums );
        return 0;
    }
    ABC_FREE( *ppDest );
    *ppDest = pNums;
    return 1;
}

static int Rwr_StudentAssignLinear( const char * pKey, const char * pValue, Rwr_StudentLinear_t * pLinear, int nRows, int nCols, int fWeight )
{
    pLinear->nRows = nRows;
    pLinear->nCols = nCols;
    return Rwr_StudentAssignNumbers( pKey, pValue, fWeight ? &pLinear->pWeight : &pLinear->pBias, fWeight ? nRows * nCols : nRows );
}

static int Rwr_StudentAssignNorm( const char * pKey, const char * pValue, Rwr_StudentLayerNorm_t * pNorm, int nDim, int fWeight )
{
    pNorm->nDim = nDim;
    return Rwr_StudentAssignNumbers( pKey, pValue, fWeight ? &pNorm->pWeight : &pNorm->pBias, nDim );
}

static int Rwr_StudentAssignEncoder( Rwr_StudentModel_t * pModel, int iGroup, const char * pSuffix, const char * pKey, const char * pValue )
{
    Rwr_StudentResidualEncoder_t * pEncoder;
    int nGroupDim;
    pEncoder = &pModel->Encoders[iGroup];
    nGroupDim = pModel->group_dims[iGroup];
    if ( strcmp( pSuffix, "main.0.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pEncoder->Main0, pModel->shared_hidden_dim, nGroupDim, 1 );
    if ( strcmp( pSuffix, "main.0.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pEncoder->Main0, pModel->shared_hidden_dim, nGroupDim, 0 );
    if ( strcmp( pSuffix, "main.1.weight" ) == 0 )
        return Rwr_StudentAssignNorm( pKey, pValue, &pEncoder->Norm, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pSuffix, "main.1.bias" ) == 0 )
        return Rwr_StudentAssignNorm( pKey, pValue, &pEncoder->Norm, pModel->shared_hidden_dim, 0 );
    if ( strcmp( pSuffix, "main.3.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pEncoder->Main1, pModel->shared_hidden_dim, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pSuffix, "main.3.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pEncoder->Main1, pModel->shared_hidden_dim, pModel->shared_hidden_dim, 0 );
    if ( strcmp( pSuffix, "skip.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pEncoder->Skip, pModel->shared_hidden_dim, nGroupDim, 1 );
    if ( strcmp( pSuffix, "skip.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pEncoder->Skip, pModel->shared_hidden_dim, nGroupDim, 0 );
    Abc_Print( -1, "Student model parse error: unsupported encoder key %s.\n", pKey );
    return 0;
}

static int Rwr_StudentAssignExpert( Rwr_StudentModel_t * pModel, int iExpert, const char * pSuffix, const char * pKey, const char * pValue )
{
    Rwr_StudentExpert_t * pExpert;
    pExpert = &pModel->pExperts[iExpert];
    if ( strcmp( pSuffix, "0.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pExpert->Fc0, pModel->regression_hidden_dim, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pSuffix, "0.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pExpert->Fc0, pModel->regression_hidden_dim, pModel->shared_hidden_dim, 0 );
    if ( strcmp( pSuffix, "2.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pExpert->Fc1, 1, pModel->regression_hidden_dim, 1 );
    if ( strcmp( pSuffix, "2.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pExpert->Fc1, 1, pModel->regression_hidden_dim, 0 );
    Abc_Print( -1, "Student model parse error: unsupported expert key %s.\n", pKey );
    return 0;
}

static int Rwr_StudentAssignState( Rwr_StudentModel_t * pModel, const char * pKey, const char * pValue )
{
    if ( strcmp( pKey, "state_dict.input_mu" ) == 0 || strcmp( pKey, "state_dict.input_inv_sigma" ) == 0 )
        return 1;
    if ( strcmp( pKey, "state_dict.group_attention.score.0.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pModel->GroupScore0, pModel->shared_hidden_dim, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pKey, "state_dict.group_attention.score.0.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pModel->GroupScore0, pModel->shared_hidden_dim, pModel->shared_hidden_dim, 0 );
    if ( strcmp( pKey, "state_dict.group_attention.score.2.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pModel->GroupScore1, 1, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pKey, "state_dict.group_attention.score.2.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pModel->GroupScore1, 1, pModel->shared_hidden_dim, 0 );
    if ( strcmp( pKey, "state_dict.post_fusion.0.weight" ) == 0 )
        return Rwr_StudentAssignNorm( pKey, pValue, &pModel->PostNorm, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pKey, "state_dict.post_fusion.0.bias" ) == 0 )
        return Rwr_StudentAssignNorm( pKey, pValue, &pModel->PostNorm, pModel->shared_hidden_dim, 0 );
    if ( strcmp( pKey, "state_dict.expert_gate.weight" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pModel->ExpertGate, pModel->num_experts, pModel->shared_hidden_dim, 1 );
    if ( strcmp( pKey, "state_dict.expert_gate.bias" ) == 0 )
        return Rwr_StudentAssignLinear( pKey, pValue, &pModel->ExpertGate, pModel->num_experts, pModel->shared_hidden_dim, 0 );
    if ( strncmp( pKey, "state_dict.group1_encoder.", 26 ) == 0 )
        return Rwr_StudentAssignEncoder( pModel, 0, pKey + 26, pKey, pValue );
    if ( strncmp( pKey, "state_dict.group2_encoder.", 26 ) == 0 )
        return Rwr_StudentAssignEncoder( pModel, 1, pKey + 26, pKey, pValue );
    if ( strncmp( pKey, "state_dict.group3_encoder.", 26 ) == 0 )
        return Rwr_StudentAssignEncoder( pModel, 2, pKey + 26, pKey, pValue );
    if ( strncmp( pKey, "state_dict.experts.", 19 ) == 0 )
    {
        int iExpert;
        const char * pSuffix;
        iExpert = pKey[19] - '0';
        pSuffix = pKey + 21;
        if ( iExpert < 0 || iExpert >= pModel->num_experts )
        {
            Abc_Print( -1, "Student model parse error: bad expert index in key %s.\n", pKey );
            return 0;
        }
        return Rwr_StudentAssignExpert( pModel, iExpert, pSuffix, pKey, pValue );
    }
    if ( strncmp( pKey, "state_dict.align_head.", 22 ) == 0 || strncmp( pKey, "state_dict.teacher_align_adapter.", 32 ) == 0 )
        return 1;
    Abc_Print( -1, "Student model parse error: unsupported state key %s.\n", pKey );
    return 0;
}

static int Rwr_StudentParseTopLevel( Rwr_StudentModel_t * pModel, const char * pKey, const char * pValue )
{
    if ( strcmp( pKey, "format_version" ) == 0 ) pModel->format_version = atoi( pValue );
    else if ( strcmp( pKey, "input_dim" ) == 0 ) pModel->input_dim = atoi( pValue );
    else if ( strcmp( pKey, "shared_hidden_dim" ) == 0 ) pModel->shared_hidden_dim = atoi( pValue );
    else if ( strcmp( pKey, "num_experts" ) == 0 ) pModel->num_experts = atoi( pValue );
    else if ( strcmp( pKey, "gate_temperature" ) == 0 ) pModel->gate_temperature = atof( pValue );
    else if ( strcmp( pKey, "clamp_min" ) == 0 ) pModel->clamp_min = atof( pValue );
    else if ( strcmp( pKey, "clamp_max" ) == 0 ) pModel->clamp_max = atof( pValue );
    else if ( strcmp( pKey, "output_mu" ) == 0 ) pModel->output_mu = atof( pValue );
    else if ( strcmp( pKey, "output_inv_sigma" ) == 0 ) pModel->output_inv_sigma = atof( pValue );
    else if ( strcmp( pKey, "model_family" ) == 0 ) snprintf( pModel->model_family, sizeof(pModel->model_family), "%s", pValue );
    else if ( strcmp( pKey, "hidden_activation" ) == 0 ) snprintf( pModel->hidden_activation, sizeof(pModel->hidden_activation), "%s", pValue );
    else if ( strcmp( pKey, "group_fusion_policy" ) == 0 ) snprintf( pModel->group_fusion_policy, sizeof(pModel->group_fusion_policy), "%s", pValue );
    else if ( strcmp( pKey, "expert_routing_policy" ) == 0 ) snprintf( pModel->expert_routing_policy, sizeof(pModel->expert_routing_policy), "%s", pValue );
    else if ( strcmp( pKey, "output_denorm" ) == 0 ) snprintf( pModel->output_denorm, sizeof(pModel->output_denorm), "%s", pValue );
    else return -1;
    return 1;
}

static int Rwr_StudentFinalizeMetadata( Rwr_StudentModel_t * pModel, double * pGroupDims, int nGroupDims )
{
    int i;
    if ( pModel->format_version != 2 )
    {
        Abc_Print( -1, "Student model load failed: unsupported format_version=%d.\n", pModel->format_version );
        return 0;
    }
    if ( strcmp( pModel->model_family, "student_group_fusion_soft_moe" ) != 0 )
    {
        Abc_Print( -1, "Student model load failed: unsupported model_family=%s.\n", pModel->model_family );
        return 0;
    }
    if ( pModel->input_dim != RWR_STUDENT_INPUT_DIM )
    {
        Abc_Print( -1, "Student model load failed: expected input_dim=%d, got %d.\n", RWR_STUDENT_INPUT_DIM, pModel->input_dim );
        return 0;
    }
    if ( nGroupDims != RWR_STUDENT_GROUP_COUNT )
    {
        Abc_Print( -1, "Student model load failed: expected %d group dims, got %d.\n", RWR_STUDENT_GROUP_COUNT, nGroupDims );
        return 0;
    }
    for ( i = 0; i < RWR_STUDENT_GROUP_COUNT; ++i )
        pModel->group_dims[i] = (int)pGroupDims[i];
    if ( pModel->num_experts <= 0 || pModel->shared_hidden_dim <= 0 )
    {
        Abc_Print( -1, "Student model load failed: invalid hidden/expert dimensions.\n" );
        return 0;
    }
    pModel->regression_hidden_dim = Abc_MaxInt( 8, pModel->shared_hidden_dim / 2 );
    pModel->pExperts = ABC_CALLOC( Rwr_StudentExpert_t, pModel->num_experts );
    return pModel->pExperts != NULL;
}

static int Rwr_StudentCheckLinear( const char * pName, Rwr_StudentLinear_t * pLinear )
{
    if ( pLinear->nRows <= 0 || pLinear->nCols <= 0 || pLinear->pWeight == NULL || pLinear->pBias == NULL )
    {
        Abc_Print( -1, "Student model load failed: missing linear layer %s.\n", pName );
        return 0;
    }
    return 1;
}

static int Rwr_StudentCheckNorm( const char * pName, Rwr_StudentLayerNorm_t * pNorm )
{
    if ( pNorm->nDim <= 0 || pNorm->pWeight == NULL || pNorm->pBias == NULL )
    {
        Abc_Print( -1, "Student model load failed: missing layer norm %s.\n", pName );
        return 0;
    }
    return 1;
}

static int Rwr_StudentValidateModel( Rwr_StudentModel_t * pModel )
{
    int i;
    if ( pModel->pInputMu == NULL || pModel->pInputInvSigma == NULL )
    {
        Abc_Print( -1, "Student model load failed: missing input normalization stats.\n" );
        return 0;
    }
    for ( i = 0; i < RWR_STUDENT_GROUP_COUNT; ++i )
    {
        if ( !Rwr_StudentCheckLinear( "encoder.main0", &pModel->Encoders[i].Main0 ) ) return 0;
        if ( !Rwr_StudentCheckNorm( "encoder.norm", &pModel->Encoders[i].Norm ) ) return 0;
        if ( !Rwr_StudentCheckLinear( "encoder.main1", &pModel->Encoders[i].Main1 ) ) return 0;
        if ( !Rwr_StudentCheckLinear( "encoder.skip", &pModel->Encoders[i].Skip ) ) return 0;
    }
    if ( !Rwr_StudentCheckLinear( "group_score0", &pModel->GroupScore0 ) ) return 0;
    if ( !Rwr_StudentCheckLinear( "group_score1", &pModel->GroupScore1 ) ) return 0;
    if ( !Rwr_StudentCheckNorm( "post_norm", &pModel->PostNorm ) ) return 0;
    if ( !Rwr_StudentCheckLinear( "expert_gate", &pModel->ExpertGate ) ) return 0;
    if ( pModel->regression_hidden_dim <= 0 )
    {
        Abc_Print( -1, "Student model load failed: expert hidden dim not inferred.\n" );
        return 0;
    }
    for ( i = 0; i < pModel->num_experts; ++i )
    {
        if ( !Rwr_StudentCheckLinear( "expert.fc0", &pModel->pExperts[i].Fc0 ) ) return 0;
        if ( !Rwr_StudentCheckLinear( "expert.fc1", &pModel->pExperts[i].Fc1 ) ) return 0;
    }
    return 1;
}

int Rwr_StudentLoadModel( const char * pPath )
{
    FILE * pFile;
    char * pLine;
    size_t nLineCap;
    ssize_t nRead;
    Rwr_StudentModel_t Model;
    double * pGroupDims;
    int nGroupDims;
    pFile = fopen( pPath, "r" );
    if ( pFile == NULL )
    {
        Abc_Print( -1, "Student model load failed: cannot open %s.\n", pPath );
        return 0;
    }
    memset( &Model, 0, sizeof(Model) );
    pLine = NULL;
    nLineCap = 0;
    pGroupDims = NULL;
    nGroupDims = 0;
    while ( (nRead = getline( &pLine, &nLineCap, pFile )) != -1 )
    {
        char * pKey;
        char * pValue;
        char * pEq;
        if ( nRead == 0 )
            continue;
        pKey = Rwr_StudentTrim( pLine );
        if ( pKey[0] == '\0' )
            continue;
        pEq = strchr( pKey, '=' );
        if ( pEq == NULL )
        {
            Abc_Print( -1, "Student model load failed: malformed line in %s.\n", pPath );
            goto fail;
        }
        *pEq = '\0';
        pValue = Rwr_StudentTrim( pEq + 1 );
        if ( strcmp( pKey, "group_dims" ) == 0 )
        {
            if ( !Rwr_StudentParseDoubleArray( pValue, &pGroupDims, &nGroupDims ) )
                goto fail;
            continue;
        }
        if ( strcmp( pKey, "input_mu" ) == 0 )
        {
            if ( !Rwr_StudentAssignNumbers( pKey, pValue, &Model.pInputMu, RWR_STUDENT_INPUT_DIM ) )
                goto fail;
            continue;
        }
        if ( strcmp( pKey, "input_inv_sigma" ) == 0 )
        {
            if ( !Rwr_StudentAssignNumbers( pKey, pValue, &Model.pInputInvSigma, RWR_STUDENT_INPUT_DIM ) )
                goto fail;
            continue;
        }
        if ( strncmp( pKey, "state_dict.", 11 ) == 0 )
        {
            if ( Model.pExperts == NULL )
            {
                if ( !Rwr_StudentFinalizeMetadata( &Model, pGroupDims, nGroupDims ) )
                    goto fail;
            }
            if ( !Rwr_StudentAssignState( &Model, pKey, pValue ) )
                goto fail;
            continue;
        }
        if ( Rwr_StudentParseTopLevel( &Model, pKey, pValue ) == -1 )
        {
            Abc_Print( -1, "Student model load failed: unsupported top-level key %s.\n", pKey );
            goto fail;
        }
    }
    if ( Model.pExperts == NULL )
    {
        if ( !Rwr_StudentFinalizeMetadata( &Model, pGroupDims, nGroupDims ) )
            goto fail;
    }
    if ( !Rwr_StudentValidateModel( &Model ) )
        goto fail;
    Model.fLoaded = 1;
    Rwr_StudentFreeModel();
    s_RwrStudentModel = Model;
    snprintf( s_RwrStudentModelPath, sizeof(s_RwrStudentModelPath), "%s", pPath );
    ABC_FREE( pGroupDims );
    if ( pLine ) free( pLine );
    fclose( pFile );
    return 1;
fail:
    ABC_FREE( pGroupDims );
    if ( pLine ) free( pLine );
    fclose( pFile );
    Rwr_StudentModelReset( &Model );
    return 0;
}

static void Rwr_StudentLinearEval( Rwr_StudentLinear_t * pLinear, const double * pIn, double * pOut )
{
    int i, k;
    for ( i = 0; i < pLinear->nRows; ++i )
    {
        double Sum = pLinear->pBias[i];
        const double * pRow = pLinear->pWeight + i * pLinear->nCols;
        for ( k = 0; k < pLinear->nCols; ++k )
            Sum += pRow[k] * pIn[k];
        pOut[i] = Sum;
    }
}

static void Rwr_StudentLayerNormEval( Rwr_StudentLayerNorm_t * pNorm, const double * pIn, double * pOut )
{
    int i;
    double Mean = 0.0, Var = 0.0, InvStd;
    for ( i = 0; i < pNorm->nDim; ++i )
        Mean += pIn[i];
    Mean /= (double)pNorm->nDim;
    for ( i = 0; i < pNorm->nDim; ++i )
    {
        double Diff = pIn[i] - Mean;
        Var += Diff * Diff;
    }
    Var /= (double)pNorm->nDim;
    InvStd = 1.0 / sqrt( Var + RWR_STUDENT_LAYER_NORM_EPS );
    for ( i = 0; i < pNorm->nDim; ++i )
        pOut[i] = ((pIn[i] - Mean) * InvStd) * pNorm->pWeight[i] + pNorm->pBias[i];
}

static void Rwr_StudentReluInPlace( double * pData, int nSize )
{
    int i;
    for ( i = 0; i < nSize; ++i )
        if ( pData[i] < 0.0 )
            pData[i] = 0.0;
}

static void Rwr_StudentSoftmaxInPlace( double * pData, int nSize, double Temperature )
{
    int i;
    double Max = pData[0], Sum = 0.0;
    for ( i = 1; i < nSize; ++i )
        if ( pData[i] > Max )
            Max = pData[i];
    for ( i = 0; i < nSize; ++i )
    {
        pData[i] = exp( (pData[i] - Max) / Temperature );
        Sum += pData[i];
    }
    if ( Sum <= 0.0 )
        Sum = 1.0;
    for ( i = 0; i < nSize; ++i )
        pData[i] /= Sum;
}

static void Rwr_StudentResidualEncoderEval( Rwr_StudentResidualEncoder_t * pEncoder, const double * pIn, double * pOut, double * pTemp0, double * pTemp1 )
{
    int i;
    Rwr_StudentLinearEval( &pEncoder->Main0, pIn, pTemp0 );
    Rwr_StudentLayerNormEval( &pEncoder->Norm, pTemp0, pTemp1 );
    Rwr_StudentReluInPlace( pTemp1, pEncoder->Norm.nDim );
    Rwr_StudentLinearEval( &pEncoder->Main1, pTemp1, pOut );
    Rwr_StudentLinearEval( &pEncoder->Skip, pIn, pTemp0 );
    for ( i = 0; i < pEncoder->Main1.nRows; ++i )
        pOut[i] += pTemp0[i];
}

double Rwr_StudentInferDelay( const double * pFeatures )
{
    Rwr_StudentModel_t * pModel;
    double x[RWR_STUDENT_INPUT_DIM];
    double g0[128], g1[128], g2[128], tmp0[128], tmp1[128], fused[128], post[128];
    double group_logits[RWR_STUDENT_GROUP_COUNT], expert_logits[32], expert_hidden[128], expert_outputs[32];
    const double * pGroups[RWR_STUDENT_GROUP_COUNT];
    double * pGroupVecs[RWR_STUDENT_GROUP_COUNT];
    int i, j, nHid;
    pModel = &s_RwrStudentModel;
    if ( !pModel->fLoaded )
    {
        Abc_Print( -1, "Student inference requested before model load.\n" );
        return 1.0e100;
    }
    nHid = pModel->shared_hidden_dim;
    pGroups[0] = x;
    pGroups[1] = x + pModel->group_dims[0];
    pGroups[2] = x + pModel->group_dims[0] + pModel->group_dims[1];
    pGroupVecs[0] = g0;
    pGroupVecs[1] = g1;
    pGroupVecs[2] = g2;
    for ( i = 0; i < RWR_STUDENT_INPUT_DIM; ++i )
        x[i] = (pFeatures[i] - pModel->pInputMu[i]) * pModel->pInputInvSigma[i];
    for ( i = 0; i < RWR_STUDENT_GROUP_COUNT; ++i )
        Rwr_StudentResidualEncoderEval( &pModel->Encoders[i], pGroups[i], pGroupVecs[i], tmp0, tmp1 );
    for ( i = 0; i < RWR_STUDENT_GROUP_COUNT; ++i )
    {
        Rwr_StudentLinearEval( &pModel->GroupScore0, pGroupVecs[i], tmp0 );
        for ( j = 0; j < nHid; ++j )
            tmp0[j] = tanh( tmp0[j] );
        Rwr_StudentLinearEval( &pModel->GroupScore1, tmp0, tmp1 );
        group_logits[i] = tmp1[0];
    }
    Rwr_StudentSoftmaxInPlace( group_logits, RWR_STUDENT_GROUP_COUNT, pModel->gate_temperature );
    for ( j = 0; j < nHid; ++j )
    {
        fused[j] = 0.0;
        for ( i = 0; i < RWR_STUDENT_GROUP_COUNT; ++i )
            fused[j] += group_logits[i] * pGroupVecs[i][j];
    }
    Rwr_StudentLayerNormEval( &pModel->PostNorm, fused, post );
    Rwr_StudentReluInPlace( post, nHid );
    Rwr_StudentLinearEval( &pModel->ExpertGate, post, expert_logits );
    Rwr_StudentSoftmaxInPlace( expert_logits, pModel->num_experts, pModel->gate_temperature );
    for ( i = 0; i < pModel->num_experts; ++i )
    {
        Rwr_StudentLinearEval( &pModel->pExperts[i].Fc0, post, expert_hidden );
        Rwr_StudentReluInPlace( expert_hidden, pModel->regression_hidden_dim );
        Rwr_StudentLinearEval( &pModel->pExperts[i].Fc1, expert_hidden, tmp0 );
        expert_outputs[i] = tmp0[0];
    }
    {
        double yScaled = 0.0, yLog;
        for ( i = 0; i < pModel->num_experts; ++i )
            yScaled += expert_logits[i] * expert_outputs[i];
        yLog = yScaled / pModel->output_inv_sigma + pModel->output_mu;
        if ( yLog < pModel->clamp_min ) yLog = pModel->clamp_min;
        if ( yLog > pModel->clamp_max ) yLog = pModel->clamp_max;
        return exp(yLog) - 1.0;
    }
}

ABC_NAMESPACE_IMPL_END
