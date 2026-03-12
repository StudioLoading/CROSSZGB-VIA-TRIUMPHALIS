#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern INT16 stamina_current;
extern INT16 euphoria_max;
extern Sprite* s_horse;

UINT8 dust_spawned = 0u;

const UINT8 a_dust[] = {4, 1,2,3,4,0};

void spawn_dust(UINT8 arg_force) BANKED;

void spawn_dust(UINT8 arg_force) BANKED{
    if(dust_spawned == 0 && (
        arg_force || stamina_current > (euphoria_max + 60u))){
        dust_spawned = 1u;
        if(s_horse->mirror == NO_MIRROR){
            Sprite* s_dust = SpriteManagerAdd(SpriteDust, s_horse->x - 5u, s_horse->y - 6u);
            s_dust ->mirror = V_MIRROR;
        }else{
            SpriteManagerAdd(SpriteDust, s_horse->x + 20u, s_horse->y - 6u);
        }
    }
}

void START(void){
    SetSpriteAnim(THIS, a_dust, 18u);
    THIS->lim_x = 12;
    THIS->lim_y = 12;
    struct SoldierData* dust_data = (struct SoldierData*) THIS->custom_data;
    dust_data->vx = 0;
    dust_data->vy = 0;
    dust_data->frmskip = 0;
    dust_data->frmskip_max = 0;
    dust_data->configured = 0;
    dust_data->reward = NOITEM;
    dust_data->points = 0;
    dust_spawned = 1u;
    if(_cpu != CGB_TYPE){
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(void){
    if(THIS->anim_frame == 3){
        SpriteManagerRemoveSprite(THIS);
    }
}

void DESTROY(void){
    dust_spawned = 0u;
}