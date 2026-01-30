#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define DISTANCE_X_POSITIVE 20
#define DISTANCE_X_NEGATIVE 28
#define DELTA_DISTANCE_MAX 12

const UINT8 a_biga_h[] = {3, 5,1,2};
const UINT8 a_biga_hit[] = {6, 0,5,0,1,0,2};
const UINT8 a_biga_down[] = {1, 3};
const UINT8 a_biga_up[] = {1, 4};

extern Sprite* s_horse;
extern INT16 stamina_current;
extern INT8 vx;
extern INT8 vy;
extern UINT16 euphoria_min;
extern UINT16 euphoria_max;
extern ITEM_TYPE weapon_atk;
extern ITEM_TYPE weapon_def;
extern INT8 flag_die;
extern MISSION current_mission;
extern INT8 mission_completed;
extern MISSION_STEP current_step;
extern Sprite* s_spawning_weapon;
extern UINT8 turn;

extern void update_weapon() BANKED;
extern void update_hp_max() BANKED;
extern void update_time_max() BANKED;
extern void pickup_config(ITEM_TYPE arg_pickedup) BANKED;

void pickup(Sprite* s_arg_item) BANKED;


void START() {
    SetSpriteAnim(THIS, a_biga_h, 32u);
    THIS->lim_x = 100;
    THIS->lim_y = 100;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    if(flag_die){ return; }
    UINT16 final_pos_x = s_horse->x - DISTANCE_X_POSITIVE; //if vx >= 0
    if (turn > 63 && turn < 192){ final_pos_x = s_horse->x + DISTANCE_X_NEGATIVE;}
    UINT16 final_pos_y = s_horse->y - 1; //if vy == 0
    if(vy < 0){ final_pos_y = s_horse->y + 8;
    }else if(vy > 0){final_pos_y = s_horse->y - 8;}
    //LIMIT X
        INT16 delta_x = s_horse->x - THIS->x;
        UINT8 off_limit_x = 0;
        if(delta_x >= (DISTANCE_X_POSITIVE + DELTA_DISTANCE_MAX)){ //troppo lontano indietro a sinistra! metto un limite
            THIS->x = s_horse->x - (DISTANCE_X_POSITIVE + DELTA_DISTANCE_MAX);
            off_limit_x = 1u;
        }
        if(delta_x <= -((DISTANCE_X_NEGATIVE + DELTA_DISTANCE_MAX))){ // troppo lontano indietro a destra! metto un limite
            THIS->x = s_horse->x + (DISTANCE_X_NEGATIVE + DELTA_DISTANCE_MAX);
            off_limit_x = 1;
        }
        if(off_limit_x == 0){ //se il limite non è stato infranto, mi sposto verso
        //la final position con una tolleranza di +-1 pixel
            if(THIS->x > (final_pos_x+1) ){THIS->x-=2;}
            else if (THIS->x < (final_pos_x -1 )){THIS->x+=2;}
        }
    //LIMIT y
        INT16 delta_y = s_horse->y - THIS->y;
        UINT8 off_limit_y = 0;
        if(delta_y >= 30){ //troppo lontano indietro in su! metto un limite
            THIS->y = s_horse->y - 15;
            off_limit_y = 1u;
        }
        if(delta_y < -30){ //troppo lontano indietro in giù! metto un limite
            THIS->y = s_horse->y + 15;
            off_limit_y = 1u;

        }
        if(off_limit_y == 0){ //se il limite non è stato infranto, mi sposto verso
            if(THIS->y > final_pos_y){THIS->y--;}
            else if (THIS->y < final_pos_y){THIS->y++;}
        }
    THIS->anim_speed = stamina_current >> 6;
    THIS->mirror = s_horse->mirror;
    //ANIMATION
        INT16 to_final_x = THIS->x - final_pos_x;
        SetSpriteAnim(THIS, a_biga_h, 1);
        if(to_final_x > 4){//devo andare a sinistra
            THIS->mirror = V_MIRROR;
        }
        if(stamina_current < euphoria_max && stamina_current > euphoria_min){
            if(vy > 1){ //sto andando in giù
                SetSpriteAnim(THIS, a_biga_down, 12);
            }else if (vy < -1){//sto andando in su
                SetSpriteAnim(THIS, a_biga_up, 12);
            }
        }
    //SPRITE COLLISION
        UINT8 scroll_b_tile;
        Sprite* ibspr;
        SPRITEMANAGER_ITERATE(scroll_b_tile, ibspr) {
            if(CheckCollision(THIS, ibspr)) {
                switch(ibspr->type){
                    case SpriteItemgladio:
                    case SpriteItemlance:
                    case SpriteItemfire:
                    case SpriteItemelmet:
                    case SpriteItemshield:
                    case SpriteItemcape:
                    case SpriteItemheart:
                    case SpriteItemglass:
                    case SpriteConfigwhip:
                    case SpriteConfigreins:
                    case SpriteConfigwheel:
                    case SpriteConfigelm:
                    case SpriteItempapirus:{
                        struct ItemData* item_data = (struct ItemData*) ibspr->custom_data;
                        if(item_data->configured == 2){
                            pickup(ibspr);
                        }
                    }break;
                }
            }
        }
}

void pickup(Sprite* s_arg_item) BANKED{
    struct ItemData* item_data = (struct ItemData*) s_arg_item->custom_data;
    switch(item_data->itemtype){
        case GLADIO: case LANCE: case FIRE:
            weapon_atk = item_data->itemtype;
            update_weapon();
        break;
        case ELMET: case SHIELD: case CAPE:
            weapon_def = item_data->itemtype;
            update_weapon();
        break;
        case HP:
            update_hp_max();
        break;
        case TIME:
            update_time_max();
        break;
        case GOLDEN_ELM:
        case GOLDEN_WHEEL:
        case GOLDEN_WHIP:
        case GOLDEN_REINS:
            pickup_config(item_data->itemtype);
        break;
        case PAPYRUS:
            if(current_mission == MISSIONSEA08 || current_mission == MISSIONGREECE12){
                current_step = EXIT;
                mission_completed = 1;
            }
            if(current_mission == MISSIONGREECE13){
                current_step = SENATOR_COLLIDED;
            }
        break;
    }    
    if(item_data->flag_continuous_spawning == 1){
        s_spawning_weapon = 0;
    }
    SpriteManagerRemoveSprite(s_arg_item);
}

void DESTROY() {
}