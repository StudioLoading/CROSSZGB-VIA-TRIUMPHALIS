#include "Banks/SetAutoBank.h"

#include "SpriteManager.h"
#include "Keys.h"
#include "ZGBMain.h"

#include "custom_datas.h"

#include "weapon_anim.h"

// Animation state enum
typedef enum {
	ANIM_GLADIO_DOWN = 0,
	ANIM_GLADIO_UP,
	N_ANIMS
} animation_weapon;

// define the animation speed conatsnt
#define ANIMATION_WEAPON_SPEED      6
#define ANIMATION_WEAPON_FRAMES     4
static animation_weapon old_anim_weapon, anim_weapon; 
static UINT8 anim_weapon_frame, anim_weapon_tick;
static UINT8 anim_weapon_speed;

const UINT8 * get_banked_pointer(UINT8 bank, const UINT8 * const * data);
void set_sprite_native_banked_data(UINT8 bank, UINT8 id, UINT8 len, const UINT8 * data);

void weapon_update_anim(Sprite* arg_s_weapon) BANKED;

extern INT8 vx;
extern void item_common_start(Sprite* s_item_arg) BANKED;
extern void item_common_update(Sprite* s_item_arg) BANKED;
extern void item_common_spritescollision(Sprite* s_item_arg) BANKED;

void START(void) {
	// initialize the animation state
	old_anim_weapon = anim_weapon = ANIM_GLADIO_DOWN;
    anim_weapon_speed = ANIMATION_WEAPON_SPEED;
	// animation frame and animation tick is zero
	anim_weapon_frame = anim_weapon_tick = 0;
	// load the very first animation frame for the sprite
	set_sprite_native_banked_data(BANK(weapon_anim), spriteIdxs[SpriteWeapon], 5, get_banked_pointer(BANK(weapon_anim), weapon_anim + anim_weapon_frame));
    
    item_common_start(THIS);
}

void weapon_update_anim(Sprite* arg_s_weapon) BANKED{
	struct ItemData* item_data = (struct ItemData*) arg_s_weapon->custom_data;
    switch(item_data->itemtype){
        case GLADIO:
            anim_weapon_tick = 0;
            anim_weapon = ANIM_GLADIO_UP;
            if(vx < 0){
                anim_weapon = ANIM_GLADIO_DOWN;
            }
        break;
    }
}

void UPDATE(void) {
    //CROSSZGB
        // save old animation state, animation state to idle (will be overwritten, if keys are pressed)
        old_anim_weapon = anim_weapon;
        // if animation state variable changed, then set the new animation for the weapon sprite
        if (old_anim_weapon != anim_weapon) anim_weapon_frame = 0;
        // tick anumation
        if (++anim_weapon_tick >= anim_weapon_speed) {
            set_sprite_native_banked_data(BANK(weapon_anim), spriteIdxs[SpriteWeapon], 5, get_banked_pointer(BANK(weapon_anim), weapon_anim + (anim_weapon << 3) + anim_weapon_frame));
            anim_weapon_tick = 0;
            if (++anim_weapon_frame == ANIMATION_WEAPON_FRAMES) anim_weapon_frame = 0;
        }

    item_common_update(THIS);
    item_common_spritescollision(THIS);
}

void DESTROY(void) {

}