#include "Banks/SetAutoBank.h"

#include "SpriteManager.h"
#include "Keys.h"
#include "ZGBMain.h"

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
    uint8_t active;
    int16_t x, y;       // Posizione logica
    int8_t dx;          // Velocità orizzontale
    int8_t dy;          // Velocità verticale (per la parabola)
    int8_t altitude;    // L'altezza "visiva" dell'oggetto
    uint8_t frame;      // Timer per la parabola
} Projectile;
Projectile boccetta = {0};

extern INT8 vx;
extern INT8 vy;

void update_boccetta(Projectile *p) BANKED;
void lancia_boccetta(Projectile *p, UINT16 horseX, UINT16 horseY, INT8 horseSpeed) BANKED;

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

    struct ItemData* item_data = (struct ItemData*) THIS->custom_data;
    if(item_data->itemtype == FIRE && item_data->configured == 3){
        lancia_boccetta(&boccetta, s_horse->x, s_horse->y - 16u, vx);//configured verrà messa a 4 qui sotto nella item_common_spritescollision(...)
    }
    item_common_spritescollision(THIS);

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
                    update_boccetta(&boccetta);
                break;
            }
        break;
    }
}

void lancia_boccetta(Projectile *p, UINT16 horseX, UINT16 horseY, INT8 horseSpeed) BANKED {
    p->active = 1;
    p->x = horseX;
    p->y = horseY;
    
    // Spinta in avanti: velocità del cavallo + una spinta fissa per coprire i 40px
    p->dx = horseSpeed + 2;
    if(s_horse->mirror == V_MIRROR){
        p->dx = -p->dx;
    }
    
    // Forza del lancio verso l'alto (valore negativo sale in alto se Y=0 è il top)
    p->dy = -8; 
    p->altitude = 0;
    p->frame = 0;
}

void update_boccetta(Projectile *p) BANKED{
    if (!p->active) return;

    // 1. Movimento orizzontale costante rispetto al lancio
    p->x += p->dx;

    // 2. Simulazione gravità sulla parabola
    p->altitude += p->dy; // Applichiamo la velocità verticale all'altezza
    p->dy += 1;           // La gravità "tira giù" la boccetta (aumenta dy)

    // 3. Controllo impatto al suolo
    // Se l'altitudine torna a 0 (o maggiore), la boccetta tocca terra
    if (p->altitude >= 0) {
        p->altitude = 0;
        p->active = 0;
        /*TODO 
        Sprite* s_fire = SpriteManagerAdd(SpriteFlame,. ..)
        struct FlameData* fire_data = (struct FlameData*) s_fire->custom_data;
        fire_data->hp = 100;
        fire_data->dropped = 1;
        */
    }
}

void DESTROY(void) {

}