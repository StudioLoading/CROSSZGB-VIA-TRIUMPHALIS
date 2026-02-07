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
#include "UtilAnim.h"

IMPORT_MAP(hudm);
IMPORT_MAP(mapmission19);

#define TIME_MAX_MISSION19 1024 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_MISSION19 32
#define TIME_MAX_TILE_ANIM 10

const UINT8 coll_m19_tiles[] = {3, 17, 18, 19, 21, 22, 23, 28, 29, 36, 44, 45, 46, 47, 49, 51, 52, 54, 56, 57, 58, 59, 61, 62, 63, 65, 66, 67, 68, 69, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 82, 83, 84, 85, 86, 87, 89, 90, 91, 92, 93, 94, 95, 118, 119, 120, 121, 0};

const UINT8 coll_m19_surface[] = {0u, 0};

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
extern UINT8 flag_night_mode;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void map_ended(void) BANKED;
extern void night_mode(void) BANKED;
extern void die(void) BANKED;
extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;

Sprite* s_statue = 0;
INT8 timer_tile_anim = 0;
UINT8 idx_tile_anim = 0u;

void START(void){
    mission_iscrono = 1;
    timemax_current = TIME_MAX_MISSION19;
    time_factor = TIME_FACTOR_MISSION19;
    timer_tile_anim = TIME_MAX_TILE_ANIM;
    idx_tile_anim = 0u;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 6u << 3;
        pos_horse_y = (UINT16) 6u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        current_step = EXIT;
        s_statue = SpriteManagerAdd(SpriteStatue, ((UINT16) 56u) << 3, ((UINT16) 37u) << 3);
        mission_completed = 0;
        time_factor = TIME_FACTOR_MISSION19;
        timemax_current = TIME_MAX_MISSION19;
        time_to_load = timemax_current;
    }else{
        mirror_horse = NO_MIRROR;
        SpriteManagerRemoveSprite(s_statue);
        current_step = EXIT;
        mission_completed = 1;
        s_statue = 0;
    }
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
    //COMMONS & START
        InitScroll(BANK(mapmission19), &mapmission19, coll_m19_tiles, coll_m19_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        Anim_mapegypt_1();
}

void UPDATE(void){
    //LIMIT MAP
        if(s_horse->x < 40u){
            s_horse->x = 40u;
        }
    //UPDATE TIME
        update_time();
        time_current--;
        if(time_current < 0 && !track_ended){
            die();
        }
    //MISSION STEP
        if(current_step == SENATOR_COLLIDED){
            pos_horse_x = s_horse->x;
            pos_horse_y = s_horse->y;
            turn_to_load = turn;
            state_move_to_papyrus(MISSION19_STATUE_MESSAGE, StateMission19egypt);
        }
    //COMMON UPDATE
        update_common();
    //TILE ANIM
        timer_tile_anim--;
        if(timer_tile_anim <= 0){
            timer_tile_anim = TIME_MAX_TILE_ANIM;
            idx_tile_anim++;
            switch(idx_tile_anim){
                case 1: Anim_mapegypt_1(); break;
                case 2: Anim_mapegypt_2(); break;
                case 3: Anim_mapegypt_3(); break;
                case 4: Anim_mapegypt_4(); break;
                case 5: Anim_mapegypt_5(); idx_tile_anim = 0u; break;
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