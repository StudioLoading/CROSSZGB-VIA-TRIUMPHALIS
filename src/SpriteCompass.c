#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#define COMPASS_COUNTER_MAX 150

const UINT8 a_compass_hidden[] = {1, 0};
const UINT8 a_compass_h[] = {1, 4};
const UINT8 a_compass_33[] = {1, 1};
const UINT8 a_compass_66[] = {1, 2};
const UINT8 a_compass_v[] = {1, 3};
const UINT8 a_compass_h_blink[] = {2, 0,4};
const UINT8 a_compass_33_blink[] = {2, 0,1};
const UINT8 a_compass_66_blink[] = {2, 0,2};
const UINT8 a_compass_v_blink[] = {2, 0,3};

extern INT8 vx;
extern INT8 vy;
extern INT8 cos;
extern INT8 sin;
extern Sprite* s_horse;
extern UINT8 track_ended;

INT8 compass_counter = 0;

void START() {
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
    compass_counter = 0;
}

void UPDATE() {
    //mettilo allo scroll_target che è SpriteCamera
        THIS->x = s_horse->x;
        THIS->y = s_horse->y - 24;
        if(track_ended == 1){
            SpriteManagerRemoveSprite(THIS);
        }
    //animation
        if(KEY_PRESSED(J_LEFT) || KEY_PRESSED(J_RIGHT)){
            compass_counter = 0;
        }
        if(compass_counter > COMPASS_COUNTER_MAX){
            SetSpriteAnim(THIS, a_compass_hidden, 1);
            return;
        }
        compass_counter++;
        INT8 using_cos = cos;
        INT8 using_sin = sin;
        if(cos < 0) {using_cos = -cos;}
        if(sin < 0) {using_sin = -sin;}
        if(using_sin >= 0 && using_sin <= 30){//tratto come se stesse andando orizzontale
            SetSpriteAnim(THIS, a_compass_h, 24);
        }else if(using_sin > 30 && using_sin < 65){// tratto come se stesse andando a 33 gradi
            SetSpriteAnim(THIS, a_compass_33, 24);
        }else if(using_sin > 65 && using_sin < 110){// tratto come se stesse andando a 66 gradi
            SetSpriteAnim(THIS, a_compass_66, 24);
        }else if(using_sin > 110){ // tratto come se stesse andando orizzontale destra
            SetSpriteAnim(THIS, a_compass_v, 24);
        }
        if(sin >= 0){
            if(cos >= 0){ THIS->mirror = NO_MIRROR;}
            else{ THIS->mirror = V_MIRROR; }
        }
        if(sin < 0){
            if(cos >= 0){ THIS->mirror = H_MIRROR; }
            else{ THIS->mirror = HV_MIRROR; }
        }
    }

void DESTROY() {
}