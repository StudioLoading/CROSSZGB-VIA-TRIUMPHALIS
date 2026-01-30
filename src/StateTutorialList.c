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

#define BLINK_TEXT 16

IMPORT_MAP(maptutoriallist);
IMPORT_TILES(font);

extern TUTORIAL_STAGE tutorial_state;
extern UINT8 prev_state;

INT8 blink_text = BLINK_TEXT;

unsigned char m00[18];
unsigned char m01[18];
unsigned char m02[18];
unsigned char m03[18];
unsigned char m04[18];
unsigned char m05[18];
unsigned char m06[18];
unsigned char m07[18];
unsigned char m08[18];
unsigned char m09[18];
unsigned char m10[18];
unsigned char m11[18];
unsigned char m12[18];
unsigned char m13[18];
unsigned char m14[18];
unsigned char EMPTY_STRING_17[18] = "                 ";

extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;
extern void set_bgm() BANKED;

void START(){
    InitScroll(BANK(maptutoriallist), &maptutoriallist, 0, 0);
	SHOW_BKG;
    set_bgm();
	INIT_FONT(font, PRINT_BKG);
    memcpy(m01, "GO STRAIGHT      ", 18);
    memcpy(m02, "GO STRAIGHT TIME ", 18);
    memcpy(m03, "TURN RIGHT       ", 18);
    memcpy(m04, "TURN LEFT        ", 18);
    memcpy(m05, "TURN RIGHT LEFT  ", 18);
    memcpy(m06, "ZIG ZAG          ", 18);
    memcpy(m07, "ZIG ZAG ON TIME  ", 18);
    memcpy(m08, "DODGE WATER      ", 18);
    memcpy(m09, "USE GLADIO       ", 18);
    memcpy(m10, "USE GLADIO LEFT  ", 18);
    memcpy(m11, "USE LANCE        ", 18);
    memcpy(m12, "USE CAPE         ", 18);
    memcpy(m13, "FIRE UP THE STRAW", 18);
    
    PRINT(2,1,m01); 
    PRINT(2,2,m02); 
    PRINT(2,3,m03); 
    PRINT(2,4,m04); 
    PRINT(2,5,m05); 
    PRINT(2,6,m06); 
    PRINT(2,7,m07); 
    PRINT(2,8,m08); 
    PRINT(2,9,m09); 
    PRINT(2,10,m10); 
    PRINT(2,11,m11);
    PRINT(2,12,m12);
    PRINT(2,13,m13);
    UINT8 idx_check = 0;
    while(idx_check < tutorial_state){
        set_bkg_tile_xy(1,1+idx_check, 58);
        idx_check++;
    }
    SetWindowY(144);
    blink_text = BLINK_TEXT;
    
    PRINT(3, 17, "TO BUTTON MAP");
}

void UPDATE(){
    if(blink_text == BLINK_TEXT){
        PRINT(2,1,m01); 
        PRINT(2,2,m02); 
        PRINT(2,3,m03); 
        PRINT(2,4,m04); 
        PRINT(2,5,m05); 
        PRINT(2,6,m06); 
        PRINT(2,7,m07); 
        PRINT(2,8,m08); 
        PRINT(2,9,m09); 
        PRINT(2,10,m10); 
        PRINT(2,11,m11);
        PRINT(2,12,m12);
        PRINT(2,13,m13);
    }
    blink_text--;
    if(blink_text == 0){
        PRINT(2,tutorial_state+1,EMPTY_STRING_17);
    }
    if(blink_text <= -BLINK_TEXT){
        blink_text = BLINK_TEXT;
    }
    if(KEY_TICKED(J_A) ||KEY_TICKED(J_B) || KEY_TICKED(J_START) || KEY_TICKED(J_SELECT)){
        if(tutorial_state < TUTORIAL_PASSED){
            INSTRUCTION instruction_to_give = 0;
            switch(tutorial_state){
                case TUTORIAL_STAGE_0_STRAIGHT: instruction_to_give = TUTORIAL00_INTRO; break;
                case TUTORIAL_STAGE_1_STRAIGHTTIME: instruction_to_give = TUTORIAL01_INTRO; break;
                case TUTORIAL_STAGE_2_TURNRIGHT: instruction_to_give = TUTORIAL02_INTRO; break;
                case TUTORIAL_STAGE_3_TURNLEFT: instruction_to_give = TUTORIAL03_INTRO; break;
                case TUTORIAL_STAGE_4_TURNRIGHTLEFT: instruction_to_give = TUTORIAL04_INTRO; break;
                case TUTORIAL_STAGE_5_ZIGZAG: instruction_to_give = TUTORIAL05_INTRO; break;
                case TUTORIAL_STAGE_6_ZIGZAG_ONTIME: instruction_to_give = TUTORIAL06_INTRO; break;
                case TUTORIAL_STAGE_7_DODGEWATER: instruction_to_give = TUTORIAL07_INTRO; break;
                case TUTORIAL_STAGE_8_GLADIO: instruction_to_give = TUTORIAL08_INTRO; break;
                case TUTORIAL_STAGE_9_GLADIOLEFT: instruction_to_give = TUTORIAL09_INTRO; break;
                case TUTORIAL_STAGE_10_LANCE: instruction_to_give = TUTORIAL10_INTRO; break;
                case TUTORIAL_STAGE_11_CAPE: instruction_to_give = TUTORIAL11_INTRO; break;
                case TUTORIAL_STAGE_12_STRAW: instruction_to_give = TUTORIAL12_INTRO; break;
            }
            state_move_to_papyrus(instruction_to_give, StateTutorialGame);
        }else{
            SetState(StateWorldmap);
        }
    }
    if(KEY_TICKED(J_LEFT)){
        SetState(StateButtons);
    }
}