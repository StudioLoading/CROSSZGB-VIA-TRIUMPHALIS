#include "Banks/SetAutoBank.h"

#include <gbdk/platform.h>
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define DISTANCE_X_POSITIVE 30
#define DISTANCE_X_NEGATIVE 28
#define DELTA_DISTANCE_MAX 12

const UINT8 a_biga_h[] = {3, 7,1,2};
const UINT8 a_biga_hit[] = {6, 0,5,0,1,0,2};
const UINT8 a_biga_down[] = {2, 3,4};
const UINT8 a_biga_up[] = {2, 5,6};

extern Sprite* s_horse;
extern INT16 stamina_current;
extern INT8 vx;
extern INT8 vy;
extern UINT16 euphoria_min;
extern UINT16 euphoria_max;
extern INT8 flag_die;
extern MISSION current_mission;
extern MISSION_STEP current_step;
extern UINT8 turn;

extern void pickup(Sprite* s_arg_item) BANKED;


void START(void){
    SetSpriteAnim(THIS, a_biga_h, 32u);
    THIS->lim_x = 100;
    THIS->lim_y = 100;
    if(_cpu != CGB_TYPE){
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(void){
    if(flag_die){ return; }
    UINT16 final_pos_x = s_horse->x + (s_horse->coll_w >> 1) - DISTANCE_X_POSITIVE; //if vx >= 0
    if (s_horse->mirror == V_MIRROR){ final_pos_x = s_horse->x + (s_horse->coll_w >> 1) + DISTANCE_X_NEGATIVE;}
    UINT16 final_pos_y = s_horse->y + (s_horse->coll_h >> 1) - 4; //if vy == 0
    if(vy < 0){ final_pos_y = s_horse->y + (s_horse->coll_h >> 1) + 8;
    }else if(vy > 0){final_pos_y = s_horse->y - 8;}
    //LIMIT X
        INT16 delta_x = s_horse->x - THIS->x;
        UINT8 off_limit_x = 0;
        if(delta_x >= (DISTANCE_X_POSITIVE + DELTA_DISTANCE_MAX)){ //troppo lontano indietro a sinistra! metto un limite
            THIS->x = s_horse->x - (DISTANCE_X_POSITIVE + DELTA_DISTANCE_MAX);
            off_limit_x = 1u;
        }
        if(delta_x <= -((DISTANCE_X_NEGATIVE + DELTA_DISTANCE_MAX))){ // troppo lontano indietro a destra! metto un limite
            THIS->x = s_horse->x + (DISTANCE_X_NEGATIVE + DELTA_DISTANCE_MAX) - 4;
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
                SetSpriteAnim(THIS, a_biga_down, 16);
            }else if (vy < -1){//sto andando in su
                SetSpriteAnim(THIS, a_biga_up, 16);
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

void DESTROY(void){
}