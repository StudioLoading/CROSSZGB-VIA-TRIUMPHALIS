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
IMPORT_MAP(mapmission00);

const UINT8 coll_m00_tiles[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 75, 76, 77, 78, 79, 80, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 107, 112, 114, 118, 119, 121, 0};

const UINT8 coll_m00_surface[] = {0u, 0};

Sprite* s_senator = 0;
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
extern INT8 mission_iscrono;

extern void start_common(void) BANKED;
extern void update_common(void) BANKED;
extern void spawn_items(void) BANKED;
extern void map_ended(void) BANKED;
extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;

void START(void){
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 6u << 3;
        pos_horse_y = (UINT16) 7u << 3;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
    }
    mission_iscrono = 0;        
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        if(current_step == LOOKING_FOR_SENATOR){
            s_senator = SpriteManagerAdd(SpriteRomansenator, ((UINT16) 60u) << 3, ((UINT16) 43u) << 3);
            mission_completed = 0;
        }else{
            SpriteManagerRemoveSprite(s_senator);
            current_step = EXIT;
            mission_completed = 1;
            s_senator = 0;
        }
    //COMMONS & START
        InitScroll(BANK(mapmission00), &mapmission00, coll_m00_tiles, coll_m00_surface);
		INIT_HUD(hudm);
		SetWindowY(104);
        spawn_items();
        start_common();
}

void UPDATE(void){
    //COMMON UPDATE
        update_common();
    //LIMIT MAP LEFT
        if(s_horse->x < 40u){
            s_horse->x = 40u;
        }
        if(s_horse->x > ((UINT16) 200u << 3)){
            if(current_step < EXIT){
                s_horse->x = ((UINT16) 200u << 3);
            }
        }
    
    //MISSION STEP
        if(current_step == SENATOR_COLLIDED){
            pos_horse_x = s_horse->x;
            pos_horse_y = s_horse->y;
            turn_to_load = turn;
            state_move_to_papyrus(MISSION00_SECRET_MESSAGE, StateMission00rome);
        }
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}