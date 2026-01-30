#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "UtilAnim.h"

#define PRESS_START_MAX 30


IMPORT_MAP(buttonmap);
IMPORT_TILES(font);

extern void start_game() BANKED;
extern void start_game_cheat(AREA arg_cheat_area) BANKED;

UINT8 pressed_up_down = 0u;
UINT8 pressed_right = 0u;
UINT8 pressed_left = 0u;
UINT8 pressed_a = 0u;
UINT8 pressed_b = 0u;
UINT8 press_start = 0u;
INT8 press_start_counter = 0;


extern UINT8 cheat_activated;
extern AREA cheat_area;


void START(){
    InitScroll(BANK(buttonmap), &buttonmap, 0, 0);
	SHOW_BKG;
	INIT_FONT(font, PRINT_BKG);
    PRINT(0, 0, "DISCOVER BUTTON MAP");
    SetWindowY(144);
    pressed_up_down = 0u;
    pressed_right = 0u;
    pressed_left = 0u;
    pressed_a = 0u;
    pressed_b = 0u;
    press_start = 0u;
    press_start_counter = 0;
}

void UPDATE(){
    if(KEY_TICKED(J_DOWN) || KEY_TICKED(J_UP)){
        if(pressed_up_down == 0u){
            pressed_up_down = 1u;
            PRINT(4, 2, "DOUBLE PRESSION");
            PRINT(4, 3, "MOVES VERTICAL ");
        }
    }
    if(KEY_TICKED(J_LEFT)){
        if(pressed_left == 0u){
            pressed_left = 1u;
            PRINT(4, 6, "TURN COUNTER, ");
            PRINT(4, 7, "     CLOCKWISE");
        }
    }
    if(KEY_TICKED(J_RIGHT)){
        if(pressed_right == 0u){
            pressed_right = 1u;
            PRINT(4, 5, "TURN CLOCKWISE");
        }
    }
    if(KEY_TICKED(J_A)){
        if(pressed_a == 0u){
            pressed_a = 1u;
            PRINT(4, 9, "WHIP GIVES A  ");
            PRINT(4, 10, "VELOCITY");
            PRINT(4, 11, "IMPULSE");
        }
    }
    if(KEY_TICKED(J_B)){
        if(pressed_b == 0u){
            pressed_b = 1u;
            PRINT(4, 13, "USE THE");
            PRINT(4, 14, "ATTACK");
            PRINT(4, 15, "WEAPON");
        }
    }
    if(pressed_up_down && pressed_right && pressed_left &&
        pressed_a && pressed_b){
        if(press_start == 0){ press_start = 1u; }
        press_start_counter++;
        if(press_start_counter < PRESS_START_MAX){
            PRINT(0, 0, "   NOW PRESS START  ");
        }else if(press_start_counter > (PRESS_START_MAX << 1)){
            press_start_counter = 0;
        }else if(press_start_counter > PRESS_START_MAX){
            PRINT(0, 0, "                   ");
        }
    }
    if(KEY_TICKED(J_START)){
        if(press_start){
            start_game();
        }
        if(cheat_activated){
            start_game_cheat(cheat_area);
        }
    }
}