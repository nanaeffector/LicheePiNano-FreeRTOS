
#include <stdint.h>
#include <stdlib.h>
#include "effect.h"
#include "effectList.h"

static uint32_t effectListSize;
stEffectListItem *effectListHead;

void initEffectList(void){
    while(effectListHead != 0){
        // all delete.
        deleteEffectItemAt(0);
    }

    effectListHead = 0;
}

void deleteEffectItemAt(uint32_t index){
    // 検索.
    stEffectListItem *current = findEffectListItem(index);
    if(current == 0){
        // data無し.
        return;
    }

    // 繋げ変え.
    if(current->pPrevEffect != 0){
        current->pPrevEffect->pNextEffect = current->pNextEffect;
    }
    else{
        effectListHead = current->pNextEffect;
    }

    // 開放.
    current->pEffect->finish(current->pEffect);
    free(current);
    effectListSize--;
}

void addEffectItem(Effect *item){
    // 終端検索.
    stEffectListItem *last = lastEffectListItem();

    // 新規作成.
    stEffectListItem *newItem = (stEffectListItem*)malloc(sizeof(stEffectListItem));
    newItem->pEffect = item;
    newItem->pPrevEffect = last;
    newItem->pNextEffect = 0;

    // 追加.
    if(last == 0){
        // 先頭.
        effectListHead = newItem;
    }
    else{
        // 追加.
        last->pNextEffect = newItem;
    }
    effectListSize++;
}

void insertEffectItemAt(Effect *item, uint32_t index){
    // 検索.
    stEffectListItem *current = findEffectListItem(index);
    if(current == 0){
        // data無し. 終端に追加.
        addEffectItem(item);
        return;
    }

    // 新規作成.
    stEffectListItem *newItem = (stEffectListItem*)malloc(sizeof(stEffectListItem));
    newItem->pEffect = item;
    newItem->pPrevEffect = current->pPrevEffect;
    newItem->pNextEffect = current;

    // 直前に追加.
    if(current->pPrevEffect != 0){
        current->pPrevEffect->pNextEffect = newItem;
    }
    current->pPrevEffect = newItem;
    effectListSize++;
}

stEffectListItem* findEffectListItem(uint32_t index){
    int i;
    stEffectListItem *current = effectListHead;

    for(i=0; i<index; i++){
        current = current->pNextEffect;
        if(current == 0){
            break;
        }
    }

    return current;
}

stEffectListItem* lastEffectListItem(void){
    stEffectListItem *current = effectListHead;
    while(current != 0 && current->pNextEffect != 0){
        current = current->pNextEffect;
    }

    return current;
}

uint32_t getEffectListSize(void){
    return effectListSize;
}