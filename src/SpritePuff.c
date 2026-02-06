#include "Banks/SetAutoBank.h"

#include "SpriteManager.h"
#include "Keys.h"
#include "ZGBMain.h"

#include "custom_datas.h"

#include "puff_anim.h"

// Animation state enum
typedef enum {
    ANIM_PUFF = 0,
	N_ANIMS
} animation_puff;

// define the animation speed conatsnt
#define ANIMATION_PUFF_SPEED      3
#define ANIMATION_PUFF_FRAMES     6
static animation_puff old_anim_puff, anim_puff; 
static UINT8 anim_puff_frame, anim_puff_tick;
static UINT8 anim_puff_speed;

const UINT8 * get_banked_pointer(UINT8 bank, const UINT8 * const * data);
void set_sprite_native_banked_data(UINT8 bank, UINT8 id, UINT8 len, const UINT8 * data);

void puff_update_anim(Sprite* arg_s_puff) BANKED;


void START(void) {
	// initialize the animation state
	old_anim_puff = anim_puff = ANIM_PUFF;
    anim_puff_speed = ANIMATION_PUFF_SPEED;
	// animation frame and animation tick is zero
	anim_puff_frame = anim_puff_tick = 0;
	// load the very first animation frame for the sprite
	set_sprite_native_banked_data(BANK(puff_anim), spriteIdxs[SpritePuff], 4, get_banked_pointer(BANK(puff_anim), puff_anim + anim_puff_frame));
    
}


void UPDATE(void) {
    UINT8 one_cycle_anim_completed = 0u;
    //CROSSZGB
        // save old animation state, animation state to idle (will be overwritten, if keys are pressed)
        old_anim_puff = anim_puff;
        // if animation state variable changed, then set the new animation for the puff sprite
        if (old_anim_puff != anim_puff) anim_puff_frame = 0;
        // tick anumation
        if (++anim_puff_tick >= anim_puff_speed) {
            set_sprite_native_banked_data(BANK(puff_anim), spriteIdxs[SpritePuff], 4, get_banked_pointer(BANK(puff_anim), puff_anim + (anim_puff * ANIMATION_PUFF_FRAMES) + anim_puff_frame));
            anim_puff_tick = 0;
            if (++anim_puff_frame == ANIMATION_PUFF_FRAMES){
                one_cycle_anim_completed = 1u;
                anim_puff_frame = 0;
                SpriteManagerRemoveSprite(THIS);
            } 
        }
}

void DESTROY(void) {

}