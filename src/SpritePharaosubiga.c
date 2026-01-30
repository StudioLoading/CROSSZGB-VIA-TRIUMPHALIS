#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


const UINT8 a_pharaosubiga_down[] = {2, 1,2};
const UINT8 a_pharaosubiga_down_blink[] = {2, 0,1};

INT16 pharaosubiga_timernet_current = 0u;
INT16 pharaosubiga_timernet_max = 320;//tengo qui così la posso modificare fuori

extern Sprite* s_gator;
extern Sprite* s_pharaobiga;
extern Sprite* s_horse;
extern INT8 pharaonet_vx;
extern INT8 pharaonet_vy;
extern INT8 pharaonet_frmskip_max;
extern INT8 pharaonet_frmskipy_max;
extern UINT8 pharaonet_collided_flag;
extern UINT8 end_game;

void pharaosubiga_throw_net() BANKED;
void pharaosubiga_change_status(INT8 arg_status, Sprite* arg_s_pharaosubiga) BANKED;

void START() {
    SetSpriteAnim(THIS, a_pharaosubiga_down, 8u);
    THIS->lim_x = 2000;
    THIS->lim_y = 2000;
    if(current_state == StateMission21egypt){
        pharaosubiga_timernet_max = 500;
    }
}

void UPDATE() {
    //STATUS
        struct PharaoData* pharao_data = (struct PharaoData*) THIS->custom_data;
        if(pharao_data->hp == 0){
            pharaosubiga_change_status(2, THIS);
        }
        switch(pharao_data->status){
            case 0: //NORMAL
                if(s_horse->x < THIS->x){
                    THIS->mirror = V_MIRROR;
                }else{
                    THIS->mirror = NO_MIRROR;
                }
                THIS->x = s_pharaobiga->x - 3u;
                THIS->y = s_pharaobiga->y - 20u;
                //NET
                    pharaosubiga_timernet_current++;
                    if(pharaosubiga_timernet_current == pharaosubiga_timernet_max){
                        pharaosubiga_timernet_current = 0;
                        pharaosubiga_throw_net();
                    }
            break;
            case 1: //HIT
                pharao_data->counter--;
                if(pharao_data->counter <= 0){
                    pharaosubiga_change_status(0, THIS);
                }
            break;
            case 2: //DEAD
                pharao_data->counter--;
                if(pharao_data->counter <= 0){
                    pharaosubiga_change_status(3, THIS);
                }
            break;
            case 3:
                //DEAD animation is over, used to procede to endgame
            break;
        }
}

void pharaosubiga_change_status(INT8 arg_status, Sprite* arg_s_pharaosubiga) BANKED{
    struct PharaoData* pharao_data = (struct PharaoData*) arg_s_pharaosubiga->custom_data;
    if(arg_status == pharao_data->status){return;}
    switch(arg_status){ //go to status
        case 0: // NORMAL
            SetSpriteAnim(arg_s_pharaosubiga, a_pharaosubiga_down, 8u);
        break;
        case 1: // HIT
            if(pharao_data->status != 1){
                if(pharao_data->hp > 0){
                    pharao_data->hp--;
                }else{
                    pharao_data->hp = 0;
                }
                SetSpriteAnim(arg_s_pharaosubiga, a_pharaosubiga_down_blink, 12u);
            }
        break;
        case 2: // DEAD
            pharao_data->hp = 0;
            SetSpriteAnim(arg_s_pharaosubiga, a_pharaosubiga_down_blink, 64u);
        break;
        case 3: //END GAME
            end_game = 1u;
        break;
    }
    pharao_data->status = arg_status;
    pharao_data->counter = 127;
}

void pharaosubiga_throw_net() BANKED{
    INT16 delta_x = s_horse->x - THIS->x;
    INT16 delta_y = s_horse->y - THIS->y;
    pharaonet_vx = 1;
    if(delta_x < 0){
        pharaonet_vx = -1;
    }
    pharaonet_vy = 1;
    if(delta_y < 4){
        pharaonet_vy = -1;
    }
    INT8 delta_x_abs = delta_x;
    if(delta_x < 0){ delta_x_abs = -delta_x;}
    if(delta_y > 80 || delta_x_abs < 24){//s_horse su
        pharaonet_frmskip_max = 1;
        pharaonet_frmskipy_max = 0;
    }else if(delta_y > 20){//s_horse su
        pharaonet_frmskipy_max = 2;
    }else{
        pharaonet_frmskipy_max = 4;
    }
    if(pharaonet_collided_flag == 0){
        SpriteManagerAdd(SpritePharaonet, THIS->x, THIS->y);
    }

}
void DESTROY() {
}