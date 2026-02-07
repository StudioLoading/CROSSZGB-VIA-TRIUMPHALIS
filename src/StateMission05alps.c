#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "Dialogs.h"
#include "custom_datas.h"

#define TIME_MAX_MISSIONALPS05 3840 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_MISSIONALPS05 120

IMPORT_MAP(hudm);
IMPORT_MAP(mapmission05);

const UINT8 coll_m05_tiles[] = {15, 16, 17, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106, 114, 118, 119, 121, 0};

const UINT8 coll_m05_surface[] = {0u, 0};

Sprite* s_barbarianshield00 = 0;
Sprite* s_barbarianshield01 = 0;
Sprite* s_barbarianshield02 = 0;
Sprite* s_barbarianshield03 = 0;
Sprite* s_barbarianshield04 = 0;
Sprite* s_barbarianshield05 = 0;
Sprite* s_barbarianshield06 = 0;

Sprite* s_general = 0;
UINT8 flag_spawn_general = 0;

void spawn_barbarianshield(void) BANKED;

extern INT8 mission_iscrono;
extern UINT16 pos_horse_x;
extern UINT16 pos_horse_y;
extern MISSION_STEP current_step;

extern Sprite* s_biga;
extern Sprite* s_horse;
extern Sprite* s_compass;
extern UINT8 track_ended;
extern INT8 track_ended_cooldown;
extern UINT8 turn;
extern UINT8 prev_state;
extern INT8 mission_completed;
extern MISSION current_mission;
extern struct CONFIGURATION configuration;
extern INT8 flag_golden_found;
extern MirrorMode mirror_horse;
extern UINT8 turn_to_load;
extern UINT8 turn;
extern INT16 time_current;
extern INT16 timemax_current;
extern INT16 time_factor;
extern INT16 time_to_load;
extern UINT8 mission_killed;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger(void) BANKED;
extern void show_danger(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void die(void) BANKED;
extern void map_ended(void) BANKED;
extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;

void START(void){
    mission_iscrono = 1;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 9u << 3;
        pos_horse_y = (UINT16) 11u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        time_factor = TIME_FACTOR_MISSIONALPS05;
        timemax_current = TIME_MAX_MISSIONALPS05;
        time_to_load = timemax_current;
    }     
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        if(current_step == LOOKING_FOR_SENATOR){
            s_general = SpriteManagerAdd(SpriteRomansenator, ((UINT16) 35u << 3), ((UINT16) 57u << 3));
            mission_completed = 0;
        }else{
            SpriteManagerRemoveSprite(s_general);
            current_step = EXIT;
            mission_completed = 1;
            s_general = 0;
        }
    //COMMONS & START
        InitScroll(BANK(mapmission05), &mapmission05, coll_m05_tiles, coll_m05_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        if(current_step == LOOKING_FOR_SENATOR){
            spawn_barbarianshield();
        }
}

void spawn_barbarianshield(void) BANKED{
    s_barbarianshield00 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 47u << 3), ((UINT16) 10u << 3)+2u);
    s_barbarianshield00->mirror = V_MIRROR;
    struct SoldierData* barbarian00_data = (struct SoldierData*) s_barbarianshield00->custom_data;
    barbarian00_data->configured = 1;
    s_barbarianshield01 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 46u << 3), ((UINT16) 11u << 3));
    s_barbarianshield01->mirror = V_MIRROR;
    struct SoldierData* barbarian01_data = (struct SoldierData*) s_barbarianshield01->custom_data;
    barbarian01_data->configured = 2;
    
    s_barbarianshield02 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 87 << 3), ((UINT16) 9u << 3));
    struct SoldierData* barbarian02_data = (struct SoldierData*) s_barbarianshield02->custom_data;
    barbarian02_data->configured = 1;

    s_barbarianshield03 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 68u << 3), ((UINT16) 22u << 3));
    struct SoldierData* barbarian03_data = (struct SoldierData*) s_barbarianshield03->custom_data;
    barbarian03_data->configured = 2;

    s_barbarianshield04 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 47u << 3), ((UINT16) 44u << 3));
    struct SoldierData* barbarian04_data = (struct SoldierData*) s_barbarianshield04->custom_data;
    barbarian04_data->configured = 1;

    s_barbarianshield05 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 57u << 3) + 7u, ((UINT16) 45u << 3) +2u);
    struct SoldierData* barbarian05_data = (struct SoldierData*) s_barbarianshield05->custom_data;
    barbarian05_data->configured = 2;
    
    s_barbarianshield06 = SpriteManagerAdd(SpriteBarbarianshield, ((UINT16) 46u << 3), ((UINT16) 58u << 3));
    struct SoldierData* barbarian06_data = (struct SoldierData*) s_barbarianshield06->custom_data;
    barbarian06_data->configured = 1;    
}

void UPDATE(void){
    //COMMON UPDATE
        update_common();
    //LIMIT MAP LEFT
        if(s_horse->y < 160 && s_horse->x < 24u){
            s_horse->x = 24u;
        }
    //UPDATE TIME
        update_time();
        time_current--;
        if(time_current < 0 && !mission_completed && !track_ended){
            die();
        }    
    //MISSION STEP
        if(current_step == SENATOR_COLLIDED){
            pos_horse_x = s_horse->x;
            pos_horse_y = s_horse->y;
            turn_to_load = turn;
            time_to_load = time_current;
            state_move_to_papyrus(MISSION05_SAVED_GENERAL, StateMission05alps);
        }
    //CALCULATE DANGER
        calculate_danger(s_barbarianshield00);
        calculate_danger(s_barbarianshield01);
        calculate_danger(s_barbarianshield02);
        calculate_danger(s_barbarianshield03);
        calculate_danger(s_barbarianshield05);
        calculate_danger(s_barbarianshield06);
        check_danger();
        show_danger();
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}