#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "points_anim.h"

// Animation state enum
typedef enum {
    ANIM_100 = 0,
	ANIM_50,
	ANIM_20,
	ANIM_10,
	ANIM_NONE,
	N_ANIMS
} animation_points;


// define the animation speed constant
#define ANIMATION_POINTS_SPEED      6
#define ANIMATION_POINTS_FRAMES     2
static animation_points old_anim_points, anim_points; 
static UINT8 anim_points_frame, anim_points_tick;
static UINT8 anim_points_speed;

const UINT8 * get_banked_pointer(UINT8 bank, const UINT8 * const * data);
void set_sprite_native_banked_data(UINT8 bank, UINT8 id, UINT8 len, const UINT8 * data);

void points_update_anim(Sprite* arg_s_points) BANKED;
extern UINT16 add_points(INT16 arg_points) BANKED;

void START(void){
    THIS->lim_x = 200;
    THIS->lim_y = 200;
    struct PointsData* points_data = (struct PointsData*) THIS->custom_data;
    points_data->configured = 0u;
    points_data->points = 0;
    points_data->hp = 60;
    
	old_anim_points = anim_points = ANIM_NONE;
    anim_points_speed = ANIMATION_POINTS_SPEED;
	// animation frame and animation tick is zero
	anim_points_frame = anim_points_tick = 0;
	// load the very first animation frame for the sprite
	set_sprite_native_banked_data(BANK(points_anim), spriteIdxs[SpritePoints], 2, get_banked_pointer(BANK(points_anim), points_anim + anim_points_frame));
}

void points_update_anim(Sprite* arg_s_points) BANKED{
    struct PointsData* points_data = (struct PointsData*) THIS->custom_data;
    switch(points_data->points){
        case 100u:
            anim_points_tick = 0;
            anim_points = ANIM_100;
        break;
        case 50u:
            anim_points_tick = 0;
            anim_points = ANIM_50;
        break;
        case 20u:
            anim_points_tick = 0;
            anim_points = ANIM_20;
        break;
        case 10u:
            anim_points_tick = 0;
            anim_points = ANIM_10;
        break;
    }
}

void UPDATE(void){
     //CROSSZGB
        // save old animation state, animation state to idle (will be overwritten, if keys are pressed)
        old_anim_points = anim_points;
        // if animation state variable changed, then set the new animation for the points sprite
        if (old_anim_points != anim_points) anim_points_frame = 0;
        // tick anumation
        if (++anim_points_tick >= anim_points_speed) {
            set_sprite_native_banked_data(BANK(points_anim), spriteIdxs[SpritePoints], 2, get_banked_pointer(BANK(points_anim), points_anim + (anim_points * ANIMATION_POINTS_FRAMES) + anim_points_frame));
            anim_points_tick = 0;
            if (++anim_points_frame == ANIMATION_POINTS_FRAMES){
                anim_points_frame = 0;
            } 
        }

    //MANAGE POINTS
        struct PointsData* points_data = (struct PointsData*) THIS->custom_data;
        switch(points_data->configured){
            case 0u: return; break;
            case 1u:
                points_data->configured = 2u;
                points_data->hp = 80;
                points_update_anim(THIS);
            break;
            case 2u:
                if(points_data->hp >= 0){
                    points_data->hp--;
                    switch(points_data->hp){
                        case 50:
                        case 40:
                        case 30:
                        case 20:
                        case 10:
                            THIS->y--;
                        break;
                    }
                }else{
                    points_data->configured = 3u;
                }
            break;
            case 3u:
                SpriteManagerRemoveSprite(THIS);
            break;
        }
}

void DESTROY(void){

}