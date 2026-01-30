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

#define sgb_running sgb_check()

#define CREDIT_WAIT_MAX 240
#define PRESSSTART_COUNTER_MAX 16

IMPORT_MAP(mapcredit0);
IMPORT_MAP(titlescreen);
IMPORT_MAP(maintitlemap);
IMPORT_TILES(font);

INT16 credit_wait = 0;
INT8 credit_step = 1;//1 SL;2 VT;3 titlescreen
INT8 pressstart_counter = 0;
UINT8 pressstart_show = 0;
UINT8 cheat_counter = 0u;
UINT8 cheat_activated = 0u;
AREA cheat_area = AREA_ROME;

extern AREA current_area;
extern UINT8 stop_music_on_new_state;
extern TUTORIAL_STAGE tutorial_state;

extern void manage_border(UINT8 my_next_state) BANKED;
extern void set_bgm() BANKED;


void START(){
	if(sgb_running){
		manage_border(current_state);
	}
    credit_wait = CREDIT_WAIT_MAX;
    switch(credit_step){
        case 1:
            InitScroll(BANK(mapcredit0), &mapcredit0, 0, 0);
            stop_music_on_new_state = 0u; 
        break;
        case 2:
            InitScroll(BANK(maintitlemap), &maintitlemap, 0, 0);
            scroll_target = SpriteManagerAdd(SpriteCamera, 80u, 72u);
        break;
        case 3:
            cheat_counter = 0u;
            cheat_activated = 0u;
            cheat_area = 0u;
            InitScroll(BANK(titlescreen), &titlescreen, 0, 0);
        break;
    }
    set_bgm();
	SHOW_BKG;
	INIT_FONT(font, PRINT_BKG);
    
    switch(credit_step){
        case 1:
            PRINT(6, 10, "PRESENTS");
        break;
    }
}

void UPDATE(){
    if(credit_step != 2){
        credit_wait--;
    }
    //CHEAT
    if(credit_step == 3){
        if(KEY_RELEASED(J_SELECT)){
            if(cheat_activated == 0){
                cheat_counter++;
                if(cheat_counter > 4){
                    cheat_activated = 1u;
                    tutorial_state = TUTORIAL_PASSED;
                    PRINT(13, 1, "CHEATER!");
                    return;
                }
            }else{
                if(KEY_RELEASED(J_SELECT)){
                    cheat_area++;
                    if(cheat_area > AREA_EGYPT){
                        cheat_area = AREA_ROME;
                    }
                    switch(cheat_area){
                        case AREA_ROME: PRINT(13, 1, "ROME    "); break;
                        case AREA_ALPS: PRINT(13, 1, "ALPS    "); break;
                        case AREA_SEA:  PRINT(13, 1, "SEA     "); break;
                        case AREA_GREECE: PRINT(13, 1, "GREECE  "); break;
                        case AREA_DESERT: PRINT(13, 1, "DESERT  "); break;
                        case AREA_EGYPT: PRINT(13, 1, "EGYPT   "); break;
                    }
                }
            }
        }
    }
    if(credit_step < 3 && (credit_wait <= 0 || KEY_TICKED(J_START))){
        credit_step++;
        SetState(StateCredit);
    }else if(credit_step == 3 && KEY_TICKED(J_START)){
        if(cheat_activated){
            current_area = cheat_area;
        }
        SetState(StateButtons);
    }
    //ANIMATIONS & CAMERA MOVEMENTS
        switch(credit_step){
            case 2:
                if(scroll_target->x < (UINT16) 147u << 3){
                    scroll_target->x+=2;
                }else{
                    credit_wait = 0;
                }
            break;
            case 3:
                pressstart_counter++;
                if(pressstart_counter >= PRESSSTART_COUNTER_MAX){
                    pressstart_counter = 0;
                    if(pressstart_show == 0){
                        pressstart_show = 1;
                        PRINT(8, 8, "PRESS");
                        PRINT(8, 9, "START");
                    }else{
                        PRINT(8, 8, "     ");
                        PRINT(8, 9, "     ");
                        pressstart_show = 0;
                    }
                }
            break;
        }
}