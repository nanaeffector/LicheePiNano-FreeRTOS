// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __EFFECTDELAY_H__
#define __EFFECTDELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "fpn.h"
#include "effect.h"
#include "fxFilterBiQuad.h"
#include "fxDelayBuf.h"

typedef struct{
    int32_t onOff;
    int32_t delayTime;
    FPN effectLv;
    FPN feedback;
    FPN tone;
    int32_t type;
    fxDelayBufParam *delayBuf;
    fxFilterBiQuadParam *biQuadLPF;
    fxFilterBiQuadParam *biQuadHPF;
}EffectDelayParam;

Effect* createDelayEffect(void *stream);

#ifdef __cplusplus
}
#endif

#endif /* __EFFECTDELAY_H__ */
