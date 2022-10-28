// author : Nana's Effector.
// date   : 2021/09/22

#ifndef __FX_H__
#define __FX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "appConfig.h"
#include "fpn.h"

typedef enum{
    fxMIX = 0,
    fxAMP,
    fxDELAY,
    fxLERP,
    fxOSCSIN,   // サイン波.
    fxOSCTRI,   // 三角波
    fxOSCSAW,   // 鋸波.
    fxOSCSQU,   // 矩形波.
}fxType;

// 線形補完.
// t = 0.0 - 1.0
static inline FPN lerp(FPN val1, FPN val2, FPN t){
    return addFPN(mulFPN(subFPN(fpnOne , t), val1) , mulFPN(t, val2));
}

// 加算(配列).
static inline FPN mixList(FPN *vals, uint32_t length){
    int i;
    FPN ret = vals[0];

    for(i=1; i<length; i++){
        ret = addFPN(ret, vals[i]);
    }

    return ret;
}

// 加算.
static inline FPN mix(FPN val1, FPN val2){
    return addFPN(val1, val2);
}

// 乗算.
static inline FPN amp(FPN val1, FPN val2){
    return mulFPN(val1, val2);
}

static inline FPN calcOSCSaw(FPN oldCount, FPN freq){
    const FPN fpnSample = {{1/APP_AUDIO_SMPL}};
    FPN o; o.iVal = oldCount.iVal + mulFPN(freq, fpnSample).iVal;
    if(o.iVal > fpnSample.iVal){
        o = fpnMOne;
    }
    return o;
}

static inline FPN calcOSCSin(FPN oldCount, FPN freq){
    return mulFPN(mulFPN(fpnTwo, fpnPI), calcOSCSaw(oldCount, freq));
}

static inline FPN calcOSCTri(FPN oldCount, FPN freq){
    FPN o = mulFPN(fpnTwo, calcOSCSaw(oldCount, freq));
    if(o.iVal < fpnOne.iVal){
        return o;
    }
    else{
        return subFPN(o, fpnTwo);
    }
}

static inline FPN calcOSCSqr(FPN oldCount, FPN freq){
    FPN o = mulFPN(fpnTwo, calcOSCSaw(oldCount, freq));
    if(o.iVal < fpnOne.iVal){
        return fpnMOne;
    }
    else{
        return fpnOne;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __FX_H__ */