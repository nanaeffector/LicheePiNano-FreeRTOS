// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __FXDELAYBUF_H__
#define __FXDELAYBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "audiodata.h"

typedef struct{
    AUDIODATA_F *buf;
    uint32_t bufLength;
    int32_t wp;
}fxDelayBufParam;

fxDelayBufParam* createFxDelayBufParam(uint32_t maxSamples);
void removeFxDelayBufParam(fxDelayBufParam* param);
void writeDelayBuf(fxDelayBufParam* param, AUDIODATA_F data);
AUDIODATA_F readDelayBuf(fxDelayBufParam* param, uint32_t delayMs);

#ifdef __cplusplus
}
#endif

#endif /* __FXDELAYBUF_H__ */
