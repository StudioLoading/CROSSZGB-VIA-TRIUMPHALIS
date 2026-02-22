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
IMPORT_MAP(mapmission01);

const UINT8 coll_m01_tiles[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 112, 114, 118, 119, 121, 0};

const UINT8 coll_m01_surface[] = {0u, 0};

Sprite* s_romansoldier00 = 0;
Sprite* s_romansoldier01 = 0;
Sprite* s_romansoldier02 = 0;
Sprite* s_romansoldier03 = 0;

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
extern INT8 spawning_weapon_counter;
extern Sprite* s_spawning_weapon;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger(void) BANKED;
extern void show_danger(void) BANKED;
extern void map_ended(void) BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;


void START(void){
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else{//initial
        pos_horse_x = (UINT16) 3u << 3;
        pos_horse_y = (UINT16) 34u << 3;
        mirror_horse = NO_MIRROR;
    }
    current_step = EXIT;
    mission_completed = 1;
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
    //COMMONS & START
        //BGP_REG = PAL_DEF(3, 3, 1, 3);// NIGHT MODE
        InitScroll(BANK(mapmission01), &mapmission01, coll_m01_tiles, coll_m01_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        start_common();
}


void UPDATE(void){
    //COMMON UPDATE
        update_common();
    //LIMIT MAP LEFT
        if(s_horse->x < 40u){
            s_horse->x = 40u;
        }
    //CALCULATE DANGER
        calculate_danger(s_romansoldier00);
        calculate_danger(s_romansoldier01);
        calculate_danger(s_romansoldier02);
        calculate_danger(s_romansoldier03);
        check_danger();
        show_danger();    
    //CONTINUOUS SPAWNING WEAPON
        if(s_spawning_weapon == 0){
            spawning_weapon_counter++;
            if(spawning_weapon_counter < 0){
                spawning_weapon_counter = 0;
                item_spawn_continuously(LANCE, ((UINT16) 39u << 3), ((UINT16) 34u << 3));
            }
        }
    //MISSION STEP
        /*if(current_step == SENATOR_COLLIDED){
            pos_horse_x = s_horse->x;
            pos_horse_y = s_horse->y;
            turn_to_load = turn;
            state_move_to_papyrus(MISSION00_SECRET_MESSAGE, StateMission00romeUINT8);
        }*/
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}