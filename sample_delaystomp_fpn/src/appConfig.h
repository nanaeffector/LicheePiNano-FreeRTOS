// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "audio_codec.h"

// 以下、設定用の定義. 変更しないこと.
#define MONO            ADC_MONO
#define STEREO          ADC_STEREO
#define SMPL_48KHZ      (48000)
#define SMPL_44_1KHZ    (44100)
#define BIT_16          ADC_16BIT
#define BIT_24          ADC_24BIT


// 以下、設定値. 設定用の定義を使って設定すること.
#define APP_STEREO              MONO
#define APP_AUDIO_SMPL          SMPL_48KHZ
#define APP_BIT_SAMPLE          BIT_16
#define APP_AUDIO_BUF_SMPL      (256)       // 128 sampl.
#define APP_SMPL_AT_MS          (APP_AUDIO_SMPL/1000)

// delay設定.
#define APP_DELAY_TIME_MAX_MS   (2000)


// 以下、自動設定領域. 変更しないこと.
#if APP_AUDIO_SMPL==SMPL_48KHZ
    #define APP_ADC_SAMPLE      ADC_SAMPLE_48KHZ      
    #define APP_DAC_SAMPLE      DAC_SAMPLE_48KHZ      // ADCと同じにすること.
#elif APP_AUDIO_SMPL==SMPL_44_1KHZ
    #define APP_ADC_SAMPLE      ADC_SAMPLE_44_1KHZ      
    #define APP_DAC_SAMPLE      DAC_SAMPLE_44_1KHZ      // ADCと同じにすること.
#else
    #error invalid setting of audio sampl.
#endif

#ifdef __cplusplus
}
#endif

#endif /* __APPCONFIG_H__ */
