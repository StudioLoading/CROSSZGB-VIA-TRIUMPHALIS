#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_statue_h[] = {1, 1};

void START(void){
    SetSpriteAnim(THIS, a_statue_h, 1u);
    THIS->lim_x = 10000;
    THIS->lim_y = 10000;
    if(_cpu != CGB_TYPE){
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(void){
}

void DESTROY(void){
}