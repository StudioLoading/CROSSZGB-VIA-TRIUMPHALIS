#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_exclamation[] = {1,1};


void START() {
    SetSpriteAnim(THIS, a_exclamation, 1u);
    struct SoldierData* exclamation_data = (struct SoldierData*)THIS->custom_data;
    exclamation_data->vx = 80;
}

void UPDATE() {
    struct SoldierData* exclamation_data = (struct SoldierData*)THIS->custom_data;
    exclamation_data->vx--;
    if(exclamation_data->vx <= 0){
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY() {
}