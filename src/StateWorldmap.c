#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "Dialogs.h"
#include "UtilAnim.h"
#include "custom_datas.h"

#define WORLDMAP_COUNTDOWN_MAX 15

IMPORT_MAP(worldmap);
IMPORT_MAP(arearome);
IMPORT_MAP(areaalps);
IMPORT_MAP(areasea);
IMPORT_MAP(areagreece);
IMPORT_MAP(areadesert);
IMPORT_MAP(areaegypt);
IMPORT_TILES(font);

extern UINT8 prev_state;
extern UINT8 flag_border_set;

INT8 worldmap_counter = 0;
AREA current_area = AREA_ROME;
MISSION current_mission = MISSIONROME00;
INT8 world_area_map = 0;//0=worldmap, 1=areamap
TUTORIAL_STAGE tutorial_state = TUTORIAL_STAGE_0_STRAIGHT;//TUTORIAL_STAGE_0_STRAIGHT

void start_game() BANKED;
void start_game_cheat(AREA arg_cheat_area) BANKED;

extern void state_move_to_papyrus(INSTRUCTION arg_instruction_to_show, UINT8 arg_prev_state) BANKED;
extern void manage_border(UINT8 my_next_state) BANKED;
extern void check_sgb_palette(UINT8 my_next_state) BANKED;

void start_game_cheat(AREA arg_cheat_area) BANKED{
    switch(current_area){
        case AREA_ALPS: current_mission = MISSIONALPS06; break;
        case AREA_SEA: current_mission = MISSIONSEA11; break;
        case AREA_GREECE: current_mission = MISSIONGREECE12; break;
        case AREA_DESERT: current_mission = MISSIONDESERT16; break;
        case AREA_EGYPT: current_mission = MISSIONEGYPT19; break;
    }
    SetState(StateWorldmap);
	manage_border(current_state);
}

void start_game() BANKED{
    SetState(StateTutorialList);
	manage_border(current_state);
}

void START() {
    if(flag_border_set == 1){
        //activate flag for border management
        switch(current_mission){
            case MISSIONALPS04:
            case MISSIONSEA08:
            case MISSIONGREECE12:
            case MISSIONDESERT16:
            case MISSIONEGYPT19:
                flag_border_set = 0u;
            break;
        }
    }
	manage_border(current_state);
    switch(world_area_map){
        case 0:
            InitScroll(BANK(worldmap), &worldmap, 0, 0);
        break;
        case 1:
            if(current_mission <= MISSIONROME03){
                InitScroll(BANK(arearome), &arearome, 0, 0);
            }else if(current_mission <= MISSIONALPS07){
                InitScroll(BANK(areaalps), &areaalps, 0, 0);
            }else if(current_mission <= MISSIONSEA11){
                InitScroll(BANK(areasea), &areasea, 0, 0);
            }else if(current_mission <= MISSIONGREECE15){
                InitScroll(BANK(areagreece), &areagreece, 0, 0);
            }else if(current_mission <= MISSIONDESERT18){
                InitScroll(BANK(areadesert), &areadesert, 0, 0);
            }else if(current_mission <= MISSIONEGYPT21){
                InitScroll(BANK(areaegypt), &areaegypt, 0, 0);
            }
        break;
    }
    SetWindowY(144);
	SHOW_BKG;
    INIT_FONT(font, PRINT_BKG);
    worldmap_counter = WORLDMAP_COUNTDOWN_MAX;
    print_target = PRINT_BKG;
    PRINT(14, 0, "CONFIG");
    switch(world_area_map){
        case 0:
            switch(current_area){
                case AREA_ROME:
                case AREA_ALPS:
                case AREA_SEA:
                    PRINT(4, 16, "ROMAN EMPIRE");
                break;
                case AREA_GREECE:
                    PRINT(4, 16, "   GREECE   ");
                break;
                case AREA_DESERT:
                    PRINT(4, 16, "  FAR EAST  ");
                break;
                case AREA_EGYPT:
                    PRINT(4, 16, "   EGYPT    ");
                break;
            }
        break;
        case 1:
            switch(current_mission){
                case MISSIONROME00: PRINT(2, 15, "A SECRET MESSAGE"); break;
                case MISSIONROME01: PRINT(4, 15, "RUN TO SAFETY"); break;
                case MISSIONROME02: PRINT(4, 15, "  SURVIVE    "); break;
                case MISSIONROME03: PRINT(4, 15, " HURRY UP!   "); break;
                case MISSIONALPS04: PRINT(4, 15, " FIRST BLOOD "); break;
                case MISSIONALPS05: PRINT(2, 15, "SAVE THE GENERAL"); break;
                case MISSIONALPS06: PRINT(4, 15, "LANDSLIDES!  "); break;
                case MISSIONALPS07: PRINT(4, 15, "   AMBUSH!   "); break;
                case MISSIONSEA08: PRINT(4, 15, "A SECRET MAP "); break;
                case MISSIONSEA09: PRINT(0, 15, "SAVE THE AMBASSADOR"); break;
                case MISSIONSEA10: PRINT(0, 15, "   TO THE SHIP!    "); break;
                case MISSIONSEA11: PRINT(4, 15, "   AMBUSH!   "); break;
                case MISSIONGREECE12: PRINT(5, 15, "GREEK IDOL"); break;
                case MISSIONGREECE13: PRINT(0, 15, "HIDE AND LISTEN"); break;
                case MISSIONGREECE14: PRINT(0, 15, "YOU'D BETTER RUN"); break;
                case MISSIONGREECE15: PRINT(5, 15, "THE PHARAOH"); break;
                case MISSIONDESERT16: PRINT(1, 15, "LOST IN THE DESERT"); break;
                case MISSIONDESERT17: PRINT(1, 15, "  NIGHTLY DESERT  "); break;
                case MISSIONDESERT18: PRINT(1, 15, "   THE BEDUINE    "); break;
                case MISSIONEGYPT19: PRINT(1, 15, " A TEMPLE ON FIRE "); break;
                case MISSIONEGYPT20: PRINT(1, 15, "    A SACRIFICE   "); break;
                case MISSIONEGYPT21: PRINT(1, 15, "    LAST BLOOD    "); break;
            }
        break;
    }
}

void UPDATE() {
    worldmap_counter--;
    if(worldmap_counter < -WORLDMAP_COUNTDOWN_MAX){ worldmap_counter = WORLDMAP_COUNTDOWN_MAX; return;}
    if(worldmap_counter > 0){
        if(world_area_map == 0){//worldmap
            Anim_worldmap_1(current_area);
        }else{//areamap
            switch(current_mission){
                case MISSIONROME00: Anim_arearome_1(); break;
                case MISSIONROME01: Anim_arearome_1(); break;
                case MISSIONROME02: Anim_arearome_2(); break;
                case MISSIONROME03: Anim_arearome_3(); break;
                case MISSIONALPS04: Anim_areaalps_1(); break;
                case MISSIONALPS05: Anim_areaalps_2(); break;
                case MISSIONALPS06: Anim_areaalps_3(); break;
                case MISSIONALPS07: Anim_areaalps_4(); break;
                case MISSIONSEA08: Anim_areasea_1(); break;
                case MISSIONSEA09: Anim_areasea_2(); break;
                case MISSIONSEA10: Anim_areasea_3(); break;
                case MISSIONSEA11: Anim_areasea_4(); break;
                case MISSIONGREECE12: Anim_areagreece_1(); break;
                case MISSIONGREECE13: Anim_areagreece_2(); break;
                case MISSIONGREECE14: Anim_areagreece_3(); break;
                case MISSIONGREECE15: Anim_areagreece_4(); break;
                case MISSIONDESERT16: Anim_areadesert_1(); break;
                case MISSIONDESERT17: Anim_areadesert_2(); break;
                case MISSIONDESERT18: Anim_areadesert_3(); break;
                case MISSIONEGYPT19: Anim_areaegypt_1(); break;
                case MISSIONEGYPT20: Anim_areaegypt_2(); break;
                case MISSIONEGYPT21: Anim_areaegypt_3(); break;
            }
        }
    }else{
        if(world_area_map == 0){//worldmap
            Anim_worldmap_0();
        }else{//areamap
            switch(current_mission){
                case MISSIONROME00:
                case MISSIONROME01:
                case MISSIONROME02:
                case MISSIONROME03:
                    Anim_arearome_0();
                break;
                case MISSIONALPS04:
                case MISSIONALPS05:
                case MISSIONALPS06:
                case MISSIONALPS07:
                    Anim_areaalps_0();
                break;
                case MISSIONSEA08:
                case MISSIONSEA09:
                case MISSIONSEA10:
                case MISSIONSEA11:
                    Anim_areasea_0();
                break;
                case MISSIONGREECE12:
                case MISSIONGREECE13:
                case MISSIONGREECE14:
                case MISSIONGREECE15:
                    Anim_areagreece_0();
                break;
                case MISSIONDESERT16:
                case MISSIONDESERT17:
                case MISSIONDESERT18:
                    Anim_areadesert_0();
                break;
                case MISSIONEGYPT19:
                case MISSIONEGYPT20:
                case MISSIONEGYPT21:
                    Anim_areaegypt_0();
                break;
            }
        }
    }
    if(KEY_TICKED(J_START)){
        world_area_map++;
        if(world_area_map > 1){
            world_area_map = 1;
            INSTRUCTION instruction_to_give = 0;
            UINT8 prev_state_to_give = 0;
            switch(current_mission){
                case MISSIONROME00:
                    prev_state_to_give = StateMission00rome;
                    instruction_to_give = MISSION00_INTRO;
                break;
                case MISSIONROME01:
                    prev_state_to_give = StateMission01rome;
                    instruction_to_give = MISSION01_INTRO;
                break;
                case MISSIONROME02:
                    prev_state_to_give = StateMission02rome;
                    instruction_to_give = MISSION02_INTRO;
                break;
                case MISSIONROME03:
                    prev_state_to_give = StateMission03rome;
                    instruction_to_give = MISSION03_INTRO;
                break;
                case MISSIONALPS04:
                    prev_state_to_give = StateMission04alps;
                    instruction_to_give = MISSION04_INTRO;
                break;
                case MISSIONALPS05:
                    prev_state_to_give = StateMission05alps;
                    instruction_to_give = MISSION05_INTRO;
                break;
                case MISSIONALPS06:
                    prev_state_to_give = StateMission06alps;
                    instruction_to_give = MISSION06_INTRO;
                break;
                case MISSIONALPS07:
                    prev_state_to_give = StateMission07alps;
                    instruction_to_give = MISSION07_INTRO;
                break;
                case MISSIONSEA08:
                    prev_state_to_give = StateMission08sea;
                    instruction_to_give = MISSION08_INTRO;
                break;
                case MISSIONSEA09:
                    prev_state_to_give = StateMission09sea;
                    instruction_to_give = MISSION09_INTRO;
                break;
                case MISSIONSEA10:
                    prev_state_to_give = StateMission10sea;
                    instruction_to_give = MISSION10_INTRO;
                break;
                case MISSIONSEA11:
                    prev_state_to_give = StateMission11sea;
                    instruction_to_give = MISSION11_INTRO;
                break;
                case MISSIONGREECE12:
                    prev_state_to_give = StateMission12greece;
                    instruction_to_give = MISSION12_INTRO;
                break;
                case MISSIONGREECE13:
                    prev_state_to_give = StateMission13greece;
                    instruction_to_give = MISSION13_INTRO;
                break;
                case MISSIONGREECE14:
                    prev_state_to_give = StateMission14greece;
                    instruction_to_give = MISSION14_INTRO;
                break;
                case MISSIONGREECE15:
                    prev_state_to_give = StateMission15greece;
                    instruction_to_give = MISSION15_INTRO;
                break;
                case MISSIONDESERT16:
                    prev_state_to_give = StateMission16desert;
                    instruction_to_give = MISSION16_INTRO;
                break;
                case MISSIONDESERT17:
                    prev_state_to_give = StateMission17desert;
                    instruction_to_give = MISSION17_INTRO;
                break;
                case MISSIONDESERT18:
                    prev_state_to_give = StateMission18desert;
                    instruction_to_give = MISSION18_INTRO;
                break;
                case MISSIONEGYPT19:
                    prev_state_to_give = StateMission19egypt;
                    instruction_to_give = MISSION19_INTRO;
                break;
                case MISSIONEGYPT20:
                    prev_state_to_give = StateMission20egypt;
                    instruction_to_give = MISSION20_INTRO;
                break;
                case MISSIONEGYPT21:
                    prev_state_to_give = StateMission21egypt;
                    instruction_to_give = MISSION21_INTRO;
                break;
            }
            state_move_to_papyrus(instruction_to_give, prev_state_to_give);
        }else{
            SetState(StateWorldmap);
        }
    }
    if(KEY_TICKED(J_SELECT)){//vai alla schermata di configurazione
        SetState(StateConfig);
    }
}

