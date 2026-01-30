#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Music.h"

#include "custom_datas.h"


DECLARE_MUSIC(titlescreen);
DECLARE_MUSIC(credits);
DECLARE_MUSIC(tutoriallist);
DECLARE_MUSIC(tutorial03);
DECLARE_MUSIC(tutorial47);
DECLARE_MUSIC(tutorial813);
DECLARE_MUSIC(stage);
DECLARE_MUSIC(end);


extern MISSION current_mission;
extern INT8 credit_step;
extern TUTORIAL_STAGE tutorial_state;

void set_bgm() BANKED{
    switch(current_state){
        case StateCredit:{
            switch(credit_step){
                case 1: PlayMusic(credits, 1);      break;
                case 3: PlayMusic(titlescreen, 1);  break;
            }
        }break;
        case StateTutorialList:{
            PlayMusic(tutoriallist, 0);
        }break;
        case StateTutorialGame:{
            if(tutorial_state < TUTORIAL_STAGE_4_TURNRIGHTLEFT){
                PlayMusic(tutorial03, 1);
            }else if(tutorial_state < TUTORIAL_STAGE_8_GLADIO){
                PlayMusic(tutorial47, 1);
            }else if(tutorial_state < TUTORIAL_PASSED){
                PlayMusic(tutorial813, 1);
            }
        }break;
        case StateMission00rome:
        case StateMission01rome:{
            PlayMusic(stage, 1);
        }break;
        case StatePapyrus:{
            PlayMusic(end, 1);
        }break;

    }
}