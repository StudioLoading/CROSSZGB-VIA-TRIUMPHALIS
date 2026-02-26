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
IMPORT_MAP(mapmission09);

const UINT8 coll_m09_tiles[] = {15, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 63, 64, 68, 69, 70, 75, 76, 77, 78, 79, 80, 81, 82, 85, 86, 90, 91, 93, 95, 96, 97, 98, 99, 102, 103, 104, 105, 106, 114, 118, 119, 121, 0};

const UINT8 coll_m09_surface[] = {0u, 0};

Sprite* s_ambassador = 0;

Sprite* s_savage00 = 0;
Sprite* s_savage01 = 0;
Sprite* s_savage02 = 0;
Sprite* s_savage03 = 0;
Sprite* s_savage04 = 0;
Sprite* s_savage05 = 0;

void spawn_savages(void) BANKED;

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
extern INT16 time_current;
extern INT16 timemax_current;
extern INT16 time_factor;
extern INT16 time_to_load;
extern UINT8 mission_killed;
extern UINT8 flag_night_mode;
extern Sprite* s_spawning_weapon;
extern INT8 spawning_weapon_counter;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger(void) BANKED;
extern void show_danger(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void die(void) BANKED;
extern void night_mode(void) BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;
extern void map_ended(void) BANKED;

void START(void){
    mission_iscrono = 0;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 11u << 3;
        pos_horse_y = (UINT16) 9u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
    }     
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        if(current_step == LOOKING_FOR_SENATOR){
            s_ambassador = SpriteManagerAdd(SpriteRomansenator, ((UINT16) 123u << 3), ((UINT16) 3u << 3));
            mission_completed = 0;
        }else{
            SpriteManagerRemoveSprite(s_ambassador);
            current_step = EXIT;
            s_ambassador = 0;
            mission_killed = 0;
            spawn_savages();
        }
    //COMMONS & START
        night_mode();
        InitScroll(BANK(mapmission09), &mapmission09, coll_m09_tiles, coll_m09_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        if(current_step == EXIT){
            spawn_items();
        }
        start_common();
}

void spawn_savages(void) BANKED{
    
    s_savage01 = SpriteManagerAdd(SpriteSavage, ((UINT16) 91u << 3), ((UINT16) 18u << 3));
    struct SoldierData* savage01_data = (struct SoldierData*) s_savage01->custom_data;
    savage01_data->frmskip_max = 8u;
    savage01_data->configured = 2;
    savage01_data->reward = NOITEM;

    s_savage02 = SpriteManagerAdd(SpriteSavage, ((UINT16) 134u << 3), ((UINT16) 18u << 3) + 2);
    struct SoldierData* savage02_data = (struct SoldierData*) s_savage02->custom_data;
    savage02_data->frmskip_max = 8u;
    savage02_data->configured = 2;
    savage02_data->reward = HP;
    
    s_savage03 = SpriteManagerAdd(SpriteSavage, ((UINT16) 63u << 3), ((UINT16) 17u << 3) + 2);
    struct SoldierData* savage03_data = (struct SoldierData*) s_savage03->custom_data;
    savage03_data->frmskip_max = 10u;
    savage03_data->configured = 2;
    savage03_data->reward = GLADIO;

}

void UPDATE(void){
    //NIGHT MODE
        if(flag_night_mode == 0){
            flag_night_mode = 1;
            night_mode();
        }
    //COMMON UPDATE
        update_common();
    //LIMIT MAP LEFT
        if(s_horse->x < 24u){
            s_horse->x = 24u;
        }
    //UPDATE TIME
        /*update_time();
        time_current--;
        if(time_current < 0 && !mission_completed && !track_ended){
            die();
        } */    
    //CONTINUOUS SPAWNING WEAPON
    if(current_step == EXIT && s_spawning_weapon == 0){
        spawning_weapon_counter++;
        if(spawning_weapon_counter < 0){
            spawning_weapon_counter = 0;
            item_spawn_continuously(GLADIO, ((UINT16)55u << 3), ((UINT16)15u << 3) + 3u);
        }
    }
    //MISSION STEP
        if(current_step == SENATOR_COLLIDED){
            pos_horse_x = s_horse->x;
            pos_horse_y = s_horse->y;
            turn_to_load = turn;
            time_to_load = time_current;
            state_move_to_papyrus(MISSION09_SAVED_AMBASSADOR, StateMission09sea);
        }
        if(mission_killed >= 3 && mission_completed == 0){
            mission_completed = 1;
        }
    //CALCULATE DANGER
        /*calculate_danger(s_barbarianshield00);
        calculate_danger(s_barbarianshield01);
        calculate_danger(s_barbarianshield02);
        calculate_danger(s_barbarianshield03);
        calculate_danger(s_barbarianshield05);
        calculate_danger(s_barbarianshield06);
        check_danger();
        show_danger();*/
    
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}