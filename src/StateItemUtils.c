#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"

extern Sprite* s_horse;
extern Sprite* s_pharaosubiga;
extern INT8 vx;
extern INT8 vy;
extern Sprite* s_spawning_weapon;

void item_common_start(Sprite* s_item_arg) BANKED;
void item_common_update(Sprite* s_item_arg) BANKED;
void item_common_spritescollision(Sprite* s_item_arg) BANKED;
void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;

extern void hit_fantoccio(Sprite* s_fantoccio_arg) BANKED;
extern void consume_weapon_atk() BANKED;
extern void consume_weapon_def() BANKED;
extern void horse_hit(INT8 arg_damage) BANKED;

extern void item_gladio_anim(Sprite* s_item_arg) BANKED;
extern void item_gladio_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_lance_anim(Sprite* s_item_arg) BANKED;
extern void item_lance_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_e_lance_anim(Sprite* s_item_arg) BANKED;
extern void item_e_lance_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_fire_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_fire_anim(Sprite* s_item_arg) BANKED;
extern void item_elmet_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_elmet_anim(Sprite* s_item_arg) BANKED;
extern void item_shield_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_shield_anim(Sprite* s_item_arg) BANKED;
extern void item_cape_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_cape_anim(Sprite* s_item_arg) BANKED;
extern void item_heart_anim(Sprite* s_item_arg) BANKED;
extern void item_glass_anim(Sprite* s_item_arg) BANKED;
extern void item_configwhip_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_configwhip_anim(Sprite* s_item_arg) BANKED;
extern void item_configwheel_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_configwheel_anim(Sprite* s_item_arg) BANKED;
extern void item_configreins_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_configreins_anim(Sprite* s_item_arg) BANKED;
extern void item_configelm_anim_blink(Sprite* s_item_arg) BANKED;
extern void item_configelm_anim(Sprite* s_item_arg) BANKED;
extern void item_papirus_anim(Sprite* s_item_arg) BANKED;
extern void pharaosubiga_change_status(INT8 arg_status, Sprite* arg_s_pharaosubiga) BANKED;

void item_common_start(Sprite* s_item_arg) BANKED{
	struct ItemData* item_data = (struct ItemData*) s_item_arg->custom_data;
    item_data->configured = 0;
    s_item_arg->lim_x = 6000;
    s_item_arg->lim_y = 4000;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void item_common_update(Sprite* s_item_arg) BANKED{
	struct ItemData* item_data = (struct ItemData*) s_item_arg->custom_data;
    switch(item_data->configured){
        case 0: return; break;
        case 1://set animation
            switch(item_data->itemtype){
                case GLADIO:
                    item_gladio_anim(s_item_arg);
                break;
                case LANCE:
                    item_lance_anim(s_item_arg);
                break;
                case FIRE:
                    item_fire_anim(s_item_arg);
                break;
                case ELMET:
                    item_elmet_anim(s_item_arg);
                break;
                case SHIELD:
                    item_shield_anim(s_item_arg);
                break;
                case CAPE:
                    item_cape_anim(s_item_arg);
                break;
                case HP:
                    item_heart_anim(s_item_arg);
                break;
                case TIME:
                    item_glass_anim(s_item_arg);
                break;
                case GOLDEN_WHIP:
                    item_configwhip_anim(s_item_arg);
                break;
                case GOLDEN_WHEEL:
                    item_configwheel_anim(s_item_arg);
                break;
                case GOLDEN_REINS:
                    item_configreins_anim(s_item_arg);
                break;
                case GOLDEN_ELM:
                    item_configelm_anim(s_item_arg);
                break;
                case ENEMY_LANCE:
                    item_e_lance_anim(s_item_arg);
                break;
                case PAPYRUS:
                    item_papirus_anim(THIS);
                break;
            }
            item_data->configured = 2;//means spawned
        break;
        case 3://using the weapon!
            switch(item_data->itemtype){
                case GLADIO:
                    item_gladio_anim(s_item_arg);
                    item_data->hp = 80;
                    if(vx > 0){ item_data->vx = 1;}
                    else if(vx < 0) {item_data->vx = -1;}
                    item_data->vy = 0;
                break;
                case LANCE:
                    item_lance_anim(s_item_arg);
                    item_data->vx = vx;
                    if(vx == 0){
                        if(s_horse->mirror == NO_MIRROR){
                            item_data->vx = 1;
                        }else{
                            item_data->vx = -1;
                        }
                    }
                    item_data->vy = 0;
                break;
                case ELMET:
                    item_elmet_anim_blink(s_item_arg);
                    item_data->hp = 80;
                break;
                case SHIELD:
                    item_shield_anim_blink(s_item_arg);
                    item_data->hp = 80;
                break;
                case CAPE:
                    item_cape_anim_blink(s_item_arg);
                    item_data->hp = 80;
                break;
                case ENEMY_LANCE:
                    item_e_lance_anim(s_item_arg);
                    if(item_data->vy < 0){
                        s_item_arg->mirror = H_MIRROR;
                    }
                break;
            }
            item_data->configured = 4;
        break;
        case 4://weapon in use!
            switch(item_data->itemtype){
                case SHIELD:
                case ELMET:
                case CAPE:
                    s_item_arg->x = s_horse->x;
                    s_item_arg->y = s_horse->y - 24;
                    item_data->hp--;
                    if(item_data->hp <= 0){
                        consume_weapon_def();
                        SpriteManagerRemoveSprite(s_item_arg);
                    }
                break;
                case LANCE:
                case ENEMY_LANCE:{
                    UINT8 lance_tile_coll = TranslateSprite(s_item_arg, item_data->vx << delta_time, item_data->vy << delta_time);
                    if(lance_tile_coll){
                        item_data->configured = 5;
                    }
                }break;
                case GLADIO:{
                    UINT16 attack_x = s_horse->x + 2;
                    UINT16 attack_y = s_horse->y + 8;
                    THIS->mirror = H_MIRROR;
                    if(s_horse->mirror == V_MIRROR){
                        attack_y = s_horse->y - 24;
                        attack_x = s_horse->x;
                        THIS->mirror = V_MIRROR;
                    }
                    s_item_arg->x = attack_x;
                    s_item_arg->y = attack_y;
                }break;
            }
        break;
        case 5://weapon start dieing
            switch(item_data->itemtype){
                case GLADIO:
                    item_gladio_anim_blink(s_item_arg);
                    consume_weapon_atk();
                break;
                case LANCE:
                    item_lance_anim_blink(s_item_arg);
                    consume_weapon_atk();
                break;
                case ENEMY_LANCE:
                    item_e_lance_anim_blink(s_item_arg);
                break;
            }
            item_data->hp = 20;
            item_data->configured = 6;
        break;
        case 6://weapon dieing
            if(item_data->flag_continuous_spawning == 1){
                s_spawning_weapon = 0;
            }
            switch(item_data->itemtype){
                case GLADIO:
                case LANCE:
                case ENEMY_LANCE:
                    item_data->hp--;
                    if(item_data->hp <= 0){
                        SpriteManagerRemoveSprite(s_item_arg);
                    }
                break;
            }
        break;
    }
}

void item_common_spritescollision(Sprite* s_item_arg) BANKED{
    struct ItemData* item_data = (struct ItemData*) s_item_arg->custom_data;
    //SPRITE COLLISION
    UINT8 scroll_i_tile;
    Sprite* iispr;
    SPRITEMANAGER_ITERATE(scroll_i_tile, iispr) {
        if(CheckCollision(s_item_arg, iispr)) {
            switch(iispr->type){
                case SpriteFantoccio:{
                    if(item_data->configured < 5){
                        hit_fantoccio(iispr);
                        s_item_arg->x = iispr->x;
                        item_data->configured = 5;
                    }
                }break;
                case SpritePharaonet:
                    if(item_data->configured > 3){
                        if(item_data->itemtype == GLADIO || item_data->itemtype == LANCE){
                            SpriteManagerRemoveSprite(iispr);
                            item_data->configured = 5;
                        }
                    }
                break;
                case SpritePharaosubiga:{
                    struct PharaoData* pharao_data = (struct PharaoData*) iispr->custom_data;
                    if(item_data->configured == 4 && pharao_data->status == 0){
                        if(item_data->itemtype == GLADIO || item_data->itemtype == LANCE){
                            pharaosubiga_change_status(1, iispr);
                            item_data->configured = 5;
                        }
                    }
                }break;
                /*case SpriteGator:
                    if(item_data->configured > 3){
                        if(item_data->itemtype == GLADIO || item_data->itemtype == LANCE){
                            item_data->configured = 5;
                        }
                    }
                break;*/
                case SpriteBarbarian:
                case SpriteSavage:
                case SpriteRomansoldier:
                case SpritePriest:
                case SpriteGreeksoldier:{
                    struct SoldierData* sprite_data = (struct SoldierData*)iispr->custom_data;
                    if(sprite_data->configured < 4){
                        sprite_data->configured = 4;
                    }
                    s_item_arg->x = iispr->x;
                    if(item_data->itemtype == GLADIO || item_data->itemtype == LANCE){
                        if(item_data->configured < 5){
                            item_data->configured = 5;
                        }
                    }
                }break;
                case SpriteBarbarianshield:{
                    struct SoldierData* barbarianshield_data = (struct SoldierData*)iispr->custom_data;
                    if(THIS->x < iispr->x){//gli sto a sinistra
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
                    item_data->configured = 5;
                }break;
                case SpriteHorse:
                case SpriteBiga:{
                    struct ItemData* weapon_data = (struct ItemData*) s_item_arg->custom_data;
                    if(weapon_data->itemtype == ENEMY_LANCE){
                        horse_hit(-6);
                    }
                }break;
            }
        }
    }
}

void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED{
	UINT8 arg_spritetype = 0u;
    if(arg_spritetype == 0){
		switch(arg_itemtype){
			case GLADIO: arg_spritetype = SpriteItemgladio; break;
			case LANCE: arg_spritetype = SpriteItemlance; break;
			case FIRE: arg_spritetype = SpriteItemfire; break;
			case ELMET: arg_spritetype = SpriteItemelmet; break;
			case CAPE: arg_spritetype = SpriteItemcape; break;
			case SHIELD: arg_spritetype = SpriteItemshield; break;
			case HP: arg_spritetype = SpriteItemheart; break;
			case TIME: arg_spritetype = SpriteItemglass; break;
			case PAPYRUS: arg_spritetype = SpriteItempapirus; break;
		}
	}
	Sprite* s_item_spawned = SpriteManagerAdd(arg_spritetype, arg_posx, arg_posy);
	struct ItemData* item_spawned_data = (struct ItemData*) s_item_spawned->custom_data;
	item_spawned_data->itemtype = arg_itemtype;
	item_spawned_data->configured = 1;
    item_spawned_data->flag_continuous_spawning = 0;
}

void item_spawn_continuously(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED{
    UINT8 arg_spritetype = 0u;
    if(arg_spritetype == 0){
		switch(arg_itemtype){
			case GLADIO: arg_spritetype = SpriteItemgladio; break;
			case LANCE: arg_spritetype = SpriteItemlance; break;
			case FIRE: arg_spritetype = SpriteItemfire; break;
			case ELMET: arg_spritetype = SpriteItemelmet; break;
			case CAPE: arg_spritetype = SpriteItemcape; break;
			case SHIELD: arg_spritetype = SpriteItemshield; break;
			case HP: arg_spritetype = SpriteItemheart; break;
			case TIME: arg_spritetype = SpriteItemglass; break;
		}
	}
	s_spawning_weapon = SpriteManagerAdd(arg_spritetype, arg_posx, arg_posy);
	struct ItemData* item_spawned_data = (struct ItemData*) s_spawning_weapon->custom_data;
	item_spawned_data->itemtype = arg_itemtype;
	item_spawned_data->configured = 1;
    item_spawned_data->flag_continuous_spawning = 1;
}