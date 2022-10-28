// author : Nana's Effector.
// date   : 2021/09/22

#ifndef __FXFILTERBIQUAD_H__
#define __FXFILTERBIQUAD_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "fpn.h"

typedef struct{
    FPN x1, x2;     // delayed input.
    FPN y1, y2;     // delayed output.
    FPN k1, k2, k3, k4, k5;     // coefficients.
}fxFilterBiQuadParam;

typedef enum{
    LPF = 0,
    HPF,
    BPFCBW,     // 帯域幅一定.
    BPFCQ,      // Q一定.
    NOTCH,
    PEQ,
    LSH,
    HSH,
    ALLPASS
}FilterType;

fxFilterBiQuadParam* createFilterBiQuad(FilterType eType, int32_t gain, uint32_t fc_div10, uint32_t q_mul10);
void calcFilterCoef(fxFilterBiQuadParam *pParam, FilterType eType, int32_t gain, uint32_t fc_div10, uint32_t q_mul10);
void removeFilterBiQuad(fxFilterBiQuadParam *pParam);
FPN execFilterBiQuad(fxFilterBiQuadParam *pParam, FPN inSample);

#ifdef __cplusplus
}
#endif

#endif /* __FXFILTERBIQUAD_H__ */
