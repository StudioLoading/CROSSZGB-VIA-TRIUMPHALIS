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

IMPORT_MAP(mappoints);
IMPORT_TILES(font);

extern UINT8 flag_night_mode;
extern UINT8 turn_to_load;
extern MISSION current_mission;
extern MISSION_STEP current_step;
extern UINT8 turn;
extern UINT8 flag_is_demo;
extern INT8 world_area_map;
extern AREA current_area;
extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;
extern UINT16 get_points(void) BANKED;

void move_to_mission_completed_papyrus(void) BANKED;

void START(void){
    InitScroll(BANK(mappoints), &mappoints, 0, 0);
	SHOW_BKG;
	INIT_FONT(font, PRINT_BKG);
    PRINT(7, 1, "SCORE");
    SetWindowY(144);
    PRINT(1, 7, "CURRENT");
    UINT16 current_points = get_points();
    PRINT(11, 7, "%u", current_points);
}

void UPDATE(void){

    if(KEY_TICKED(J_START)){
        move_to_mission_completed_papyrus();
    }
}

void move_to_mission_completed_papyrus(void) BANKED{
	flag_night_mode = 0;//RESET
	turn_to_load = turn;//missione successiva comincia nello stesso verso di dove finisce missione corrente
	INSTRUCTION instruction_to_give = 0;
	switch(current_mission){
		case MISSIONROME00: instruction_to_give = MISSION00_COMPLETED; break;
		case MISSIONROME01: 
			if(flag_is_demo){
				instruction_to_give = DEMO_COMPLETED; 
			}else{
				instruction_to_give = MISSION01_COMPLETED; 
			}
		break;
		case MISSIONROME02: 
			turn_to_load = 0;
			instruction_to_give = MISSION02_COMPLETED;
		break;
		case MISSIONROME03:
			world_area_map = 0;
			current_area = AREA_ALPS;
			instruction_to_give = MISSION03_COMPLETED;
		break;
		case MISSIONALPS04:
			instruction_to_give = MISSION04_COMPLETED; break;
		case MISSIONALPS05: 
			turn_to_load = 0;
			instruction_to_give = MISSION05_COMPLETED;
		break;
		case MISSIONALPS06: 
			turn_to_load = 0;
			instruction_to_give = MISSION06_COMPLETED;
		break;
		case MISSIONALPS07:
			world_area_map = 0;
			current_area = AREA_SEA;
			instruction_to_give = MISSION07_COMPLETED;
		break;
		case MISSIONSEA08: instruction_to_give = MISSION08_COMPLETED; break;
		case MISSIONSEA09: instruction_to_give = MISSION09_COMPLETED; break;
		case MISSIONSEA10: instruction_to_give = MISSION10_COMPLETED; break;
		case MISSIONSEA11: 
			current_area = AREA_GREECE;
			instruction_to_give = MISSION11_COMPLETED;
		break;
		case MISSIONGREECE12: instruction_to_give = MISSION12_COMPLETED;break;
		case MISSIONGREECE13: instruction_to_give = MISSION13_COMPLETED;break;
		case MISSIONGREECE14: instruction_to_give = MISSION14_COMPLETED;break;
		case MISSIONGREECE15: 
			current_area = AREA_DESERT;
			instruction_to_give = MISSION15_COMPLETED;
		break;
		case MISSIONDESERT16: instruction_to_give = MISSION16_COMPLETED; break;
		case MISSIONDESERT17: instruction_to_give = MISSION17_COMPLETED; break;
		case MISSIONDESERT18:
			current_area = AREA_EGYPT;
			instruction_to_give = MISSION18_COMPLETED;
		break;
		case MISSIONEGYPT19: instruction_to_give = MISSION19_COMPLETED; break;
		case MISSIONEGYPT20: instruction_to_give = MISSION20_COMPLETED; break;
		case MISSIONEGYPT21: instruction_to_give = MISSION21_COMPLETED; break;
	}
	current_mission++;
	current_step = LOOKING_FOR_SENATOR;
	state_move_to_papyrus(instruction_to_give, StateWorldmap);
}