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
IMPORT_MAP(mapmission21);

const UINT8 coll_m21_tiles[] = {3, 15, 16, 17, 18, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 48, 49, 50, 51, 52, 53, 56, 57, 63, 64, 67, 68, 69, 75, 76, 77, 78, 79, 82, 85, 86, 93, 95, 96, 97, 98, 99, 101, 102, 103, 104, 105, 106, 108, 109, 118, 119, 120, 121, 0};

const UINT8 coll_m21_surface[] = {0u, 0};

extern Sprite* s_gator;
extern Sprite* s_pharaobiga;
extern Sprite* s_pharaosubiga;

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

extern void start_common() BANKED;
extern void update_common() BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger() BANKED;
extern void show_danger() BANKED;
extern void update_time() BANKED;
extern void spawn_items() BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void die() BANKED;
extern void map_ended() BANKED;

void START(){
    mission_iscrono = 0;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 8u << 3;
        pos_horse_y = (UINT16) 10u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        time_to_load = timemax_current;
    }     
    //SPRITES
        SpriteManagerAdd(SpritePharaonet, 0,0);
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        //s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        //s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        if(current_step == LOOKING_FOR_SENATOR){
            current_step = EXIT;
            mission_completed = 1;
        }
    //COMMONS & START
        InitScroll(BANK(mapmission21), &mapmission21, coll_m21_tiles, coll_m21_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        s_gator = SpriteManagerAdd(SpriteGator, ((UINT16) 20u << 3), ((UINT16) 10u << 3));
        s_pharaobiga = SpriteManagerAdd(SpritePharaobiga, s_gator->x + 18u, s_gator->y);
        s_pharaosubiga = SpriteManagerAdd(SpritePharaosubiga, s_pharaobiga->x, s_pharaobiga->y - 15u);
        struct PharaoData* pharao_data = (struct PharaoData*) s_pharaosubiga->custom_data;
        pharao_data->status = 0;
        pharao_data->hp = 5;
        pharao_data->counter = 127;
}


void UPDATE(){
    //COMMON UPDATE
        update_common();  
    //CONTINUOUS SPAWNING WEAPON
        if(s_spawning_weapon == 0){
            spawning_weapon_counter++;
            if(spawning_weapon_counter < 0){
                spawning_weapon_counter = 0;
                item_spawn_continuously(GLADIO, ((UINT16)21u << 3), ((UINT16)19u << 3) + 3u);
            }
        }
    //MISSION STEP
        /*if(mission_killed == 3 && mission_completed == 0){
            mission_completed = 1;
        }*/
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}