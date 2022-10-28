// author : Nana's Effector.
// date   : 2022/07/14

#ifndef __REG_AUDIO_CODEC_H__
#define __REG_AUDIO_CODEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef enum{
    Normal = 0,
    SoftVolume = 2,
    SoftEnable = 3,
}eHPVL_SOFTMOD;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t HPVL_CTRL_OUT  :6;     // status of headphone PA volume control. 0:-63dB. 63:0dB.
            uint32_t HPVL_STEP_CTRL :6;     // headphone volume soft tuning step[msec] control.(f1c200 pdf p.161)
            uint32_t DVOL_ATT       :6;     // digital volume control. ATT = DVOL*-1.16[db].
            uint32_t HPF_EN         :1;     // high pass filter enable.
            uint32_t HPPA_MXRD_ENA  :1;     // status of headphone PA mixer all-input mute control.
            uint32_t DACA_CHND_ENA  :1;     // readonly. status of DAC Analog channel enable control.
            uint32_t HPVL_SOFT_MOD  :2;     // 0-1:normal. 2:soft volume. 3:DAC soft disable or enable.
            uint32_t reserve1        :1;
            uint32_t DWA            :1;     // DWA 0:disable. 1:enable. Digital Weighted Averagingの略と思われ.
            uint32_t MODQU          :4;     // internal DAC Quantization Levels. 7*(21+MODQU)/128.  default => 1.15.
            uint32_t reserve2        :2;
            uint32_t EN_DA          :1;     // DAC Digital part enable.
        }bit;
    };
}regDAC_DPC;

typedef enum{
    FS_48K = 0,
    FS_32K = 1,
    FS_24K = 2,
    FS_16K = 3,
    FS_12K = 4,
    FS_8K = 5,
    FS_192K = 6,
    FS_96K = 7,
} eCODEC_FS;

typedef enum{
    LASAT_ZERO = 0,
    LASAT_LAST = 1,
}eDAC_LASAT;

typedef enum{
    SAMPL_16BITS = 0,
    SAMPL_24BITS = 1,
}eSAMPLBITS;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t TXFIFO_FLUSH       :1;     // DAC FIFO flush. write 1 to flush TX FIFO. self clear to 0.
            uint32_t FIFO_OVRN_IRQ_EN   :1;     // DAC FIFO overrun irq enable.
            uint32_t FIFO_UNRN_IRQ_EN   :1;     // DAC FIFO underrun irq enable.
            uint32_t DAC_IRQ_EN         :1;     // DAC FIFO empty irq enable. 
            uint32_t DAC_DRQ_EN         :1;     // DAC FIFO empty drq enable. 多分dreq.
            eSAMPLBITS TX_SAMPL_BITS    :1;     // tx sample 0:16bit. 1:24bit
            uint32_t MONO_EN            :1;     // 0:stereo, 64levels FIFO. 1:mono,128levels FIFO. when mono, L&R channel need same data.
            uint32_t ADDA_LOOP_EN       :1;     // 0:disable, 1:enable.
            uint32_t TX_TRIG_LV         :7;     // TX FIFO empty trigger level.(=TXTL?)
            uint32_t reserve1           :6;
            uint32_t DAC_DRQ_CLR_CNT    :2;     // IRQ / DRQ deassert when WLEVEL > TXTL. 多分. 現在の空きレベルがトリガLvを超えたらクリアされる？
            uint32_t reserve2           :1;
            uint32_t FIFO_MODE          :2;     // when 24bit mode. X0:FIFO_I=TXDATA[31:8],X1:reserved. when 16bit mode. X0:FIFO_I=TXDATA[31:16], X1:FIFO_I=TXDATA[15:0].
            eDAC_LASAT SEND_LASAT       :1;     // when tx fifo underrun, audio sample is, 0:sending zero. 1:sending last audio sample.
            uint32_t reserve3           :1;
            uint32_t FIR_VER            :1;     // 0:64tap FIR. 1:32tap FIR.
            eCODEC_FS DAC_FS            :3;     // sample rate for DAC.
        }bit;
    };
}regDAC_FIFOC;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t reserve1           :1;
            uint32_t TXO_INT            :1;     // pending status for TX FIFO underrun irq.
            uint32_t TXU_INT            :1;     // pending status for TX FIFO overrun irq.
            uint32_t TXE_INT            :1;     // pending status for TX FIFO empty irq.
            uint32_t reserve2           :4;
            uint32_t TXE_CNT            :15;    // TX FIFO empty space word counter.
            uint32_t TX_EMPTY           :1;     // readonly. empty for tx FIFO.
            uint32_t reserve3           :8;
        }bit;
    };
}regDAC_FIFOS;

typedef struct{
    uint32_t word;
}regDAC_TXDATA;

typedef enum{
    ADCFDT_5ms = 0,
    ADCFDT_10ms = 1,
    ADCFDT_20ms = 2,
    ADCFDT_30ms = 3,
}eADCFDT;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t FIFO_FLUSH         :1;     // FIFO flush. write 1 to flush TX(?) FIFO. self clear to 0.
            uint32_t FIFO_OVRN_IRQ_EN   :1;     // FIFO overrun irq enable.
            uint32_t reserve1           :1;
            uint32_t ADC_IRQ_EN         :1;     // FIFO available irq enable. 
            uint32_t ADC_DRQ_EN         :1;     // FIFO available drq enable. 多分dreq.
            uint32_t reserve2           :1;
            eSAMPLBITS RX_SAMPLE_BITS   :1;     // 0:16bit. 1:24bit.
            uint32_t ADC_MONO_EN        :1;     // 0:stereo,FIFO 16step. 1:mono, FIFO 32step.
            uint32_t RX_FIFO_TRG_LV     :5;     // RX FIFO trigger level. (=RXTL ?) 
            uint32_t reserve3           :3;
            uint32_t ADCDFEN            :1;     // ADC FIFO delay function enable.
            eADCFDT  ADCFDT             :2;     // ADC FIFO delay time for writing data after adc digital part enable.
            uint32_t reserve4           :5;
            uint32_t RX_FIFO_MODE       :1;     // RX FIFO Output mode. when 24bit, 0:RXDATA={FIFO_O[23:0], 0},1:reserved. when 16bit mode. 0:RXDATA = {FIFO_O[23:8], 0}, 1:RXDATA = {FIFO_O[23]*16, FIFO_O[23:8]}.
            uint32_t reserve5           :3;
            uint32_t EN_AD              :1;     // adc digital part enable.
            eCODEC_FS ADC_FS            :3;     // sample rate for ADC.
        }bit;
    };
}regADC_FIFOC;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t reserve1           :1;
            uint32_t RXO_INT            :1;     // pending status for FIFO overrun irq.
            uint32_t reserve2           :1;
            uint32_t RXA_INT            :1;     // pending status for FIFO available irq.
            uint32_t reserve3           :4;
            uint32_t RXA_COUNT          :6;     // FIFO available sample word counter.
            uint32_t reserve4           :9;
            uint32_t RXA                :1;     // FIFO available status. 0:no data. 1:>= 1 word.
            uint32_t reserve5           :8;
        }bit;
    };
}regADC_FIFOS;

typedef struct{
    int32_t word;
}regADC_RXDATA;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t HP_VOL             :6;     // Headphone volume control. 0db to -62db. 0 is mute.
            uint32_t RTLNMUTE           :1;     // right headphone negative 0:mute. 1:unmute.
            uint32_t LTRNMUTE           :1;     // left headphone negative 0:mute. 1:unmute.
            uint32_t LEFT_MIX_MUTE      :5;     // 0:mute. 1:unmute. bit0:rightDAC. bit1:leftDAC. bit2:FMINL. bit3:LINEIN. bit4:MICIN Boost stage.
            uint32_t reserve1           :2;
            uint32_t HPPA_EN            :1;     // right & left headphone power amp enable.
            uint32_t RIGHT_MIX_MUTE     :5;     // 0:mute. 1:unmute. bit0:LeftDAC. bit1:rightDAC. bit2:FMINR. bit3:LINEIN. bit4:MICIN Boost stage.
            uint32_t HPCOM_PRTC_EN      :1;     // HPCOM output protection enable. when direct drive.
            uint32_t HPCOM_FC           :2;     // 00:HPCOM off & output is floating. 01:HPL inverting. 10:hpr invert. 11:direct drive hpl&hpr.
            uint32_t LEFT_HPPA_SRC      :1;     // 0:DAC. 1:Analog mixer.
            uint32_t RIGHT_HPPA_SRC     :1;     // 0:DAC. 1:Analog mixer.
            uint32_t LEFT_HPPA_MUTE     :1;     // left headphone 0:mute. 1: unmute.
            uint32_t RIGHT_HPPA_MUTE    :1;     // right headphone 0:mute. 1: unmute.
            uint32_t LEFT_MIXER_EN      :1;     // left output mixer enable.
            uint32_t RIGHT_MIXER_EN     :1;     // right output mixer enable.
            uint32_t DALEFT_EN          :1;     // left analog dac enable.
            uint32_t DARIGHT_EN         :1;     // right analog dac enable.
        }bit;
    };
}regDAC_MIXER_CTRL;

typedef enum{
    COS_ANTIPOP_131ms = 0,
    COS_ANTIPOP_262ms = 1,
    COS_ANTIPOP_393ms = 2,
    COS_ANTIPOP_524ms = 3,
}eCOS_ANTIPOP;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t MICBOOST           :3;     // MIC Boost amp gain. 0 is 0db. 1 to 7 meaning 24dB to 42dB. 3db/step.
            uint32_t MICBOOST_EN        :1;     // MIC Boost amp enable.
            uint32_t FMINLVOL           :3;     // FMINL/R to L/R output mixer gain control. 0dB to 14dB. 2dB step.
            uint32_t PA_SPEED_SEL       :1;     // 0:normal. 1:fast.
            uint32_t ADC_MIX_MUTE       :6;     // 0:mute. 1:unmute. bit0:right output. bit1:left output. bit2:LINEIN. bit3:FMINR. bit4:FMINL. bit5:MICIN boost stage.
            eCOS_ANTIPOP COS_SLOPE_CTRL :2;     // anti pop. 0:131msc.
            uint32_t ADCG               :3;     // ADC input gain. -4.5dB to 6dB. 1.5dB/step.
            uint32_t reserve1           :2;
            uint32_t LINEIN_VOL         :3;     // LINEIN to L/R output mixer gain. 0dB to 14dB. 2dB/step.
            uint32_t MICG               :3;     // MICIN Boost stage to L or R output mixer gain. -4.5dB to 6dB. 1.5dB/step.
            uint32_t reserve2           :4;
            uint32_t ADC_EN             :1;     // ADC enable.
        }bit;
    };
}regADC_MIXER_CTRL;

typedef enum{
    PA_ANTIPOP_131ms = 0,
    PA_ANTIPOP_262ms = 1,
    PA_ANTIPOP_393ms = 2,
    PA_ANTIPOP_524ms = 3,
    PA_ANTIPOP_655ms = 4,
    PA_ANTIPOP_789ms = 5,
    //PA_ANTIPOP_789ms = 6,
    PA_ANTIPOP_1048ms = 7,
}ePA_ANTIPOP;

typedef enum{
    ZEROCROSS_32ms = 0,
    ZEROCROSS_64ms = 1,
}eZEROCROSS_TIME;

typedef struct{
    union{
        uint32_t word;
        struct{
            uint32_t BIHE_CTRL                      :2;     // BIHE control. 0:no bihe. 1:bihe=7.5hosc. 2:bihe=11.5hosc. 3:bihe=15.5hosc.
            uint32_t DITHER_CLK_SEL                 :2;     // adc dither clock sel. 
            uint32_t DITHER_EN                      :1;     // dither enable.
            uint32_t USB_BIAS_CUR                   :3;     // usb bias current control.
            uint32_t OPAAF_BIAS_CUR                 :2;     // opaaf bias current control.
            uint32_t OPADC2_BIAS_CUR                :2;     // opadc2 bias current control.
            uint32_t OPADC1_BIAS_CUR                :2;     // opadc1 bias current control.
            uint32_t OPDRV_OPCOM_CUR                :2;     // opdrv/opcom output current setting.
            uint32_t OPMIX_BIAS_CUR                 :2;     // opmix/oplpf/opdrv/opcom bias current control.
            uint32_t OPDAC_BIAS_CUR                 :2;     // opdac bias current control.
            uint32_t OPVR_BIAS_CUR                  :2;     // opvr bias current control.
            uint32_t OPMIC_BIAS_CUR                 :2;     // opmic bias current control.
            ePA_ANTIPOP PA_ANTI_POP_CTRL               :3;     // PA anti-pop time.
            uint32_t PA_SLOPE_SEL                   :1;     // PA slope select. 0:cosine. 1:ramp.
            uint32_t PTDBS                          :2;     // HPCOM protect debonce time. 0:2-3ms. 1:4-6ms. 2:8-12ms. 3:16-24ms.
            eZEROCROSS_TIME ZERO_CROSS_TIME_SEL     :1;     // timeout select zerocross.
            uint32_t ZERO_CROSS_EN                  :1;     // master volume change at zerocross enable.
        }bit;
    };
}regADDA_TUNE;

typedef struct{
    uint32_t word;                                      // this register must write 0x00000004 at the power on.
}regBIAS_DA16_CAL_CTRL0;

typedef struct{
    uint32_t word;                                      // this register must write 0x10000000 at the power on.
}regBIAS_DA16_CAL_CTRL1;

typedef struct{
    uint32_t word;                                      // TX sample counter. count of sending into txfifo.
}regDAC_CNT;

typedef struct{
    uint32_t word;                                      // RX sample counter. count of writing into rxfifo.
}regADC_CNT;

typedef struct{
    uint32_t word;
}regDAC_DBG;

typedef struct{
    uint32_t word;
}regADC_DBG;

typedef struct{
    uint32_t word;
}regADC_DAP_CTR;

typedef struct{
    uint32_t word;
}regADC_DAP_LCTR;

typedef struct{
    uint32_t word;
}regADC_DAP_RCTR;

typedef struct{
    uint32_t word;
}regADC_DAP_PARA;

typedef struct{
    uint32_t word;
}regADC_DAP_LAC;

typedef struct{
    uint32_t word;
}regADC_DAP_LDAT;

typedef struct{
    uint32_t word;
}regADC_DAP_RAC;

typedef struct{
    uint32_t word;
}regADC_DAP_RDAT;

typedef struct{
    uint32_t word;
}regADC_DAP_HPFC;

typedef struct{
    uint32_t word;
}regADC_DAP_LINAC;

typedef struct{
    uint32_t word;
}regADC_DAP_RINAC;

typedef struct{
    uint32_t word;
}regADC_DAP_ORT;

typedef struct{
    regDAC_DPC                  DAC_DPC;
    regDAC_FIFOC                DAC_FIFOC;
    regDAC_FIFOS                DAC_FIFOS; 
    regDAC_TXDATA               DAC_TXDATA;
    regADC_FIFOC                ADC_FIFOC;
    regADC_FIFOS                ADC_FIFOS;
    regADC_RXDATA               ADC_RXDATA;
    uint32_t                    reserve1;
    regDAC_MIXER_CTRL           DAC_MIXER_CTRL;
    regADC_MIXER_CTRL           ADC_MIXER_CTRL;
    regADDA_TUNE                ADDA_TUNE;
    regBIAS_DA16_CAL_CTRL0      BIAS_DA16_CAL_CTRL0;    // this register must write 0x00000004 at the power on.
    uint32_t                    reserve2;
    regBIAS_DA16_CAL_CTRL1      BIAS_DA16_CAL_CTRL1;    // this register must write 0x10000000 at the power on.
    uint32_t                    reserve3[2];
    regDAC_CNT                  DAC_CNT;
    regADC_CNT                  ADC_CNT;
    regDAC_DBG                  DAC_DBG;
    regADC_DBG                  ADC_DBG;
    uint32_t                    reserve4[9];
    regADC_DAP_CTR              ADC_DAP_CTR;
    regADC_DAP_LCTR             ADC_DAP_LCTR;
    regADC_DAP_RCTR             ADC_DAP_RCTR;
    regADC_DAP_PARA             ADC_DAP_PARA;
    regADC_DAP_LAC              ADC_DAP_LAC;
    regADC_DAP_LDAT             ADC_DAP_LDAT;
    regADC_DAP_RAC              ADC_DAP_RAC;
    regADC_DAP_RDAT             ADC_DAP_RDAT;
    regADC_DAP_HPFC             ADC_DAP_HPFC;
    regADC_DAP_LINAC            ADC_DAP_LINAC;
    regADC_DAP_RINAC            ADC_DAP_RINAC;
    regADC_DAP_ORT              ADC_DAP_ORT;
}_st_AUDIO_CODEC;


#define F1C100S_AUDIOCODEC_BASE		(0x01c23c00)

#define AUDIO_CODEC				(*(volatile _st_AUDIO_CODEC *)F1C100S_AUDIOCODEC_BASE)


#ifdef __cplusplus
}
#endif

#endif /* __REG_AUDIO_CODEC_H__ */