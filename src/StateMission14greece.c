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
IMPORT_MAP(mapmission14);

const UINT8 coll_m14_tiles[] = {15, 16, 17, 18, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 48, 49, 50, 51, 52, 53, 56, 57, 63, 64, 67, 68, 69, 75, 76, 77, 78, 79, 82, 85, 86, 93, 95, 96, 97, 98, 99, 101, 102, 103, 104, 105, 106, 108, 109, 118, 119, 120, 121, 0};

const UINT8 coll_m14_surface[] = {0u, 0};

Sprite* s_greeksoldier00;
Sprite* s_greeksoldier01;
Sprite* s_greeksoldier02;
Sprite* s_greeksoldier03;
Sprite* s_greeksoldier04;
Sprite* s_greeksoldier05;
Sprite* s_greeksoldier06;
UINT8 flag_spawned_greek2 = 0u;

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

extern void start_common() BANKED;
extern void update_common() BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger() BANKED;
extern void show_danger() BANKED;
extern void update_time() BANKED;
extern void spawn_items() BANKED;
extern void die() BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void map_ended() BANKED;
extern void night_mode() BANKED;

void m14_spawn_greeks() BANKED;
void m14_spawn_straw() BANKED;
void m14_spawn_greeks2() BANKED;

void START(){
    mission_iscrono = 0;
    if(flag_golden_found == 1){//uso pos_horse_x per come l'ho salvata
        flag_golden_found = 0;
    }else if(current_step == LOOKING_FOR_SENATOR){//initial
        pos_horse_x = (UINT16) 10u << 3;
        pos_horse_y = ((UINT16) 7u << 3) +3u;
        mirror_horse = NO_MIRROR;
        turn_to_load = 0;
        current_step = EXIT;
        mission_completed = 1;
    }
    //SPRITES
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 14, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
    //COMMONS & START
        InitScroll(BANK(mapmission14), &mapmission14, coll_m14_tiles, coll_m14_surface);
		INIT_HUD(hudm);
        flag_spawned_greek2 = 0u;
		SetWindowY(104);
        m14_spawn_greeks();
        m14_spawn_straw();
        spawn_items();
        start_common();
}

void m14_spawn_straw() BANKED{
    SpriteManagerAdd(SpriteStraw, ((UINT16) 94u << 3) , ((UINT16) 6u << 3) + 4);
    SpriteManagerAdd(SpriteStraw, ((UINT16) 162u << 3) , ((UINT16) 7u << 3) + 4);
}

void m14_spawn_greeks() BANKED{
    s_greeksoldier00 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 33u << 3), ((UINT16) 4u << 3));
    struct SoldierData* greeksoldier00_data = (struct SoldierData*) s_greeksoldier00->custom_data;
    greeksoldier00_data->frmskip_max = 2u;
    greeksoldier00_data->configured = 6;
    greeksoldier00_data->reward = NOITEM;

    s_greeksoldier01 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 40u << 3), ((UINT16) 8u << 3));
    struct SoldierData* greeksoldier01_data = (struct SoldierData*) s_greeksoldier01->custom_data;
    greeksoldier01_data->frmskip_max = 3u;
    greeksoldier01_data->configured = 6;
    greeksoldier01_data->reward = NOITEM;

    s_greeksoldier02 = SpriteManagerAdd(SpriteKiller, ((UINT16) 114u << 3), ((UINT16) 5u << 3));
    s_greeksoldier03 = SpriteManagerAdd(SpriteKiller, ((UINT16) 120u << 3), ((UINT16) 5u << 3));
    s_greeksoldier04 = SpriteManagerAdd(SpriteKiller, ((UINT16) 132u << 3), ((UINT16) 5u << 3));

    s_greeksoldier05 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 141u << 3), ((UINT16) 4u << 3));
    struct SoldierData* greeksoldier05_data = (struct SoldierData*) s_greeksoldier05->custom_data;
    greeksoldier05_data->frmskip_max = 2u;
    greeksoldier05_data->configured = 6;
    greeksoldier05_data->reward = NOITEM;

    s_greeksoldier06 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 142u << 3), ((UINT16) 10u << 3));
    struct SoldierData* greeksoldier06_data = (struct SoldierData*) s_greeksoldier06->custom_data;
    greeksoldier06_data->frmskip_max = 2u;
    greeksoldier06_data->configured = 6;
    greeksoldier06_data->reward = NOITEM;

}

void m14_spawn_greeks2() BANKED{

    SpriteManagerAdd(SpriteStraw, ((UINT16) 169u << 3) , ((UINT16) 7u << 3) + 4);
    s_greeksoldier00 = SpriteManagerAdd(SpriteKiller, ((UINT16) 159u << 3), ((UINT16) 5u << 3));
    s_greeksoldier01 = SpriteManagerAdd(SpriteKiller, ((UINT16) 167u << 3) + 3u, ((UINT16) 3u << 3) + 2u);
    s_greeksoldier02 = SpriteManagerAdd(SpriteKiller, ((UINT16) 187u << 3), ((UINT16) 4u << 3));

    item_spawn(FIRE, ((UINT16) 199u << 3), ((UINT16) 7u << 3) + 1u);
    SpriteManagerAdd(SpriteStraw, ((UINT16) 208u << 3) , ((UINT16) 7u << 3) + 4);
    s_greeksoldier03 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 191u << 3), ((UINT16) 4u << 3));
    struct SoldierData* greeksoldier03_data = (struct SoldierData*) s_greeksoldier03->custom_data;
    greeksoldier03_data->frmskip_max = 2u;
    greeksoldier03_data->configured = 6;
    greeksoldier03_data->reward = NOITEM;
    s_greeksoldier04 = SpriteManagerAdd(SpriteGreeksoldier, ((UINT16) 190u << 3), ((UINT16) 10u << 3));
    struct SoldierData* greeksoldier04_data = (struct SoldierData*) s_greeksoldier04->custom_data;
    greeksoldier04_data->frmskip_max = 2u;
    greeksoldier04_data->configured = 6;
    greeksoldier04_data->reward = NOITEM;
}

void UPDATE(){
    //COMMON UPDATE
        update_common();
    //FLAG 2
        if(flag_spawned_greek2 == 0u && s_horse->x > ((UINT16) 150u << 3)){
            flag_spawned_greek2 = 1u;
            m14_spawn_greeks2();
        }
    //CALCULATE DANGER
        calculate_danger(s_greeksoldier00);
        calculate_danger(s_greeksoldier01);
        calculate_danger(s_greeksoldier02);
        check_danger();
        show_danger();
    //IS MISSION COMPLETED?
        if(mission_completed && track_ended){
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                map_ended();
            }
        }
}