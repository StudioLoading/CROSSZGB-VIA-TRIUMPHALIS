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

const struct SpawningMapRect spawning_map_mission02[7] = {
    {
        .spawn_x = ((UINT16) 110u << 3) - 2u,
        .spawn_y = ((UINT16) 75u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 90u << 3),
        .box_y = ((UINT16) 75u << 3),
        .box_width =  ((UINT16) 40u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    },
    {
        .spawn_x = ((UINT16) 60u << 3),
        .spawn_y = ((UINT16) 75u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 40u << 3),
        .box_y = ((UINT16) 75u << 3),
        .box_width =  ((UINT16) 40u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    },
    {
        .spawn_x = ((UINT16) 151u << 3),
        .spawn_y = ((UINT16) 28u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 136u << 3),
        .box_y = ((UINT16) 28u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    },
    {
        .spawn_x = ((UINT16) 64u << 3),
        .spawn_y = ((UINT16) 3u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 49u << 3),
        .box_y = ((UINT16) 3u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    },
    {
        .spawn_x = ((UINT16) 84u << 3),
        .spawn_y = ((UINT16) 3u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 69u << 3),
        .box_y = ((UINT16) 3u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    },
    {
        .spawn_x = ((UINT16) 104u << 3),
        .spawn_y = ((UINT16) 3u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 89u << 3),
        .box_y = ((UINT16) 3u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 8u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    },
    {
        .spawn_x = ((UINT16) 25u << 3),
        .spawn_y = ((UINT16) 12u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 12u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 18u << 3),
        .box_data.killer = {
            .configured = 0,//99: wait, 0: hidden, 1:blink, 2: visible, 3: blink&disappear
            .timeout = 1, .time_visible = 120,
            .time_blink = 60, .time_attack = 60
        },
        .type = SpriteKiller
    }
};
/*
    s_traitor00 = SpriteManagerAdd(SpriteRomansoldier, ((UINT16) 70u << 3), ((UINT16) 14u << 3));
    struct SoldierData* traitor00_data = (struct SoldierData*) s_traitor00->custom_data;
    traitor00_data->frmskip_max = 10u;
    traitor00_data->configured = 1;
    traitor00_data->reward = NOITEM;
    traitor00_data->points = 100u;
 */

const struct SpawningMapRect spawning_map_mission04[1] = {
    {
        .spawn_x = ((UINT16) 70u << 3),
        .spawn_y = ((UINT16) 15u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 50u << 3),
        .box_y = ((UINT16) 13u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 18u << 3),
        .box_data.soldier = { 
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 1, .reward = 0, .points = 100 
        },
        .type = SpriteRomansoldier
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
            current_spawning_map[1] = spawning_map_mission02[1];
            current_spawning_map[2] = spawning_map_mission02[2];
            current_spawning_map[3] = spawning_map_mission02[3];
            current_spawning_map[4] = spawning_map_mission02[4];
            current_spawning_map[5] = spawning_map_mission02[5];
            current_spawning_map[6] = spawning_map_mission02[6];
            
            current_enemies_total_count = 7;
        break;
        case StateMission04alps:
            current_spawning_map[0] = spawning_map_mission04[0];

            current_enemies_total_count = 1;
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

UINT8 check_horse_in_box(UINT8 arg_index, UINT16 arg_current_horse_x, UINT16 arg_current_horse_y) BANKED {
    // 1. Creiamo un puntatore locale per evitare di indicizzare l'array 100 volte (lento su GB)
    struct SpawningMapRect* ptr = &current_spawning_map[arg_index];
    
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