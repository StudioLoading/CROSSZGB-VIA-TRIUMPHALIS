#include "Banks/SetAutoBank.h"

#include <gbdk/platform.h>
#include "BankManager.h"
#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "SGB.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "Dialogs.h"
#include "sgb_palette.h"

IMPORT_MAP(border);
IMPORT_MAP(borderalps);
IMPORT_MAP(bordersea);
IMPORT_MAP(bordergreece);
IMPORT_MAP(borderdesert);
IMPORT_MAP(borderegypt);

static const palette_color_t palette_data_rome[] = {RGB(0,0,0),RGB(0,0,0),RGB(29,2,0),RGB(0,0,0)};
static const palette_color_t palette_data_alps[] = {RGB(0,0,0),RGB(9,24,31),RGB(15,0,25),RGB(0,0,0)};
static const palette_color_t palette_data_sea[] = {RGB(0,0,0),RGB(14,10,1),RGB(4,22,0),RGB(0,0,0)};
static const palette_color_t palette_data_greece[] = {RGB(0,0,0),RGB(13,12,1),RGB(0,0,10),RGB(0,0,0)};
static const palette_color_t palette_data_greece_03[] = {RGB(0,0,0),RGB(0,0,0),RGB(0,7,5),RGB(0,0,0)};
static const palette_color_t palette_data_greece_04[] = {RGB(0,0,0),RGB(0,0,0),RGB(14,10,1),RGB(0,0,0)};
static const palette_color_t palette_data_desert[] = {RGB(14,7,1),RGB(0,10,19),RGB(0,0,0),RGB(0,0,0)};
static const palette_color_t palette_data_desert_01[] = {RGB(0,0,0),RGB(0,0,0),RGB(0,7,5),RGB(0,0,0)};
static const palette_color_t palette_data_desert_02[] = {RGB(14,7,1),RGB(0,0,0),RGB(0,0,0),RGB(0,10,19)};

UINT8 flag_night_mode = 0u;
UINT8 flag_border_set = 0u;

void die(void) BANKED;
void spawn_items(void) BANKED;
void night_mode(void) BANKED;
void map_ended(void) BANKED;
void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;
void manage_border(UINT8 my_next_state) BANKED;
void check_sgb_palette(UINT8 new_state) BANKED;
void state_move_to_points(void) BANKED;

extern struct CONFIGURATION configuration;
extern AREA current_area;
extern UINT8 scroll_bottom_movement_limit;//= 100;
extern UINT16 stamina_max;// = 960;
extern UINT16 euphoria_min;// = 600;
extern UINT16 euphoria_max;// = 700;
extern INT8 vx;
extern INT8 vy;
extern INT8 sin;
extern INT8 cos;
extern TURNING_VERSE turn_verse;
extern INT8 onwater_countdown;
extern MISSION_STEP current_step;
extern MISSION current_mission;
extern UINT8 prev_state;
extern UINT8 turn_to_load;
extern UINT8 turn;
extern INT8 world_area_map;
extern UINT8 credit_step;
extern INSTRUCTION instruction_given;
extern ITEM_TYPE weapon_atk;
extern ITEM_TYPE weapon_def;
extern INT8 mission_completed;
extern Sprite* s_spawning_weapon;
extern UINT8 flag_using_atk;

extern void update_hp(INT8 variation) BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void update_weapon(void) BANKED;
extern void update_hp_max(void) BANKED;
extern void update_time_max(void) BANKED;
extern void pickup_config(ITEM_TYPE arg_pickedup) BANKED;
extern UINT16 add_points(POINTS_TYPE arg_points_type, INT16 arg_points) BANKED;

extern void consume_weapon_def(void) BANKED;
extern void consume_weapon_atk(void) BANKED;
void pickup(Sprite* s_arg_item) BANKED;

void START(void){

}

void UPDATE(void){
	
}

void state_move_to_points(void) BANKED{
	reset_sgb_palette_statusbar();
	SetState(StatePoints);
}

void map_ended(void) BANKED{
	reset_sgb_palette_statusbar();
	state_move_to_points();
}

void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED{
	reset_sgb_palette_statusbar();
	instruction_given = arg_instruction_to_show;
	GetLocalizedDialog_EN(arg_instruction_to_show);
	if(arg_prev_state){
		prev_state = arg_prev_state;
	}
	SetState(StatePapyrus);
}

void die(void) BANKED{
	reset_sgb_palette_statusbar();
	switch(current_mission){
		case MISSIONROME00: current_step = LOOKING_FOR_SENATOR; break;
		case MISSIONROME01:
		case MISSIONROME02:
		case MISSIONROME03:
			current_step = LOOKING_FOR_SENATOR; 
			current_mission = MISSIONROME00; 
		break;
		case MISSIONALPS04:
		case MISSIONALPS05:
		case MISSIONALPS06:
		case MISSIONALPS07:
			current_step = LOOKING_FOR_SENATOR; 
			current_mission = MISSIONALPS04; 
		break;
		case MISSIONSEA08:
		case MISSIONSEA09:
		case MISSIONSEA10:
		case MISSIONSEA11:
			turn_to_load = 0;
			current_step = LOOKING_FOR_SENATOR; 
			current_mission = MISSIONSEA08; 
		break;
		case MISSIONGREECE12:
		case MISSIONGREECE13:
		case MISSIONGREECE14:
			turn_to_load = 0;
			current_step = LOOKING_FOR_SENATOR; 
			current_mission = MISSIONGREECE12; 
		break;
		case MISSIONGREECE15://forced death and golden item loss
			configuration.elm = NORMAL;
			configuration.reins = NORMAL;
			configuration.wheel = NORMAL;
			configuration.whip = NORMAL;
			map_ended();
			return;
		break;
		case MISSIONDESERT16:
		case MISSIONDESERT17:
		case MISSIONDESERT18:
			turn_to_load = 0;
			current_step = LOOKING_FOR_SENATOR; 
			current_mission = MISSIONDESERT16;
		break;
		case MISSIONEGYPT19:
		case MISSIONEGYPT20:
		case MISSIONEGYPT21:
			turn_to_load = 0;
			current_step = LOOKING_FOR_SENATOR; 
			current_mission = MISSIONEGYPT19; 
		break;
	}
	world_area_map = 0;
	flag_night_mode = 0;//RESET
	update_hp(16);
	consume_weapon_atk();
	consume_weapon_def();
	
	state_move_to_papyrus(DEAD, StateWorldmap);
}

void spawn_items(void) BANKED{
	switch(current_mission){
		case MISSIONROME00:
			if(configuration.whip == NORMAL){
				Sprite* s_config_whip = SpriteManagerAdd(SpriteConfigwhip, ((UINT16)144u << 3), ((UINT16)43u << 3));
				struct ItemData* whip_data = (struct ItemData*)s_config_whip->custom_data;
				whip_data->itemtype = GOLDEN_WHIP;
				whip_data->configured = 1;
			}
		break;
		case MISSIONROME03:
			if(configuration.wheel == NORMAL){
				Sprite* s_config_wheel = SpriteManagerAdd(SpriteConfigwheel, ((UINT16)152u << 3), ((UINT16)23u << 3));
				struct ItemData* wheel_data = (struct ItemData*)s_config_wheel->custom_data;
				wheel_data->itemtype = GOLDEN_WHEEL;
				wheel_data->configured = 1;
			}
		break;
		case MISSIONALPS06:
			if(configuration.reins == NORMAL){
				Sprite* s_config_reins = SpriteManagerAdd(SpriteConfigreins, ((UINT16)212u << 3), ((UINT16)10u << 3));
				struct ItemData* reins_data = (struct ItemData*)s_config_reins->custom_data;
				reins_data->itemtype = GOLDEN_REINS;
				reins_data->configured = 1;
			}
		break;
		case MISSIONALPS07:
			SpriteManagerAdd(SpriteFlame, (UINT16) 13u << 3, (UINT16) 13u << 3);
		break;
		case MISSIONSEA10:
			if(configuration.elm == NORMAL){
				Sprite* s_config_elm = SpriteManagerAdd(SpriteConfigelm, ((UINT16)83 << 3), ((UINT16)79u << 3));
				struct ItemData* elm_data = (struct ItemData*)s_config_elm->custom_data;
				elm_data->itemtype = GOLDEN_ELM;
				elm_data->configured = 1;
			}
		break;
		case MISSIONSEA11:
			SpriteManagerAdd(SpriteFlame, (UINT16) 10u << 3, (UINT16) 11u << 3);
		break;
		case MISSIONGREECE15:
			item_spawn(GLADIO, ((UINT16) 7u << 3), ((UINT16) 6u << 3) + 3u);
			item_spawn(GLADIO, ((UINT16) 20u << 3), ((UINT16) 11u << 3) + 3u);
		break;
		case MISSIONDESERT16:
			if(configuration.whip == NORMAL){
				Sprite* s_config_whip = SpriteManagerAdd(SpriteConfigwhip, ((UINT16)61u << 3), ((UINT16)3u << 3));
				struct ItemData* whip_data = (struct ItemData*)s_config_whip->custom_data;
				whip_data->itemtype = GOLDEN_WHIP;
				whip_data->configured = 1;
			}
			if(configuration.wheel == NORMAL){
				Sprite* s_config_wheel = SpriteManagerAdd(SpriteConfigwheel, ((UINT16)95u << 3), ((UINT16)21u << 3));
				struct ItemData* wheel_data = (struct ItemData*)s_config_wheel->custom_data;
				wheel_data->itemtype = GOLDEN_WHEEL;
				wheel_data->configured = 1;
			}
			if(configuration.reins == NORMAL){
				Sprite* s_config_reins = SpriteManagerAdd(SpriteConfigreins, ((UINT16)84u << 3), ((UINT16)43u << 3));
				struct ItemData* reins_data = (struct ItemData*)s_config_reins->custom_data;
				reins_data->itemtype = GOLDEN_REINS;
				reins_data->configured = 1;
			}
			if(configuration.elm == NORMAL){	
				Sprite* s_config_elm = SpriteManagerAdd(SpriteConfigelm, ((UINT16)9u << 3), ((UINT16)95u << 3));
				struct ItemData* elm_data = (struct ItemData*)s_config_elm->custom_data;
				elm_data->itemtype = GOLDEN_ELM;
				elm_data->configured = 1;
			}
		break;
	}
}

void night_mode(void) BANKED{
	if(_cpu != CGB_TYPE){
		BGP_REG = PAL_DEF(2, 1, 3, 3);// NIGHT MODE
		OBP0_REG = PAL_DEF(3, 3, 1, 3);
	}else{
 		switch(current_area){
			case AREA_ROME://(30,29,19), (30,25,14), (29,2,0), (13,12,1)
				{
					set_bkg_palette(BKGF_CGB_PAL0, 1, palette_data_rome);
					set_bkg_palette(BKGF_CGB_PAL2, 1, palette_data_rome);
				}
			break;
			case AREA_ALPS:
				set_bkg_palette(BKGF_CGB_PAL0, 1, palette_data_alps);
				set_bkg_palette(BKGF_CGB_PAL2, 1, palette_data_alps);
			break;
			case AREA_SEA:
				set_bkg_palette(BKGF_CGB_PAL0, 1, palette_data_sea);
				set_bkg_palette(BKGF_CGB_PAL1, 1, palette_data_alps);
				set_bkg_palette(BKGF_CGB_PAL2, 1, palette_data_sea);
				set_bkg_palette(BKGF_CGB_PAL3, 1, palette_data_sea);
				set_bkg_palette(BKGF_CGB_PAL4, 1, palette_data_sea);
			break;
			case AREA_GREECE:
				set_bkg_palette(BKGF_CGB_PAL0, 1, palette_data_greece);
				set_bkg_palette(BKGF_CGB_PAL1, 1, palette_data_greece);
				set_bkg_palette(BKGF_CGB_PAL2, 1, palette_data_greece);
				set_bkg_palette(BKGF_CGB_PAL3, 1, palette_data_greece_03);
				set_bkg_palette(BKGF_CGB_PAL4, 1, palette_data_greece_04);
			break;
			case AREA_DESERT:
				set_bkg_palette(BKGF_CGB_PAL0, 1, palette_data_desert);
				set_bkg_palette(BKGF_CGB_PAL1, 1, palette_data_desert_01);
				set_bkg_palette(BKGF_CGB_PAL2, 1, palette_data_desert_02);
				set_bkg_palette(BKGF_CGB_PAL3, 1, palette_data_desert_02);
			break;
		}
	}
}

void check_sgb_palette(UINT8 arg_new_state) BANKED{
	switch(arg_new_state){
		case StateCredit:
		{
			switch(credit_step){
				case 1: set_sgb_palette_credit_studioloading();break;
				case 2: set_sgb_palette_credit_viatriumphalis();break;
				case 3: set_sgb_palette_credit_titlescreen();break;		
			}
		}
		break;
		case StateTutorialList:
		break;
		case StateWorldmap:
		{
			switch(current_area){
				case AREA_ROME: set_sgb_palette_arearome(); break;
				case AREA_ALPS: set_sgb_palette_areaalps(); break;
				case AREA_SEA:  set_sgb_palette_areasea(); break;
				case AREA_GREECE: set_sgb_palette_areagreece(); break;
				case AREA_DESERT: set_sgb_palette_areadesert(); break;
				case AREA_EGYPT:  break;
			}
		}
		break;
	}
}

void pickup(Sprite* s_arg_item) BANKED{
    struct ItemData* item_data = (struct ItemData*) s_arg_item->custom_data;
    switch(item_data->itemtype){
        case GLADIO: case LANCE: case FLAME:
			if(flag_using_atk){ return; }
            weapon_atk = item_data->itemtype;
			flag_using_atk = 0;
            update_weapon();
        break;
        case ELMET: case SHIELD: case CAPE:
            weapon_def = item_data->itemtype;
            update_weapon();
        break;
        case HP:
			add_points(PICKUP_HP, -20);
            update_hp_max();
        break;
        case TIME:
            update_time_max();
        break;
        case GOLDEN_ELM:
        case GOLDEN_WHEEL:
        case GOLDEN_WHIP:
        case GOLDEN_REINS:
			add_points(PICKUP_GOLDEN, 50);
            pickup_config(item_data->itemtype);
        break;
        case PAPYRUS:
            if(current_mission == MISSIONSEA08 || current_mission == MISSIONGREECE12){
                current_step = EXIT;
                mission_completed = 1;
            }
            if(current_mission == MISSIONGREECE13){
                current_step = SENATOR_COLLIDED;
            }
        break;
    }    
    if(item_data->flag_continuous_spawning == 1){
        s_spawning_weapon = 0;
    }
    SpriteManagerRemoveSprite(s_arg_item);
}

void manage_border(UINT8 my_next_state) BANKED{
	if(sgb_check()){
		if(flag_border_set == 0u){
			switch(current_area){
				case AREA_ROME: LOAD_SGB_BORDER(border); break;
				case AREA_ALPS: LOAD_SGB_BORDER(borderalps); break;
				case AREA_SEA: LOAD_SGB_BORDER(bordersea); break;
				case AREA_GREECE: LOAD_SGB_BORDER(bordergreece); break;
				case AREA_DESERT: LOAD_SGB_BORDER(borderdesert); break; 
				case AREA_EGYPT: LOAD_SGB_BORDER(borderegypt); break; 
			}
			flag_border_set = 1u;
		}
    	check_sgb_palette(my_next_state);
    }
}