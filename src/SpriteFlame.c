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

const UINT8 a_flame[] = {4, 0,1,2,3};
const UINT8 a_fog[] = {4, 4,5,6,7};

void START() {
    SetSpriteAnim(THIS, a_flame, 24u);
    THIS->lim_x = 1000;
    struct FlameData* fire_data = (struct FlameData*) THIS->custom_data;
    fire_data->vx = 0;
    fire_data->hp = 0;
    fire_data->dropped = -1;
}

void UPDATE() {
    struct FlameData* fire_data = (struct FlameData*) THIS->custom_data;
    switch(fire_data->dropped){
        case 2://just dropped
            fire_data->hp = 2;
            fire_data->dropped = 1;
        break;
        case 1:{
            TranslateSprite(THIS, fire_data->vx << delta_time, 0);
            fire_data->hp--;
            if(fire_data->hp <= 0){
                fire_data->dropped = 0;
            }
            UINT8 scroll_f_tile;
            Sprite* ifspr;
            SPRITEMANAGER_ITERATE(scroll_f_tile, ifspr) {
                if(CheckCollision(THIS, ifspr)) {
                    switch(ifspr->type){
                        case SpriteStraw:{
                            SpriteManagerRemoveSprite(ifspr);
                            fire_data->dropped = 2;
                            THIS->y += 4;
                            Sprite* s_fire2 = SpriteManagerAdd(SpriteFlame, ifspr->x + 16, ifspr->y + 2);
                            struct FlameData* fire_data2 = (struct FlameData*) s_fire2->custom_data;
                            fire_data2->dropped = 2;
                        }break;
                    }
                }
            }
        }break;
    }
}

void DESTROY() {
}