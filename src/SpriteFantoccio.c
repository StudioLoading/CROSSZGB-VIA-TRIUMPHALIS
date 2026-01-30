#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "CircleMath.h"
#include "custom_datas.h"

extern INT8 fantoccio_counter;

const UINT8 a_fantoccio_idle[] = {1, 0};
const UINT8 a_fantoccio_hit[] = {6, 0,1,2,3,4,5};

void fantoccio_move(Sprite* s_fantoccio_arg) BANKED;

void START() {
    SetSpriteAnim(THIS, a_fantoccio_idle, 24);
    THIS->mirror = NO_MIRROR;
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
    struct FantoccioData* f_data = (struct FantoccioData*) THIS->custom_data;
    f_data->fantoccio_counter = 0;
}

void UPDATE() {
    struct FantoccioData* f_data = (struct FantoccioData*) THIS->custom_data;
    if(f_data->fantoccio_counter >= 4){
        f_data->fantoccio_counter--;
        if(f_data->fantoccio_counter < 4){
            SetSpriteAnim(THIS, a_fantoccio_idle, 24);
            f_data->fantoccio_counter = 0;
        }
    }
}

void DESTROY() {
}

void fantoccio_move(Sprite* s_fantoccio_arg) BANKED{
    SetSpriteAnim(s_fantoccio_arg, a_fantoccio_hit, 32);
}