#include "Banks/SetAutoBank.h"

#include "SpriteManager.h"
#include "Keys.h"
#include "ZGBMain.h"

#include "CircleMath.h"
#include "custom_datas.h"

#include "weapon_anim.h"

// Animation state enum
typedef enum {
    ANIM_GLADIO_HIDDEN = 0,
	ANIM_GLADIO_DOWN,
	ANIM_GLADIO_UP,
	ANIM_LANCE,
	ANIM_FIRE,
	N_ANIMS
} animation_weapon;

// define the animation speed constant
#define ANIMATION_WEAPON_SPEED      6
#define ANIMATION_WEAPON_FRAMES     5
static animation_weapon old_anim_weapon, anim_weapon; 
static UINT8 anim_weapon_frame, anim_weapon_tick;
static UINT8 anim_weapon_speed;

const UINT8 * get_banked_pointer(UINT8 bank, const UINT8 * const * data);
void set_sprite_native_banked_data(UINT8 bank, UINT8 id, UINT8 len, const UINT8 * data);

void weapon_update_anim(Sprite* arg_s_weapon) BANKED;

extern Sprite* s_horse;
extern UINT8 flag_using_atk;
extern void item_common_start(Sprite* s_item_arg) BANKED;
extern void item_common_update(Sprite* s_item_arg) BANKED;
extern void item_common_spritescollision(Sprite* s_item_arg) BANKED;

typedef struct {
    UINT8 active;
    INT8 dx;          // Velocità orizzontale
    INT8 dy;          // Velocità verticale (per la parabola)
    UINT8 circle_index;
    UINT16 circle_center_x;
    UINT16 circle_center_y;
    INT8 circle_center_vx;
    INT8 circle_center_vy;
    UINT8 clockwise;
    INT8 v_round;
} Projectile;
Projectile boccetta = {0};

extern INT8 vx;
extern INT8 vy;

void update_boccetta(Sprite* arg_s_boccetta) BANKED;
void lancia_boccetta(INT8 arg_horse_vx, INT8 arg_horse_vy) BANKED;

void START(void) {
	// initialize the animation state
	old_anim_weapon = anim_weapon = ANIM_GLADIO_HIDDEN;
    anim_weapon_speed = ANIMATION_WEAPON_SPEED;
	// animation frame and animation tick is zero
	anim_weapon_frame = anim_weapon_tick = 0;
	// load the very first animation frame for the sprite
	set_sprite_native_banked_data(BANK(weapon_anim), spriteIdxs[SpriteWeapon], 6, get_banked_pointer(BANK(weapon_anim), weapon_anim + anim_weapon_frame));
    
    item_common_start(THIS);
}

void weapon_update_anim(Sprite* arg_s_weapon) BANKED{
	struct ItemData* item_data = (struct ItemData*) arg_s_weapon->custom_data;
    switch(item_data->itemtype){
        case GLADIO:
            anim_weapon_tick = 0;
            anim_weapon = ANIM_GLADIO_DOWN;
            if(s_horse->mirror == V_MIRROR){
                anim_weapon = ANIM_GLADIO_UP;
            }
        break;
        case LANCE:
            anim_weapon_tick = 0;
            anim_weapon = ANIM_LANCE;
            arg_s_weapon->mirror = s_horse->mirror;
        break;
        case FIRE:
            anim_weapon_tick = 0;
            anim_weapon = ANIM_FIRE;
            arg_s_weapon->mirror = s_horse->mirror;
        break;
    }
}

void UPDATE(void) {
    UINT8 one_cycle_anim_completed = 0u;
    //CROSSZGB
        // save old animation state, animation state to idle (will be overwritten, if keys are pressed)
        old_anim_weapon = anim_weapon;
        // if animation state variable changed, then set the new animation for the weapon sprite
        if (old_anim_weapon != anim_weapon) anim_weapon_frame = 0;
        // tick anumation
        if (++anim_weapon_tick >= anim_weapon_speed) {
            set_sprite_native_banked_data(BANK(weapon_anim), spriteIdxs[SpriteWeapon], 6, get_banked_pointer(BANK(weapon_anim), weapon_anim + (anim_weapon * ANIMATION_WEAPON_FRAMES) + anim_weapon_frame));
            anim_weapon_tick = 0;
            if (++anim_weapon_frame == ANIMATION_WEAPON_FRAMES){
                one_cycle_anim_completed = 1u;
                anim_weapon_frame = 0;
            } 
        }

    item_common_update(THIS);
    item_common_spritescollision(THIS);
    
    struct ItemData* item_data = (struct ItemData*) THIS->custom_data;
    switch(item_data->configured){
        case 4: //in use
            switch(item_data->itemtype){
                case GLADIO:
                    if(one_cycle_anim_completed){
                        flag_using_atk = 0u;
                        SpriteManagerRemoveSprite(THIS);
                    }
                break;
                case FIRE:
                    update_boccetta(THIS);
                break;
            }
        break;
    }
}

void lancia_boccetta(INT8 arg_horse_vx, INT8 arg_horse_vy) BANKED {
    boccetta.active = 1;
    // Spinta in avanti: velocità del cavallo + una spinta fissa per coprire i 40px
    INT8 initial_dx = 1;
    if(s_horse->mirror == V_MIRROR){
        initial_dx = -1;
    }
    boccetta.dx = arg_horse_vx + initial_dx;
    
    // Forza del lancio verso l'alto (valore negativo sale in alto se Y=0 è il top)
    boccetta.dy = -1 + arg_horse_vy; 
    boccetta.circle_index = 155u;
    boccetta.circle_center_y = s_horse->y;
    boccetta.clockwise = s_horse->mirror == NO_MIRROR;
    if(boccetta.clockwise){
        boccetta.circle_center_x = s_horse->x + 32u;
    }else{
        boccetta.circle_center_x = s_horse->x - 28u;
    }
    boccetta.v_round = 4;
    boccetta.circle_center_vx = arg_horse_vx;
    boccetta.circle_center_vy = arg_horse_vy;
}

void update_boccetta(Sprite* arg_s_boccetta) BANKED{
    UINT8 cos_position = boccetta.circle_index + 64u;
    UINT16 new_posx = boccetta.circle_center_x + ((sine_wave[cos_position]) / 4);
    UINT16 new_posy = boccetta.circle_center_y + ((sine_wave[boccetta.circle_index]) / 4);
    if(boccetta.circle_center_vx){
        new_posx += boccetta.circle_center_vx;
    }
    if(boccetta.circle_center_vy){
        new_posy += boccetta.circle_center_vy;
    }
    INT16 boccetta_vx = (INT16)new_posx - (INT16)arg_s_boccetta->x;
    INT16 boccetta_vy = (INT16)new_posy - (INT16)arg_s_boccetta->y;
    UINT8 boccetta_coll_tile = TranslateSprite(arg_s_boccetta, boccetta_vx << delta_time, boccetta_vy << delta_time);
    if(boccetta_coll_tile){
        THIS->x += boccetta_vx;
        THIS->y += boccetta_vy; 
    }
    if(boccetta.clockwise){//CLOCKWISE
        boccetta.circle_index = boccetta.circle_index + boccetta.v_round;
        if(boccetta.circle_index > 250u){
            boccetta.active = 0;
            SpriteManagerRemoveSprite(arg_s_boccetta);
        }
    }else{//COUNTERCLOCK
        boccetta.circle_index = boccetta.circle_index - boccetta.v_round;
        if(boccetta.circle_index < 180u){
            boccetta.active = 0;
            SpriteManagerRemoveSprite(arg_s_boccetta);
        }
    }
}

void DESTROY(void) {
    struct ItemData* item_data = (struct ItemData*) THIS->custom_data;
    switch(item_data->itemtype){
        case FIRE:
            UINT16 puff_spawn_x = THIS->x;
            if(!boccetta.clockwise){puff_spawn_x = THIS->x + 4u;}
            SpriteManagerAdd(SpritePuff, puff_spawn_x, THIS->y + 2u);
            Sprite* s_fire = SpriteManagerAdd(SpriteFlame, puff_spawn_x, THIS->y);
            struct FlameData* fire_data = (struct FlameData*) s_fire->custom_data;
            fire_data->hp = 100;
            fire_data->dropped = 1;
        break;
    }
}