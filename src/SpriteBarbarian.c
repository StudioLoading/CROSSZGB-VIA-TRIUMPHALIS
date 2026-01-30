#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_barbarian_blink[] = {2, 0,1};
const UINT8 a_barbarian_h[] = {2, 1,2};
const UINT8 a_barbarian_u[] = {2, 3,4};

extern UINT8 mission_killed;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

void START() {
    SetSpriteAnim(THIS, a_barbarian_h, 8u);
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
    struct SoldierData* barbarian_data = (struct SoldierData*) THIS->custom_data;
    barbarian_data->configured = 0;
    barbarian_data->frmskip = 0;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct SoldierData* barbarian_data = (struct SoldierData*) THIS->custom_data;
    switch(barbarian_data->configured){
        case 0:
            return;
        break;
        case 1://horizontal
            SetSpriteAnim(THIS, a_barbarian_h, barbarian_data->frmskip_max << 3);
            barbarian_data->vx = 1;
            barbarian_data->configured = 3;
            return;
        break;
        case 2://vertical
            SetSpriteAnim(THIS, a_barbarian_h, barbarian_data->frmskip_max << 1);
            barbarian_data->vy = 1;
            barbarian_data->configured = 3;
            return;
        break;
        case 3:
            barbarian_data->frmskip++;
            if(barbarian_data->frmskip >= barbarian_data->frmskip_max){
                barbarian_data->frmskip = 0;
            }
        break;
        case 4://activate dieing
            mission_killed++;
            barbarian_data->vx = 40;//usato come countdown di morte
            barbarian_data->configured = 5;
            SetSpriteAnim(THIS, a_barbarian_blink, 24u);
            return;
        break;
        case 5://dieing
            barbarian_data->vx--;
            if(barbarian_data->vx <= 0){
                SpriteManagerRemoveSprite(THIS);
            }
            return;
        break;
    }
    if(barbarian_data->frmskip > 0){return;}
    UINT8 romansoldier_coll_t = TranslateSprite(THIS, barbarian_data->vx << delta_time, barbarian_data->vy << delta_time);
    if(romansoldier_coll_t){
        if(barbarian_data->vx != 0){
            barbarian_data->vx = -barbarian_data->vx;
            THIS->mirror = NO_MIRROR;
            if(barbarian_data->vx < 0){
                THIS->mirror = V_MIRROR;
            }
        }
        if(barbarian_data->vy != 0){
            barbarian_data->vy = -barbarian_data->vy;
            SetSpriteAnim(THIS, a_barbarian_h, barbarian_data->frmskip_max << 1);
            if(barbarian_data->vy < 0){
                SetSpriteAnim(THIS, a_barbarian_u, barbarian_data->frmskip_max << 1);
            }
        }
    }
    
    //SPRITE COLLISION
    UINT8 scroll_rs_tile;
    Sprite* rsspr;
    SPRITEMANAGER_ITERATE(scroll_rs_tile, rsspr) {
        if(CheckCollision(THIS, rsspr)) {
            switch(rsspr->type){
                case SpriteItemlance:
                case SpriteItemgladio:
                case SpriteFlame:
                    if(barbarian_data->configured < 4){
                        barbarian_data->configured = 4;
                    }
                break;
            }
        }
    }
}

void DESTROY() {    
    struct SoldierData* barbarian_data = (struct SoldierData*) THIS->custom_data;
    if(barbarian_data->reward != NOITEM){
        item_spawn(barbarian_data->reward, THIS->x + 2u, THIS->y);
    }
}