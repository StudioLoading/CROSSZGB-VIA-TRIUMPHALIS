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

#define TIME_ROLLINGSTONE_MIN 80
#define TIME_ROLLINGSTONE_MED 150
#define TIME_ROLLINGSTONE_MAX 210

#define DELAY_TREMBLE_MIN 3
#define DELAY_TREMBLE_MAX 10
#define ROLLINGSTONE_IDX_MAX 4

IMPORT_MAP(hudm);
IMPORT_MAP(mapmission06);

const UINT8 coll_m06_tiles[] = {15, 16, 17, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106, 114, 118, 119, 121, 0};

const UINT8 coll_m06_surface[] = {0u, 0};

Sprite* s_rollingstone = 0;
Sprite* s_rollingstone2 = 0;
Sprite* s_rollingstone3 = 0;
Sprite* s_rollingstone4 = 0;
UINT16 timer_rollingstone = 0;
UINT8 flag_turn_on_tremble = 0u;
INT8 delay_tremble = 0;
INT8 rollingstone_pos_x[] = {45, 30, 50, 40};
INT8 rollingstone_pos_y[] = {80, 40, 80, 60};
UINT8 rollingstone_idx = 0u;
INT8 delta_x[] = {13, 0, 10, -8, 8, 0, -2, 5, 0, -10, -4, 14, 0, 2};
INT8 delta_x_idx = 0;

void roll_stone(INT8 arg_verse, UINT8 arg_max_frmskip_y) BANKED;

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

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger(void) BANKED;
extern void show_danger(void) BANKED;
extern void update_time(void) BANKED;
extern void spawn_items(void) BANKED;
extern void die(void) BANKED;
extern void map_ended(void) BANKED;

void START(void){
    mission_iscrono = 0;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 5u << 3;
        pos_horse_y = (UINT16) 6u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
    }     
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        current_step = EXIT;
        mission_completed = 1;
    //COMMONS & START
        InitScroll(BANK(mapmission06), &mapmission06, coll_m06_tiles, coll_m06_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
        timer_rollingstone = TIME_ROLLINGSTONE_MAX;
        flag_turn_on_tremble = 0u;
        delay_tremble = 0;
}

void roll_stone(INT8 arg_verse, UINT8 arg_max_frmskip_y) BANKED{
    rollingstone_idx++;
    if(rollingstone_idx >= ROLLINGSTONE_IDX_MAX){
        rollingstone_idx = 0u;
    }
    if(delta_x_idx > delta_x[0]){
        delta_x_idx = 1;
    }
    Sprite* s_rollingstone_in_use = 0;
    switch(rollingstone_idx){
        case 0u: s_rollingstone_in_use = s_rollingstone; break;
        case 1u: s_rollingstone_in_use = s_rollingstone2; break;
        case 2u: s_rollingstone_in_use = s_rollingstone3; break;
        case 3u: s_rollingstone_in_use = s_rollingstone4; break;
    }
    s_rollingstone_in_use = SpriteManagerAdd(SpriteRollingstone, s_horse->x + rollingstone_pos_x[rollingstone_idx] + delta_x[delta_x_idx], s_horse->y - rollingstone_pos_y[rollingstone_idx]);
    struct RollingStoneData* rollingstone_data = (struct RollingStoneData*) s_rollingstone_in_use->custom_data;
    rollingstone_data->verse_x = arg_verse;
    rollingstone_data->frmskip_y = arg_max_frmskip_y;
    rollingstone_data->max_frmskip_y = arg_max_frmskip_y;
}

void UPDATE(void){
    //COMMON UPDATE
        update_common();
    //LIMIT MAP LEFT
        if(s_horse->x < 24u){
            s_horse->x = 24u;
        }
    //CHECK ROLLINGSTONE
        if(s_horse->x > ((UINT16) 40u << 3)){
            if(flag_turn_on_tremble){//screen tremble
                delay_tremble--;
                if(delay_tremble <= 0){
                    delay_tremble = DELAY_TREMBLE_MIN;
                    if(scroll_target->y < (s_horse->y - 6u)){
                        scroll_target->y = s_horse->y + 6u;
                    }else if(scroll_target->y < (s_horse->y + 18u)){
                        scroll_target->y = s_horse->y - 10u;
                    }
                }
            }
            timer_rollingstone--;
            if(timer_rollingstone < 40u){//flag tremble
                if(flag_turn_on_tremble == 0u){
                    flag_turn_on_tremble = 1u;
                }
            }
            if(timer_rollingstone < 10u){//rolling stone
                if(s_horse->x < ((UINT16) 70u << 3)){
                    roll_stone(-1, 10);
                    roll_stone(1, 6);
                    flag_turn_on_tremble = 0u;
                    delay_tremble = 0;
                    timer_rollingstone = TIME_ROLLINGSTONE_MAX;
                }else if(s_horse->x < ((UINT16) 160u << 3)){
                    roll_stone(1, 6);
                    roll_stone(-1, 6);
                    flag_turn_on_tremble = 0u;
                    delay_tremble = 0;
                    timer_rollingstone = TIME_ROLLINGSTONE_MED;
                }else if(s_horse->x < ((UINT16) 190u << 3)){
                    roll_stone(1, 3);
                    roll_stone(1, 6);
                    flag_turn_on_tremble = 0u;
                    delay_tremble = 0;
                    timer_rollingstone = TIME_ROLLINGSTONE_MIN;
                }
            }
        }
    //UPDATE TIME
        /*update_time();
        time_current--;
        if(time_current < 0 && !mission_completed && !track_ended){
            die();
        }*/   
    //MISSION STEP
        /*if(current_step == SENATOR_COLLIDED){
            pos_horse_x = s_horse->x;
            pos_horse_y = s_horse->y;
            turn_to_load = turn;
            time_to_load = time_current;
            state_move_to_papyrus(MISSION05_SAVED_GENERAL, StateMission05alps;
        }*/
    //CALCULATE DANGER
        /*calculate_danger(s_rollingstone);
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