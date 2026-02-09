#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "ZGBMain.h"

#include "CircleMath.h"
#include "custom_datas.h"
#include "horse_anim.h"


#define STAMINA_MAX 960
#define EUPHORIA_MIN 400
#define EUPHORIA_MAX 660
#define ONFIRE_COUNTDOWN_MAX 160
#define WHIP_POWER 3
#define GOLDEN_WHIP_POWER 5
#define HUD_TURN_COOLDOWN_MAX 20
#define COUNTER_HIT_MAX 80
#define FORCE_UPDOWN_MAX 60

// Animation state enum
typedef enum {
	ANIM_RIGHT_WALK,
    ANIM_RIGHT_HIT,
    ANIM_RIGHT_TROT_IDLE,
    ANIM_RIGHT_HIT_IDLE,
    ANIM_RIGHT_TROT,
	ANIM_RIGHT_RUN,
    ANIM_LEFT_IDLE,
    ANIM_LEFT_HIT_IDLE,
	N_ANIMS
} anim_horse_e;
// define the animation speed conatsnt
#define ANIM_HORSE_SPEED      8
#define ANIM_HORSE_FRAMES     8

// define the previous and the new animation state variables
static anim_horse_e old_horse_anim, anim; 
static UINT8 anim_frame, anim_tick;
static UINT8 anim_horse_speed;

// banked data loading helper function declarations
const UINT8 * get_banked_pointer(UINT8 bank, const UINT8 * const * data);
void set_sprite_native_banked_data(UINT8 bank, UINT8 id, UINT8 len, const UINT8 * data);
/*
const UINT8 a_horse_h[] = {6, 8,1,3,4,6,7};
const UINT8 a_horse_hit[] = {13, 0,8,0,1,0,3,0,4,0,6,0,7,0};
const UINT8 a_horse_h_idle[] = {2, 6,7};
const UINT8 a_horse_h_idle_hit[] = {4, 0,2,0,5};
*/

INT8 velocity_counter = 2; //praticamente il frm_skip_max
INT8 velocity = 1;
INT8 vy = 0;
INT8 vx = 0;
INT8 frm_skip = 0;
INT8 whip_power_over_stamina = WHIP_POWER;
INT8 current_whip_power = WHIP_POWER;
INT8 whip_counter = 0;
INT8 whip_counter_max = 36;
UINT16 euphoria_min = EUPHORIA_MIN;
UINT16 euphoria_max = EUPHORIA_MAX;
INT16 stamina_current = 0;
INT8 stamina_verso = 0;
INT8 no_whip_counter = 0;
INT8 no_whip_over_stamina = -8;
INT8 no_whip_counter_max = 10;
UINT8 turn = 0;
INT8 sin = 0;
INT8 cos = 0;
INT8 turn_samepressure_counter = 0;
UINT8 past_coll_tile = 0u;
TURNING_VERSE turn_verse = NONE;
UINT8 orme_spawned = 0u;
INT16 onfire_countdown = -1;//-1 normale, 0 è onfire su acqua, >0 onfire
Sprite* s_flame = 0;
INT8 onwater_countdown = -1;
INT8 flag_hit = 0;
INT8 counter_hit = 0;
INT8 flag_bouncing = 0;
INT8 counter_bouncing = 80;
UINT8 horse_netcatching = 0u;
UINT8 force_updown_counter = 0u;


void horse_hit(INT8 arg_damage) BANKED;
void change_stamina_current(INT16 start, INT16 increase) BANKED;


extern UINT8 J_WHIP;
extern UINT8 J_ATK;
extern INT8 hud_turn_cooldown;
extern ITEM_TYPE weapon_atk;
extern ITEM_TYPE weapon_def;
extern UINT8 track_ended;
extern MISSION_STEP current_step;
extern struct CONFIGURATION configuration;
extern INT8 flag_die;
extern INT16 pharanonet_caught_timer;

extern void update_hp(INT8 variation) BANKED;
extern void use_weapon(INT8 is_defence) BANKED;
extern void pickup(Sprite* s_arg_item) BANKED;
extern INT8 is_track_ended(void) BANKED;


/* velocity_counter in realtà è la velocità assoluta */

void START(void){
	// initialize the animation state
	old_horse_anim = anim = ANIM_RIGHT_WALK;
	anim_horse_speed = ANIM_HORSE_SPEED;
	// animation frame and animation tick is zero
	anim_frame = anim_tick = 0;
	// load the very first animation frame for the sprite
	set_sprite_native_banked_data(BANK(horse_anim), spriteIdxs[SpriteHorse], 8, get_banked_pointer(BANK(horse_anim), horse_anim + anim_frame));

    //SetSpriteAnim(THIS, a_horse_h, 16u);
    frm_skip = velocity_counter;
    THIS->lim_x = 1000;
    THIS->lim_y = 500;
    onfire_countdown = -1;
    turn_samepressure_counter = 0;
    flag_hit = 0;
    vx = 0;
    vy = 0;
    force_updown_counter = 0u;
    counter_hit = COUNTER_HIT_MAX;
    if(configuration.whip == GOLDEN){
        current_whip_power = GOLDEN_WHIP_POWER;
        whip_power_over_stamina = current_whip_power;
    }
    /*if(_cpu != CGB_TYPE){
        SPRITE_SET_PALETTE(THIS,1);
    }*/
}

void UPDATE(void){
    //CROSSZGB
        // save old animation state, animation state to idle (will be overwritten, if keys are pressed)
        old_horse_anim = anim;
        // if animation state variable changed, then set the new animation for the horse sprite
        if (old_horse_anim != anim) anim_frame = 0;
        // tick anumation
        if (++anim_tick >= anim_horse_speed) {
            set_sprite_native_banked_data(BANK(horse_anim), spriteIdxs[SpriteHorse], 8, get_banked_pointer(BANK(horse_anim), horse_anim + (anim << 3) + anim_frame));
            anim_tick = 0;
            if (++anim_frame == ANIM_HORSE_FRAMES) anim_frame = 0;
        }

    //ATTACK
        if(KEY_TICKED(J_ATK)){
            if(weapon_atk != NONE){
                use_weapon(0);
            }
        }
    //NET CATCHING
        if(pharanonet_caught_timer > 0){
            horse_netcatching++;
            if((horse_netcatching & 0x07) == 0){
                THIS->x++;
            }else if((horse_netcatching & 0x03) == 0){
                THIS->x--;
            }
        }
    //BOUNCING & COUNTER
        if(counter_bouncing > 0){
            counter_bouncing--;
            if(counter_bouncing <= 0){
                counter_bouncing = 0;
                flag_bouncing = 0;
            }
        }
    //HIT COUNTER & FLAG
        if(flag_hit == 1){
            counter_hit--;
            if(counter_hit <= 0){
                flag_hit = 0;
                counter_hit = COUNTER_HIT_MAX;
            }
        }
    //IF TRACK ENDED, GO ON UNTILL THE END OF THE SCREEN
        if(track_ended == 1u){
            THIS->x += vx;
            return;
        }
    //ON WATER TO ZERO
        if(onwater_countdown > 0){
            onwater_countdown--;
        }
    //WHIP
        if(whip_counter <= 0){
            if(KEY_TICKED(J_WHIP)){
                stamina_verso = 1;
                whip_counter = whip_counter_max;
                no_whip_counter = no_whip_counter_max;
            }else{
                stamina_verso = -1;
            }
        }else{
            whip_counter--;
        }
        if(whip_counter > 0){
            change_stamina_current(stamina_current, (whip_power_over_stamina * stamina_verso));
        }else{
            if(no_whip_counter > 0){
                no_whip_counter--;
                if(no_whip_counter == 0){
                    change_stamina_current(stamina_current, no_whip_over_stamina);no_whip_counter = no_whip_counter_max;
                }
            }
        }    
    //VELOCITY COUNTER FRAMESKIP
        INT16 delta_stamina_euphoria = 0;
        if(stamina_current > euphoria_max){
            delta_stamina_euphoria = stamina_current - euphoria_max;
        }else if(stamina_current < euphoria_min){
            delta_stamina_euphoria = euphoria_min - stamina_current;
        }
        if(delta_stamina_euphoria > 32){
            velocity_counter = delta_stamina_euphoria >> 4;
            if(delta_stamina_euphoria > (euphoria_max - 10) && delta_stamina_euphoria <= euphoria_max){
                velocity_counter = 0;//cioè vai a cannone
            }
        }else{
            velocity_counter = 2;
        }        
    //FIRE EFFECTS
        if(onfire_countdown > 0){//sta impazzendo
            onfire_countdown--;
            switch(onfire_countdown){
                case 50: case 100: case 150:
                case 200: case 250: case 300: case 350:
                    update_hp(-1);
                break;
            }
            velocity_counter = 1;//cioè vai a cannone
            change_stamina_current(0, EUPHORIA_MAX+1);//fissa la stamina a euphoria_max
            s_flame->x = THIS->x +2;
            s_flame->y = THIS->y-7;
            if(weapon_def == CAPE){
                use_weapon(1);
                SpriteManagerRemoveSprite(s_flame);
                onfire_countdown = 2;
            }            
        }else{
            if(euphoria_min != EUPHORIA_MIN){
                euphoria_min = EUPHORIA_MIN;
            }
            if(euphoria_max != EUPHORIA_MAX){
                euphoria_max = EUPHORIA_MAX;
            }
            if(whip_power_over_stamina != current_whip_power){//se entro qui, sto risistemando la whip ed è appena terminato l'onfire.
                whip_power_over_stamina = current_whip_power;
                change_stamina_current(0, EUPHORIA_MIN - (EUPHORIA_MIN >> 3));
                SpriteManagerRemoveSprite(s_flame);
            }
        }

    //TURN SINGLE PRESSURE COUNTER
        if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT)){
            turn_samepressure_counter = 0;
            hud_turn_cooldown = HUD_TURN_COOLDOWN_MAX;
        }
    //TURN
        if(KEY_PRESSED(J_RIGHT)){
            turn_verse = CLOCK;
            turn--;
            if(configuration.reins == GOLDEN){
                turn -= 2;
            }
            turn_samepressure_counter++;
            if(turn_samepressure_counter >= 40 && ((sin > 30 && sin < 78) || (sin < -30 && sin > -78))){
                turn_samepressure_counter = 40;
                turn-=2;
            }
        }else if(KEY_PRESSED(J_LEFT)){
            turn_verse = COUNTERCLOCK;
            turn += 2;
            if(configuration.reins == GOLDEN){
                turn += 2;
            }
            turn_samepressure_counter++;
            if(turn_samepressure_counter > 40){
                turn_samepressure_counter = 40;
                turn += 1;
            }
        }else{
            turn_verse = NONE; 
        }
        if(force_updown_counter > 0){
            force_updown_counter++;
            if(force_updown_counter >= FORCE_UPDOWN_MAX){
                force_updown_counter = 0;
            } 
        }
        if(KEY_TICKED(J_UP)){
            if(force_updown_counter == 0){
                force_updown_counter = 1;
            }else if(force_updown_counter < FORCE_UPDOWN_MAX){
                turn = 64;
                force_updown_counter = 0u;
                turn_samepressure_counter = 0;
            }
        }
        if(KEY_TICKED(J_DOWN)){
            if(force_updown_counter == 0){
                force_updown_counter = 1;
            }else if(force_updown_counter < FORCE_UPDOWN_MAX){
                turn = 192;
                force_updown_counter = 0;
                turn_samepressure_counter = 0;
            }
        }
        sin = sine_wave[turn];
        UINT8 cos_idx = turn+64;
        cos = sine_wave[cos_idx];
    //SPRITE ANIMATION SPEED animation speed
        if(stamina_current < 120){
            anim = ANIM_RIGHT_TROT;
            anim_horse_speed = 24u;
        }else if(stamina_current < 240){
            anim = ANIM_RIGHT_TROT;
            anim_horse_speed = (STAMINA_MAX - stamina_current) >> 6;
        }else{
            anim_horse_speed = (STAMINA_MAX - stamina_current) >> 6;
            anim = ANIM_RIGHT_RUN;  
            if(stamina_current >= (euphoria_min - 100)){
                anim = ANIM_RIGHT_WALK;
            }
        }
    
        if(flag_hit == 1){
            anim = ANIM_RIGHT_HIT;
            anim_horse_speed = 4u;
        }
    //ACTUAL MOVEMENT & COLLISION & OVER
        if(frm_skip > 0){frm_skip--;}
        if(frm_skip == 0){
            frm_skip = velocity_counter;
            //VX col coseno
                //cos max = tutto su asse x
                //90 gradi sono 64 posizioni dell'array
                //90 gradi sono 128 valori nell'array
                //33 gradi sono 21 posizioni dell'array
                //33 gradi sono 42 valori nell'array
                if(cos >= 0 && cos <= 30){//tratto come se stesse andando verticale
                    vx = 0;
                }else if(cos > 30 && cos < 65){// tratto come se stesse andando a 66 gradi
                    vx = 1;
                }else if(cos > 65 && cos < 110){// tratto come se stesse andando a 33 gradi
                    vx = 2;
                }else if(cos > 110){ // tratto come se stesse andando orizzontale destra
                    vx = 3;
                }else if(cos < 0 && cos > -30){//tratto come se stesse andando verticale
                    vx = 0;
                }else if(cos < -30 && cos > -65){// tratto come se stesse andando a 66 gradi sinistra
                    vx = -1;
                }else if(cos < -65 && cos > -110){// tratto come se stesse andando a 33 gradi sinistra
                    vx = -2;
                }else if(cos < 0 && cos < -110){ // tratto come se stesse andando orizzontale sinistra
                    vx = -3;
                }
            //VY col seno
                if(sin >= 0 && sin <= 30){//tratto come se stesse andando orizzontale
                    vy = 0;
                }else if(sin > 30 && sin < 54){// tratto come se stesse andando a 33 gradi
                    vy = -1;
                }else if(sin > 54 && sin < 78){// tratto come se stesse andando a 66 gradi
                    vy = -1;
                }else if(sin > 78){ // tratto come se stesse andando verticale alto
                    vy = -2;
                }else if(sin < 0 && sin > -30){//tratto come se stesse andando orizzontale
                    vy = 0;
                }else if(sin < -30 && sin > -54){// tratto come se stesse andando a -33 gradi 
                    vy = 1;
                }else if(sin < -54 && sin > -78){// tratto come se stesse andando a -66 gradi 
                    vy = 1;
                }else if(sin < -78){ // tratto come se stesse andando verticale basso
                    vy = 2;
                }
            //MOVE
                if(flag_die){ return; }
                if(stamina_current < 20){
                    vx = 0;
                    vy = 0;
                }
                UINT8 horse_coll = TranslateSprite(THIS, vx << delta_time, vy << delta_time);
            //COLLISIONI TILE
                if(horse_coll){//collido con tile ambiente di collisione
                    if(horse_coll == 118 || horse_coll == 119 || horse_coll == 120 || horse_coll == 121){//FINE TRACCIA!!
                        track_ended = is_track_ended();
                        if(current_state == StateTutorialGame){
                            track_ended = 1u;
                        }
                    }else{
                        past_coll_tile = horse_coll;
                        INT8 vxbounce = vx * (-1); 
                        INT8 vybounce = vy * (-1);
                        if(vx < 2){
                            TranslateSprite(THIS, vxbounce << delta_time, 0);
                        }else{
                            TranslateSprite(THIS, 0, vybounce << delta_time);
                        }
                    }
            //OVER TILE
                }else{//non collido, cerco cosa sto calpestando
                    UINT8 tile_over = GetScrollTile((THIS->x + 4) >> 3, (THIS->y+4) >> 3);
                    if(vx < 0){
                        tile_over = GetScrollTile((THIS->x + 4) >> 3, (THIS->y+4) >> 3);
                    }
                    switch(tile_over){
                        case 3: //ghiaia: incrementa il frameskip
                            if(onfire_countdown == -1 && configuration.wheel == NORMAL){//non sta impazzendo
                                frm_skip+=4;
                            }
                            if(THIS->anim_frame == 1 || THIS->anim_frame == 5){
                                if(orme_spawned == 0){
                                    UINT16 orma_posy = THIS->y-9;
                                    orme_spawned = 1;
                                    if(vx > 0){
                                        SpriteManagerAdd(SpriteStep, THIS->x - 2, orma_posy);
                                        SpriteManagerAdd(SpriteStep, THIS->x + 8, orma_posy);
                                    }else if(vx < 0){
                                        SpriteManagerAdd(SpriteStep, THIS->x, orma_posy);
                                        SpriteManagerAdd(SpriteStep, THIS->x + 8, orma_posy);
                                    }
                                }
                            }else{orme_spawned = 0;}
                        break;
                        case 8: case 9: case 10:
                        case 11: case 12: case 13: //acqua: simula pantano limitando stamina
                            onwater_countdown = 80;
                            if(onfire_countdown > 0){//sono onfire!
                                onfire_countdown = 0;
                            }else if(onfire_countdown == -1){
                                if(stamina_current > (euphoria_min >> 1)){
                                    if(configuration.wheel == NORMAL){
                                        change_stamina_current(0, euphoria_min >> 1);
                                    }else{
                                        change_stamina_current(0, euphoria_min);
                                    }
                                }
                            }
                            if(THIS->anim_frame == 0 || THIS->anim_frame == 2 || THIS->anim_frame == 4){
                                if(orme_spawned == 0){
                                    orme_spawned = 1;
                                    UINT16 orma0_posx = THIS->x - 2;
                                    UINT16 orma_posy = THIS->y-9;
                                    UINT16 orma1_posx = THIS->x + 8;
                                    if(vx <= 0){
                                        orma0_posx = THIS->x;
                                    }
                                    Sprite* s_orma0 = SpriteManagerAdd(SpriteStep, orma0_posx, orma_posy);
                                    struct ItemData* orma0_data = (struct ItemData*)s_orma0->custom_data;
                                    orma0_data->configured = 1;
                                    Sprite* s_orma1 = SpriteManagerAdd(SpriteStep, orma1_posx, orma_posy);
                                    struct ItemData* orma1_data = (struct ItemData*) s_orma1->custom_data;
                                    orma1_data->configured = 1;
                                }
                            }else{orme_spawned = 0;}
                        break;
                        case 122: case 123: case 124: case 125: // SKULL teschio!
                            if(stamina_current > 100){
                                stamina_current-=30;
                            }
                            if(counter_hit == COUNTER_HIT_MAX){
                                if(weapon_def == ELMET){
                                    flag_hit = 1;
                                    use_weapon(1);
                                }else{
                                    horse_hit(-4);
                                }
                            }
                        break;
                        default:
                            if(onfire_countdown == 0){//se ho spento il fuoco con l'acqua rimettimi il countdown negativo
                                onfire_countdown = -1;
                            }
                        break;
                    }
                }
            
        }
    //SPRITE MIRROR
        if(vx > 0){
            THIS->mirror = NO_MIRROR;
        }else if (vx < 0){
            THIS->mirror = V_MIRROR;
            /*switch(anim){
                case ANIM_RIGHT_HIT: anim = ANIM_LEFT_HIT; break;
                case ANIM_RIGHT_HIT_IDLE: anim = ANIM_LEFT_HIT_IDLE; break;
                case ANIM_RIGHT_IDLE: anim = ANIM_LEFT_IDLE; break;
                case ANIM_RIGHT_WALK: anim = ANIM_LEFT_WALK; break;
            }*/
        }
    
    //SPRITE COLLISION
        UINT8 scroll_o_tile;
        Sprite* iospr;
        SPRITEMANAGER_ITERATE(scroll_o_tile, iospr) {
            if(CheckCollision(THIS, iospr)) {
                switch(iospr->type){
                    case SpriteFlame:
                        struct FlameData* fire_data = (struct FlameData*) iospr->custom_data;
                        if(fire_data->dropped == -1){
                            if(onfire_countdown == -1){//if not already, go onfire!
                                if(weapon_def == CAPE){
                                    use_weapon(1);
                                    SpriteManagerRemoveSprite(iospr);
                                }else{
                                    onfire_countdown = ONFIRE_COUNTDOWN_MAX;
                                    change_stamina_current(0, euphoria_min >> 2);
                                    whip_power_over_stamina = 1;
                                    s_flame = iospr;
                                }
                            }
                        }
                    break;                    
                    case SpriteItemgladio:
                    case SpriteItemlance:
                    case SpriteItemfire:
                    case SpriteItemelmet:
                    case SpriteItemshield:
                    case SpriteItemcape:
                    case SpriteItemheart:
                    case SpriteItemglass:
                    case SpriteConfigwhip:
                    case SpriteConfigreins:
                    case SpriteConfigwheel:
                    case SpriteConfigelm:
                    case SpriteItempapirus:{
                        struct ItemData* item_data = (struct ItemData*) iospr->custom_data;
                        if(item_data->configured == 2){
                            pickup(iospr);
                        }
                    }break;
                    case SpriteRomansenator:
                    case SpriteStatue:
                        if(current_step != SENATOR_COLLIDED){
                            current_step = SENATOR_COLLIDED;
                        }
                    break;
                    case SpriteSavage:
                    case SpriteBarbarian:
                        if(weapon_def == SHIELD){
                            use_weapon(1);
                            counter_hit = COUNTER_HIT_MAX;
                        }else if(flag_hit == 0 && counter_hit == COUNTER_HIT_MAX){
                            struct SoldierData* soldier_data = (struct SoldierData*)iospr->custom_data;
                            if(soldier_data->configured < 4 && flag_hit == 0 && counter_hit == COUNTER_HIT_MAX){
                                horse_hit(-5);
                            }
                        }
                    break;
                    case SpriteGreekphilosopher:
                    case SpriteRomansoldier:
                    case SpriteGreeksoldier:
                        {
                            struct SoldierData* soldier_data = (struct SoldierData*)iospr->custom_data;
                            if(soldier_data->configured != 4 && soldier_data->configured != 5 && flag_hit == 0 && counter_hit == COUNTER_HIT_MAX){
                                horse_hit(-16);
                                SpriteManagerAdd(SpriteExclamation, iospr->x + 4, iospr->y - 16u);
                                soldier_data->vx = 0;
                                soldier_data->vy = 0;
                                iospr->anim_speed = 32u;
                            }
                        }
                    break;
                    case SpriteBarbarianshield:{
                        struct SoldierData* barbarianshield_data = (struct SoldierData*)iospr->custom_data;
                        if(barbarianshield_data->configured == 1 || barbarianshield_data->configured == 2){
                            if(stamina_current > euphoria_min && stamina_current <= euphoria_max){//sfonda
                                if(THIS->x < iospr->x){//gli sto a sinistra
                                    switch(barbarianshield_data->configured){
                                        case 1: barbarianshield_data->configured = 4; break;
                                        case 2: barbarianshield_data->configured = 6; break;
                                    }
                                }else{//gli sto a destra
                                    switch(barbarianshield_data->configured){
                                        case 1: barbarianshield_data->configured = 3; break;
                                        case 2: barbarianshield_data->configured = 5; break;
                                    }
                                }
                                stamina_current -= 140;
                            }else if(flag_bouncing == 0){//rimbalza
                                if(stamina_current < 200){ stamina_current = 200;}
                                else{stamina_current -= 200;}
                                turn = (turn + 128u) % 254;
                                flag_bouncing = 1;
                                counter_bouncing = 80;
                                //turn + 128 modulo 256, IL MODULO lo fa da solo perché é unsigned
                            }
                        }
                    }break;
                    case SpriteRollingstone:{
                        if(weapon_def == ELMET){
                            use_weapon(1);
                            SpriteManagerRemoveSprite(iospr);
                        }else if(flag_hit == 0 && counter_hit == COUNTER_HIT_MAX){
                            horse_hit(-4);
                        }
                    }break;
                }
            }
        }
}

void change_stamina_current(INT16 start, INT16 increase) BANKED{
    INT16 new_stamina_current = start + increase;
    if(stamina_current < 0){
        stamina_current = 0;
    }else if(stamina_current > STAMINA_MAX){
        stamina_current = STAMINA_MAX;
    }else{
        stamina_current = new_stamina_current;
    }
}

void horse_hit(INT8 arg_damage) BANKED{
    if(flag_hit == 0){
        update_hp(arg_damage);
        flag_hit = 1;
        counter_hit = COUNTER_HIT_MAX;
    }
}

void DESTROY(void){
}