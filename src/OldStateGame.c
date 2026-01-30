#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "horse_anim.h"

IMPORT_MAP(map);

void START(void) {
#if defined(NINTENDO)
	// set DMG palettes: white objects on the black screen
	OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
	OBP1_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
	BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
#endif
	// just black screen
	InitScroll(BANK(map), &map, 0, 0);
	// spawn the player sprite at the center of the screen
	SpriteManagerAdd(SpriteHorse, ((SCREEN_WIDTH - (horse_anim_WIDTH << 3)) / 2), (SCREEN_HEIGHT - (horse_anim_HEIGHT << 3) - 8));
}

void UPDATE(void) {
}
