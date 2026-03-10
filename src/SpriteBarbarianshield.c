#include "Banks/SetAutoBank.h"

#include <gbdk/platform.h>
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


const UINT8 a_barbarianshield_blink[] = {4, 1,0,2,0};
const UINT8 a_barbarianshield_h[] = {2, 1,2};
const UINT8 a_barbarianshield_u[] = {2, 3,4};

extern Sprite* s_horse;
extern Sprite* spawn_points(UINT8 arg_points, UINT16 arg_x, UINT16 arg_y) BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

void START(void){
    SetSpriteAnim(THIS, a_barbarianshield_h, 8u);
    THIS->lim_x = 3000;
    THIS->lim_y = 3000;
    struct SoldierData* barbarianshield_data = (struct SoldierData*) THIS->custom_data;
    barbarianshield_data->configured = 0;
    barbarianshield_data->frmskip = 0;
    if(_cpu != CGB_TYPE){
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(void){
    struct SoldierData* barbarianshield_data = (struct SoldierData*) THIS->custom_data;
    switch(barbarianshield_data->configured){
        case 0:
            return;
        break;
        case 1://hold, andrà in alto
            if(s_horse->x < THIS->x){ THIS->mirror = V_MIRROR;}
            if(s_horse->x > THIS->x){ THIS->mirror = NO_MIRROR;}
            return;
        break;
        case 2://hold, andrà in basso
            if(s_horse->x < THIS->x){ THIS->mirror = V_MIRROR;}
            if(s_horse->x > THIS->x){ THIS->mirror = NO_MIRROR;}
            return;
        break;
        case 3://running up-left
            barbarianshield_data->vy = -1;
            barbarianshield_data->vx = -1;
            barbarianshield_data->frmskip = 100u;
            SetSpriteAnim(THIS, a_barbarianshield_u, 24u);
            SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 2u);
            barbarianshield_data->configured = 7;
        break;
        case 4://running up-right
            barbarianshield_data->vy = -1;
            barbarianshield_data->vx = 1;
            barbarianshield_data->frmskip = 100u;
            SetSpriteAnim(THIS, a_barbarianshield_u, 24u);
            SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 2u);
            barbarianshield_data->configured = 7;
        break;
        case 5://running down-left
            barbarianshield_data->vy = 1;
            barbarianshield_data->vx = -1;
            barbarianshield_data->frmskip = 100u;
            SetSpriteAnim(THIS, a_barbarianshield_h, 24u);
            SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 2u);
            barbarianshield_data->configured = 7;
        break;
        case 6://running down-right
            barbarianshield_data->vy = 1;
            barbarianshield_data->vx = 1;
            barbarianshield_data->frmskip = 100u;
            SetSpriteAnim(THIS, a_barbarianshield_h, 24u);
            SpriteManagerAdd(SpritePuff, THIS->x, THIS->y + 2u);
            barbarianshield_data->configured = 7;
        break;
        case 7://running
            THIS->x += barbarianshield_data->vx;
            THIS->y += barbarianshield_data->vy;
            barbarianshield_data->frmskip--;
            if(barbarianshield_data->frmskip < 10){
                SpriteManagerRemoveSprite(THIS);
            }
        break;
        case 9://die
            struct SoldierData* soldier_data = (struct SoldierData*) THIS->custom_data;
            if(soldier_data->reward != NOITEM){
                item_spawn(soldier_data->reward, THIS->x + 2u, THIS->y);
            }
            Sprite* s_puff = SpriteManagerAdd(SpritePuff, THIS->x, THIS->y);
            spawn_points(soldier_data->points, THIS->x, THIS->y);
            SpriteManagerRemoveSprite(THIS);
        break;
    }
}

void DESTROY(void){
}