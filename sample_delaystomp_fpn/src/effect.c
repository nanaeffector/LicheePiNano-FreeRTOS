
#include <stdint.h>
#include <stdio.h>
#include "effect.h"
#include "effectList.h"
#include "dbgmsg.h"
#include "effectDelay.h"
#include "appConfig.h"

void initEffect(){
    initEffectList();

    // TODO. ユーザが順番など指定できるようにしたい.
    insertEffectItemAt(createDelayEffect(0), 0);
}

void addEffect(Effect effect){
    insertEffectItemAt(&effect, 0);
}

void runEffect(const AUDIODATA_F *pDryData, AUDIODATA_F *pWetData, uint32_t size){
    int i, j;
    int listsize = getEffectListSize();

	dbgMsgPrintln("es", pdFALSE);

    for(i=0; i<size; i++){
        if(listsize == 0){
            // loopback.
            pWetData[i] = pDryData[i];
        }
        else{
            stEffectListItem *pCurrentEffect = effectListHead;
            for(j=0; j<listsize; j++){
                if(pCurrentEffect != 0 && pCurrentEffect->pEffect != 0){
                    pCurrentEffect->pEffect->fxAtSample(pCurrentEffect->pEffect->stParamValue, &pDryData[i], &pWetData[i]);
                    pCurrentEffect = pCurrentEffect->pNextEffect;
                }
                else{
                    // invalid size.
                    break;
                }
            }
        }
    }

    dbgMsgPrintln("ee", pdFALSE);
}