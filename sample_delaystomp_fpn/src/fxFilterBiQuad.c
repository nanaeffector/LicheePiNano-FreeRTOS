#include <stdlib.h>
#include <string.h>
#include "audiodata.h"
#include "appConfig.h"
#include "fxFilterBiQuad.h"

static inline void calcLPFCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha);
static inline void calcHPFCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha);
static inline void calcBPFCBWCoef(fxFilterBiQuadParam* pParam, FPN somg, FPN comg, FPN alpha);
static inline void calcBPFCQCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha);
static inline void calcNOTCHCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha);
static inline void calcPEQCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha, FPN a);
static inline void calcLSHCoef(fxFilterBiQuadParam* pParam, FPN somg, FPN comg, FPN alpha, FPN a, FPN beta);
static inline void calcHSHCoef(fxFilterBiQuadParam* pParam, FPN somg, FPN comg, FPN alpha, FPN a, FPN beta);
static inline void calcALLPASSCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha);

/* fc to sin(omega) table. */
// 0Hz - 9990Hz step:10Hz. ( 0 - 999)
extern const FPN fpnSinOmegaBQTable[1000];	
/* fc to cos(omega) table. */
// 0Hz - 9990Hz step:10Hz. ( 0 - 999)
extern const FPN fpnCOSOmegaBQTable[1000];
/* Q value table*/
// 0.1 - 9.9 (1 - 99)
extern const FPN fpnQValBQTable[99];
/* gain-db to A value table*/
// -128/40db - +128/40db. (-128 - +128)
extern const FPN fpnAValBQTable[257];
/* gain-db to beta value table*/
// -128/40db - +128/40db. (-128 - +128)
extern const FPN fpnBetaValBQTable[257];

fxFilterBiQuadParam* createFilterBiQuad(FilterType eType, int32_t gain, uint32_t fc_div10, uint32_t q_mul10){
    fxFilterBiQuadParam *pParam = (fxFilterBiQuadParam*)malloc(sizeof(fxFilterBiQuadParam));

    calcFilterCoef(pParam, eType, gain, fc_div10, q_mul10);
    return pParam;
}

void calcFilterCoef(fxFilterBiQuadParam *pParam, FilterType eType, int32_t gain, uint32_t fc_div10, uint32_t q_mul10){
    FPN sOmg = fpnSinOmegaBQTable[fc_div10];
    FPN cOmg = fpnCOSOmegaBQTable[fc_div10];
    FPN valQ = fpnQValBQTable[q_mul10];
    FPN valAlpha = divFPN(sOmg, mulFPN(fpnTwo, valQ));
    FPN valA, valBeta;
    
    pParam->x1.iVal = 0; pParam->x2.iVal = 0;
    pParam->y1.iVal = 0; pParam->y2.iVal = 0;

    switch(eType){
        case LPF:
            calcLPFCoef(pParam, cOmg, valAlpha);
            break;
        case HPF:
            calcHPFCoef(pParam, cOmg, valAlpha);
            break;
        case BPFCBW:
            calcBPFCBWCoef(pParam, sOmg, cOmg, valAlpha);
            break;
        case BPFCQ:
            calcBPFCQCoef(pParam, cOmg, valAlpha);
            break;
        case NOTCH:
            calcNOTCHCoef(pParam, cOmg, valAlpha);
            break;
        case PEQ:
            valA = fpnAValBQTable[gain];
            calcPEQCoef(pParam, cOmg, valAlpha, valA);
            break;
        case LSH:
            valA = fpnAValBQTable[gain];
            valBeta = fpnBetaValBQTable[gain];
            calcLSHCoef(pParam, sOmg, cOmg, valAlpha, valA, valBeta);
            break;
        case HSH:
            valA = fpnAValBQTable[gain];
            valBeta = fpnBetaValBQTable[gain];
            calcHSHCoef(pParam, sOmg, cOmg, valAlpha, valA, valBeta);
            break;
        case ALLPASS:
            calcALLPASSCoef(pParam, cOmg, valAlpha);
            break;
        default:
            pParam->k1 = fpnZero;
            pParam->k2 = fpnZero;
            pParam->k3 = fpnZero;
            pParam->k4 = fpnZero;
            pParam->k5 = fpnZero;
            break;
    }
}

void removeFilterBiQuad(fxFilterBiQuadParam *pParam){
    free(pParam);
}

// BiQuad フィルタ.
FPN execFilterBiQuad(fxFilterBiQuadParam *pParam, FPN inSample){
    // k1*in + k2*in1 + k3*in2 - k4*out1 - k5*out2
    FPN result = subFPN(subFPN(addFPN(addFPN(mulFPN(pParam->k1, inSample), mulFPN(pParam->k2, pParam->x1)), mulFPN(pParam->k3, pParam->x2)), mulFPN(pParam->k4, pParam->y1)), mulFPN(pParam->k5, pParam->y2));

    pParam->x2 = pParam->x1;
    pParam->x1 = inSample;

    pParam->y2 = pParam->y1;
    pParam->y1 = result;

    return result;
}

static inline void calcLPFCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha){
    FPN a1, a2, a3, b1, b2, b3;
    
    FPN tmp = subFPN(fpnOne, comg);
    
    b1 = divFPN(tmp, fpnTwo);
    b2 = tmp;
    b3 = b1;
    a1 = addFPN(fpnOne, alpha);
    a2 = mulFPN(fpnMTwo, comg);
    a3 = subFPN(fpnOne, alpha);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcHPFCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha){
    FPN a1, a2, a3, b1, b2, b3;
    
    FPN tmp = addFPN(fpnOne, comg);
    
    b1 = divFPN(tmp, fpnTwo);
    b2.iVal = -tmp.iVal;
    b3 = b1;
    a1 = addFPN(fpnOne, alpha);
    a2 = mulFPN(fpnMTwo, comg);
    a3 = subFPN(fpnOne, alpha);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcBPFCBWCoef(fxFilterBiQuadParam* pParam, FPN somg, FPN comg, FPN alpha){
    FPN a1, a2, a3, b1, b2, b3;
    
    b1 = divFPN(somg, fpnTwo);
    b2 = fpnZero;
    b3.iVal = -b1.iVal;
    a1 = addFPN(fpnOne, alpha);
    a2 = mulFPN(fpnMTwo, comg);
    a3 = subFPN(fpnOne, alpha);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcBPFCQCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha){
    FPN a1, a2, a3, b1, b2, b3;
    
    b1 = alpha;
    b2 = fpnZero;
    b3.iVal = -b1.iVal;
    a1 = addFPN(fpnOne, alpha);
    a2 = mulFPN(fpnMTwo, comg);
    a3 = subFPN(fpnOne, alpha);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcNOTCHCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha){
    FPN a1, a2, a3, b1, b2, b3;
    
    FPN tmp = mulFPN(fpnMTwo, comg);
    
    b1 = fpnOne;
    b2 = tmp;
    b3 = b1;
    a1 = addFPN(fpnOne, alpha);
    a2 = tmp;
    a3 = subFPN(fpnOne, alpha);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcPEQCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha, FPN a){
    FPN a1, a2, a3, b1, b2, b3;
    
    FPN tmpA = mulFPN(alpha, a);
    FPN tmp = mulFPN(fpnMTwo, comg);
    
    b1 = addFPN(fpnOne, tmpA);
    b2 = tmp;
    b3 = subFPN(fpnOne, tmpA);
    a1 = b1;
    a2 = b2;
    a3 = b3;
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcLSHCoef(fxFilterBiQuadParam* pParam, FPN somg, FPN comg, FPN alpha, FPN a, FPN beta){
    FPN a1, a2, a3, b1, b2, b3;
    
    // beta * sin(omg).
    FPN tmpB1 = mulFPN(beta, somg);
    // (A+1) - (A-1) * cos(omg).
    FPN tmpA1 = subFPN(addFPN(a, fpnOne), mulFPN(subFPN(a, fpnOne), comg));
    // (A-1) - (A+1) * cos(omg).
    FPN tmpA2 = subFPN(subFPN(a, fpnOne), mulFPN(addFPN(a, fpnOne), comg));
    // (A+1) + (A-1) * cos(omg).
    FPN tmpA3 = addFPN(addFPN(a, fpnOne), mulFPN(subFPN(a, fpnOne), comg));
    // (A-1) + (A+1) * cons(omg).
    FPN tmpA4 = addFPN(subFPN(a, fpnOne), mulFPN(addFPN(a, fpnOne), comg));

    b1 = mulFPN(a, addFPN(tmpA1, tmpB1));
    b2 = mulFPN(mulFPN(fpnTwo, a), tmpA2);
    b3 = mulFPN(a, subFPN(tmpA1, tmpB1));
    a1 = addFPN(tmpA3, tmpB1);
    a2 = mulFPN(fpnMTwo, tmpA4);
    a3 = subFPN(tmpA3, tmpB1);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcHSHCoef(fxFilterBiQuadParam* pParam, FPN somg, FPN comg, FPN alpha, FPN a, FPN beta){
    FPN a1, a2, a3, b1, b2, b3;
    
    // beta * sin(omg).
    FPN tmpB1 = mulFPN(beta, somg);
    // (A+1) - (A-1) * cos(omg).
    FPN tmpA1 = addFPN(addFPN(a, fpnOne), mulFPN(subFPN(a, fpnOne), comg));
    // (A-1) - (A+1) * cos(omg).
    FPN tmpA2 = addFPN(subFPN(a, fpnOne), mulFPN(addFPN(a, fpnOne), comg));
    // (A+1) + (A-1) * cos(omg).
    FPN tmpA3 = subFPN(addFPN(a, fpnOne), mulFPN(subFPN(a, fpnOne), comg));
    // (A-1) + (A+1) * cons(omg).
    FPN tmpA4 = subFPN(subFPN(a, fpnOne), mulFPN(addFPN(a, fpnOne), comg));

    b1 = mulFPN(a, addFPN(tmpA1, tmpB1));
    b2 = mulFPN(mulFPN(fpnMTwo, a), tmpA2);
    b3 = mulFPN(a, subFPN(tmpA1, tmpB1));
    a1 = addFPN(tmpA3, tmpB1);
    a2 = mulFPN(fpnTwo, tmpA4);
    a3 = subFPN(tmpA3, tmpB1);
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}

static inline void calcALLPASSCoef(fxFilterBiQuadParam* pParam, FPN comg, FPN alpha){
    FPN a1, a2, a3, b1, b2, b3;
    
    FPN tmp = subFPN(fpnOne, comg);
    
    b1 = subFPN(fpnOne, alpha);
    b2 = tmp;
    b3 = addFPN(fpnOne, alpha);
    a1 = b3;
    a2 = tmp;
    a3 = b1;
    
    pParam->k1 = divFPN(b1, a1);
    pParam->k2 = divFPN(b2, a1);
    pParam->k3 = divFPN(b3, a1);
    pParam->k4 = divFPN(a2, a1);
    pParam->k5 = divFPN(a3, a1);
}
