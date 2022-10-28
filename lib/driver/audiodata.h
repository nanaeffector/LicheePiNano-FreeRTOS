// author : Nana's Effector.
// date   : 2021/09/22

#ifndef __AUDIODATA_H__
#define __AUDIODATA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct{
    int32_t left;
    int32_t right;
}AUDIODATA;

#ifdef __cplusplus
}
#endif

#endif /* __AUDIODATA_H__ */