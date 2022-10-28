#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "audiodata.h"
#include "audioFxTask.h"
#include "appConfig.h"
#include "effect.h"
#include "fxDelayBuf.h"
#include "dbgmsg.h"
#include "fx.h"
#include "fxFilterBiQuad.h"
#include "effectDelay.h"

#define MAX_DELAY_SMPL      (APP_SMPL_AT_MS * APP_DELAY_TIME_MAX_MS)

static void effectHandler(const void *pParamValue, const AUDIODATA_F *pInData, AUDIODATA_F *pOutData);
static void init(void *stParamValue);
static void finish(Effect *pEffect);

static void setOnOff(void *stParamValue, int32_t val);
static int32_t getOnOff(void *stParamValue);
static void setDelayTime(void *stParamValue, int32_t val);
static int32_t getDelayTime(void *stParamValue);
static void setEffectLv(void *stParamValue, int32_t val);
static int32_t getEffectLv(void *stParamValue);
static void setFeedback(void *stParamValue, int32_t val);
static int32_t getFeedback(void *stParamValue);
static void setTone(void *stParamValue, int32_t val);
static int32_t getTone(void *stParamValue);
static void setType(void *stParamValue, int32_t val);
static int32_t getType(void *stParamValue);

static const EffectParam paramList[] = {
    {"OnOff", 0, 1, 1, setOnOff, getOnOff},
    {"Delay Time", 15, MAX_DELAY_SMPL, APP_SMPL_AT_MS, setDelayTime, getDelayTime},
    {"Effect Lv", 0, 100, 1, setEffectLv, getEffectLv},
    {"Feedback", 0, 90, 1, setFeedback, getFeedback},
    {"Tone", 0, 100, 1, setTone, getTone},
    {"Effect Type", 0, 1, 1, setType, getType},
};

Effect* createDelayEffect(void *stream){
    // 領域確保.
    Effect *fx = (Effect*)malloc(sizeof(Effect));
    EffectDelayParam *pFxParam;

    // パラメータ領域確保.
    fx->stParamValue = (EffectDelayParam*)malloc(sizeof(EffectDelayParam));
    pFxParam = fx->stParamValue;

    // パラメータリスト設定.
    fx->paramList = paramList;
    fx->paramNum = PARAM_LIST_SIZE(paramList);
    
    // パラメータ設定.
    if(stream != 0){
        // パラメータコピー.
        memcpy(pFxParam, stream, sizeof(EffectDelayParam));
    }
    else{
        // 初期値投入.
        setOnOff(pFxParam, 0);
        setDelayTime(pFxParam, 500);
        setEffectLv(pFxParam, 50);
        setFeedback(pFxParam, 50);
        setTone(pFxParam, 100);
        setType(pFxParam, 0);
    }

    pFxParam->biQuadLPF = createFilterBiQuad(LPF, 0, 400, 7);   // fc = 4000hz, Q=1/√2.
    pFxParam->biQuadHPF = createFilterBiQuad(HPF, 0, 40, 7);    // fc = 400Hz, Q=1/√2.

    // ハンドラ設定.
    fx->init = init;
    fx->fxAtSample = effectHandler;
    fx->finish = finish;

    return fx;
}

static void effectHandler(const void *pParamValue, const AUDIODATA_F *pDryData, AUDIODATA_F *pWetData){
    AUDIODATA_F tmpWet1, tmpWet2;
    EffectDelayParam *pParam = (EffectDelayParam*)pParamValue;
    
    // 出力遅延データ読み込み.
    tmpWet1 = readDelayBuf(pParam->delayBuf, pParam->delayTime);
    
    // 遅延データフィルタ処理.
    tmpWet1.left = execFilterBiQuad(pParam->biQuadLPF, tmpWet1.left);
    tmpWet1.left = execFilterBiQuad(pParam->biQuadHPF, tmpWet1.left);

    // 遅延データ&原音をバッファに設定.
    tmpWet2.left = amp(tmpWet1.left, pParam->feedback);
    if(pParam->onOff){
        tmpWet2.left = mix(tmpWet2.left, pDryData->left);
    }
    tmpWet2.right = pDryData->right;
    writeDelayBuf(pParam->delayBuf, tmpWet2);

    // 原音+出力エフェクトレベル設定.
    tmpWet1.left = mix(pDryData->left, amp(tmpWet1.left, pParam->effectLv));
    tmpWet1.right = pDryData->right;

    // 出力に設定.
    *pWetData = tmpWet1;
}

// 起動時処理.
static void init(void *pParamValue){
    //EffectDelayParam *pFxParam = (EffectDelayParam*)pParamValue;
}

// 終了時処理.
static void finish(Effect *fx){
    EffectDelayParam *pFxParam = (EffectDelayParam*)fx->stParamValue;

    // delayバッファ解放.
    removeFxDelayBufParam(pFxParam->delayBuf);

    // フィルタ解放.
    removeFilterBiQuad(pFxParam->biQuadLPF);
    removeFilterBiQuad(pFxParam->biQuadHPF);

    // エフェクトパラメータ解放.
    free(pFxParam);

    // 本体解放.
    free(fx);
}

// ----------------------------------------------------------------------------
// 以下、パラメータ設定ハンドラ.
// ----------------------------------------------------------------------------

static void setOnOff(void *stParamValue, int32_t val){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    pFxParam->onOff = val;
}
static int32_t getOnOff(void *stParamValue){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    return pFxParam->onOff;
}

static void setDelayTime(void *stParamValue, int32_t val){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    pFxParam->delayTime = val;
}
static int32_t getDelayTime(void *stParamValue){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    return pFxParam->delayTime;
}

static void setEffectLv(void *stParamValue, int32_t val){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    pFxParam->effectLv = convertFromPercent(val);
}
static int32_t getEffectLv(void *stParamValue){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    return  convertToPercent(pFxParam->effectLv);
}

static void setFeedback(void *stParamValue, int32_t val){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    pFxParam->feedback = convertFromPercent(val);
}
static int32_t getFeedback(void *stParamValue){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    return convertToPercent(pFxParam->feedback);
}

static void setTone(void *stParamValue, int32_t val){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    pFxParam->tone = convertFromPercent(val);
}
static int32_t getTone(void *stParamValue){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    return convertToPercent(pFxParam->tone);
}

static void setType(void *stParamValue, int32_t val){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    pFxParam->type = val;
}
static int32_t getType(void *stParamValue){
    EffectDelayParam *pFxParam = (EffectDelayParam*)(stParamValue);
    return pFxParam->type;
}
