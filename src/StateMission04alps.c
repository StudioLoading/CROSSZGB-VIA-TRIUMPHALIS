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

#define TIME_MAX_MISSIONALPS04 2560 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_MISSIONALPS04 80

IMPORT_MAP(hudm);
IMPORT_MAP(mapmission04);

const UINT8 coll_m04_tiles[] = {15, 16, 17, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106, 114, 118, 119, 121, 0};

const UINT8 coll_m04_surface[] = {0u, 0};

Sprite* s_traitor00 = 0;

void spawn_spy(void) BANKED;

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
extern Sprite* s_spawning_weapon;
extern INT8 spawning_weapon_counter;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void die(void) BANKED;
extern void map_ended(void) BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

void START(void){
    mission_iscrono = 1;
    s_traitor00 = 0;
    timemax_current = TIME_MAX_MISSIONALPS04;
    time_factor = TIME_FACTOR_MISSIONALPS04;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 10u << 3;
        pos_horse_y = (UINT16) 68u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        time_to_load = timemax_current;
    }     
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        current_step = EXIT;
        mission_completed = 0;
    //COMMONS & START
        InitScroll(BANK(mapmission04), &mapmission04, coll_m04_tiles, coll_m04_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        spawn_spy();
}

void spawn_spy(void) BANKED{
    s_traitor00 = SpriteManagerAdd(SpriteRomansoldier, ((UINT16) 74u << 3), ((UINT16) 9u << 3));
    struct SoldierData* traitor00_data = (struct SoldierData*) s_traitor00->custom_data;
    traitor00_data->frmskip_max = 10u;
    traitor00_data->configured = 1;
    traitor00_data->reward = NOITEM;
    traitor00_data->points = 100u;
}

void UPDATE(void){
    //COMMON UPDATE
        update_common();
    //LIMIT MAP LEFT    
    //UPDATE TIME
        update_time();
        time_current--;
        if(time_current < 0 && !mission_completed && !track_ended){
            die();
        }
    //MISSION STEP
        if(mission_killed > 0 && mission_completed == 0){
            mission_completed = 1;
        }
    //CONTINUOUS SPAWNING WEAPON
        if(mission_completed == 0 && s_spawning_weapon == 0){
            spawning_weapon_counter++;
            if(spawning_weapon_counter < 0){
                spawning_weapon_counter = 0;
                item_spawn_continuously(FIRE, ((UINT16) 38u << 3), ((UINT16) 68u << 3));
            }
        }
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}