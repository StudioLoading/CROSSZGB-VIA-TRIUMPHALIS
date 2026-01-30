#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_step[] = {1,0}; //The first number indicates the number of frames
const UINT8 a_steponwater[] = {4,0,1,2,3}; 

void START(){
	THIS->lim_x = 1u;
	THIS->lim_y = 1u;
    SetSpriteAnim(THIS, a_step, 16u);
	struct ItemData* step_data = (struct ItemData*) THIS->custom_data;
	step_data->hp = 30u;
}

void UPDATE(){
	struct ItemData* step_data = (struct ItemData*) THIS->custom_data;
	if(step_data->configured == 1){//orme su pozza
    	SetSpriteAnim(THIS, a_steponwater, 16u);
	}
	step_data->hp--;
	if(step_data->hp == 0){
		SpriteManagerRemoveSprite(THIS);
	}
}

void DESTROY(){

}