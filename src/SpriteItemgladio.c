#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_gladio_blink[] = {2, 0,1};
const UINT8 a_gladio[] = {1,1};
const UINT8 a_gladio_inuse[] = {1,2};
const UINT8 a_gladio_inuse_blink[] = {2, 0,2};

void item_gladio_anim_blink(Sprite* s_item_arg) BANKED;
void item_gladio_anim(Sprite* s_item_arg) BANKED;
void item_gladio_anim_inuse(Sprite* s_item_arg) BANKED;
void item_gladio_anim_inuse_blink(Sprite* s_item_arg) BANKED;

extern void item_common_start(Sprite* s_item_arg) BANKED;
extern void item_common_update(Sprite* s_item_arg) BANKED;

void START(void){
    item_common_start(THIS);
}

void UPDATE(void){
    item_common_update(THIS);
}

void item_gladio_anim_blink(Sprite* s_item_arg) BANKED{
    SetSpriteAnim(s_item_arg, a_gladio_blink, 32);
}

void item_gladio_anim_inuse(Sprite* s_item_arg) BANKED{
    SetSpriteAnim(s_item_arg, a_gladio_inuse, 1);
}

void item_gladio_anim_inuse_blink(Sprite* s_item_arg) BANKED{
    SetSpriteAnim(s_item_arg, a_gladio_inuse_blink, 32);
}

void item_gladio_anim(Sprite* s_item_arg) BANKED{
    SetSpriteAnim(s_item_arg, a_gladio, 32);
}

void DESTROY(void){
}