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
#include "Dialogs.h"
#include "UtilAnim.h"

#define BLINK_ABSENT_MAX 1

typedef enum{
    NO_ELM,
    ELM,
    NO_WHEEL,
    WHEEL,
    NO_WHIP,
    WHIP,
    NO_REINS,
    REINS
}FOCUS;


IMPORT_MAP(configmap);
IMPORT_TILES(font);

UINT8 configwhip_blink = 0u;

INT8 blink_absent = 0;
FOCUS current_focus = NO_ELM;
struct CONFIGURATION configuration = {.elm = NORMAL, .wheel=NORMAL, .whip=NORMAL, .reins=NORMAL};
INT8 flag_golden_found = 0u;

void change_description(void) BANKED;
void pickup_config(ITEM_TYPE arg_pickedup) BANKED;

extern Sprite* s_horse;
extern UINT16 pos_horse_x;
extern UINT16 pos_horse_y;
extern MirrorMode mirror_horse;
extern MISSION_STEP current_step;
extern UINT8 turn_to_load;
extern INT16 time_to_load;
extern UINT8 turn;
extern INT16 time_current;
extern MISSION current_mission;

extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;


void START(void){
    InitScroll(BANK(configmap), &configmap, 0, 0);
	SHOW_BKG;
	INIT_FONT(font, PRINT_BKG);
    PRINT(16, 17, "BACK");
    SetWindowY(144);
    blink_absent = 0;
}

void UPDATE(void){
    if(KEY_TICKED(J_RIGHT)){
        Anim_config_no();
        if(current_focus == REINS || current_focus == NO_REINS){current_focus = ELM;}
        else{current_focus+=2;}
    }
    if(KEY_TICKED(J_LEFT)){
        Anim_config_no();
        if(current_focus == ELM || current_focus == NO_ELM){current_focus = REINS;}
        else{current_focus-=2;}
    }
    if(KEY_TICKED(J_SELECT)){
        SetState(StateWorldmap);
    }
    switch(current_focus){
        case ELM:
        case NO_ELM:
            PRINT(1, 0, " ELM ");
            change_description();
        break;
        case WHEEL:
        case NO_WHEEL:
            PRINT(1, 0, "WHEEL");
            change_description();
        break;
        case WHIP:
        case NO_WHIP:
            PRINT(1, 0, "WHIP ");
            change_description();
            if(configuration.whip == GOLDEN){
                Anim_config_whip();
            }
        break;
        case REINS:
        case NO_REINS:
            PRINT(1, 0, "REINS");
            change_description();
        break;
    }
    blink_absent++;
    if(blink_absent >= BLINK_ABSENT_MAX){
        blink_absent = 0;
        switch(current_focus){
            case NO_ELM:
                Anim_config_elm();
                current_focus = ELM;
            break;
            case ELM:
                if(configuration.elm == NORMAL){
                    Anim_config_no();
                    current_focus = NO_ELM;
                }else{
                    Anim_config_elm();
                }
            break;
            case NO_WHEEL:
                Anim_config_wheel();
                current_focus = WHEEL;
            break;
            case WHEEL:
                if(configuration.wheel == NORMAL){
                    Anim_config_no();
                    current_focus = NO_WHEEL;
                }else{
                    Anim_config_wheel();
                }
            break;
            case WHIP:
                if(configuration.whip == NORMAL){
                    Anim_config_no();
                    current_focus = NO_WHIP;
                }else{
                    Anim_config_whip();
                }
            break;
            case NO_WHIP: 
                Anim_config_whip();
                current_focus = WHIP;
            break;
            case NO_REINS:
                Anim_config_reins();
                current_focus = REINS;
            break;
            case REINS:
                if(configuration.reins == NORMAL){
                    Anim_config_no();
                    current_focus = NO_REINS;
                }else{
                    Anim_config_reins();
                }
            break;
        }
    }
}

void change_description(void) BANKED{
    switch(current_focus){
        case ELM:
            if(configuration.elm == NORMAL){
                PRINT(7,0, "NORMAL ELM   ");
                PRINT(7,1, "             ");
            }
        break;
        case WHEEL:
            if(configuration.wheel == NORMAL){
                PRINT(7,0, "NORMAL WHEELS");
                PRINT(7,1, "RALLY IS OFF.");
            }else if(configuration.wheel == GOLDEN){
                PRINT(7,0, "GOLDEN WHEELS");
                PRINT(7,1, "RALLY IS ON! ");
            }
        break;
        case WHIP:
            if(configuration.whip == NORMAL){
                PRINT(7,0, "NORMAL WHIP  ");
                PRINT(7,1, "IMPULSE(3    ");
            }else if(configuration.whip == GOLDEN){
                PRINT(7,0, "GOLDEN WHIP  ");
                PRINT(7,1, "IMPULSE(8    ");
            }
        break;
        case REINS:
            if(configuration.reins == NORMAL){
                PRINT(7,0, "NORMAL REINS ");
                PRINT(7,1, "             ");
            }else if(configuration.reins == GOLDEN){
                PRINT(7,0, "GOLDEN REINS ");
                PRINT(7,1, "SPINNING (2  ");
            }
        break;
    }
}

void pickup_config(ITEM_TYPE arg_pickedup) BANKED{
    INSTRUCTION instruction_to_give = 0;
    switch(arg_pickedup){
        case GOLDEN_WHIP:
            configuration.whip = GOLDEN;
            instruction_to_give = DESCRIPTION_GOLDEN_WHIP;
        break;
        case GOLDEN_WHEEL:
            configuration.wheel = GOLDEN;
            instruction_to_give = DESCRIPTION_GOLDEN_WHEEL;
        break;
        case GOLDEN_REINS:
            configuration.reins = GOLDEN;
            instruction_to_give = DESCRIPTION_GOLDEN_REINS;
        break;
        case GOLDEN_ELM:
            configuration.elm = GOLDEN;
            instruction_to_give = DESCRIPTION_GOLDEN_ELM;
        break;
    }
    if(current_state != StateCredit){//activating cheating from titlescreen
        flag_golden_found = 1;
        time_to_load = time_current;
        pos_horse_x = s_horse->x;
        pos_horse_y = s_horse->y;
        mirror_horse = s_horse->mirror;
        if(current_mission == MISSIONDESERT16){
            pos_horse_x = scroll_target->x;
            pos_horse_y = scroll_target->y;
            mirror_horse = scroll_target->mirror;
        }
        turn_to_load = turn;
        state_move_to_papyrus(instruction_to_give, 0);
    }
}