#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#define DELTA_X_INC_CGB 2
#define DELTA_X_INC_DMG 3

INT8 delta_x_inc = 0;

extern Sprite* s_horse;
extern INT8 vx;
extern UINT8 track_ended;
extern UINT8 flag_turn_on_tremble;

void START() {
    THIS->lim_x = 1000;
    THIS->lim_y = 1000;
    delta_x_inc = DELTA_X_INC_CGB;
    if(_cpu != CGB_TYPE){
        delta_x_inc = DELTA_X_INC_DMG;
    }
}

void UPDATE() {
    if(track_ended == 1){
        return;
    }
    if(flag_turn_on_tremble == 0){
        THIS->y = s_horse->y - 18;
    }
    INT8 delta_x = 0;
    if(vx > 1){
        UINT16 final_camera_x = s_horse->x + 56;
        UINT16 final_camera_x_min = final_camera_x -8;
        UINT16 final_camera_x_max = final_camera_x +8;
        if(THIS->x > final_camera_x_max){delta_x=-delta_x_inc;}
        else if(THIS->x < final_camera_x_min){delta_x=+delta_x_inc;}
    }
    if(vx < -1){
        UINT16 final_camera_x = s_horse->x - 40;
        UINT16 final_camera_x_min = final_camera_x -8;
        UINT16 final_camera_x_max = final_camera_x +8;
        if(THIS->x > final_camera_x_max){delta_x=-delta_x_inc;}
        else if(THIS->x < final_camera_x_min){delta_x=+delta_x_inc;}
    }
    if(delta_x){
        INT16 camera_horse_delta_x = THIS->x - s_horse->x;
        if(camera_horse_delta_x < 60 && camera_horse_delta_x > -60){
            THIS->x += delta_x;
        }else{THIS->x += vx;}
    }
}

void DESTROY() {
    THIS->y--;
    THIS->x++;
}