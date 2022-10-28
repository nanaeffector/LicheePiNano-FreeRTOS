// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __AUDIOFXTASK_H__
#define __AUDIOFXTASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "FreeRTOS.h"
#include "audiodata.h"
#include "effect.h"

void createAudioFxTask();
void setAudioFxData(AUDIODATA *buf, int32_t iSampleNum);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIOFXTASK_H__ */
