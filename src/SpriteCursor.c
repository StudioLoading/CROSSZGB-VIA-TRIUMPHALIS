#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_cursor[] = {2, 0,1};

void START() {
    SetSpriteAnim(THIS, a_cursor, 32u);
}

void UPDATE() {
}

void DESTROY() {
}