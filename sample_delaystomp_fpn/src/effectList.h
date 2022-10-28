// author : Nana's Effector.
// date   : 2022/07/20

#ifndef __EFFECTLIST_H__
#define __EFFECTLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "effect.h"

typedef struct _stEffectListItem {
    Effect *pEffect;
    struct _stEffectListItem *pPrevEffect;      // in:1.
    struct _stEffectListItem *pNextEffect;     // out:多.
}stEffectListItem;

void initEffectList(void);
void deleteEffectItemAt(uint32_t index);
void addEffectItem(Effect *item);
void insertEffectItemAt(Effect *item, uint32_t index);
stEffectListItem* findEffectListItem(uint32_t index);
stEffectListItem* lastEffectListItem(void);
uint32_t getEffectListSize(void);

extern stEffectListItem *effectListHead;

#ifdef __cplusplus
}
#endif

#endif /* __EFFECTLIST_H__ */
