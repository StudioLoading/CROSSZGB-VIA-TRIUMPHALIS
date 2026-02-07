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

#define TIME_MAX_MISSION12 960 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_MISSION12 30

IMPORT_MAP(hudm);
IMPORT_MAP(mapmission12);

const UINT8 coll_m12_tiles[] = {15, 16, 17, 18, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 48, 49, 50, 51, 52, 53, 56, 57, 63, 64, 67, 68, 69, 75, 76, 77, 78, 79, 82, 85, 86, 93, 95, 96, 97, 98, 99, 101, 102, 103, 104, 105, 106, 108, 109, 118, 119, 120, 121, 0};

const UINT8 coll_m12_surface[] = {0u, 0};

UINT8 spawned_greeks_flag = 0u;

void spawn_greeks(void) BANKED;

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

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger(void) BANKED;
extern void show_danger(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void die(void) BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void map_ended(void) BANKED;

void START(void){
    mission_iscrono = 1;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else{//initial
        current_step = LOOKING_FOR_SENATOR;
        item_spawn(PAPYRUS, ((UINT16) 18u << 3), ((UINT16) 17u << 3) + 2);
        pos_horse_x = (UINT16) 4u << 3;
        pos_horse_y = (UINT16) 26u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        time_factor = TIME_FACTOR_MISSION12;
        timemax_current = TIME_MAX_MISSION12;
        time_to_load = timemax_current;
    }
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        if(current_step == LOOKING_FOR_SENATOR){
            current_step = EXIT;
            mission_completed = 1;
        }
    //COMMONS & START
        InitScroll(BANK(mapmission12), &mapmission12, coll_m12_tiles, coll_m12_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        start_common();
        spawned_greeks_flag = 0u;
}

void spawn_greeks(void) BANKED{
    Sprite* s_greeksoldier00 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 43u << 3), ((UINT16) 17u << 3));
    struct SoldierData* greeksoldier00_data = (struct SoldierData*) s_greeksoldier00->custom_data;
    greeksoldier00_data->frmskip_max = 8u;
    greeksoldier00_data->configured = 2;
    greeksoldier00_data->reward = NOITEM;
    Sprite* s_greeksoldier01 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 54u << 3), ((UINT16) 10u << 3));
    struct SoldierData* greeksoldier01_data = (struct SoldierData*) s_greeksoldier01->custom_data;
    greeksoldier01_data->frmskip_max = 10u;
    greeksoldier01_data->configured = 1;
    greeksoldier01_data->reward = NOITEM;
    Sprite* s_greeksoldier02 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 99u << 3), ((UINT16) 9u << 3));
    struct SoldierData* greeksoldier02_data = (struct SoldierData*) s_greeksoldier02->custom_data;
    greeksoldier02_data->frmskip_max = 14u;
    greeksoldier02_data->configured = 2;
    greeksoldier02_data->reward = NOITEM;
    Sprite* s_greeksoldier03 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 126u << 3), ((UINT16) 11u << 3));
    struct SoldierData* greeksoldier03_data = (struct SoldierData*) s_greeksoldier03->custom_data;
    greeksoldier03_data->frmskip_max = 6u;
    greeksoldier03_data->configured = 1;
    greeksoldier03_data->reward = NOITEM;
}

void UPDATE(void){
    //COMMON UPDATE
        update_common();
    //SPAWN GREEKS
        if(current_step == EXIT && mission_completed == 1 && spawned_greeks_flag == 0u){
            spawn_greeks();
            spawn_items();
            spawned_greeks_flag = 1u;
        }
    //UPDATE TIME
        update_time();
        time_current--;
        if(time_current < 0 && !(mission_completed && track_ended)){
            die();
        }
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}