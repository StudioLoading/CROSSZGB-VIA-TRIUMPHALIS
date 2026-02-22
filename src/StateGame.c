#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"
#include "sgb_palette.h"

#define PIXEL_STAMINA 96
#define ENDED_TRACK_COOLDOWN 80
#define DIE_COUNTER_MAX 80
#define COUNTER_DANGER_MAX 60

IMPORT_MAP(hudm);
IMPORT_MAP(map);
IMPORT_MAP(maprome00);

IMPORT_TILES(hudt);

const UINT8 coll_tiles[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 0};
const UINT8 coll_surface[] = {0u, 0};
Sprite* s_horse = 0;
Sprite* s_biga = 0;
Sprite* s_compass = 0;
Sprite* s_weapon = 0;
UINT16 euphoria_min_current = 0u;
UINT16 euphoria_max_current = 0u;
HORSE_DIRECTION horse_direction = EEE;
HORSE_DIRECTION horse_direction_old = EEE;
INT8 hp_current = 16;
INT8 hud_turn_cooldown = 0;
INT16 timemax_current = 0;
INT16 time_current = 0;
INT16 time_factor = 0;
ITEM_TYPE weapon_atk = NONE;
ITEM_TYPE weapon_def = NONE;
UINT8 track_ended = 0u;
INT8 track_ended_cooldown = ENDED_TRACK_COOLDOWN;
UINT8 hud_initialized = 0u;
INT8 mission_completed = 0;
UINT16 pos_horse_x = 0;
UINT16 pos_horse_y = 0;
MirrorMode mirror_horse = NO_MIRROR;
UINT8 turn_to_load = 0;
INT16 time_to_load = 0;
INT8 flag_die = 0;
INT8 die_counter = DIE_COUNTER_MAX;
INT8 flag_danger_right, flag_danger_left, flag_danger_up, flag_danger_down = 0;
INT8 counter_danger = 0;
INT8 flag_exclamation = 0;
INT8 mission_iscrono = 0;
MISSION_STEP current_step = LOOKING_FOR_SENATOR;
UINT8 mission_killed = 0u;
Sprite* s_spawning_weapon;
INT8 spawning_weapon_counter;
UINT8 reset_combo_counter = 0u;
UINT8 flag_using_atk = 0u;
UINT8 end_game = 0u;

void update_stamina(void) BANKED;
void update_euphoria(void) BANKED;
void update_compass(void) BANKED;
void update_turning(void) BANKED;
void update_hp(INT8 variation) BANKED;
void update_hp_max(void) BANKED;
void update_time(void) BANKED;
void update_time_max(void) BANKED;
void update_weapon(void) BANKED;
void consume_weapon_def(void) BANKED;
void consume_weapon_atk(void) BANKED;
void use_weapon(INT8 is_defence) BANKED;
void start_common(void) BANKED;
void update_common(void) BANKED;
INT8 is_track_ended(void) BANKED;
void calculate_danger(Sprite* s_danger) BANKED;
void check_danger(void) BANKED;
void show_danger(void) BANKED;

extern UINT8 scroll_bottom_movement_limit;//= 100;

extern UINT16 stamina_max;// = 960;
extern UINT16 euphoria_min;// = 600;
extern UINT16 euphoria_max;// = 700;
extern INT16 stamina_current;// = 0;
extern INT8 vx;
extern INT8 vy;
extern INT8 sin;
extern INT8 cos;
extern TURNING_VERSE turn_verse;
extern INT8 onwater_countdown;
extern MISSION current_mission;
extern UINT8 prev_state;
extern UINT8 turn_to_load;
extern INT16 time_to_load;
extern UINT8 turn;

extern void die(void) BANKED;
extern void set_bgm(void) BANKED;
extern void init_enemies_map(void) BANKED;
extern void spawn_enemies(void) BANKED;

void START(void){
}

void start_common(void) BANKED{
	set_sgb_palette_statusbar();
	scroll_bottom_movement_limit = 40;
	euphoria_min_current = euphoria_min;
	euphoria_max_current = euphoria_max;
	hud_turn_cooldown = 0;
	update_euphoria();
	track_ended = 0;
	track_ended_cooldown = ENDED_TRACK_COOLDOWN;
	if(mission_iscrono){
		time_current = time_to_load;
	}else{
		update_time_max();
	}
	stamina_current = 0;
	turn_verse = NONE;
	hud_initialized = 0u;
	onwater_countdown = -1;
	flag_exclamation = 0;
	flag_die = 0;
	counter_danger = 0;
	flag_danger_right, flag_danger_left, flag_danger_up, flag_danger_down = 0;
	s_horse->mirror = mirror_horse;
    turn = turn_to_load;
	if(turn >= 127){ s_horse->mirror = V_MIRROR; }
	mission_killed = 0u;
	s_spawning_weapon = 0;
	spawning_weapon_counter = 0;
	reset_combo_counter = 0u;
	flag_using_atk = 0u;
	set_bgm();
	BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
	init_enemies_map();
}

void update_stamina(void) BANKED{
	//12 tiles di hud
	//devo scrivere stamina_current in termini di pixels.
	//stamina_max : stamina_current = PIXEL_STAMINA : x
	INT16 stamina_hud = stamina_current/10;// (stamina_current * PIXEL_STAMINA)/stamina_max;
	UINT16 stamina_intero = stamina_hud >> 3;
	UINT16 stamina_resto = stamina_hud % 8;
	INT16 idx = 0;
	if(stamina_intero > 0){
		for(idx = 0; idx < stamina_intero; idx++){
			UPDATE_HUD_TILE(7+idx,0, 52);
		}
	}
	if(stamina_resto > 0){
		UPDATE_HUD_TILE(7+idx,0, 60 - stamina_resto);
		idx++;
	}
	while(idx < 12){
		UPDATE_HUD_TILE(7+idx,0, 60);
		idx++;
	}
}

void update_euphoria(void) BANKED{
	//clean
		UPDATE_HUD_TILE(6,1,1);
		UPDATE_HUD_TILE(7,1,1);
		UPDATE_HUD_TILE(8,1,1);
		UPDATE_HUD_TILE(9,1,1);
		UPDATE_HUD_TILE(10,1,1);
		UPDATE_HUD_TILE(11,1,1);
		UPDATE_HUD_TILE(12,1,1);
		UPDATE_HUD_TILE(13,1,1);
		UPDATE_HUD_TILE(14,1,1);
		UPDATE_HUD_TILE(15,1,1);
		UPDATE_HUD_TILE(16,1,1);
		UPDATE_HUD_TILE(17,1,1);
		UPDATE_HUD_TILE(18,1,1);
		UPDATE_HUD_TILE(19,1,1);
	//update
		euphoria_min_current = euphoria_min;
		euphoria_max_current = euphoria_max;
		UINT8 euphoria_init = euphoria_min_current / 10 / 8;
		UINT8 euphoria_final = euphoria_max_current / 10 / 8;
		UINT8 delta_euphoria = euphoria_final - euphoria_init;
		UINT8 idx_delta_euphoria = 0u;
		while(idx_delta_euphoria <= (delta_euphoria-1)){
			if(idx_delta_euphoria < 3){
			UPDATE_HUD_TILE(7+euphoria_init+idx_delta_euphoria,1, 7+idx_delta_euphoria);
			}else{ UPDATE_HUD_TILE(7+euphoria_init+idx_delta_euphoria,1, 1);}
			idx_delta_euphoria++;
		}
		UPDATE_HUD_TILE(7+euphoria_final,1, 10);
}

void update_compass(void) BANKED{
	/* LOOK AT hudmbkup.gbm
	INT8 using_cos = cos;
	INT8 using_sin = sin;
	if(cos < 0) {using_cos = -cos;}
	if(sin < 0) {using_sin = -sin;}
	if(using_sin >= 0 && using_sin <= 30){//tratto come se stesse andando orizzontale
		horse_direction = EEE;
	}else if(using_sin > 30 && using_sin < 54){// tratto come se stesse andando a 33 gradi
		horse_direction = ENE;
	}else if(using_sin > 54 && using_sin < 78){// tratto come se stesse andando a 66 gradi
		horse_direction = NNE;
	}else if(using_sin > 78){ // tratto come se stesse andando verticale su
		horse_direction = NNN;
	}
	//if(sin > 0 && cos > 0){horse_direction = EEE;}
	if(sin > 0 && cos < 0){
		switch(horse_direction){//THIS->mirror = V_MIRROR;
			case EEE: horse_direction = WWW; break;
			case ENE: horse_direction = WNW; break;
			case NNE: horse_direction = NNW; break;
		}			
	}
	if(sin < 0 && cos > 0){//THIS->mirror = H_MIRROR;
		switch(horse_direction){
			case ENE: horse_direction = ESE; break;
			case NNE: horse_direction = SSE; break;
			case NNN: horse_direction = SSS; break;
		}
	}
	if(sin < 0 && cos < 0){//THIS->mirror = HV_MIRROR;
		switch(horse_direction){
			case EEE: horse_direction = WWW; break;
			case ENE: horse_direction = WSW; break;
			case NNE: horse_direction = SSW; break;
			case NNN: horse_direction = SSS; break;
		}			
	}
	if(horse_direction_old != horse_direction){
		horse_direction_old = horse_direction;
		switch(horse_direction_old){
			case EEE: UPDATE_HUD_TILE(1,2,63); break;
			case ENE: UPDATE_HUD_TILE(1,2,64); break;
			case NNE: UPDATE_HUD_TILE(1,2,65); break;
			case NNN: UPDATE_HUD_TILE(1,2,66); break;
			case NNW: UPDATE_HUD_TILE(1,2,67); break;
			case WNW: UPDATE_HUD_TILE(1,2,68); break;
			case WWW: UPDATE_HUD_TILE(1,2,69); break;
			case WSW: UPDATE_HUD_TILE(1,2,70); break;
			case SSW: UPDATE_HUD_TILE(1,2,71); break;
			case SSS: UPDATE_HUD_TILE(1,2,72); break;
			case SSE: UPDATE_HUD_TILE(1,2,73); break;
			case ESE: UPDATE_HUD_TILE(1,2,74); break;

		}
	}
	*/
}

void update_turning(void) BANKED{
	/* LOOK AT hudmbkup.gbm
	if(hud_turn_cooldown > 0){
		hud_turn_cooldown--;
	}else{
		switch(turn_verse){
			case NONE: UPDATE_HUD_TILE(3,2,1); UPDATE_HUD_TILE(3,3,1);break;	
			case CLOCK: UPDATE_HUD_TILE(3,2,48); UPDATE_HUD_TILE(3,3,49);break;	
			case COUNTERCLOCK: UPDATE_HUD_TILE(3,2,75); UPDATE_HUD_TILE(3,3,76);break;	
		}
	}
	*/
}

void update_hp_max(void) BANKED{
	hp_current = 16;
	update_hp(16);
}

void update_hp(INT8 variation) BANKED{
	hp_current += variation;
	if(hp_current <= 0){
		flag_die = 1;
		die_counter = DIE_COUNTER_MAX;
		//return;
	}else if(hp_current > 16){
		hp_current = 16;
	}
	INT8 hp_intero = hp_current / 8;
	INT8 hp_resto = hp_current % 8;
	INT8 idx_hp = 0;
	if(hp_intero > 0){
		while(idx_hp < hp_intero){
			UPDATE_HUD_TILE(3+idx_hp,2,52);
			idx_hp++;
		}
	}
	if(hp_resto > 0){
		UPDATE_HUD_TILE(3+idx_hp,2,60-hp_resto);
		idx_hp++;
	}
	while(idx_hp < 2){
		UPDATE_HUD_TILE(3+idx_hp,2, 60);
		idx_hp++;
	}
}

void update_time(void) BANKED{
	INT16 time_hud = time_current / time_factor;
	INT16 time_intero = time_hud / 8;
	INT8 time_resto = time_hud % 8;
	INT8 idx_time = 0;
	if(time_intero > 0){
		while(idx_time < time_intero){
			UPDATE_HUD_TILE(15+idx_time,2,52);
			idx_time++;
		}
	}
	if(time_resto > 0){
		UPDATE_HUD_TILE(15+idx_time,2,60-time_resto);
	}else{
		while(idx_time < 4){
			UPDATE_HUD_TILE(15+idx_time,2, 60);
			idx_time++;
		}
	}
}

void update_weapon(void) BANKED{
	switch(weapon_atk){
		case NONE:
			UPDATE_HUD_TILE(5,3,20);
			UPDATE_HUD_TILE(5,4,21);
			UPDATE_HUD_TILE(6,3,22);
			UPDATE_HUD_TILE(6,4,23);
		break;
		case GLADIO:
			UPDATE_HUD_TILE(5,3,28);
			UPDATE_HUD_TILE(5,4,29);
			UPDATE_HUD_TILE(6,3,30);
			UPDATE_HUD_TILE(6,4,31);
		break;
		case LANCE:
			UPDATE_HUD_TILE(5,3,32);
			UPDATE_HUD_TILE(5,4,33);
			UPDATE_HUD_TILE(6,3,34);
			UPDATE_HUD_TILE(6,4,35);
		break;
		case FIRE:
			UPDATE_HUD_TILE(5,3,36);
			UPDATE_HUD_TILE(5,4,37);
			UPDATE_HUD_TILE(6,3,38);
			UPDATE_HUD_TILE(6,4,39);
		break;
	}
	switch(weapon_def){
		case NONE:
			UPDATE_HUD_TILE(13,3,24);
			UPDATE_HUD_TILE(13,4,25);
			UPDATE_HUD_TILE(14,3,26);
			UPDATE_HUD_TILE(14,4,27);
		break;
		case ELMET:
			UPDATE_HUD_TILE(13,3,40);
			UPDATE_HUD_TILE(13,4,41);
			UPDATE_HUD_TILE(14,3,42);
			UPDATE_HUD_TILE(14,4,43);
		break;
		case SHIELD:
			UPDATE_HUD_TILE(13,3,44);
			UPDATE_HUD_TILE(13,4,45);
			UPDATE_HUD_TILE(14,3,46);
			UPDATE_HUD_TILE(14,4,47);
		break;
		case CAPE:
			UPDATE_HUD_TILE(13,3,63);
			UPDATE_HUD_TILE(13,4,64);
			UPDATE_HUD_TILE(14,3,65);
			UPDATE_HUD_TILE(14,4,66);
		break;
	}
}

void use_weapon(INT8 is_defence) BANKED{
	if(is_defence){
		switch(weapon_def){
			case SHIELD:
				s_weapon = SpriteManagerAdd(SpriteItemshield, s_horse->x + 16, s_horse->y);
			break;
			case ELMET:
				s_weapon = SpriteManagerAdd(SpriteItemelmet, s_horse->x + 16, s_horse->y);
			break;
			case CAPE:
				s_weapon = SpriteManagerAdd(SpriteItemcape, s_horse->x + 16, s_horse->y);
			break;
		}
		struct ItemData* weapon_data = (struct ItemData*) s_weapon->custom_data;
		weapon_data->itemtype = weapon_def;
		weapon_data->configured = 3;
		consume_weapon_def();
	}else{//attack!
		if(flag_using_atk){
			return;
		}
		flag_using_atk = 1u;
		UINT16 attack_x = s_horse->x;
		UINT16 attack_y = s_horse->y + 8;
		if(s_horse->mirror == V_MIRROR){
			attack_y = s_horse->y - 20;
			attack_x = s_horse->x - 16;
		}
		if(weapon_atk == LANCE){
			attack_y = s_horse->y - 8;
		}else if(weapon_atk == FIRE){
			attack_x = s_horse->x + 16;
			if(s_horse->mirror == V_MIRROR){
				attack_x = s_horse->x;
			}
			attack_y = s_horse->y;
		}
		switch(weapon_atk){
			case GLADIO:
				s_weapon = SpriteManagerAdd(SpriteWeapon, attack_x, attack_y);
			break;
			case LANCE:
				s_weapon = SpriteManagerAdd(SpriteWeapon, attack_x, attack_y);
				if(vx < 0){
					s_weapon->mirror = V_MIRROR;
				}
			break;
			case FIRE:
				Sprite* s_fire = SpriteManagerAdd(SpriteFlame, attack_x, attack_y);
				struct FlameData* fire_data = (struct FlameData*) s_fire->custom_data;
				fire_data->vx = vx;
				fire_data->hp = 20;
				fire_data->dropped = 2;
			break;
		}
		if(s_weapon != 0){
			struct ItemData* weapon_data = (struct ItemData*) s_weapon->custom_data;
			weapon_data->itemtype = weapon_atk;
			weapon_data->configured = 3;
		}
		switch(weapon_atk){
			case LANCE:
			case FLAME:
				consume_weapon_atk();
			break;
		}
	}
}

void update_time_max(void) BANKED{
	time_current = timemax_current ;
}

void consume_weapon_atk(void) BANKED{
	weapon_atk = NONE;
	flag_using_atk = 0u;
	update_weapon();
}

void consume_weapon_def(void) BANKED{
	weapon_def = NONE;
	update_weapon();
}

INT8 is_track_ended(void) BANKED{// == is mission completed
	INT8 result = 0;
	switch(current_state){
		case StateMission00rome:
			result = mission_completed;
		break;
		case StateMission01rome:
		case StateMission02rome:
		case StateMission03rome:
			result = mission_completed;
		break;
		case StateMission04alps:
		case StateMission05alps:
		case StateMission06alps:
		case StateMission07alps:
			result = mission_completed;
		break;
		case StateMission08sea:
		case StateMission09sea:
		case StateMission10sea:
		case StateMission11sea:
			result = mission_completed;
		break;
		case StateMission12greece:
		case StateMission13greece:
		case StateMission14greece:
		case StateMission15greece:
			result = mission_completed;
		break;
		case StateMission17desert:
		case StateMission18desert:
			result = mission_completed;
		break;
		case StateMission19egypt:
		case StateMission20egypt:
			result = mission_completed;
		break;
		case StateMission21egypt:
			result = end_game;
		break;
	}
	return result;
}

void calculate_danger(Sprite* s_danger) BANKED{
	INT16 distance_x = s_danger->x - s_horse->x;
	INT16 distance_y = s_danger->y - s_horse->y;
	if(distance_y < 0){distance_y = -distance_y;}
	if(distance_y > 32){ return;}
	INT8 flag_danger = flag_danger_right | flag_danger_left | flag_danger_up | flag_danger_down;
	if(flag_danger){return;}
	if(distance_x > 120 && distance_x < 200){//s_danger più a dx di s_horse
		flag_danger_right = 1;
	}
	if(distance_x > -200 && distance_x < -120){//s_danger più a sx di s_horse
		flag_danger_left = 1;
	}
	/*
	}else{
		if(distance_y > 80){//s_danger più sotto di s_horse
			flag_danger_down = 1;
		}
		if(distance_y < -80){//s_danger più sopra di s_horse
			flag_danger_up = 1;
		}
	}
		*/
}

void check_danger(void) BANKED{
	INT8 flag_danger = flag_danger_right | flag_danger_left | flag_danger_up | flag_danger_down;
	if(flag_danger && counter_danger == 0){
		counter_danger = COUNTER_DANGER_MAX;
		UPDATE_HUD_TILE(7,2,67);
		UPDATE_HUD_TILE(8,2,68);
		UPDATE_HUD_TILE(9,2,69);		
	}
}

void show_danger(void) BANKED{
	if(counter_danger > 0){
		counter_danger--;
		switch(counter_danger){
			case 58:
				if(flag_danger_right && vx > 0){
					UPDATE_HUD_TILE(8,3,70);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,1);
				}
				if(flag_danger_left && vx < 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,72);
				}
			break;
			case 40:
				if(flag_danger_right && vx > 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,70);
					UPDATE_HUD_TILE(10,3,1);
				}
				if(flag_danger_left && vx < 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,72);
					UPDATE_HUD_TILE(10,3,1);
				}
			break;
			case 20:
				if(flag_danger_right && vx > 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,70);
				}
				if(flag_danger_left && vx < 0){
					UPDATE_HUD_TILE(8,3,72);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,1);
				}
			break;
		}
		if(counter_danger <= 0){//azzera i flag
			UPDATE_HUD_TILE(7,2,1);
			UPDATE_HUD_TILE(8,2,1);
			UPDATE_HUD_TILE(9,2,1);
			UPDATE_HUD_TILE(8,3,1);
			UPDATE_HUD_TILE(9,3,1);
			UPDATE_HUD_TILE(10,3,1);
			flag_danger_right = 0;
			flag_danger_left = 0;
			flag_danger_up = 0;
			flag_danger_down = 0;
		}
	}
}

void update_common(void) BANKED{
    //DIEING
        if(flag_die == 1){
            die_counter--;
            if(die_counter <= 0){
                flag_die = 0;
                die();
            }
            return;
        }
	//HUD
		if(hud_initialized == 0){
			update_hp(0);
			hud_initialized = 1u;
		}
	//UPDATE STAMINA
		update_stamina();
	//UPDATE TURNING
		update_turning();
	//UPDATE EUPHORIA?
		if(euphoria_min_current != euphoria_min || euphoria_max_current != euphoria_max){
			update_euphoria();
		}
	//COMBO TO RESTART THE LEVEL
		if(KEY_PRESSED(J_UP) && KEY_PRESSED(J_START)){
			reset_combo_counter++;
			if(reset_combo_counter >= 120u){
				reset_combo_counter = 0u;
				die();
			}
		}
	//SPAWN ENEMIES
		spawn_enemies();
}


void UPDATE(void){

}
