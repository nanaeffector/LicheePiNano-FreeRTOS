// author : Nana's Effector.
// date   : 2021/09/22

#include "stdint.h"
#include "string.h"

#include "clk.h"
#include "busreset.h"
#include "intc.h"
#include "reg_audio_codec.h"
#include "audio_codec.h"

static AUDIODATA *adcDataBuffer;
static uint32_t adcDataBufferSize;
static uint32_t adcDataBufferPointer;	// current buf index;
static uint32_t endADCLeftRight;

static adc_cb cbADCIrq;
static dac_cb cbDACIrq;

static AUDIODATA *dacDataBuffer;
static uint32_t dacDataBufferPointer;
static uint32_t dacDataBufferSize;
static uint32_t endDACLeftRight;


static void adc_init(AdcSetting* adcSetting){
	// FIFO 設定.
	AUDIO_CODEC.ADC_FIFOC.bit.RX_SAMPLE_BITS = (adcSetting->bits == ADC_24BIT) ? SAMPL_24BITS : SAMPL_16BITS;
	// adc-sampling.
	switch(adcSetting->sample){
		case ADC_SAMPLE_48KHZ:
		case ADC_SAMPLE_44_1KHZ:
			AUDIO_CODEC.ADC_FIFOC.bit.ADC_FS = FS_48K;
			break;
		case ADC_SAMPLE_24KHZ:
		case ADC_SAMPLE_22_05KHZ:
			AUDIO_CODEC.ADC_FIFOC.bit.ADC_FS = FS_24K;
			break;
		case ADC_SAMPLE_12KHZ:
		case ADC_SAMPLE_11_025KHZ:
			AUDIO_CODEC.ADC_FIFOC.bit.ADC_FS = FS_12K;
			break;
		default:
			AUDIO_CODEC.ADC_FIFOC.bit.ADC_FS = FS_48K;
			break;
	}
	AUDIO_CODEC.ADC_FIFOC.bit.RX_FIFO_MODE = 0;		// 0:RXDATA={FIFO_O[23:0], 0}.
	AUDIO_CODEC.ADC_FIFOC.bit.ADCDFEN = 0;			// disable.
	AUDIO_CODEC.ADC_FIFOC.bit.ADCFDT = ADCFDT_5ms;	// 5msec.
	AUDIO_CODEC.ADC_FIFOC.bit.RX_FIFO_TRG_LV = 0xF;	// trg level. default.
	AUDIO_CODEC.ADC_FIFOC.bit.ADC_MONO_EN = (adcSetting->mono == ADC_MONO) ? 1 : 0;		// stereo / mono.
	AUDIO_CODEC.ADC_FIFOC.bit.EN_AD = 0;			// adc digital enable.
	
	// ADC完了割り込み設定.
	cbADCIrq = adcSetting->cbADCIrq;
	AUDIO_CODEC.ADC_FIFOS.bit.RXA_INT = 1;			// arive irq フラグクリア.
	AUDIO_CODEC.ADC_FIFOS.bit.RXO_INT = 1;			// overrun irq フラグクリア.
	if(cbADCIrq){
		AUDIO_CODEC.ADC_FIFOC.bit.FIFO_OVRN_IRQ_EN = 0;	// arive irq 割り込み有効化.
		AUDIO_CODEC.ADC_FIFOC.bit.ADC_IRQ_EN = 1;		// overrun irq 割り込み有効化.
	}
	else{
		AUDIO_CODEC.ADC_FIFOC.bit.FIFO_OVRN_IRQ_EN = 0;	// arive irq 割り込み有効化.
		AUDIO_CODEC.ADC_FIFOC.bit.ADC_IRQ_EN = 0;		// overrun irq 割り込み有効化.
	}

	// ミキサー初期設定.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.MICG = 3;						// mic gain:0dB.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.FMINLVOL = 0;					// FMINL_R gain:0dB.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.LINEIN_VOL = 0;					// LINEIN gain:0dB.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.ADCG = 3;						// ADC gain:0dB.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.COS_SLOPE_CTRL = COS_ANTIPOP_262ms;	// anti-pop:262msec.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.ADC_MIX_MUTE = 0;				// all mute.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.PA_SPEED_SEL = 1;				// fast.
}

static void dac_init(DacSetting* dacSetting){
	// dac setting.
	AUDIO_CODEC.DAC_DPC.bit.EN_DA = 0;		// dac disable.
	AUDIO_CODEC.DAC_DPC.bit.DVOL_ATT = 0;	// att 0dB.

	// dac-sampling.
	switch(dacSetting->sample){
		case DAC_SAMPLE_48KHZ:
		case DAC_SAMPLE_44_1KHZ:
			AUDIO_CODEC.DAC_FIFOC.bit.DAC_FS = FS_48K;
			break;
		case DAC_SAMPLE_24KHZ:
		case DAC_SAMPLE_22_05KHZ:
			AUDIO_CODEC.DAC_FIFOC.bit.DAC_FS = FS_24K;
			break;
		case DAC_SAMPLE_12KHZ:
		case DAC_SAMPLE_11_025KHZ:
			AUDIO_CODEC.DAC_FIFOC.bit.DAC_FS = FS_12K;
			break;
		default:
			AUDIO_CODEC.DAC_FIFOC.bit.DAC_FS = FS_48K;
			break;
	}

	// FIFO 設定. 
	AUDIO_CODEC.DAC_FIFOC.bit.TX_SAMPL_BITS = (dacSetting->bits == ADC_24BIT) ? SAMPL_24BITS : SAMPL_16BITS;
	AUDIO_CODEC.DAC_FIFOC.bit.MONO_EN = (dacSetting->mono == ADC_MONO) ? 1 : 0;		// stereo / mono.
	AUDIO_CODEC.DAC_FIFOC.bit.ADDA_LOOP_EN = 0;	// loop disable.
	AUDIO_CODEC.DAC_FIFOC.bit.TX_TRIG_LV = 0xF;
	AUDIO_CODEC.DAC_FIFOC.bit.FIFO_MODE = 0;
	AUDIO_CODEC.DAC_FIFOC.bit.FIR_VER = 1;
 
	// 割り込み設定.
	cbDACIrq = dacSetting->cbDACIRQ;
	AUDIO_CODEC.DAC_FIFOS.bit.TXE_INT = 1;	// フラグクリア.
	AUDIO_CODEC.DAC_FIFOS.bit.TXO_INT = 1;	// フラグクリア.
	AUDIO_CODEC.DAC_FIFOS.bit.TXU_INT = 1;	// フラグクリア.
	if(cbDACIrq){
		// DAC FIFO エラー割り込み.
		AUDIO_CODEC.DAC_FIFOC.bit.FIFO_OVRN_IRQ_EN = 0;
		AUDIO_CODEC.DAC_FIFOC.bit.FIFO_UNRN_IRQ_EN = 0;
		// DAC FIFO empty 割り込み.
		AUDIO_CODEC.DAC_FIFOC.bit.DAC_IRQ_EN = 0;
	}
	else{
		// DAC FIFO エラー割り込み.
		AUDIO_CODEC.DAC_FIFOC.bit.FIFO_OVRN_IRQ_EN = 0;
		AUDIO_CODEC.DAC_FIFOC.bit.FIFO_UNRN_IRQ_EN = 0;
		// DAC FIFO empty 割り込み.
		AUDIO_CODEC.DAC_FIFOC.bit.DAC_IRQ_EN = 0;
	}

	// headphone setting.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HP_VOL = 0;				// volume -62dB.(mute)
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPPA_EN = 0;				// headphone amp enable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPCOM_PRTC_EN = 0;		// headphone com protect.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPCOM_FC = 0;			// headphone com is 00:floating.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_HPPA_SRC = 0;		// headphone source:DAC.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_HPPA_SRC = 0;		// headphone source:DAC.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_HPPA_MUTE = 0;		// left headphone mute.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_HPPA_MUTE = 0;		// right headphone mute.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RTLNMUTE = 0;			// left headphone negative mute.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LTRNMUTE = 0;			// right headphone negative mute.

	// mixer setting.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_MIX_MUTE = 0;		// left output mixer mute.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_MIX_MUTE = 0;		// left output mixer mute.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_MIXER_EN = 0;		// left output mixer all disable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_MIXER_EN = 0;		// left output mixer all disable.

	// dac enable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.DALEFT_EN = 0;			// left dac disable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.DARIGHT_EN = 0;			// right dac disable.
}

static inline void sdelay(int loops)
{
	__asm__ __volatile__ ("1:\n" "subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0"(loops));
}

int32_t audiocodec_init(AdcSetting* adcSetting, DacSetting* dacSetting){
	if((uint32_t)(adcSetting->sample & ADC_SAMPLE_MASK) != (uint32_t)(dacSetting->sample & DAC_SAMPLE_MASK)){
		// 違うPLL設定にはできない.
		return -1;
	}

	if(adcSetting->sample & ADC_SAMPLE_MASK){
		// set audio pll.(44.1KHz)
		clock_set_pll_audio(PLL_AUDIO_22_528MHZ);
	}
	else{
		// set audio_pll.(48KHz)
		clock_set_pll_audio(PLL_AUDIO_24_576MHZ);
	}
	
	// audio reset -> assert.
	assertReset(SoftResetUnit_ACODEC);
	
	// wait 100clk.
	sdelay(100);

	// audio reset -> deassert.
	deassertReset(SoftResetUnit_ACODEC);

	// クロックゲートを開ける.
	openClockGate(ClkGateUnit_AUDIO_CODEC);
	CCU.AUDIO_CODEC_CLK.bit.SCLK_GATING = 1;

	// power on 時に書き込み必要と仕様書に指示あり.
	AUDIO_CODEC.ADDA_TUNE.word = 0x44555556;
	AUDIO_CODEC.BIAS_DA16_CAL_CTRL1.word = 0x10000000;		// this register must write 0x10000000 at the power on.
	AUDIO_CODEC.BIAS_DA16_CAL_CTRL0.word = 0x00000004;		// this register must write 0x00000004 at the power on.
	//while(AUDIO_CODEC.BIAS_DA16_CAL_CTRL0.word & 0x4);		// wait finish.

	// ADC 設定.
	adc_init(adcSetting);

	// DAC 設定.
	dac_init(dacSetting);

	intc_enable_interrupt(IRQ_AUDIO);

	return 0;
}

//----------------------------------------------------
// DAC
//----------------------------------------------------

// DAC 開始.
void dac_start(AUDIODATA *pBuf, uint32_t bufLength){
	dacDataBuffer = pBuf;
	memset(pBuf, 0, bufLength * sizeof(AUDIODATA));
	dacDataBufferPointer = 0;
	dacDataBufferSize = bufLength;
	endDACLeftRight = 0;

	AUDIO_CODEC.DAC_FIFOS.bit.TXE_INT = 1;	// フラグクリア.
	AUDIO_CODEC.DAC_FIFOS.bit.TXO_INT = 1;	// フラグクリア.
	AUDIO_CODEC.DAC_FIFOS.bit.TXU_INT = 1;	// フラグクリア.

	AUDIO_CODEC.DAC_MIXER_CTRL.bit.DALEFT_EN = 1;			// left dac enable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.DARIGHT_EN = 1;			// right dac enable.
	AUDIO_CODEC.DAC_DPC.bit.EN_DA = 1;
}

// DAC 停止.
void dac_stop(void){
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.DALEFT_EN = 0;		// left dac disable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.DARIGHT_EN = 0;		// left dac disable.
	AUDIO_CODEC.DAC_DPC.bit.EN_DA = 0;
	
	// DAC FIFO エラー割り込み.
	AUDIO_CODEC.DAC_FIFOC.bit.FIFO_OVRN_IRQ_EN = 0;
	AUDIO_CODEC.DAC_FIFOC.bit.FIFO_UNRN_IRQ_EN = 0;
	// DAC FIFO empty 割り込み.
	AUDIO_CODEC.DAC_FIFOC.bit.DAC_IRQ_EN = 0;
}

// DAC 出力ミキサー設定.
void dac_selectOutputMixerChannel(eDAC_MIX_LCH lch, eDAC_MIX_RCH rch){
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_MIX_MUTE = lch;
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_MIX_MUTE = rch;
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_MIXER_EN = lch != CH_MIXL_NONE;
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_MIXER_EN = rch != CH_MIXR_NONE;
}

// DAC ヘッドホン出力設定.
void dac_setHeadphoneOut(uint8_t enable, uint8_t vol, uint8_t mute, uint8_t mix_en){
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HP_VOL = vol & 0x3F;		// headphone volume. 0 => -62dB(mute).
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPPA_EN = enable;		// headphone amp enable.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPCOM_PRTC_EN = 1;		// headphone com protect.
	//AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPCOM_FC = enable ? 0x3 : 0;			// headphone com is 00:.floating
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.HPCOM_FC = 0;			// headphone com is 00:.floating
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_HPPA_SRC = (mix_en&0x1) == 0x1 ? 1 : 0;		// headphone source.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_HPPA_SRC = (mix_en&0x2) == 0x2 ? 1 : 0;		// headphone source.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.LEFT_HPPA_MUTE = ((mute&0x1) == 0x1 ? 1 : 0);	// left headphone mute.
	AUDIO_CODEC.DAC_MIXER_CTRL.bit.RIGHT_HPPA_MUTE = ((mute&0x2) == 0x2 ? 1 : 0);	// right headphone mute.
	//AUDIO_CODEC.DAC_MIXER_CTRL.bit.RTLNMUTE = 0;			// left headphone negative mute. センター消し用？.
	//AUDIO_CODEC.DAC_MIXER_CTRL.bit.LTRNMUTE = 0;			// right headphone negative mute. センター消し用？.
}

int dac_writeBuffer(void){
	uint32_t is24bit = AUDIO_CODEC.ADC_FIFOC.bit.RX_SAMPLE_BITS == 1;
	uint32_t isStereo = AUDIO_CODEC.ADC_FIFOC.bit.ADC_MONO_EN == 0;

	// dac bufferをFIFOに書き込み.
	while(AUDIO_CODEC.DAC_FIFOS.bit.TXE_CNT > 1){
		AUDIODATA *pBuf = &dacDataBuffer[dacDataBufferPointer];
		int32_t val;
		if(isStereo){
			if(endDACLeftRight == 0){
				val = pBuf->left;
				endADCLeftRight |= 0x1;
			}
			else {
				val = pBuf->right;
				endDACLeftRight |= 0x2;
			}
		}
		else{
			val = pBuf->left;
			endDACLeftRight |= 0x3;
		}
		
		// left 書き込み.
		AUDIO_CODEC.DAC_TXDATA.word = val << (is24bit ? 8 : 16);

		if(endDACLeftRight == 0x3){
			endDACLeftRight = 0;

			dacDataBufferPointer++;
			if(dacDataBufferPointer >= dacDataBufferSize){
				dacDataBufferPointer = 0;
				return dacDataBufferSize;
			}
		}
	}

	return dacDataBufferPointer;
}

// DAC 書き込み.
uint32_t dac_writeReq(AUDIODATA *data, uint32_t length){

	dacDataBufferSize = length;
	// save buffer.
	memcpy(dacDataBuffer, data, sizeof(AUDIODATA) * length);

	// write to FIFO.
	if(dac_writeBuffer() != dacDataBufferSize){
		AUDIO_CODEC.DAC_FIFOC.bit.DAC_IRQ_EN = 1;
	}

	return 0;
}

//----------------------------------------------------
// ADC
//----------------------------------------------------

// ADC 結果読み出し.
uint32_t adc_readbuffer(void){
	uint32_t is24bit = AUDIO_CODEC.ADC_FIFOC.bit.RX_SAMPLE_BITS == 1;
	uint32_t isStereo = AUDIO_CODEC.ADC_FIFOC.bit.ADC_MONO_EN == 0;

	// read register.
	while(AUDIO_CODEC.ADC_FIFOS.bit.RXA_COUNT > 0){
		AUDIODATA *pBuf = &adcDataBuffer[adcDataBufferPointer];
		int32_t rxdata = AUDIO_CODEC.ADC_RXDATA.word >> (is24bit ? 8 : 16);
		
		if(isStereo){
			if(endADCLeftRight == 0){
				pBuf->left = rxdata;
				endADCLeftRight |= 0x1;
			}
			else {
				pBuf->right = rxdata;
				endADCLeftRight |= 0x2;
			}
		}
		else{
			pBuf->left = rxdata;
			pBuf->right = rxdata;	// same as left ch.
			endADCLeftRight = 0x3;
		}

		if(endADCLeftRight == 0x3){
			endADCLeftRight = 0;

			// update buf pointer.
			adcDataBufferPointer++;
			if(adcDataBufferPointer >= adcDataBufferSize){
				adcDataBufferPointer = 0;
				return adcDataBufferSize;
			}
		}
	}

	return adcDataBufferPointer;
}

// ADC 開始.
void adc_start(AUDIODATA *pBuf, uint32_t bufLength){
	// buffer 初期化.
	adcDataBuffer = pBuf;
	adcDataBufferSize = bufLength;
	memset(pBuf, 0, bufLength * sizeof(AUDIODATA));
	adcDataBufferPointer = 0;

	endADCLeftRight = 0;

	// fifo flush.
	AUDIO_CODEC.ADC_FIFOC.bit.FIFO_FLUSH = 1;
	while(AUDIO_CODEC.ADC_FIFOS.bit.RXA);

	// adc enable.
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.ADC_EN = 1;
	AUDIO_CODEC.ADC_FIFOC.bit.EN_AD = 1;
}

// ADC 停止.
void adc_stop(void){
	// adc disable.
	AUDIO_CODEC.ADC_FIFOC.bit.EN_AD = 0;
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.ADC_EN = 0;
	
	// fifo flush.
	AUDIO_CODEC.ADC_FIFOC.bit.FIFO_FLUSH = 1;
	while(AUDIO_CODEC.ADC_FIFOS.bit.RXA);
}

// ADC入力チャンネル選択.
void adc_selectChannel(eADC_CH ch){
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.ADC_MIX_MUTE = ch;
}

// ADC入力直前のampゲイン設定.
void adc_setADCGain(eADC_GAIN gain){
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.ADCG = gain;
}

// MIC入力直後のampゲイン設定.
void adc_setMIC_AMP_Gain(eMICAMP_GAIN gain){
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.MICBOOST = gain;
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.MICBOOST_EN = (gain != MIC_GAIN_0_DB);
}

// 出力ミキサー用のMICゲイン設定.
void adc_setMIC_MIX_Gain(eMIC_MIX_GAIN gain){
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.MICG = gain;
}

// 出力ミキサー用のLINEINゲイン設定.
void adc_setLINEIN_MIX_Gain(eIN_MIX_VOL gain){
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.LINEIN_VOL = gain;
}

// 出力ミキサー用のFMINL/Rゲイン設定.
void adc_setFMIN_MIX_Gain(eIN_MIX_VOL gain){
	AUDIO_CODEC.ADC_MIXER_CTRL.bit.FMINLVOL = gain;
}

// ---------------------------------------------------
// AudioCodec 割り込みハンドラ.
// ---------------------------------------------------
void AudioCodec_Handler(void){
	// adc data available.
	if(AUDIO_CODEC.ADC_FIFOS.bit.RXA_INT && AUDIO_CODEC.ADC_FIFOC.bit.ADC_IRQ_EN){
		// audio データ取得.
		if(adc_readbuffer() == adcDataBufferSize){
			if(cbADCIrq){
				if(cbADCIrq(ADCIRQ_FIFOARIVE, adcDataBuffer, adcDataBufferSize) == 0){
					// stop要求.
					adc_stop();
				}
			}
		}

		// clear irq flag.
		AUDIO_CODEC.ADC_FIFOS.bit.RXA_INT = 1;
	}

	// dac FIFO empty.
	if(AUDIO_CODEC.DAC_FIFOS.bit.TXE_INT && AUDIO_CODEC.DAC_FIFOC.bit.DAC_IRQ_EN){
		// write buffer to fifo.
		if(dac_writeBuffer() == dacDataBufferSize && cbDACIrq){
			// callback dac write end.
			cbDACIrq(DACIRQ_WREND);
			// DAC FIFO empty 割り込み.
			AUDIO_CODEC.DAC_FIFOC.bit.DAC_IRQ_EN = 0;
		}

		// clear fifo empty flg.
		AUDIO_CODEC.DAC_FIFOS.bit.TXE_INT = 1;
	}

	// adc FIFO overrun.
	if(AUDIO_CODEC.ADC_FIFOS.bit.RXO_INT && AUDIO_CODEC.ADC_FIFOC.bit.FIFO_OVRN_IRQ_EN){
		// fifo 捨てる.
		AUDIO_CODEC.ADC_FIFOC.bit.FIFO_FLUSH = 1;
		while(AUDIO_CODEC.ADC_FIFOS.bit.RXA);

		// clear irq flag.
		AUDIO_CODEC.ADC_FIFOS.bit.RXO_INT = 1;

		if(cbADCIrq){
			// callback fifo overrun.
			cbADCIrq(ADCIRQ_FIFOOVRN, adcDataBuffer, adcDataBufferPointer);
		}
	}

	// dac FIFO overrun.
	if(AUDIO_CODEC.DAC_FIFOS.bit.TXO_INT && AUDIO_CODEC.DAC_FIFOC.bit.FIFO_OVRN_IRQ_EN){
		if(cbDACIrq){
			// fifo overrun irq.
			cbDACIrq(DACIRQ_FIFOOVRN);
		}

		// clear fifo overrun flg.
		AUDIO_CODEC.DAC_FIFOS.bit.TXO_INT = 1;
	}
	// dac FIFO underrun.
	if(AUDIO_CODEC.DAC_FIFOS.bit.TXU_INT && AUDIO_CODEC.DAC_FIFOC.bit.FIFO_UNRN_IRQ_EN){
		if(cbDACIrq){
			// fifo underrun irq.
			cbDACIrq(DACIRQ_FIFOUDRN);
		}

		// clear fifo underrun flg.
		AUDIO_CODEC.DAC_FIFOS.bit.TXU_INT = 1;
	}
}
