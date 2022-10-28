#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "audio_codec.h"
#include "fpn.h"
#include "dbgmsg.h"
#include "audioFxTask.h"
#include "appConfig.h"
#include "effect.h"

// イベントフラグ.
#define AFX_EV_ARIV		(0x00000001)
#define AFX_EV_END		(0x00000002)
#define AFX_EV_ALL		(AFX_EV_ARIV|AFX_EV_END)

static void vAudioFxTask(void *vParameters);
static xTaskHandle xAudioFxTask;

static AUDIODATA_F bufData[2][APP_AUDIO_BUF_SMPL];		// [0]:in data. [1]:out data.
static AUDIODATA bufOutAudioData[APP_AUDIO_BUF_SMPL];		// out data.

void createAudioFxTask(){
	xTaskCreate( vAudioFxTask, "AudioFxTask", 192, NULL, 4, &xAudioFxTask);
}

void setAudioFxData(AUDIODATA *buf, int32_t iSampleNum){
	int i;
	
	// 固定小数点数に変換 & 正規化.
	// TODO. 正規化.
	for(i=0; i<iSampleNum; i++){
		bufData[0][i].left = createFPN16_under16(buf[i].left);
		bufData[0][i].right = createFPN16_under16(buf[i].right);
	}
	
    // event発行.
	xTaskNotify(xAudioFxTask, AFX_EV_ARIV, eSetBits);
}

void decodeForAudioData(AUDIODATA_F *buf, int32_t iSampleNum){
	int i;
	
	// 実数に変換.
	// TODO. 正規化解除.
	for(i=0; i<iSampleNum; i++){
		bufOutAudioData[i].left = convertToDecimal(buf[i].left);
		bufOutAudioData[i].right = convertToDecimal(buf[i].right);
	}
}

void vAudioFxTask(void *vParameters){
	uint32_t evbits = 0;

	initEffect();

	while(1){
		// audio フレーム完了待機.
		xTaskNotifyWait(0, AFX_EV_ALL, &evbits, portMAX_DELAY);

		if(evbits & AFX_EV_ARIV){  // audio フレーム完了.
			// audio effect実施.
			runEffect(bufData[0], bufData[1], APP_AUDIO_BUF_SMPL);
			
			// effect完了を通知.
			xTaskNotify(xAudioFxTask, AFX_EV_END, eSetBits);
		}

		if(evbits & AFX_EV_END){
			// audiodataに変換.
			decodeForAudioData(bufData[1], APP_AUDIO_BUF_SMPL);
			
			// dacへ出力.
			dac_writeReq(bufOutAudioData, APP_AUDIO_BUF_SMPL);
		}
	}
}
