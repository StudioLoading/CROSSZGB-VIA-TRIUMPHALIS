#include "Banks/SetAutoBank.h"

#include <gbdk/platform.h>
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

#define CREDIT_WAIT_MAX 240
#define PRESSSTART_COUNTER_MAX 16

IMPORT_MAP(mapcredit0);
IMPORT_MAP(mapcredit1);
IMPORT_MAP(mapcredit2);
IMPORT_MAP(titlescreen);
IMPORT_MAP(maintitlemap);
IMPORT_TILES(font);
IMPORT_TILES(fontp);

INT16 credit_wait = 0;
INT8 credit_step = 1;//1 SL;2 VT;3 titlescreen
INT8 pressstart_counter = 0;
UINT8 pressstart_show = 0;
UINT8 cheat_counter = 0u;
UINT8 cheat_activated = 0u;
AREA cheat_area = AREA_ROME;
AREA current_area = AREA_ROME;

extern AREA current_area;
extern UINT8 stop_music_on_new_state;
extern TUTORIAL_STAGE tutorial_state;

extern void manage_border(UINT8 my_next_state) BANKED;
extern void set_bgm(void) BANKED;
extern void pickup_config(ITEM_TYPE arg_pickedup) BANKED;
extern void start_game_cheat(AREA arg_cheat_area) BANKED;


void START(void){
    manage_border(current_state);
	OBP0_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
	OBP1_REG = DMG_PALETTE(DMG_WHITE, DMG_WHITE, DMG_DARK_GRAY, DMG_BLACK);
	BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    credit_wait = CREDIT_WAIT_MAX;
    switch(credit_step){
        case 1:
            NR52_REG = 0x80; // 1000 0000 -> Accende il chip audio
            NR51_REG = 0xFF; // Abilita tutti e 4 i canali a sinistra e a destra
            NR50_REG = 0x77; // Imposta il volume master al massimo
            InitScroll(BANK(mapcredit0), &mapcredit0, 0, 0);
            stop_music_on_new_state = 0u; 
        break;
        case 2:
            InitScroll(BANK(mapcredit1), &mapcredit1, 0, 0);
        break;
        case 3:
            InitScroll(BANK(mapcredit2), &mapcredit2, 0, 0);
        break;
        case 4:
            InitScroll(BANK(maintitlemap), &maintitlemap, 0, 0);
            scroll_target = SpriteManagerAdd(SpriteCamera, 80u, 72u);
        break;
        case 5:
            cheat_counter = 0u;
            cheat_activated = 0u;
            cheat_area = 0u;
            InitScroll(BANK(titlescreen), &titlescreen, 0, 0);
        break;
    }
    set_bgm();
	SHOW_BKG;
    
    switch(credit_step){
        case 1:
	        INIT_FONT(font, PRINT_BKG);
            PRINT(6, 10, "PRESENTS");
            if(sgb_check()){
                PRINT(2, 12, "ON SUPER GAME BOY");
            }else if(_cpu == CGB_TYPE){
                PRINT(2, 12, "ON GAME BOY COLOR");
            }
        break;
        case 2:
	        INIT_FONT(fontp, PRINT_BKG);
            PRINT(5, 0, "POWERED BY");
            PRINT(7, 17, "ENGINE");
        break;
        case 3:
	        INIT_FONT(fontp, PRINT_BKG);
            PRINT(7, 0, "ART BY");
        break;
        case 5:
	        INIT_FONT(font, PRINT_BKG);
        break;
    }
}

void UPDATE(void){
    if(credit_step != 4){
        credit_wait--;
    }
    //CHEAT
        if(credit_step == 5){
            if(KEY_RELEASED(J_SELECT)){
                if(cheat_activated == 0){
                    cheat_counter++;
                    if(cheat_counter > 4){
                        cheat_activated = 1u;
                        tutorial_state = TUTORIAL_PASSED;
                        pickup_config(GOLDEN_ELM);
                        pickup_config(GOLDEN_REINS);
                        pickup_config(GOLDEN_WHEEL);
                        pickup_config(GOLDEN_WHIP);
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
    if(credit_step < 5 && (credit_wait <= 0 || KEY_TICKED(J_START))){
        credit_step++;
        SetState(StateCredit);
    }else if(credit_step == 5 && KEY_TICKED(J_START)){
        if(cheat_activated){
            current_area = cheat_area;
            start_game_cheat(cheat_area);
            return;
        }else{
            SetState(StateButtons);
        }
    }
    //ANIMATIONS & CAMERA MOVEMENTS
        switch(credit_step){
            case 4:
                if(scroll_target->x < (UINT16) 147u << 3){
                    scroll_target->x+=2;
                }else{
                    credit_wait = 0;
                }
            break;
            case 5:
                pressstart_counter++;
                if(pressstart_counter >= PRESSSTART_COUNTER_MAX){
                    pressstart_counter = 0;
                    if(pressstart_show == 0){
                        pressstart_show = 1;
                        PRINT(7, 8, "PRESS");
                        PRINT(7, 9, "START");
                    }else{
                        PRINT(7, 8, "     ");
                        PRINT(7, 9, "     ");
                        pressstart_show = 0;
                    }
                }
            break;
        }
}