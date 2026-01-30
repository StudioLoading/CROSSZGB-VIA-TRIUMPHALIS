#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_killer_v[] = {1, 1};
const UINT8 a_killer_v_blink[] = {2, 0,1};
const UINT8 a_killer_hidden[] = {1, 0};

extern Sprite* s_horse;

void START() {
    SetSpriteAnim(THIS, a_killer_hidden, 4u);
    THIS->lim_x = 2000;
    THIS->lim_y = 2000;
    struct KillerData* killer_data = (struct KillerData*) THIS->custom_data;
    killer_data->configured = 99;
    killer_data->timeout = 1;
    killer_data->time_hidden = 100;
    killer_data->time_visible = 120;
    killer_data->time_attack = 60;
    killer_data->time_blink = 40;
}

void UPDATE() {
    struct KillerData* killer_data = (struct KillerData*) THIS->custom_data;
    /*
    UINT8 configured;
    UINT8 timeout;
    UINT8 time_hidden;
    UINT8 time_visible;
    UINT8 time_attack;*/
    switch(killer_data->configured){
        case 99:{
            INT16 distance_delta_x = THIS->x - s_horse->x;
            if(distance_delta_x < 0){ distance_delta_x = -distance_delta_x;}
            if(distance_delta_x < 160){
                INT16 distance_delta_y = s_horse->y - THIS->y;
                if(distance_delta_y > 16 && distance_delta_y < 60){//gli è poco sotto
                    killer_data->configured = 0;
                }
            }
        }break;
        case 0: //hidden
            killer_data->timeout--;
            if(killer_data->timeout <= 0){
                killer_data->timeout = killer_data->time_blink;
                SetSpriteAnim(THIS, a_killer_v_blink, 32u);
                killer_data->configured = 1u;
            }
        break;
        case 1: //blink
            killer_data->timeout--;
            if(killer_data->timeout <= 0){
                killer_data->timeout = killer_data->time_visible;
                SetSpriteAnim(THIS, a_killer_v, 32u);
                Sprite* s_killer_weapon = SpriteManagerAdd(SpriteEitemlance, THIS->x, THIS->y + 14);
                struct ItemData* killer_weapon_data = (struct ItemData*) s_killer_weapon->custom_data;
                killer_weapon_data->itemtype = ENEMY_LANCE;
                killer_weapon_data->vx = 0;
                killer_weapon_data->vy = 1;
                killer_weapon_data->configured = 3;
                killer_data->configured = 2u;
            }
        break;
        case 2: //visible
            killer_data->timeout--;
            if(killer_data->timeout == killer_data->instant_attack){
                //attack! lancia lancia
            }
            if(killer_data->timeout <= 0){
                killer_data->timeout = killer_data->time_blink;
                SetSpriteAnim(THIS, a_killer_v_blink, 32u);
                killer_data->configured = 3u;
            }
        break;
        case 3: //blink
            killer_data->timeout--;
            if(killer_data->timeout <= 0){
                SpriteManagerRemoveSprite(THIS);
            }
        break;
    }
}

void DESTROY() {
}