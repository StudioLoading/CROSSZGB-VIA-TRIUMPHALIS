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

//#define TIME_MAX_MISSIONSEA11 3200
//#define TIME_FACTOR_MISSIONSEA11 100
 
IMPORT_MAP(hudm);
IMPORT_MAP(mapmission11);

const UINT8 coll_m11_tiles[] = {15, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 63, 64, 68, 69, 70, 75, 76, 77, 78, 79, 80, 81, 82, 85, 86, 90, 91, 93, 95, 96, 97, 98, 99, 102, 103, 104, 105, 106, 114, 118, 119, 121, 0};

const UINT8 coll_m11_surface[] = {0u, 0};

void spawn_m11_killers(void) BANKED;
void spawn_m11_savage(void) BANKED;

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
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger(void) BANKED;
extern void show_danger(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void die(void) BANKED;
extern void map_ended(void) BANKED;

void START(void){
    mission_iscrono = 0;
    //timemax_current = TIME_MAX_MISSIONSEA11;
    //time_factor = TIME_FACTOR_MISSIONSEA11;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 7u << 3;
        pos_horse_y = ((UINT16) 14u << 3) + 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        time_to_load = timemax_current;
    }     
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        if(current_step == LOOKING_FOR_SENATOR){
            mission_completed = 0;
        }else{
            current_step = EXIT;
            mission_completed = 1;
        }
    //COMMONS & START
        spawning_weapon_counter = 0;
        InitScroll(BANK(mapmission11), &mapmission11, coll_m11_tiles, coll_m11_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        spawn_m11_killers();
        spawn_m11_savage();
}

void spawn_m11_killers(void) BANKED{
    SpriteManagerAdd(SpriteKiller, ((UINT16) 56 << 3), ((UINT16) 3 << 3));
    SpriteManagerAdd(SpriteKiller, ((UINT16) 62 << 3), ((UINT16) 4 << 3));
    SpriteManagerAdd(SpriteKiller, ((UINT16) 85 << 3), ((UINT16) 66 << 3));
}

void spawn_m11_savage(void) BANKED{
    Sprite* s_savage00 = SpriteManagerAdd(SpriteSavage, ((UINT16) 35 << 3), ((UINT16) 13u << 3));
    struct SoldierData* savage00_data = (struct SoldierData*) s_savage00->custom_data;
    savage00_data->frmskip_max = 12u;
    savage00_data->configured = 1;
    savage00_data->reward = HP;
    
    Sprite* s_savage01 = SpriteManagerAdd(SpriteSavage, ((UINT16) 60 << 3), ((UINT16) 16u << 3));
    struct SoldierData* savage01_data = (struct SoldierData*) s_savage01->custom_data;
    savage01_data->frmskip_max = 6u;
    savage01_data->configured = 2;
    savage01_data->reward = LANCE;

    Sprite* s_savage02 = SpriteManagerAdd(SpriteSavage, ((UINT16) 34 << 3), ((UINT16) 39u << 3));
    struct SoldierData* savage02_data = (struct SoldierData*) s_savage02->custom_data;
    savage02_data->frmskip_max = 12u;
    savage02_data->configured = 1;
    savage02_data->reward = HP;    
}

void UPDATE(void){
    //COMMON UPDATE
        update_common();    
    //UPDATE TIME
        /*update_time();
        time_current--;
        if(time_current < 0 && !mission_completed && !track_ended){
            die();
        }*/
    //CONTINUOUS SPAWNING WEAPON
        if(s_spawning_weapon == 0){
            spawning_weapon_counter++;
            if(spawning_weapon_counter < 0){
                spawning_weapon_counter = 0;
                item_spawn_continuously(LANCE, ((UINT16)46u << 3), ((UINT16)43u << 3) + 3u);
            }
        }
    //MISSION STEP
        if(mission_killed == 3 && mission_completed == 0){
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