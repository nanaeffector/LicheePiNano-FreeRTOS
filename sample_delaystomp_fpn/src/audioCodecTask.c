#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "audio_codec.h"
#include "dbgmsg.h"
#include "audioFxTask.h"
#include "appConfig.h"

#include "pio.h"

#define ADC_EV_ARIV		(0x00000001)
#define ADC_EV_OVF		(0x00000002)
#define ADC_EVENTS		(ADC_EV_ARIV|ADC_EV_OVF)
#define DAC_EV_END		(0x00010000)
#define DAC_EV_ORUN		(0x00020000)
#define DAC_EV_URUN		(0x00040000)
#define DAC_EVENTS		(DAC_EV_END|DAC_EV_URUN|DAC_EV_ORUN)

static void vAudioCodecTask(void *vParameters);

static xTaskHandle xAudioCodecTask;

static AUDIODATA bufAudioData[2][APP_AUDIO_BUF_SMPL];
static AUDIODATA bufAudioDataDac[APP_AUDIO_BUF_SMPL];

extern BaseType_t xHigherPriorityTaskWoken;
static uint32_t adc_irq(eADCIRQ_REASON reason, AUDIODATA *data, uint32_t length){

	switch(reason){
		case ADCIRQ_FIFOARIVE:
			// audio データ 1フレーム完成.
			memcpy(bufAudioData[1], bufAudioData[0], sizeof(AUDIODATA) * APP_AUDIO_BUF_SMPL);
			xTaskNotifyFromISR(xAudioCodecTask, ADC_EV_ARIV, eSetBits, &xHigherPriorityTaskWoken);
			break;
		case ADCIRQ_FIFOOVRN:
			xTaskNotifyFromISR(xAudioCodecTask, ADC_EV_OVF, eSetBits, &xHigherPriorityTaskWoken);
			break;
		default:
			break;
	}

	return 1;	// adc continue.
}

static uint32_t dac_irq(eDACIRQ_REASON reason){
	_st_PinItem* item = pio_getPinItemPointer(PE3);

	switch(reason){
    	case DACIRQ_WREND:
			xTaskNotifyFromISR(xAudioCodecTask, DAC_EV_END, eSetBits, &xHigherPriorityTaskWoken);
			break;
    	case DACIRQ_FIFOOVRN:
			xTaskNotifyFromISR(xAudioCodecTask, DAC_EV_ORUN, eSetBits, &xHigherPriorityTaskWoken);
			break;
    	case DACIRQ_FIFOUDRN:
			xTaskNotifyFromISR(xAudioCodecTask, DAC_EV_URUN, eSetBits, &xHigherPriorityTaskWoken);
			break;
		default:
			break;
	}

	pio_write(item->port, item->num, 0);

	return 1;
}

static void initAudioCodec(){
	AdcSetting adcSetting = {APP_ADC_SAMPLE, APP_STEREO, APP_BIT_SAMPLE, adc_irq};
	DacSetting dacSetting = {APP_DAC_SAMPLE, APP_STEREO, APP_BIT_SAMPLE, dac_irq};
	// adcコーデック初期化.
	audiocodec_init(&adcSetting, &dacSetting);
}

void startADC(void){
	// adcチャンネル選択.
	//adc_selectChannel(CH_FMIN_LEFT | CH_FMIN_RIGHT);	// FMINは回路上でオープンだった・・・
	adc_selectChannel(CH_LINE_IN);

	// adcゲイン設定.
	adc_setADCGain(ADC_GAIN_6p0_DB);

	adc_start(bufAudioData[0], APP_AUDIO_BUF_SMPL);
}

static void startDAC(){
	// dac チャンネル選択.
	dac_selectOutputMixerChannel(CH_MIXL_LEFT , CH_MIXR_RIGHT);

	// ヘッドホン出力設定.
	dac_setHeadphoneOut(1, 0x3F, 0x3, 0);	// enable, vol:-46dB. LR mute-off, outsrc:dac

	dac_start(bufAudioDataDac, APP_AUDIO_BUF_SMPL);
}

void createAudioCodecTask(){
  // audio codec初期化. 
	initAudioCodec();

	xTaskCreate( vAudioCodecTask, "AudioCodecTask", 192, NULL, 3, &xAudioCodecTask);
}


void vAudioCodecTask(void *vParameters){
	uint32_t evbits = 0;
	
	//start adc.
	startADC();

	// start dac.
	startDAC();

	while(1){
		// adc待機.
		xTaskNotifyWait(0, ADC_EVENTS|DAC_EVENTS, &evbits, portMAX_DELAY);
		
		if(evbits & DAC_EV_END){
			//dbgMsgPrintln("de", pdFALSE);
		}

		if(evbits & DAC_EV_ORUN){
			//dbgMsgPrintln("do", pdFALSE);
		}

		if(evbits & DAC_EV_URUN){
			//dbgMsgPrintln("du", pdFALSE);
		}

		if(evbits & ADC_EV_ARIV){
			// audio 1フレーム完成.
			//dbgMsgPrintln("a", pdFALSE);
			
			// AudioFxTask 駆動.
			setAudioFxData(bufAudioData[1], APP_AUDIO_BUF_SMPL);
		}

		if(evbits & ADC_EV_OVF){
			//dbgMsgPrintln("aov", pdFALSE);
		}
	}
}
