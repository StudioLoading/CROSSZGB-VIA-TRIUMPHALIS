#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_greek_blink[] = {2, 0,1};
const UINT8 a_greek_h[] = {2, 1,2};
const UINT8 a_greek_u[] = {2, 3,4};

extern UINT8 mission_killed;
extern Sprite* s_horse;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

void START() {
    SetSpriteAnim(THIS, a_greek_h, 8u);
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
    struct SoldierData* greeksoldier_data = (struct SoldierData*) THIS->custom_data;
    greeksoldier_data->configured = 0;
    greeksoldier_data->frmskip = 0;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    struct SoldierData* greeksoldier_data = (struct SoldierData*) THIS->custom_data;
    switch(greeksoldier_data->configured){
        case 0:
            return;
        break;
        case 1://horizontal
            SetSpriteAnim(THIS, a_greek_h, greeksoldier_data->frmskip_max >> 1);
            greeksoldier_data->vx = 1;
            greeksoldier_data->configured = 3;
            return;
        break;
        case 2://vertical
            SetSpriteAnim(THIS, a_greek_h, greeksoldier_data->frmskip_max >> 1);
            greeksoldier_data->vy = 1;
            greeksoldier_data->configured = 3;
            return;
        break;
        case 3:
            greeksoldier_data->frmskip++;
            if(greeksoldier_data->frmskip >= greeksoldier_data->frmskip_max){
                greeksoldier_data->frmskip = 0;
            }
        break;
        case 4://activate dieing
            mission_killed++;
            greeksoldier_data->vx = 40;//usato come countdown di morte
            greeksoldier_data->configured = 5;
            SetSpriteAnim(THIS, a_greek_blink, 24u);
            return;
        break;
        case 5://dieing
            greeksoldier_data->vx--;
            if(greeksoldier_data->vx <= 0){
                if(greeksoldier_data->reward != NOITEM){
                    item_spawn(greeksoldier_data->reward, THIS->x + 2u, THIS->y);
                }
                SpriteManagerRemoveSprite(THIS);
            }
            return;
        break;
        case 6://horizontal runner
            if(s_horse->x > THIS->x){
                INT16 delta_x = s_horse->x - THIS->x;
                if(delta_x > 24){
                    SetSpriteAnim(THIS, a_greek_h, greeksoldier_data->frmskip_max >> 1);
                    greeksoldier_data->vx = 1;
                    greeksoldier_data->configured = 7;
                }
            }
            return;
        break;
        case 7://horizontal runner running
            greeksoldier_data->frmskip++;
            if(greeksoldier_data->frmskip >= greeksoldier_data->frmskip_max){
                if(THIS->y < (s_horse->y - 4)){
                    THIS->y++;
                }else if(THIS->y > (s_horse->y + 8)){
                    THIS->y--;
                }
                greeksoldier_data->frmskip = 0;
                INT16 delta_x = THIS->x - s_horse->x;
                if(delta_x > 640){//se troppo lontano in avanti e sta running, lo elimino
                    greeksoldier_data->configured = 4;
                }
                delta_x = s_horse->x - THIS->x;
                if(delta_x > 38){
                    THIS->x = s_horse->x - 38u;
                }
            }
        break;
    }
    if(greeksoldier_data->frmskip > 0){return;}
    UINT8 greeksoldier_coll_t = TranslateSprite(THIS, greeksoldier_data->vx << delta_time, greeksoldier_data->vy << delta_time);
    if(greeksoldier_coll_t && greeksoldier_data->configured == 7){
        THIS->x += greeksoldier_data->vx;
    }
    if(greeksoldier_coll_t && greeksoldier_data->configured != 7){
        if(greeksoldier_data->vx != 0){
            greeksoldier_data->vx = -greeksoldier_data->vx;
            THIS->mirror = NO_MIRROR;
            if(greeksoldier_data->vx < 0){
                THIS->mirror = V_MIRROR;
            }
        }
        if(greeksoldier_data->vy != 0){
            greeksoldier_data->vy = -greeksoldier_data->vy;
            SetSpriteAnim(THIS, a_greek_h, greeksoldier_data->frmskip_max >> 1);
            if(greeksoldier_data->vy < 0){
                SetSpriteAnim(THIS, a_greek_u, greeksoldier_data->frmskip_max >> 1);
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
                    if(greeksoldier_data->configured < 4 || greeksoldier_data->configured == 7){
                        greeksoldier_data->configured = 4;
                    }
                break;
            }
        }
    }
}

void DESTROY() {
}