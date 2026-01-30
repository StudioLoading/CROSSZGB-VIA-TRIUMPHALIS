#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Music.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define SCROLL_COUNTER_MAX 1

IMPORT_MAP(papyrusmap);
IMPORT_TILES(fontp);

UINT8 prev_state = StateMission00rome;
INT8 scroll_step = 0; //0-6
INT8 scroll_counter = 0;
INT8 trigger_unscroll = 0;

extern unsigned char d0[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern unsigned char d5[];
extern unsigned char d6[];
extern unsigned char d7[];
extern unsigned char d8[];
extern unsigned char d9[];
extern unsigned char d10[];
extern unsigned char d11[];
extern unsigned char d12[];

extern UINT8 flag_is_demo;
extern TUTORIAL_STAGE tutorial_state;

INSTRUCTION instruction_given = 0;

void update_scroll() BANKED;

extern void set_bgm() BANKED;

void START(){
    InitScroll(BANK(papyrusmap), &papyrusmap, 0, 0);
    SetWindowY(144);
	SHOW_BKG;
    INIT_FONT(fontp, PRINT_BKG);
    update_scroll();
    scroll_step = 0;
    scroll_counter = 0;
    trigger_unscroll = 0;
    PauseMusic;
    if(instruction_given == DEMO_COMPLETED){//tutorial_state != TUTORIAL_PASSED
        set_bgm();
    }
}

void UPDATE(){
    scroll_counter++;
    if(trigger_unscroll == 0){
        if(scroll_counter > SCROLL_COUNTER_MAX && scroll_step < 6){
            scroll_counter = 0;
            scroll_step++;
            update_scroll();
            return;
        }
        if(scroll_step > 3){
            if(KEY_RELEASED(J_START) || KEY_RELEASED(J_A) || KEY_RELEASED(J_B)){
                trigger_unscroll = 1;
            }
        }
    }
    if(trigger_unscroll == 1){
        if(flag_is_demo && instruction_given == DEMO_COMPLETED){
            return;
        }
        if(scroll_counter > SCROLL_COUNTER_MAX){
            scroll_step--;
            update_scroll();
        }
        if(scroll_step == 0){
            if(tutorial_state == TUTORIAL_PASSED && prev_state != StateWorldmap){
                ResumeMusic;
            }
            SetState(prev_state);
        }
    }
       
}

void update_scroll() BANKED{
    //scroll in alto
        set_bkg_tile_xy(1, 7-scroll_step-1, 0);
        set_bkg_tile_xy(2, 7-scroll_step-1, 0);
        set_bkg_tile_xy(3, 7-scroll_step-1, 0);
        set_bkg_tile_xy(4, 7-scroll_step-1, 0);
        set_bkg_tile_xy(5, 7-scroll_step-1, 0);
        set_bkg_tile_xy(6, 7-scroll_step-1, 0);
        set_bkg_tile_xy(7, 7-scroll_step-1, 0);
        set_bkg_tile_xy(8, 7-scroll_step-1, 0);
        set_bkg_tile_xy(9, 7-scroll_step-1, 0);
        set_bkg_tile_xy(10, 7-scroll_step-1, 0);
        set_bkg_tile_xy(11, 7-scroll_step-1, 0);
        set_bkg_tile_xy(12, 7-scroll_step-1, 0);
        set_bkg_tile_xy(13, 7-scroll_step-1, 0);
        set_bkg_tile_xy(14, 7-scroll_step-1, 0);
        set_bkg_tile_xy(15, 7-scroll_step-1, 0);
        set_bkg_tile_xy(16, 7-scroll_step-1, 0);
        set_bkg_tile_xy(17, 7-scroll_step-1, 0);
        set_bkg_tile_xy(18, 7-scroll_step-1, 0);
        set_bkg_tile_xy(1, 7-scroll_step, 1);
        set_bkg_tile_xy(2, 7-scroll_step, 2);
        set_bkg_tile_xy(3, 7-scroll_step, 2);
        set_bkg_tile_xy(4, 7-scroll_step, 2);
        set_bkg_tile_xy(5, 7-scroll_step, 2);
        set_bkg_tile_xy(6, 7-scroll_step, 2);
        set_bkg_tile_xy(7, 7-scroll_step, 2);
        set_bkg_tile_xy(8, 7-scroll_step, 2);
        set_bkg_tile_xy(9, 7-scroll_step, 2);
        set_bkg_tile_xy(10, 7-scroll_step, 2);
        set_bkg_tile_xy(11, 7-scroll_step, 2);
        set_bkg_tile_xy(12, 7-scroll_step, 2);
        set_bkg_tile_xy(13, 7-scroll_step, 2);
        set_bkg_tile_xy(14, 7-scroll_step, 2);
        set_bkg_tile_xy(15, 7-scroll_step, 2);
        set_bkg_tile_xy(16, 7-scroll_step, 2);
        set_bkg_tile_xy(17, 7-scroll_step, 3);
        set_bkg_tile_xy(18, 7-scroll_step, 4);
    //scroll in basso
        set_bkg_tile_xy(1, 9+scroll_step+1, 0);
        set_bkg_tile_xy(2, 9+scroll_step+1, 0);
        set_bkg_tile_xy(3, 9+scroll_step+1, 0);
        set_bkg_tile_xy(4, 9+scroll_step+1, 0);
        set_bkg_tile_xy(5, 9+scroll_step+1, 0);
        set_bkg_tile_xy(6, 9+scroll_step+1, 0);
        set_bkg_tile_xy(7, 9+scroll_step+1, 0);
        set_bkg_tile_xy(8, 9+scroll_step+1, 0);
        set_bkg_tile_xy(9, 9+scroll_step+1, 0);
        set_bkg_tile_xy(10, 9+scroll_step+1, 0);
        set_bkg_tile_xy(11, 9+scroll_step+1, 0);
        set_bkg_tile_xy(12, 9+scroll_step+1, 0);
        set_bkg_tile_xy(13, 9+scroll_step+1, 0);
        set_bkg_tile_xy(14, 9+scroll_step+1, 0);
        set_bkg_tile_xy(15, 9+scroll_step+1, 0);
        set_bkg_tile_xy(16, 9+scroll_step+1, 0);
        set_bkg_tile_xy(17, 9+scroll_step+1, 0);
        set_bkg_tile_xy(18, 9+scroll_step+1, 0);
        set_bkg_tile_xy(1, 9+scroll_step, 22);
        set_bkg_tile_xy(2, 9+scroll_step, 40);
        set_bkg_tile_xy(3, 9+scroll_step, 41);
        set_bkg_tile_xy(4, 9+scroll_step, 41);
        set_bkg_tile_xy(5, 9+scroll_step, 41);
        set_bkg_tile_xy(6, 9+scroll_step, 41);
        set_bkg_tile_xy(7, 9+scroll_step, 41);
        set_bkg_tile_xy(8, 9+scroll_step, 41);
        set_bkg_tile_xy(9, 9+scroll_step, 41);
        set_bkg_tile_xy(10, 9+scroll_step, 41);
        set_bkg_tile_xy(11, 9+scroll_step, 41);
        set_bkg_tile_xy(12, 9+scroll_step, 41);
        set_bkg_tile_xy(13, 9+scroll_step, 41);
        set_bkg_tile_xy(14, 9+scroll_step, 41);
        set_bkg_tile_xy(15, 9+scroll_step, 41);
        set_bkg_tile_xy(16, 9+scroll_step, 41);
        set_bkg_tile_xy(17, 9+scroll_step, 43);
        set_bkg_tile_xy(18, 9+scroll_step, 44);

    set_bkg_tile_xy(1, 8-scroll_step, 15);
    set_bkg_tile_xy(2, 8-scroll_step, 14);
    set_bkg_tile_xy(3, 8-scroll_step, 7);
    set_bkg_tile_xy(4, 8-scroll_step, 7);
    set_bkg_tile_xy(5, 8-scroll_step, 7);
    set_bkg_tile_xy(6, 8-scroll_step, 7);
    set_bkg_tile_xy(7, 8-scroll_step, 7);
    set_bkg_tile_xy(8, 8-scroll_step, 7);
    set_bkg_tile_xy(9, 8-scroll_step, 7);
    set_bkg_tile_xy(10, 8-scroll_step, 7);
    set_bkg_tile_xy(11, 8-scroll_step, 7);
    set_bkg_tile_xy(12, 8-scroll_step, 7);
    set_bkg_tile_xy(13, 8-scroll_step, 7);
    set_bkg_tile_xy(14, 8-scroll_step, 7);
    set_bkg_tile_xy(15, 8-scroll_step, 7);
    set_bkg_tile_xy(16, 8-scroll_step, 7);
    set_bkg_tile_xy(17, 8-scroll_step, 25);
    set_bkg_tile_xy(18, 8-scroll_step, 26);

    
    set_bkg_tile_xy(1, 8+scroll_step, 15);
    set_bkg_tile_xy(2, 8+scroll_step, 14);
    set_bkg_tile_xy(3, 8+scroll_step, 7);
    set_bkg_tile_xy(4, 8+scroll_step, 7);
    set_bkg_tile_xy(5, 8+scroll_step, 7);
    set_bkg_tile_xy(6, 8+scroll_step, 7);
    set_bkg_tile_xy(7, 8+scroll_step, 7);
    set_bkg_tile_xy(8, 8+scroll_step, 7);
    set_bkg_tile_xy(9, 8+scroll_step, 7);
    set_bkg_tile_xy(10, 8+scroll_step, 7);
    set_bkg_tile_xy(11, 8+scroll_step, 7);
    set_bkg_tile_xy(12, 8+scroll_step, 7);
    set_bkg_tile_xy(13, 8+scroll_step, 7);
    set_bkg_tile_xy(14, 8+scroll_step, 7);
    set_bkg_tile_xy(15, 8+scroll_step, 7);
    set_bkg_tile_xy(16, 8+scroll_step, 7);
    set_bkg_tile_xy(17, 8+scroll_step, 25);
    set_bkg_tile_xy(18, 8+scroll_step, 26);

    switch(scroll_step){
        case 6:
            PRINT(3,2, d0);
            PRINT(3,14, d12);
        break;
        case 5:
            PRINT(3,3, d1);
            PRINT(3,13, d11);
        break;
        case 4:
            PRINT(3,4, d2);
            PRINT(3,12, d10);
        break;
        case 3:
            PRINT(3,5, d3);
            PRINT(3,11, d9);
        break;
        case 2:
            PRINT(3,6, d4);
            PRINT(3,10, d8);
        break;
        case 1:
            PRINT(3,7, d5);
            PRINT(3,9, d7);
        break;
        case 0:
            PRINT(3,8, d6);
        break;
    }
}