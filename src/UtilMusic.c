#include "Banks/SetAutoBank.h"

#include <gbdk/platform.h>
#include "BankManager.h"
#include "ZGBMain.h"
#include "Music.h"

#include "custom_datas.h"


DECLARE_MUSIC(J1);
DECLARE_MUSIC(J2);
DECLARE_MUSIC(J3);
DECLARE_MUSIC(L1);
DECLARE_MUSIC(L2);
DECLARE_MUSIC(L3);
DECLARE_MUSIC(L4);
DECLARE_MUSIC(L5);
DECLARE_MUSIC(L6);
DECLARE_MUSIC(B1);
DECLARE_MUSIC(B2);


extern MISSION current_mission;
extern INT8 credit_step;
extern TUTORIAL_STAGE tutorial_state;
extern INSTRUCTION instruction_given;

void set_bgm(void) BANKED;


void set_bgm(void) BANKED{
    switch(current_state){
        case StateCredit:
            switch(credit_step){
                case 1:
                    PlayMusic(J1, 1);
                break;
                case 5:
                    PlayMusic(J3, 1);
                break;
            }
        break;
        case StateTutorialList:
            PlayMusic(J3, 0);
        break;
        case StateTutorialGame:
            if(tutorial_state < TUTORIAL_STAGE_4_TURNRIGHTLEFT){
                PlayMusic(L1, 1);
            }else if(tutorial_state < TUTORIAL_STAGE_8_GLADIO){
                PlayMusic(L2, 1);
            }else if(tutorial_state < TUTORIAL_PASSED){
                PlayMusic(L3, 1);
            }
        break;
        case StateMission00rome:
        case StateMission01rome:
        case StateMission02rome:
        case StateMission03rome:
            PlayMusic(L1, 1);
        break;
        case StateMission04alps:
        case StateMission05alps:
        case StateMission06alps:
            PlayMusic(L2, 1);
        break;
        case StateMission07alps:
            PlayMusic(B1, 1);
        break;
        case StateMission08sea:
        case StateMission09sea:
        case StateMission10sea:
            PlayMusic(L3, 1);
        break;
        case StateMission11sea:
            PlayMusic(B1, 1);
        break;
        case StateMission12greece:
        case StateMission13greece:
        case StateMission14greece:
            PlayMusic(L4, 1);
        break;
        case StateMission15greece:
            PlayMusic(B2, 1);
        break;
        case StateMission16desert:
        case StateMission17desert:
        case StateMission18desert:
            PlayMusic(L5, 1);
        break;
        case StateMission19egypt:
        case StateMission20egypt:
            PlayMusic(L6, 1);
        break;
        case StateMission21egypt:
            PlayMusic(B2, 1);
        break;
        case StatePapyrus:
            if(instruction_given == DEAD){
                PlayMusic(J2, 1);
            }else{
                PlayMusic(J3, 1);
            }
        break;
    }
}