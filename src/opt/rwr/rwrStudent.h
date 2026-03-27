#ifndef ABC__opt__rwr__rwrStudent_h
#define ABC__opt__rwr__rwrStudent_h

#include "base/abc/abc.h"

ABC_NAMESPACE_HEADER_START

#define RWR_STUDENT_INPUT_DIM 19
#define RWR_STUDENT_GROUP_COUNT 3
#define RWR_STUDENT_LAYER_NORM_EPS 1.0e-5

typedef struct Rwr_StudentLinear_t_
{
    int nRows;
    int nCols;
    double * pWeight;
    double * pBias;
} Rwr_StudentLinear_t;

typedef struct Rwr_StudentLayerNorm_t_
{
    int nDim;
    double * pWeight;
    double * pBias;
} Rwr_StudentLayerNorm_t;

typedef struct Rwr_StudentResidualEncoder_t_
{
    Rwr_StudentLinear_t Main0;
    Rwr_StudentLayerNorm_t Norm;
    Rwr_StudentLinear_t Main1;
    Rwr_StudentLinear_t Skip;
} Rwr_StudentResidualEncoder_t;

typedef struct Rwr_StudentExpert_t_
{
    Rwr_StudentLinear_t Fc0;
    Rwr_StudentLinear_t Fc1;
} Rwr_StudentExpert_t;

typedef struct Rwr_StudentModel_t_
{
    int fLoaded;
    int format_version;
    int input_dim;
    int group_dims[RWR_STUDENT_GROUP_COUNT];
    int shared_hidden_dim;
    int regression_hidden_dim;
    int num_experts;
    double gate_temperature;
    double clamp_min;
    double clamp_max;
    double output_mu;
    double output_inv_sigma;
    char model_family[64];
    char hidden_activation[32];
    char group_fusion_policy[64];
    char expert_routing_policy[64];
    char output_denorm[128];
    double * pInputMu;
    double * pInputInvSigma;
    Rwr_StudentResidualEncoder_t Encoders[RWR_STUDENT_GROUP_COUNT];
    Rwr_StudentLinear_t GroupScore0;
    Rwr_StudentLinear_t GroupScore1;
    Rwr_StudentLayerNorm_t PostNorm;
    Rwr_StudentLinear_t ExpertGate;
    Rwr_StudentExpert_t * pExperts;
} Rwr_StudentModel_t;

extern int Rwr_StudentLoadModel( const char * pPath );
extern void Rwr_StudentFreeModel( void );
extern int Rwr_StudentHasModel( void );
extern const char * Rwr_StudentModelPath( void );
extern const char * Rwr_StudentDefaultModelPath( void );
extern double Rwr_StudentInferDelay( const double * pFeatures );

ABC_NAMESPACE_HEADER_END

#endif
