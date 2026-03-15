#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern Sprite* s_horse;
extern INT8 vx;
extern MISSION_STEP current_step;
extern UINT8 spawned_greeks_flag;

UINT8 current_items_total_count = 0u;
struct SpawningMapRect current_spawning_items[5];

UINT8 check_horse_in_box_items(UINT8 arg_index, UINT16 arg_current_horse_x, UINT16 arg_current_horse_y) BANKED ;

extern void item_spawn(ITEM_TYPE arg_itemtype, UINT16 arg_posx, UINT16 arg_posy) BANKED;


void init_items_map(void) BANKED;
void spawn_items_onmap(void) BANKED;

const struct SpawningMapRect spawning_items_mission01[2] = {
    {
        .spawn_x = ((UINT16) 21u << 3),
        .spawn_y = ((UINT16) 34u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 2u << 3),
        .box_y = ((UINT16) 31u << 3),
        .box_width =  ((UINT16) 3u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.item = { .itemtype = LANCE, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 84u << 3),
        .spawn_y = ((UINT16) 33u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 69u << 3),
        .box_y = ((UINT16) 31u << 3),
        .box_width =  ((UINT16) 37u << 3),
        .box_height =  ((UINT16) 5u << 3),
        .box_data.item = { .itemtype = LANCE, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission02[2] = {
    {
        .spawn_x = ((UINT16) 38u << 3),
        .spawn_y = ((UINT16) 79u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 19u << 3),
        .box_y = ((UINT16) 71u << 3),
        .box_width =  ((UINT16) 40u << 3),
        .box_height =  ((UINT16) 12u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 113u << 3),
        .spawn_y = ((UINT16) 55u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 96u << 3),
        .box_y = ((UINT16) 42u << 3),
        .box_width =  ((UINT16) 35u << 3),
        .box_height =  ((UINT16) 26u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission05[4] = {
    {
        .spawn_x = ((UINT16) 22u << 3),
        .spawn_y = ((UINT16) 10u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 31u << 3),
        .box_y = ((UINT16) 7u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.item = { .itemtype = FLAME, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 104u << 3),
        .spawn_y = ((UINT16) 20u << 3) + 2u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 85u << 3),
        .box_y = ((UINT16) 18u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = FLAME, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 26u << 3),
        .spawn_y = ((UINT16) 41u << 3) + 2u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 44u << 3),
        .box_y = ((UINT16) 42u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 7u << 3),
        .box_data.item = { .itemtype = LANCE, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 79u << 3),
        .spawn_y = ((UINT16) 56u << 3) + 2u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 59u << 3),
        .box_y = ((UINT16) 52u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 11u << 3),
        .box_data.item = { .itemtype = LANCE, .flag_continuous_spawning = 0 },
        .type = 0
    }        
};
const struct SpawningMapRect spawning_items_mission06[5] = {
    {
        .spawn_x = ((UINT16) 44u << 3),
        .spawn_y = ((UINT16) 15u << 3) - 4u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 30u << 3),
        .box_y = ((UINT16) 2u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = ELMET, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 166u << 3),
        .spawn_y = ((UINT16) 14u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 149u << 3),
        .box_y = ((UINT16) 5u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = ELMET, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 111u << 3),
        .spawn_y = ((UINT16) 19u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 94u << 3),
        .box_y = ((UINT16) 7u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 15u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 128u << 3),
        .spawn_y = ((UINT16) 18u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 113u << 3),
        .box_y = ((UINT16) 17u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.item = { .itemtype = ELMET, .flag_continuous_spawning = 0 },
        .type = 0
    },
    {
        .spawn_x = ((UINT16) 155u << 3),
        .spawn_y = ((UINT16) 9u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 137u << 3),
        .box_y = ((UINT16) 11u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 14u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission07[2] = {
    {
        .spawn_x = ((UINT16) 18u << 3),
        .spawn_y = ((UINT16) 16u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 10u << 3),
        .box_y = ((UINT16) 13u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = SHIELD, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 60u << 3),
        .spawn_y = ((UINT16) 41u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 39u << 3),
        .box_y = ((UINT16) 32u << 3),
        .box_width =  ((UINT16) 17u << 3),
        .box_height =  ((UINT16) 5u << 3),
        .box_data.item = { .itemtype = LANCE, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission08[3] = {
    {
        .spawn_x = ((UINT16) 70u << 3),
        .spawn_y = ((UINT16) 29u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 66u << 3),
        .box_y = ((UINT16) 14u << 3),
        .box_width =  ((UINT16) 11u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 44u << 3),
        .spawn_y = ((UINT16) 61u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 28u << 3),
        .box_y = ((UINT16) 59u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 20u << 3),
        .spawn_y = ((UINT16) 63u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 6u << 3),
        .box_y = ((UINT16) 59u << 3),
        .box_width =  ((UINT16) 8u << 3),
        .box_height =  ((UINT16) 3u << 3),
        .box_data.item = { .itemtype = PAPYRUS, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission09[2] = {
    {
        .spawn_x = ((UINT16) 21u << 3),
        .spawn_y = ((UINT16) 8u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 9u << 3),
        .box_y = ((UINT16) 6u << 3),
        .box_width =  ((UINT16) 3u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = SHIELD, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 113u << 3),
        .spawn_y = ((UINT16) 10u << 3) + 3u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 123u << 3),
        .box_y = ((UINT16) 7u << 3),
        .box_width =  ((UINT16) 6u << 3),
        .box_height =  ((UINT16) 9u << 3),
        .box_data.item = { .itemtype = SHIELD, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission10[4] = {
    {
        .spawn_x = ((UINT16) 93u << 3),
        .spawn_y = ((UINT16) 9u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 78u << 3),
        .box_y = ((UINT16) 1u << 3),
        .box_width =  ((UINT16) 3u << 3),
        .box_height =  ((UINT16) 40u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 52u << 3),
        .spawn_y = ((UINT16) 25u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 42u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 20u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 39u << 3) + 4u,
        .spawn_y = ((UINT16) 42u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 24u << 3),
        .box_y = ((UINT16) 44u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 91u << 3),
        .spawn_y = ((UINT16) 53u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 1u << 3),
        .box_y = ((UINT16) 48u << 3),
        .box_width =  ((UINT16) 98u << 3),
        .box_height =  ((UINT16) 30u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission11[2] = {
    {
        .spawn_x = ((UINT16) 17u << 3),
        .spawn_y = ((UINT16) 14u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 9u << 3),
        .box_y = ((UINT16) 11u << 3),
        .box_width =  ((UINT16) 8u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.item = { .itemtype = LANCE, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 65u << 3),
        .spawn_y = ((UINT16) 11u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 52u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 26u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    },
};
const struct SpawningMapRect spawning_items_mission12[2] = {
    {
        .spawn_x = ((UINT16) 72u << 3),
        .spawn_y = ((UINT16) 20u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 52u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 20u << 3),
        .box_height =  ((UINT16) 24u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 126u << 3),
        .spawn_y = ((UINT16) 25u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 102u << 3),
        .box_y = ((UINT16) 21u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission13[3] = {
    {
        .spawn_x = ((UINT16) 36u << 3),
        .spawn_y = ((UINT16) 47u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 47u << 3),
        .box_y = ((UINT16) 46u << 3),
        .box_width =  ((UINT16) 5u << 3),
        .box_height =  ((UINT16) 9u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 21u << 3),
        .spawn_y = ((UINT16) 74u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 21u << 3),
        .box_y = ((UINT16) 56u << 3),
        .box_width =  ((UINT16) 8u << 3),
        .box_height =  ((UINT16) 3u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 86u << 3),
        .spawn_y = ((UINT16) 58u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 87u << 3),
        .box_y = ((UINT16) 56u << 3),
        .box_width =  ((UINT16) 3u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission14[3] = {
    {
        .spawn_x = ((UINT16) 29u << 3),
        .spawn_y = ((UINT16) 7u << 3) + 3u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 16u << 3),
        .box_y = ((UINT16) 6u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = FLAME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 130u << 3),
        .spawn_y = ((UINT16) 7u << 3) + 3u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 112u << 3),
        .box_y = ((UINT16) 5u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = FLAME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 199u << 3),
        .spawn_y = ((UINT16) 7u << 3) + 1u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 180u << 3),
        .box_y = ((UINT16) 5u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = FLAME, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission17[2] = {
    {
        .spawn_x = ((UINT16) 57u << 3),
        .spawn_y = ((UINT16) 10u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 39u << 3),
        .box_y = ((UINT16) 6u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.item = { .itemtype = ELMET, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 67u << 3),
        .spawn_y = ((UINT16) 36u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 66u << 3),
        .box_y = ((UINT16) 20u << 3),
        .box_width =  ((UINT16) 18u << 3),
        .box_height =  ((UINT16) 4u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission18[2] = {
    {
        .spawn_x = ((UINT16) 75u << 3),
        .spawn_y = ((UINT16) 6u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 53u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 12u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 132u << 3),
        .spawn_y = ((UINT16) 9u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 110u << 3),
        .box_y = ((UINT16) 2u << 3),
        .box_width =  ((UINT16) 18u << 3),
        .box_height =  ((UINT16) 20u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission19[3] = {
    {
        .spawn_x = ((UINT16) 38u << 3),
        .spawn_y = ((UINT16) 28u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 54u << 3),
        .box_y = ((UINT16) 20u << 3),
        .box_width =  ((UINT16) 10u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 129u << 3),
        .spawn_y = ((UINT16) 39u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 109u << 3),
        .box_y = ((UINT16) 34u << 3),
        .box_width =  ((UINT16) 8u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 110u << 3),
        .spawn_y = ((UINT16) 29u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 88u << 3),
        .box_y = ((UINT16) 7u << 3),
        .box_width =  ((UINT16) 4u << 3),
        .box_height =  ((UINT16) 11u << 3),
        .box_data.item = { .itemtype = HP, .flag_continuous_spawning = 0 },
        .type = 0
    }
};
const struct SpawningMapRect spawning_items_mission20[4] = {
    {
        .spawn_x = ((UINT16) 12u << 3),
        .spawn_y = ((UINT16) 4u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 4u << 3),
        .box_y = ((UINT16) 2u << 3),
        .box_width =  ((UINT16) 10u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = GLADIO, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 108u << 3),
        .spawn_y = ((UINT16) 8u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 83u << 3),
        .box_y = ((UINT16) 7u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 206u << 3),
        .spawn_y = ((UINT16) 7u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 182u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 6u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    },{
        .spawn_x = ((UINT16) 176u << 3),
        .spawn_y = ((UINT16) 10u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 153u << 3),
        .box_y = ((UINT16) 6u << 3),
        .box_width =  ((UINT16) 2u << 3),
        .box_height =  ((UINT16) 11u << 3),
        .box_data.item = { .itemtype = TIME, .flag_continuous_spawning = 0 },
        .type = 0
    }
};

void init_items_map(void) BANKED{
    switch(current_state){
        case StateMission01rome:
            current_spawning_items[0] = spawning_items_mission01[0];
            current_spawning_items[1] = spawning_items_mission01[1];
            current_items_total_count = 2;
        break;
        case StateMission02rome:
            current_spawning_items[0] = spawning_items_mission02[0];
            current_spawning_items[1] = spawning_items_mission02[1];
            current_items_total_count = 2;
        break;
        case StateMission05alps:
            current_spawning_items[0] = spawning_items_mission05[0];
            current_spawning_items[1] = spawning_items_mission05[1];
            current_spawning_items[2] = spawning_items_mission05[2];
            current_spawning_items[3] = spawning_items_mission05[3];
            current_items_total_count = 4;
        break;
        case StateMission06alps:
            current_spawning_items[0] = spawning_items_mission06[0];
            current_spawning_items[1] = spawning_items_mission06[1];
            current_spawning_items[2] = spawning_items_mission06[2];
            current_spawning_items[3] = spawning_items_mission06[3];
            current_spawning_items[4] = spawning_items_mission06[4];
            current_items_total_count = 5;
        break;
        case StateMission07alps:
            current_spawning_items[0] = spawning_items_mission07[0];
            current_spawning_items[1] = spawning_items_mission07[1];
            current_items_total_count = 2;
        break;
        case StateMission08sea:
            current_spawning_items[0] = spawning_items_mission08[0];
            current_spawning_items[1] = spawning_items_mission08[1];
            current_spawning_items[2] = spawning_items_mission08[2];
            current_items_total_count = 3;
        break;
        case StateMission09sea:
            current_spawning_items[0] = spawning_items_mission09[0];
            current_spawning_items[1] = spawning_items_mission09[1];
            current_items_total_count = 2;
        break;
        case StateMission10sea:
            current_spawning_items[0] = spawning_items_mission10[0];
            current_spawning_items[1] = spawning_items_mission10[1];
            current_spawning_items[2] = spawning_items_mission10[2];
            current_spawning_items[3] = spawning_items_mission10[3];
            current_items_total_count = 4;
        break;
        case StateMission11sea:
            current_spawning_items[0] = spawning_items_mission11[0];
            current_spawning_items[1] = spawning_items_mission11[1];
            current_items_total_count = 2;
        break;
        case StateMission12greece:
            current_spawning_items[0] = spawning_items_mission12[0];
            current_spawning_items[1] = spawning_items_mission12[1];
            current_items_total_count = 2;
        break;
        case StateMission13greece:
            current_spawning_items[0] = spawning_items_mission13[0];
            current_spawning_items[1] = spawning_items_mission13[1];
            current_spawning_items[2] = spawning_items_mission13[2];
            current_items_total_count = 3;
        break;
        case StateMission14greece:
            current_spawning_items[0] = spawning_items_mission14[0];
            current_spawning_items[1] = spawning_items_mission14[1];
            current_spawning_items[2] = spawning_items_mission14[2];
            current_items_total_count = 3;
        break;
        case StateMission17desert:
            current_spawning_items[0] = spawning_items_mission17[0];
            current_spawning_items[1] = spawning_items_mission17[1];
            current_items_total_count = 2;
        break;
        case StateMission18desert:
            current_spawning_items[0] = spawning_items_mission18[0];
            current_spawning_items[1] = spawning_items_mission18[1];
            current_items_total_count = 2;
        break;
        case StateMission19egypt:
            current_spawning_items[0] = spawning_items_mission19[0];
            current_spawning_items[1] = spawning_items_mission19[1];
            current_spawning_items[2] = spawning_items_mission19[2];
            current_items_total_count = 3;
        break;
        case StateMission20egypt:
            current_spawning_items[0] = spawning_items_mission20[0];
            current_spawning_items[1] = spawning_items_mission20[1];
            current_spawning_items[2] = spawning_items_mission20[2];
            current_spawning_items[3] = spawning_items_mission20[3];
            current_items_total_count = 4;
        break;
    }
}


void spawn_items_onmap(void) BANKED {
    UINT16 current_horse_x = s_horse->x;
    UINT16 current_horse_y = s_horse->y;
    for(UINT8 i = 0u; i < current_items_total_count; i++){
        if(check_horse_in_box_items(i, current_horse_x, current_horse_y) && current_spawning_items[i].box_flag_spawned == 0u){
            item_spawn(current_spawning_items[i].box_data.item.itemtype, current_spawning_items[i].spawn_x, current_spawning_items[i].spawn_y);
            current_spawning_items[i].box_flag_spawned = 1u;
        }
    }
}


UINT8 check_horse_in_box_items(UINT8 arg_index, UINT16 arg_current_horse_x, UINT16 arg_current_horse_y) BANKED {
    // 1. Creiamo un puntatore locale per evitare di indicizzare l'array 100 volte (lento su GB)
    struct SpawningMapRect* ptr = &current_spawning_items[arg_index];
    
    // 2. Estraiamo i valori in variabili locali UINT16
    UINT16 rx1 = ptr->box_x;
    UINT16 ry1 = ptr->box_y;
    
    // 3. Facciamo il calcolo in modo esplicito per evitare bug del compilatore
    UINT16 rw  = ptr->box_width;
    UINT16 rh  = ptr->box_height;
    
    // Controllo X
    if (arg_current_horse_x >= rx1) {
        if (arg_current_horse_x <= (rx1 + rw)) {
            // Controllo Y
            if (arg_current_horse_y >= ry1) {
                if (arg_current_horse_y <= (ry1 + rh)) {
                    return 1u;
                }
            }
        }
    }    
    return 0u;
}