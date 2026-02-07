#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern UINT8 mission_killed;

const UINT8 a_priest_down[] = {1, 1};
const UINT8 a_priest_blink[] = {2, 0,1};

extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

void START(void){
    SetSpriteAnim(THIS, a_priest_down, 8u);
    THIS->lim_x = 2000;
    THIS->lim_y = 2000;
}

void UPDATE(void){
    struct SoldierData* priest_data = (struct SoldierData*) THIS->custom_data;
    switch(priest_data->configured){
        case 0:
            return;
        break;
        case 1://horizontal
            SetSpriteAnim(THIS, a_priest_down, 8);
            priest_data->vx = 1;
            priest_data->configured = 3;
            return;
        break;
        case 4://activate dieing
            priest_data->vx = 20;//usato come countdown di morte
            priest_data->configured = 5;
            mission_killed++;
            return;
        break;
        case 5://dieing
            priest_data->vx--;
            if(priest_data->vx <= 0){
                SpriteManagerRemoveSprite(THIS);
            }
            return;
        break;
    }
}

void DESTROY(void){
    struct SoldierData* greek_data = (struct SoldierData*) THIS->custom_data;
    if(greek_data->reward != NOITEM){
        item_spawn(greek_data->reward, THIS->x + 2u, THIS->y);
    }
    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y);
}