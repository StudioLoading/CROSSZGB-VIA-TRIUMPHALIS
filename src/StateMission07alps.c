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

IMPORT_MAP(hudm);
IMPORT_MAP(mapmission07);

const UINT8 coll_m07_tiles[] = {15, 16, 17, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106, 114, 118, 119, 121, 0};

const UINT8 coll_m07_surface[] = {0u, 0};

Sprite* s_barbarian00 = 0;
Sprite* s_barbarian01 = 0;
Sprite* s_barbarian02 = 0;
Sprite* s_barbarian03 = 0;
Sprite* s_barbarian04 = 0;
Sprite* s_barbarian05 = 0;
extern Sprite* s_spawning_weapon;
extern INT8 spawning_weapon_counter;

void spawn_barbarian(void) BANKED;

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
extern INT8 flag_golden_found;
extern MirrorMode mirror_horse;
extern UINT8 turn_to_load;
extern UINT8 turn;
extern UINT8 flag_night_mode;
extern INT16 time_current;
extern INT16 timemax_current;
extern INT16 time_factor;
extern INT16 time_to_load;
extern UINT8 mission_killed;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void night_mode(void) BANKED;
extern void die(void) BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void map_ended(void) BANKED;

void START(void){
    mission_iscrono = 0;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 12u << 3;
        pos_horse_y = (UINT16) 16u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
    }     
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        current_step = EXIT;
        mission_completed = 0;
    //COMMONS & START
        spawning_weapon_counter = 0;
        night_mode();
        InitScroll(BANK(mapmission07), &mapmission07, coll_m07_tiles, coll_m07_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        spawn_barbarian();
}

void spawn_barbarian(void) BANKED{
    s_barbarian00 = SpriteManagerAdd(SpriteBarbarian, ((UINT16) 36u << 3), ((UINT16) 20u << 3));
    struct SoldierData* barbarian00_data = (struct SoldierData*) s_barbarian00->custom_data;
    barbarian00_data->frmskip_max = 4u;
    barbarian00_data->configured = 1;
    barbarian00_data->reward = HP;
    
    s_barbarian01 = SpriteManagerAdd(SpriteBarbarian, ((UINT16) 30u << 3), ((UINT16) 22u << 3));
    struct SoldierData* barbarian01_data = (struct SoldierData*) s_barbarian01->custom_data;
    barbarian01_data->frmskip_max = 6u;
    barbarian01_data->configured = 1;
    barbarian01_data->reward = GLADIO;
    
    s_barbarian02 = SpriteManagerAdd(SpriteBarbarian, ((UINT16) 41u << 3), ((UINT16) 38u << 3));
    struct SoldierData* barbarian02_data = (struct SoldierData*) s_barbarian02->custom_data;
    barbarian02_data->frmskip_max = 2u;
    barbarian02_data->configured = 2;
    barbarian02_data->reward = LANCE;
    
    s_barbarian03 = SpriteManagerAdd(SpriteBarbarian, ((UINT16) 55u << 3), ((UINT16) 36u << 3));
    struct SoldierData* barbarian03_data = (struct SoldierData*) s_barbarian03->custom_data;
    barbarian03_data->frmskip_max = 4u;
    barbarian03_data->configured = 2;
    barbarian03_data->reward = HP;
    
    s_barbarian04 = SpriteManagerAdd(SpriteBarbarian, ((UINT16) 35u << 3), ((UINT16) 10u << 3));
    struct SoldierData* barbarian04_data = (struct SoldierData*) s_barbarian04->custom_data;
    barbarian04_data->frmskip_max = 6u;
    barbarian04_data->configured = 2;
    barbarian04_data->reward = LANCE;
}

void UPDATE(void){
    //NIGHT MODE
        if(flag_night_mode == 0){
            flag_night_mode = 1;
            night_mode();
        }
    //CONTINUOUS SPAWNING WEAPON
        if(s_spawning_weapon == 0){
            spawning_weapon_counter++;
            if(spawning_weapon_counter < 0){
                spawning_weapon_counter = 0;
                item_spawn_continuously(GLADIO, ((UINT16)24u << 3), ((UINT16)40u << 3) + 3u);
            }
        }
    //COMMON UPDATE
        update_common();
    //MISSION STEP
        if(mission_killed >= 4 && mission_completed == 0){
            mission_completed = 1;
        }
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}