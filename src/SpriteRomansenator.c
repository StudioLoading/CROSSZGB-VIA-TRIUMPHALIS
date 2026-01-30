#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_senator_h[] = {4, 1,2,2,2};

void START() {
    SetSpriteAnim(THIS, a_senator_h, 8u);
    THIS->lim_x = 10000;
    THIS->lim_y = 10000;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    SetSpriteAnim(THIS, a_senator_h, 8u);
    THIS->lim_x = 10000;
    THIS->lim_y = 10000;
}

void DESTROY() {
}