#include "ZGBMain.h"
#include "Math.h"

// initialize the game state at start
UINT8 next_state = StateCredit;
UINT8 J_WHIP=J_A;//0x10;
UINT8 J_ATK=J_B;//0x20;


// we don't need tile replacement here because this example has no background
UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	tile_ptr; tile; // suppress warning
	return 255u;    // don't replace anything
}