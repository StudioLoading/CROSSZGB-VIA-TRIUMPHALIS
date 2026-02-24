#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_e_lance_hidden[] = {1, 0};
const UINT8 a_e_lance_blink[] = {2, 0,1};
const UINT8 a_e_lance[] = {1,1};

void item_e_lance_anim_blink(Sprite* s_item_arg) BANKED;
void item_e_lance_anim(Sprite* s_item_arg) BANKED;

extern INT8 flag_hit;

extern void item_common_start(Sprite* s_item_arg) BANKED;
extern void item_common_update(Sprite* s_item_arg) BANKED;
extern void item_common_spritescollision(Sprite* s_item_arg) BANKED;

extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;
extern Sprite* spawn_points(UINT8 arg_points, UINT16 arg_x, UINT16 arg_y) BANKED;

void START(void){
    item_common_start(THIS);
}

void UPDATE(void){
    item_common_update(THIS);
    item_common_spritescollision(THIS);
}

void item_e_lance_anim_blink(Sprite* s_item_arg) BANKED{
    SetSpriteAnim(s_item_arg, a_e_lance_blink, 32);
}

void item_e_lance_anim(Sprite* s_item_arg) BANKED{
    SetSpriteAnim(s_item_arg, a_e_lance, 32);
}

void DESTROY(void){
    SetSpriteAnim(THIS, a_e_lance_hidden, 1u);
    SpriteManagerAdd(SpritePuff, THIS->x - 4u, THIS->y - 4u);
    if(flag_hit == 0){
        spawn_points(10u, THIS->x, THIS->y - 12u);
    }
}