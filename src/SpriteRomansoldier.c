#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_roman_blink[] = {2, 0,1};
const UINT8 a_roman_h[] = {2, 1,2};
const UINT8 a_roman_u[] = {2, 3,4};

extern UINT8 mission_killed;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

void START() {
    SetSpriteAnim(THIS, a_roman_h, 8u);
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
    struct SoldierData* romansoldier_data = (struct SoldierData*) THIS->custom_data;
    romansoldier_data->configured = 0;
    romansoldier_data->frmskip = 0;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct SoldierData* romansoldier_data = (struct SoldierData*) THIS->custom_data;
    switch(romansoldier_data->configured){
        case 0:
            return;
        break;
        case 1://horizontal
            SetSpriteAnim(THIS, a_roman_h, romansoldier_data->frmskip_max >> 1);
            romansoldier_data->vx = 1;
            romansoldier_data->configured = 3;
            return;
        break;
        case 2://vertical
            SetSpriteAnim(THIS, a_roman_h, romansoldier_data->frmskip_max >> 1);
            romansoldier_data->vy = 1;
            romansoldier_data->configured = 3;
            return;
        break;
        case 3:
            romansoldier_data->frmskip++;
            if(romansoldier_data->frmskip >= romansoldier_data->frmskip_max){
                romansoldier_data->frmskip = 0;
            }
        break;
        case 4://activate dieing
            mission_killed++;
            romansoldier_data->vx = 40;//usato come countdown di morte
            romansoldier_data->configured = 5;
            SetSpriteAnim(THIS, a_roman_blink, 24u);
            return;
        break;
        case 5://dieing
            romansoldier_data->vx--;
            if(romansoldier_data->vx <= 0){
                if(romansoldier_data->reward != NOITEM){
                    item_spawn(romansoldier_data->reward, THIS->x + 2u, THIS->y);
                }
                SpriteManagerRemoveSprite(THIS);
            }
            return;
        break;
    }
    if(romansoldier_data->frmskip > 0){return;}
    UINT8 romansoldier_coll_t = TranslateSprite(THIS, romansoldier_data->vx << delta_time, romansoldier_data->vy << delta_time);
    if(romansoldier_coll_t){
        if(romansoldier_data->vx != 0){
            romansoldier_data->vx = -romansoldier_data->vx;
            THIS->mirror = NO_MIRROR;
            if(romansoldier_data->vx < 0){
                THIS->mirror = V_MIRROR;
            }
        }
        if(romansoldier_data->vy != 0){
            romansoldier_data->vy = -romansoldier_data->vy;
            SetSpriteAnim(THIS, a_roman_h, romansoldier_data->frmskip_max >> 1);
            if(romansoldier_data->vy < 0){
                SetSpriteAnim(THIS, a_roman_u, romansoldier_data->frmskip_max >> 1);
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
                    if(romansoldier_data->configured < 4){
                        romansoldier_data->configured = 4;
                    }
                break;
            }
        }
    }
}

void DESTROY() {
}