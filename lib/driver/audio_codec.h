// author : Nana's Effector.
// date   : 2022/07/14

#ifndef __AUDIO_CODEC_H__
#define __AUDIO_CODEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "audiodata.h"

typedef enum{
	ADC_SAMPLE_48KHZ = 0x00,
	ADC_SAMPLE_24KHZ = 0x01,
	ADC_SAMPLE_12KHZ = 0x02,
	ADC_SAMPLE_44_1KHZ = 0x10,
	ADC_SAMPLE_22_05KHZ = 0x11,
	ADC_SAMPLE_11_025KHZ = 0x12,
    ADC_SAMPLE_MASK = 0x10,
}eADC_SAMPLE;

typedef enum{
	DAC_SAMPLE_48KHZ = 0x00,
	DAC_SAMPLE_24KHZ = 0x01,
	DAC_SAMPLE_12KHZ = 0x02,
    DAC_SAMPLE_96KHZ = 0x03,
    DAC_SAMPLE_192KHZ = 0x04,
	DAC_SAMPLE_44_1KHZ = 0x10,
	DAC_SAMPLE_22_05KHZ = 0x11,
	DAC_SAMPLE_11_025KHZ = 0x12,
    DAC_SAMPLE_MASK = 0x10,
}eDAC_SAMPLE;

typedef enum{
	ADC_STEREO = 0,
	ADC_MONO = 1,
}eADC_MONO;

typedef enum{
	ADC_16BIT = 0,
	ADC_24BIT = 1,
}eADC_BITS;

typedef enum{
    CH_LINE_IN = 0x04,
    CH_FMIN_LEFT = 0x08,
    CH_FMIN_RIGHT = 0x10,
    CH_MIC = 0x20,
}eADC_CH;

typedef enum{
    CH_MIXL_NONE = 0,
    CH_MIXL_LEFT = 0x01,
    CH_MIXL_RIGHT = 0x02,
    CH_MIXL_FMINL = 0x04,
    CH_MIXL_LINEIN = 0x08,
    CH_MIXL_MIC = 0x10,
}eDAC_MIX_LCH;

typedef enum{
    CH_MIXR_NONE = 0,
    CH_MIXR_RIGHT = 0x01,
    CH_MIXR_LEFT = 0x02,
    CH_MIXR_FMINR = 0x04,
    CH_MIXR_LINEIN = 0x08,
    CH_MIXR_MIC = 0x10,
}eDAC_MIX_RCH;

typedef enum{
    // ADC手前のAmpゲイン.
    ADC_GAIN_m4p5_DB = 0,
    ADC_GAIN_m3p0_DB = 1,
    ADC_GAIN_m1p5_DB = 2,
    ADC_GAIN_0_DB = 3,
    ADC_GAIN_1p5_DB = 4,
    ADC_GAIN_3p0_DB = 5,
    ADC_GAIN_4p5_DB = 6,
    ADC_GAIN_6p0_DB = 7,
}eADC_GAIN;

typedef enum{
    // MIC入力直後のAmpゲイン.
    MIC_GAIN_0_DB = 0, 
    MIC_GAIN_24_DB = 1,
    MIC_GAIN_27_DB = 2,
    MIC_GAIN_30_DB = 3,
    MIC_GAIN_33_DB = 4,
    MIC_GAIN_36_DB = 5,
    MIC_GAIN_39_DB = 6,
    MIC_GAIN_42_DB = 7,
}eMICAMP_GAIN;

typedef enum{
    // MICのmixerへのゲイン.
    MIC_MIX_GAIN_m4p5_DB = 0,
    MIC_MIX_GAIN_m3p0_DB = 1,
    MIC_MIX_GAIN_m1p5_DB = 2,
    MIC_MIX_GAIN_0_DB = 3,
    MIC_MIX_GAIN_1p5_DB = 4,
    MIC_MIX_GAIN_3p0_DB = 5,
    MIC_MIX_GAIN_4p5_DB = 6,
    MIC_MIX_GAIN_6p0_DB = 7,
}eMIC_MIX_GAIN;

typedef enum{
    // FMIN/LINEINのmixerへのゲイン.
    IN_MIX_GAIN_0_DB = 0, 
    IN_MIX_GAIN_2_DB = 1,
    IN_MIX_GAIN_4_DB = 2,
    IN_MIX_GAIN_6_DB = 3,
    IN_MIX_GAIN_8_DB = 4,
    IN_MIX_GAIN_10_DB = 5,
    IN_MIX_GAIN_12_DB = 6,
    IN_MIX_GAIN_14_DB = 7,
}eIN_MIX_VOL;

typedef enum{
    DACIRQ_FIFOEMPTY = 0,
    DACIRQ_FIFOOVRN,
    DACIRQ_FIFOUDRN,
    DACIRQ_WREND,
}eDACIRQ_REASON;

typedef enum{
    ADCIRQ_FIFOARIVE = 0,
    ADCIRQ_FIFOOVRN,
}eADCIRQ_REASON;

typedef uint32_t(*dac_cb)(eDACIRQ_REASON reason);
typedef uint32_t(*adc_cb)(eADCIRQ_REASON reason, AUDIODATA *data, uint32_t length);


typedef struct {
	eADC_SAMPLE sample;
	eADC_MONO mono;
	eADC_BITS bits;
    adc_cb cbADCIrq;
}AdcSetting;

typedef struct {
    eDAC_SAMPLE sample;
	eADC_MONO mono;
	eADC_BITS bits;
    dac_cb cbDACIRQ;
}DacSetting;

int32_t audiocodec_init(AdcSetting* adcSetting, DacSetting* dacSetting);
void adc_selectChannel(eADC_CH ch);
void adc_start(AUDIODATA *pBuf, uint32_t bufLength);
void adc_stop(void);
void adc_setADCGain(eADC_GAIN gain);
void adc_setMIC_AMP_Gain(eMICAMP_GAIN gain);
void adc_setMIC_MIX_Gain(eMIC_MIX_GAIN gain);
void adc_setLINEIN_MIX_Gain(eIN_MIX_VOL gain);
void adc_setFMIN_MIX_Gain(eIN_MIX_VOL gain);
uint32_t adc_readbuffer(void);

void dac_start(AUDIODATA *pBuf, uint32_t bufLength);
void dac_stop(void);
void dac_selectOutputMixerChannel(eDAC_MIX_LCH lch, eDAC_MIX_RCH rch);
void dac_setHeadphoneOut(uint8_t enable, uint8_t vol, uint8_t mute, uint8_t mix_en);
uint32_t dac_writeReq(AUDIODATA *data, uint32_t length);
int dac_writeBuffer(void);

#ifdef __cplusplus
}
#endif

#endif /* __REG_AUDIO_CODEC_H__ */