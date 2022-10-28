
#include <stdlib.h>
#include <string.h>
#include "fpn.h"
#include "fxDelayBuf.h"
#include "appConfig.h"


fxDelayBufParam* createFxDelayBufParam(uint32_t maxSamples){
    fxDelayBufParam *param = (fxDelayBufParam*)malloc(sizeof(fxDelayBufParam));
    param->buf = (AUDIODATA_F*)malloc(maxSamples*sizeof(AUDIODATA_F));
    memset(param->buf, 0, maxSamples*sizeof(AUDIODATA_F));
    param->bufLength = maxSamples;
    param->wp = 0;

    return param;
}

void removeFxDelayBufParam(fxDelayBufParam* param){
    if(param){
        free(param->buf);
        free(param);
    }
}

void writeDelayBuf(fxDelayBufParam* param, AUDIODATA_F data){
    param->buf[param->wp] = data;

    param->wp++;
    if(param->wp >= param->bufLength){
        param->wp = 0;
    }
}

AUDIODATA_F readDelayBuf(fxDelayBufParam* param, uint32_t delaySample){
    // 読み込み位置取得.
    int32_t rp = param->wp - delaySample;

    if(rp >= 0){
        return param->buf[rp];
    }
    else{
        rp = param->bufLength + rp;
    }
    
    return param->buf[rp];
}
