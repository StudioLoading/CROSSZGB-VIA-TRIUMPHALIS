#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"

const UINT8 a_marcus_idle[] = {2, 1,2};
const UINT8 a_marcus_h[] = {2, 1,2};
const UINT8 a_marcus_u[] = {2, 3,4};


extern INT8 velocity_counter; //praticamente il frm_skip_max
extern INT8 velocity;
extern INT8 vy;
extern INT8 vx;
extern INT8 frm_skip;
extern INT8 flag_hit;
extern INT8 counter_hit;
extern INT8 flag_die;
extern UINT8 orme_spawned;
extern INT16 time_current;
extern UINT8 track_ended;
extern INT8 mission_completed;

void marcus_update_anim() BANKED;

extern void update_hp_max() BANKED;
extern void pickup(Sprite* s_arg_item) BANKED;

void START() {
    SetSpriteAnim(THIS, a_marcus_idle, 8u);
    flag_hit = 0;
    vx = 0;
    vy = 0;
    velocity_counter = 0;
    frm_skip = 1;
    velocity = 1;
    counter_hit = 0;
    orme_spawned = 0u;
}

void marcus_update_anim() BANKED{
    if(vx == 0 && vy == 0){
        SetSpriteAnim(THIS, a_marcus_idle, 4u);
    }else{
        SetSpriteAnim(THIS, a_marcus_h, 12u);
        if(vy < 0){
            SetSpriteAnim(THIS, a_marcus_u, 12u);
        }
    }
}

void UPDATE() {
    INT8 new_vx = 0;
    INT8 new_vy = 0;
    if(KEY_PRESSED(J_RIGHT)){ new_vx = 1;}
    if(KEY_PRESSED(J_LEFT)){ new_vx = -1;}
    if(KEY_PRESSED(J_UP)){ new_vy = -1;}
    if(KEY_PRESSED(J_DOWN)){ new_vy = 1;}
    if(new_vy != vy || new_vx != vx){
        vx = new_vx;
        vy = new_vy;
        marcus_update_anim();
    }
    //FRAMESKIP & MOVEMENT
        if(frm_skip > 0){frm_skip--;}
        if(frm_skip == 0){
            frm_skip = velocity_counter;
            if(flag_die){ return; }
            UINT8 marcus_coll = TranslateSprite(THIS, vx << delta_time, vy << delta_time);
            if(vx != 0 || vy != 0){
                time_current--;
            }
            //ORME
                if(THIS->anim_frame == 0){
                    if(orme_spawned == 0 && (vx != 0 || vy != 0)){ 
                        UINT16 orma_posy = THIS->y - 1;
                        orme_spawned = 1;
                        if(vx > 0){
                            SpriteManagerAdd(SpriteStep, THIS->x - 2, orma_posy);
                        }else  if(vx < 0){
                            SpriteManagerAdd(SpriteStep, THIS->x + 4, orma_posy);
                        }
                    }
                }else{orme_spawned = 0;}
            //COLLISIONE
                if(marcus_coll){//collido con tile ambiente di collisione
                
                }else{
            //OVERTILE
                    UINT8 tile_over = GetScrollTile((THIS->x + 4) >> 3, (THIS->y+4) >> 3);
                    if(vx < 0){
                        tile_over = GetScrollTile((THIS->x + 4) >> 3, (THIS->y+4) >> 3);
                    }
                    switch(tile_over){
                        case 8u: case 9u: case 10u: case 11u:
                        case 12u: case 13u:
                            update_hp_max();
                        break;
                    }
                }

        }
    //SPRITE MIRROR
        if(vx > 0){
            THIS->mirror = NO_MIRROR;
        }else if (vx < 0){
            THIS->mirror = V_MIRROR;
        }
    //SPRITE COLLISION
        UINT8 scroll_m_tile;
        Sprite* imspr;
        SPRITEMANAGER_ITERATE(scroll_m_tile, imspr) {
            if(CheckCollision(THIS, imspr)) {
                switch(imspr->type){
                    case SpriteConfigwhip:
                    case SpriteConfigreins:
                    case SpriteConfigwheel:
                    case SpriteConfigelm:{
                        struct ItemData* item_data = (struct ItemData*) imspr->custom_data;
                        if(item_data->configured == 2){
                            pickup(imspr);
                        }
                    }break;
                    case SpriteMarcushorse:
                        track_ended = 1;
                        mission_completed = 1;
                    break;
                }
            }
        }
}

void DESTROY() {
}