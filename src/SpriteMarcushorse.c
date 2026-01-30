#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"

const UINT8 a_marcushorse_idle[] = {8, 0,1,2,3,0,0,1,2};

void START() {
    THIS->lim_x = 4000;
    THIS->lim_y = 4000;
    SetSpriteAnim(THIS, a_marcushorse_idle, 8u);
}

void UPDATE() {
}

void DESTROY() {
}