// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __FPN_H__
#define __FPN_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <math.h>

#define FRAC_BIT    (16)
#define DEC_BIT     (32-FRAC_BIT-1)

typedef struct{
    union{
        int32_t iVal;
        struct{
            uint32_t frac : FRAC_BIT;
            uint32_t dec  : DEC_BIT;
            uint32_t sign : 1;
        }fpn16;
    };
}FPN;

typedef struct{
	FPN left;
	FPN right;
}AUDIODATA_F;
	

extern const FPN fpnMax;
extern const FPN fpnMin;
extern const FPN fpnZero;
extern const FPN fpnOne;
extern const FPN fpnMOne;
extern const FPN fpnTwo;
extern const FPN fpnMTwo;
extern const FPN fpnPI;

#define VALUE2FRAC(VAL)     {{(((int64_t)VAL * (1 << FRAC_BIT)) & 0xFFFF0000)}}

// 下位16btに入っている int16 データを 16bit の固定小数点数に変換.
static inline FPN createFPN16_under16(int16_t val){
    FPN fpn;
    fpn.iVal = (val << FRAC_BIT) & 0xFFFF0000;
    return fpn;
}

// int16 データを 16bit の固定小数点数に変換.
static inline FPN createFPN16(int16_t val){
    FPN fpn;
    fpn.iVal = val;
    return fpn;
}

// 0 - 100 % (0.00 - 1.00) を 16bit の固定少数点に変換.
static inline FPN convertFromPercent(int32_t per){
    FPN fpn;
    fpn.iVal = per * 0x00010000 / 100;
    return fpn;
}

static inline int32_t convertToPercent(FPN fpn){
    return (fpn.iVal * 100) >> 16;
}

static inline int32_t convertToDecimal(FPN fpn){
    return fpn.iVal >> 16;
}

static inline FPN clipFPN(int64_t val){
    if(val > (int64_t)fpnMax.iVal){
        // 正クリップ.
        return fpnMax;
    }
    else if(val < (int64_t)fpnMin.iVal){
        // 負クリップ.
        return fpnMin;
    }
    else{
        FPN fpn;
        fpn.iVal = (int32_t)val;
        return fpn;
    }
}

// fpn加算.
static inline FPN addFPN(FPN val1, FPN val2){
    FPN fpn;
    int64_t tmp = (int64_t)val1.iVal + (int64_t)val2.iVal;

    if(tmp > (int64_t)fpnMax.iVal){
        // 正クリップ.
        return fpnMax;
    }
    else if(tmp < (int64_t)fpnMin.iVal){
        // 負クリップ.
        return fpnMin;
    }
    else{
        fpn.iVal = (int32_t)tmp;
        return fpn;
    }
}

// fpn減算.
static inline FPN subFPN(FPN val1, FPN val2){
    FPN fpn;
    int64_t tmp = (int64_t)val1.iVal - (int64_t)val2.iVal;

    if(tmp > (int64_t)fpnMax.iVal){
        // 正クリップ.
        return fpnMax;
    }
    else if(tmp < (int64_t)fpnMin.iVal){
        // 負クリップ.
        return fpnMin;
    }
    else{
        fpn.iVal = (int32_t)tmp;
        return fpn;
    }
}

// fpn比較. 一致 = 1, 不一致 = 0.
static inline int compFPN(FPN val1, FPN val2){
    return (subFPN(val1, val2).iVal == 0);
}

static inline FPN absFPN(FPN val){
    FPN o; o.iVal = (val.iVal < 0 ? -val.iVal : val.iVal);
    return o;
}

// fpn乗算.
static inline FPN mulFPN(FPN val1, FPN val2){
    FPN fpn;
    int64_t tmp = ((int64_t)val1.iVal * (int64_t)val2.iVal) >> 16;

    if(tmp > (int64_t)fpnMax.iVal){
        // 正クリップ.
        return fpnMax;
    }
    else if(tmp < (int64_t)fpnMin.iVal){
        // 負クリップ.
        return fpnMin;
    }
    else{
        fpn.iVal = (int32_t)tmp;
        return fpn;
    }
}

// fpn除算.
static inline FPN divFPN(FPN val1, FPN val2){
    FPN fpn;
    int64_t tmp = (((int64_t)val1.iVal << 32) / (int64_t)val2.iVal) >> 16;

    if(tmp > (int64_t)fpnMax.iVal){
        // 正クリップ.
        return fpnMax;
    }
    else if(tmp < (int64_t)fpnMin.iVal){
        // 負クリップ.
        return fpnMin;
    }
    else{
        fpn.iVal = (int32_t)tmp;
        return fpn;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __FPN_H__ */
