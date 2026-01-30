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


const UINT8 a_rollingstone_idle[] = {8, 0,1,0,2,0,3,0,4};


void START() {
    SetSpriteAnim(THIS, a_rollingstone_idle, 48);
    THIS->lim_x = 100;
    THIS->lim_y = 100;
}

void UPDATE() {
    THIS->y++;
    struct RollingStoneData* rollingstone_data = (struct RollingStoneData*) THIS->custom_data;
    rollingstone_data->frmskip_y--;
    if(rollingstone_data->frmskip_y <= 0){
        THIS->x += rollingstone_data->verse_x;
        rollingstone_data->frmskip_y = rollingstone_data->max_frmskip_y;
    }
}

void DESTROY() {
}
