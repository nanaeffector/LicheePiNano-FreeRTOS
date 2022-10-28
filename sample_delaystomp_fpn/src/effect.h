// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __EFFECT_H__
#define __EFFECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "audiodata.h"
#include "fpn.h"

#define PARAM_LIST_SIZE(p)     (sizeof(p)/sizeof(EffectParam))

typedef struct{
    char name[16];  // 有効文字数15文字.
    int32_t minVal;
    int32_t maxVal;
    int32_t digit;
    void (*set)(void *stParamValue, int32_t val);
    int32_t (*get)(void *stParamValue);
}EffectParam;

typedef struct stEffect{
    uint32_t paramNum;
    const EffectParam *paramList;
    void *stParamValue;
    void (*init)(void *pParamValue);
    void (*fxAtSample)(const void *pParamValue, const AUDIODATA_F *pDryData, AUDIODATA_F *pWetData);
    void (*finish)(struct stEffect *pEffect);
}Effect;

void initEffect();
void runEffect(const AUDIODATA_F *pDryData, AUDIODATA_F *pWetData, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __EFFECT_H__ */
