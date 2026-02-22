#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern Sprite* s_horse;
UINT8 current_enemies_total_count = 0u;

const struct SpawningMapRect spawning_map_mission01[4] = {
    {
        .spawn_x = ((UINT16) 84u << 3),
        .spawn_y = ((UINT16) 5u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 60u << 3),
        .box_y = ((UINT16) 1u << 3),
        .box_width =  ((UINT16) 100u << 3),
        .box_height =  ((UINT16) 15u << 3),
        .box_data.soldier = { 
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 0, 
            .configured = 0, .reward = 0, .points = 0 
        },
        .type = SpriteRomansoldier
    },
    {
        .spawn_x = ((UINT16) 132u << 3),
        .spawn_y = ((UINT16) 17u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 111u << 3),
        .box_y = ((UINT16) 17u << 3),
        .box_width =  ((UINT16) 40u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.soldier = { 
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 0, 
            .configured = 0, .reward = 0, .points = 0 
        },
        .type = SpriteRomansoldier
    },
    {
        .spawn_x = ((UINT16) 104u << 3),
        .spawn_y = ((UINT16) 51u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 87u << 3),
        .box_y = ((UINT16) 48u << 3),
        .box_width =  ((UINT16) 40u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.soldier = { 
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 0, 
            .configured = 0, .reward = 0, .points = 0 
        },
        .type = SpriteRomansoldier
    },
    {
        .spawn_x = ((UINT16) 154u << 3),
        .spawn_y = ((UINT16) 48u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 139u << 3),
        .box_y = ((UINT16) 41u << 3),
        .box_width =  ((UINT16) 40u << 3),
        .box_height =  ((UINT16) 20u << 3),
        .box_data.soldier = { 
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 0, 
            .configured = 0, .reward = 0, .points = 0 
        },
        .type = SpriteRomansoldier
    }
};
/*
    s_killer00 = SpriteManagerAdd(SpriteKiller, ((UINT16) 100u << 3), ((UINT16) 75u << 3));
    s_killer01 = SpriteManagerAdd(SpriteKiller, ((UINT16) 60u << 3), ((UINT16) 75u << 3));
    s_killer02 = SpriteManagerAdd(SpriteKiller, ((UINT16) 90u << 3), ((UINT16) 56u << 3));
    s_killer03 = SpriteManagerAdd(SpriteKiller, ((UINT16) 151u << 3), ((UINT16) 28u << 3));
    s_killer04 = SpriteManagerAdd(SpriteKiller, ((UINT16) 64u << 3), ((UINT16) 3u << 3));
    s_killer05 = SpriteManagerAdd(SpriteKiller, ((UINT16) 84u << 3), ((UINT16) 3u << 3));
    s_killer06 = SpriteManagerAdd(SpriteKiller, ((UINT16) 104u << 3), ((UINT16) 3u << 3));
    s_killer07 = SpriteManagerAdd(SpriteKiller, ((UINT16) 25u << 3), ((UINT16) 16u << 3)); */

const struct SpawningMapRect spawning_map_mission02[7] = {
    {
        .spawn_x = ((UINT16) 100u << 3),
        .spawn_y = ((UINT16) 75u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 75u << 3),
        .box_y = ((UINT16) 75u << 3),
        .box_width =  ((UINT16) 50u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.killer = {
            .configured = 99,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_hidden = 100, .time_visible = 120,
            .time_blink = 40, .time_attack = 60
        },
        .type = SpriteKiller
    }
};

struct SpawningMapRect current_spawning_map[10];
int current_spawning_map_count = 0;

void init_enemies_map(void) BANKED;
void spawn_enemies(void) BANKED;
UINT8 check_horse_in_box(UINT8 arg_index, UINT16 arg_current_horse_x, UINT16 arg_current_horse_y) BANKED;


void init_enemies_map(void) BANKED{//invoked on START of a StateMission
    switch(current_state){
        case StateMission01rome:
            struct SoldierData romansoldier00_data = {
                .frmskip_max = 12u, .configured = 2, .reward = NOITEM, .points = 10u
            };
            struct SoldierData romansoldier01_data = {
                .frmskip_max = 12u, .configured = 2, .reward = HP, .points = 10u
            };
            struct SoldierData romansoldier02_data = {
                .frmskip_max = 12u, .configured = 2, .reward = NOITEM, .points = 10u
            };
            struct SoldierData romansoldier03_data = {
                .frmskip_max = 12u, .configured = 2, .reward = NOITEM, .points = 10u
            };
            current_spawning_map[0] = spawning_map_mission01[0];
            current_spawning_map[1] = spawning_map_mission01[1];
            current_spawning_map[2] = spawning_map_mission01[2];
            current_spawning_map[3] = spawning_map_mission01[3];

            current_spawning_map[0].box_data.soldier = romansoldier00_data;
            current_spawning_map[1].box_data.soldier = romansoldier01_data;
            current_spawning_map[2].box_data.soldier = romansoldier02_data;
            current_spawning_map[3].box_data.soldier = romansoldier03_data;

            current_enemies_total_count = 4;
        break;
        case StateMission02rome:
            current_spawning_map[0] = spawning_map_mission02[0];
            
            current_enemies_total_count = 7;
        break;
    }
}

void spawn_enemies(void) BANKED{
    UINT16 current_horse_x = s_horse->x;
    UINT16 current_horse_y = s_horse->y;
    for(UINT8 i = 0u; i < current_enemies_total_count; i++){
        if(check_horse_in_box(i, current_horse_x, current_horse_y) && current_spawning_map[i].box_flag_spawned == 0u){
            Sprite* s_spawned_enemy = SpriteManagerAdd(current_spawning_map[i].type, current_spawning_map[i].spawn_x, current_spawning_map[i].spawn_y);
            //s_spawned_enemy->custom_data = current_spawning_map[i].box_soldierdata;
            if(current_spawning_map[i].type == SpriteKiller){
                memcpy(s_spawned_enemy->custom_data, &current_spawning_map[i].box_data.killer, sizeof(struct KillerData));
            }else{
                memcpy(s_spawned_enemy->custom_data, &current_spawning_map[i].box_data.soldier, sizeof(struct SoldierData));
            }
            current_spawning_map[i].box_flag_spawned = 1u;
        }
    }
}

UINT8 check_horse_in_box(UINT8 arg_index, UINT16 arg_current_horse_x, UINT16 arg_current_horse_y) BANKED{
    if(arg_current_horse_x > current_spawning_map[arg_index].box_x && 
        arg_current_horse_x < (current_spawning_map[arg_index].box_x + current_spawning_map[arg_index].box_width) 
        && 
        arg_current_horse_y > current_spawning_map[arg_index].box_y && 
        arg_current_horse_y < (current_spawning_map[arg_index].box_y + current_spawning_map[arg_index].box_height)){
            return 1u;
    }
    return 0u;
}