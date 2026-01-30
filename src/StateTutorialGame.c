#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Music.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"

#define TIME_MAX_TUTORIAL1 3840 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_TUTORIAL1 120

#define TIME_MAX_TUTORIAL2 3200 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_TUTORIAL2 100

#define TIME_MAX_TUTORIAL3 1600 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR_TUTORIAL3 50

#define TIME_MAX 7680 //32 fattore 1, 320 fattore 10, 640 fattore 20, ...
#define TIME_FACTOR 240


IMPORT_MAP(hudm);
IMPORT_MAP(map);
IMPORT_MAP(maptut00straight);
IMPORT_MAP(maptut01turnright);
IMPORT_MAP(maptut02turnleft);
IMPORT_MAP(maptut03turnrightleft);
IMPORT_MAP(maptut04dodgewater);
IMPORT_MAP(maptut04zigzag);
IMPORT_MAP(maptut05straight);

const UINT8 coll_rome_tiles[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 118, 119, 121, 0};
const UINT8 coll_rome_surface[] = {0u, 0};

Sprite* s_fantoccio = 0;
INT8 is_crono = 0;
INT8 fantoccio_hit = 0;

void hit_fantoccio(Sprite* s_fantoccio_arg) BANKED;

extern TUTORIAL_STAGE tutorial_state;
extern Sprite* s_biga;
extern Sprite* s_horse;
extern Sprite* s_compass;
extern UINT16 euphoria_min_current;
extern UINT16 euphoria_max_current;
extern UINT16 euphoria_min;
extern UINT16 euphoria_max;
extern INT16 time_current; 
extern INT16 timemax_current;
extern INT16 time_factor;
extern UINT8 track_ended;
extern INT8 track_ended_cooldown;
extern UINT8 hud_initialized;
extern INT8 onwater_countdown; 
extern INT8 velocity;
extern UINT8 turn;
extern UINT16 pos_horse_x;
extern UINT16 pos_horse_y;
extern UINT8 turn_to_load;
extern Sprite* s_spawning_weapon;
extern INT8 spawning_weapon_counter;

extern void start_common() BANKED;
extern void update_stamina() BANKED;
extern void update_compass() BANKED;
extern void update_turning() BANKED;
extern void update_euphoria() BANKED;
extern void update_time() BANKED;
extern void update_hp(INT8 variation) BANKED;
extern void calculate_danger(Sprite* s_danger) BANKED;
extern void check_danger() BANKED;
extern void show_danger() BANKED;
extern void fantoccio_move(Sprite* s_fantoccio_arg) BANKED;
extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern void set_bgm() BANKED;

void START() {
    fantoccio_hit = 0;
    pos_horse_x = 0;
    pos_horse_y = 0;
	spawning_weapon_counter = 0;
    //INITIAL POSITIONS & OPTIONS
        switch(tutorial_state){
            case TUTORIAL_STAGE_0_STRAIGHT:
            case TUTORIAL_STAGE_2_TURNRIGHT:
            case TUTORIAL_STAGE_5_ZIGZAG:
            case TUTORIAL_STAGE_7_DODGEWATER:
            case TUTORIAL_STAGE_8_GLADIO:
            case TUTORIAL_STAGE_10_LANCE:
            case TUTORIAL_STAGE_11_CAPE:
            case TUTORIAL_STAGE_12_STRAW:
                pos_horse_x = 56;
                pos_horse_y = 88;
                is_crono = 0;
            break;
            case TUTORIAL_STAGE_1_STRAIGHTTIME:
                pos_horse_x = 56;
                pos_horse_y = 88;
                is_crono = 1;
                timemax_current = TIME_MAX_TUTORIAL3;
                time_factor = TIME_FACTOR_TUTORIAL3;
            break;
            case TUTORIAL_STAGE_3_TURNLEFT:
                pos_horse_x = 56;
                pos_horse_y = 200;
                is_crono = 0;
            break;
            case TUTORIAL_STAGE_4_TURNRIGHTLEFT:
                pos_horse_x = 56;
                pos_horse_y = 88;
                is_crono = 1;
                timemax_current = TIME_MAX_TUTORIAL2;
                time_factor = TIME_FACTOR_TUTORIAL2;
            break;
            case TUTORIAL_STAGE_6_ZIGZAG_ONTIME:
                pos_horse_x = 56;
                pos_horse_y = 88;
                is_crono = 1;
                timemax_current = TIME_MAX_TUTORIAL3;
                time_factor = TIME_FACTOR_TUTORIAL3;
            break;
            case TUTORIAL_STAGE_9_GLADIOLEFT:
                pos_horse_x = 1120;
                pos_horse_y = 88;
                is_crono = 0;
            break;
        }
    //SPRITES
        s_biga = SpriteManagerAdd(SpriteBiga, pos_horse_x - 20, pos_horse_y + 9);
        s_horse = SpriteManagerAdd(SpriteHorse, pos_horse_x, pos_horse_y);
        s_compass = SpriteManagerAdd(SpriteCompass, pos_horse_x, pos_horse_y);
        scroll_target = SpriteManagerAdd(SpriteCamera, pos_horse_x + 8, pos_horse_y - 16);
    //COMMON AND VARS
        turn_to_load = 0;
        switch(tutorial_state){
            case TUTORIAL_STAGE_0_STRAIGHT:
		        InitScroll(BANK(maptut00straight), &maptut00straight, coll_rome_tiles, coll_rome_surface);
                update_hp(16); 
            break;
            case TUTORIAL_STAGE_1_STRAIGHTTIME:
		        InitScroll(BANK(maptut00straight), &maptut00straight, coll_rome_tiles, coll_rome_surface);
            break;
            case TUTORIAL_STAGE_2_TURNRIGHT:
		        InitScroll(BANK(maptut01turnright), &maptut01turnright, coll_rome_tiles, coll_rome_surface);
            break;
            case TUTORIAL_STAGE_3_TURNLEFT:
		        InitScroll(BANK(maptut02turnleft), &maptut02turnleft, coll_rome_tiles, coll_rome_surface);
            break;
            case TUTORIAL_STAGE_4_TURNRIGHTLEFT:
		        InitScroll(BANK(maptut03turnrightleft), &maptut03turnrightleft, coll_rome_tiles, coll_rome_surface);
            break;
            case TUTORIAL_STAGE_5_ZIGZAG:
            case TUTORIAL_STAGE_6_ZIGZAG_ONTIME:
		        InitScroll(BANK(maptut04zigzag), &maptut04zigzag, coll_rome_tiles, coll_rome_surface);
            break;
            case TUTORIAL_STAGE_7_DODGEWATER:
		        InitScroll(BANK(maptut04dodgewater), &maptut04dodgewater, coll_rome_tiles, coll_rome_surface);
            break;
            case TUTORIAL_STAGE_8_GLADIO:{
                s_fantoccio = SpriteManagerAdd(SpriteFantoccio, s_horse->x + 256u, s_horse->y + 8u);
		        InitScroll(BANK(maptut00straight), &maptut00straight, coll_rome_tiles, coll_rome_surface);
            }break;
            case TUTORIAL_STAGE_9_GLADIOLEFT:{
                //velocity = -1;
                s_biga->x = s_horse->x + 20u;
                s_biga->mirror = V_MIRROR;
                turn_to_load = 127;
                s_horse->mirror = V_MIRROR;
                s_fantoccio = SpriteManagerAdd(SpriteFantoccio, s_horse->x - 400u, s_horse->y - 12u);
		        InitScroll(BANK(maptut05straight), &maptut05straight, coll_rome_tiles, coll_rome_surface);
            }break;
            case TUTORIAL_STAGE_10_LANCE:{
                s_fantoccio = SpriteManagerAdd(SpriteFantoccio, s_horse->x + 600u, s_horse->y);
      	        InitScroll(BANK(maptut00straight), &maptut00straight, coll_rome_tiles, coll_rome_surface);
            }break;
            case TUTORIAL_STAGE_11_CAPE:{
                item_spawn(TIME, s_horse->x + 32u, s_horse->y + 8u);
                SpriteManagerAdd(SpriteFlame, s_horse->x + 200u, s_horse->y);
		        InitScroll(BANK(maptut00straight), &maptut00straight, coll_rome_tiles, coll_rome_surface);
            }break;
            case TUTORIAL_STAGE_12_STRAW:{
                item_spawn(FIRE, s_horse->x + 80u, s_horse->y);
                Sprite* s_straw = SpriteManagerAdd(SpriteStraw, s_horse->x + 200u, s_horse->y);
                Sprite* s_straw2 = SpriteManagerAdd(SpriteStraw, s_horse->x + 224u, s_horse->y);
		        InitScroll(BANK(maptut00straight), &maptut00straight, coll_rome_tiles, coll_rome_surface);
            }break;
        }
        update_hp(0); 
		INIT_HUD(hudm);
		SetWindowY(104);
        start_common();
}

void UPDATE(){
	//LIMIT MAP LEFT
        if(tutorial_state < TUTORIAL_PASSED && tutorial_state != TUTORIAL_STAGE_9_GLADIOLEFT){
            if(s_horse->x < 40u){
                s_horse->x = 40u;
            }
        }
    //LIMIT MAP RIGHT
        if(tutorial_state == TUTORIAL_STAGE_9_GLADIOLEFT){
            if(s_horse->x > 1120u){
                s_horse->x = 1120u;
            }
        }
        
	//HUD
		print_target = PRINT_WIN;
        if(hud_initialized == 0){
            update_hp(0);
            hud_initialized = 1u;
        }
    //CONTINUOUS SPAWNING WEAPON
        if(s_spawning_weapon == 0){
            spawning_weapon_counter++;
            if(spawning_weapon_counter < 0){
                spawning_weapon_counter = 0;
                switch(tutorial_state){
                    case TUTORIAL_STAGE_8_GLADIO:
                        item_spawn_continuously(GLADIO, pos_horse_x + 40u, pos_horse_y + 4u);
                    break;
                    case TUTORIAL_STAGE_9_GLADIOLEFT:
                        item_spawn_continuously(GLADIO, pos_horse_x - 128u, pos_horse_y + 4u);
                    break;
                    case TUTORIAL_STAGE_10_LANCE:
                        item_spawn_continuously(LANCE, pos_horse_x + 80u, pos_horse_y);
                    break;
                    case TUTORIAL_STAGE_11_CAPE:
                        item_spawn_continuously(CAPE, pos_horse_x + 80u, pos_horse_y);
                    break;                    
                }
            }
        }
    //CALCULATE DANGER
        if(fantoccio_hit == 0){
            switch(tutorial_state){
                case TUTORIAL_STAGE_8_GLADIO:
                case TUTORIAL_STAGE_9_GLADIOLEFT:
                case TUTORIAL_STAGE_10_LANCE:
                    calculate_danger(s_fantoccio);
                    check_danger();
                    show_danger();
                break;
            }
        }
	//UPDATE STAMINA
		update_stamina();
	//UPDATE COMPASS
		update_compass();
	//UPDATE TURNING
		update_turning();
	//UPDATE EUPHORIA?
		if(euphoria_min_current != euphoria_min || euphoria_max_current != euphoria_max){
			update_euphoria();
		}
	//UPDATE TIME
        if(is_crono == 1){
            update_time();
            time_current--;
            if(time_current < 0){
                SetState(StateTutorialGame);
            }
        }
    //CHECK ON WATER
        if(tutorial_state == TUTORIAL_STAGE_7_DODGEWATER && onwater_countdown > 0){
            SetState(StateTutorialGame);
        }
    //IS TRACK COMPLETED?
        if(track_ended == 1u){
            PauseMusic;
            track_ended_cooldown--;
            if(track_ended_cooldown <= 0){//cambia stato
                INT8 can_go_on = 1;
                if(tutorial_state == TUTORIAL_STAGE_8_GLADIO || 
                    tutorial_state == TUTORIAL_STAGE_9_GLADIOLEFT ||
                    tutorial_state == TUTORIAL_STAGE_10_LANCE){//check fantoccio hit
                    if(fantoccio_hit == 0){
                        can_go_on = 0;
                    }
                }
                if(can_go_on == 1){
                    tutorial_state++;
                }
                SetState(StateTutorialList);
            }
        }
}

void hit_fantoccio(Sprite* s_fantoccio_arg) BANKED{
    fantoccio_hit = 1;
    struct FantoccioData* f_data = (struct FantoccioData*) s_fantoccio_arg->custom_data;
    f_data->fantoccio_counter = 81;
    fantoccio_move(s_fantoccio_arg);
}