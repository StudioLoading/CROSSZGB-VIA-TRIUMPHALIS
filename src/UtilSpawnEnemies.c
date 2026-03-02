#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern Sprite* s_horse;
extern INT8 vx;

UINT8 current_enemies_total_count = 0u;
INT8 flag_danger_right, flag_danger_left, flag_danger_up, flag_danger_down = 0;

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
const struct SpawningMapRect spawning_map_mission05[7] = {
    {
        .spawn_x = ((UINT16) 47u << 3),
        .spawn_y = ((UINT16) 10u << 3)+2u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 32u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 15u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 1, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    },
    {
        .spawn_x = ((UINT16) 46u << 3),
        .spawn_y = ((UINT16) 11u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 32u << 3),
        .box_y = ((UINT16) 8u << 3),
        .box_width =  ((UINT16) 30u << 3),
        .box_height =  ((UINT16) 15u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 2, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    },
    {
        .spawn_x = ((UINT16) 87u << 3),
        .spawn_y = ((UINT16) 9u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 69u << 3),
        .box_y = ((UINT16) 6u << 3),
        .box_width =  ((UINT16) 36u << 3),
        .box_height =  ((UINT16) 14u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 1, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    },
    {
        .spawn_x = ((UINT16) 68u << 3),
        .spawn_y = ((UINT16) 22u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 58u << 3),
        .box_y = ((UINT16) 20u << 3),
        .box_width =  ((UINT16) 20u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 2, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    },
    {
        .spawn_x = ((UINT16) 47u << 3),
        .spawn_y = ((UINT16) 44u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 34u << 3),
        .box_y = ((UINT16) 41u << 3),
        .box_width =  ((UINT16) 26u << 3),
        .box_height =  ((UINT16) 7u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 2, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    },
    {
        .spawn_x = ((UINT16) 57u << 3) + 7u,
        .spawn_y = ((UINT16) 45u << 3) + 2u,
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 44u << 3),
        .box_y = ((UINT16) 42u << 3),
        .box_width =  ((UINT16) 26u << 3),
        .box_height =  ((UINT16) 7u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 2, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    },
    {
        .spawn_x = ((UINT16) 46u << 3),
        .spawn_y = ((UINT16) 58u << 3),
        .box_flag_spawned = 0u,
        .box_x = ((UINT16) 44u << 3),
        .box_y = ((UINT16) 53u << 3),
        .box_width =  ((UINT16) 20u << 3),
        .box_height =  ((UINT16) 10u << 3),
        .box_data.soldier = {
            .vx = 0, .vy = 0, .frmskip = 0, .frmskip_max = 10, 
            .configured = 1, .reward = 0, .points = 10 
        },
        .type = SpriteBarbarianshield
    }
};

#define DIE_COUNTER_MAX 80
#define COUNTER_DANGER_MAX 60
INT8 die_counter = DIE_COUNTER_MAX;
INT8 counter_danger = 0;
struct SpawningMapRect current_spawning_map[10];
int current_spawning_map_count = 0;

void init_enemies_map(void) BANKED;
void spawn_enemies(void) BANKED;
UINT8 check_horse_in_box(UINT8 arg_index, UINT16 arg_current_horse_x, UINT16 arg_current_horse_y) BANKED;
void calculate_danger(Sprite* s_danger) BANKED;


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
        case StateMission05alps:
            current_spawning_map[0] = spawning_map_mission05[0];
            current_spawning_map[1] = spawning_map_mission05[1];
            current_spawning_map[2] = spawning_map_mission05[2];
            current_spawning_map[3] = spawning_map_mission05[3];
            current_spawning_map[4] = spawning_map_mission05[4];
            current_spawning_map[5] = spawning_map_mission05[5];
            current_spawning_map[6] = spawning_map_mission05[6];

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
            calculate_danger(s_spawned_enemy);
            check_danger();
            show_danger();
        }
    }
}


void calculate_danger(Sprite* s_danger) BANKED{
	INT16 distance_x = s_danger->x - s_horse->x;
	INT16 distance_y = s_danger->y - s_horse->y;
	if(distance_y < 0){distance_y = -distance_y;}
	if(distance_y > 32){ return;}
	INT8 flag_danger = flag_danger_right | flag_danger_left | flag_danger_up | flag_danger_down;
	if(flag_danger){return;}
	if(distance_x > 120 && distance_x < 200){//s_danger più a dx di s_horse
		flag_danger_right = 1;
	}
	if(distance_x > -200 && distance_x < -120){//s_danger più a sx di s_horse
		flag_danger_left = 1;
	}
}


void check_danger(void) BANKED{
	INT8 flag_danger = flag_danger_right | flag_danger_left | flag_danger_up | flag_danger_down;
	if(flag_danger && counter_danger == 0){
		counter_danger = COUNTER_DANGER_MAX;
		UPDATE_HUD_TILE(7,2,67);
		UPDATE_HUD_TILE(8,2,68);
		UPDATE_HUD_TILE(9,2,69);		
	}
}

void show_danger(void) BANKED{
	if(counter_danger > 0){
		counter_danger--;
		switch(counter_danger){
			case 58:
				if(flag_danger_right && vx > 0){
					UPDATE_HUD_TILE(8,3,70);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,1);
				}
				if(flag_danger_left && vx < 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,72);
				}
			break;
			case 40:
				if(flag_danger_right && vx > 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,70);
					UPDATE_HUD_TILE(10,3,1);
				}
				if(flag_danger_left && vx < 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,72);
					UPDATE_HUD_TILE(10,3,1);
				}
			break;
			case 20:
				if(flag_danger_right && vx > 0){
					UPDATE_HUD_TILE(8,3,1);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,70);
				}
				if(flag_danger_left && vx < 0){
					UPDATE_HUD_TILE(8,3,72);
					UPDATE_HUD_TILE(9,3,1);
					UPDATE_HUD_TILE(10,3,1);
				}
			break;
		}
		if(counter_danger <= 0){//azzera i flag
			UPDATE_HUD_TILE(7,2,1);
			UPDATE_HUD_TILE(8,2,1);
			UPDATE_HUD_TILE(9,2,1);
			UPDATE_HUD_TILE(8,3,1);
			UPDATE_HUD_TILE(9,3,1);
			UPDATE_HUD_TILE(10,3,1);
			flag_danger_right = 0;
			flag_danger_left = 0;
			flag_danger_up = 0;
			flag_danger_down = 0;
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