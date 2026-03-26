#include "Banks/SetAutoBank.h"

#include "Sprite.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "custom_datas.h"

UINT16 current_points = 0u; // max 65.535

UINT16 add_points(POINTS_TYPE arg_points_type, INT16 arg_points) BANKED;
UINT16 get_points(void) BANKED;
Sprite* spawn_points(POINTS_TYPE arg_points_type, INT16 arg_points, UINT16 arg_x, UINT16 arg_y) BANKED;
void init_current_level_points(void) BANKED;

#define NUM_POINTS_TYPES 9

struct LEVEL_POINTS current_level_points[NUM_POINTS_TYPES] = {
    {PICKUP_GOLDEN,   0},
    {PICKUP_HP,       0},
    {ELANCE_DODGED,   0},
    {ENEMY_KILLED,    0},
    {HP_REMAINED,     0},
    {TIME_REMAINED,   0},
    {BY_ENEMY_HIT,    0},
    {BY_SKULL_HIT,    0},
    {BY_ELANCE_HIT,   0}
};

void init_current_level_points(void) BANKED{
    current_level_points[PICKUP_GOLDEN].points =  0;
    current_level_points[PICKUP_HP].points =  0;
    current_level_points[ELANCE_DODGED].points =  0;
    current_level_points[ENEMY_KILLED].points =  0;
    current_level_points[BY_ENEMY_HIT].points =  0;
    current_level_points[BY_SKULL_HIT].points =  0;
    current_level_points[BY_ELANCE_HIT].points =  0;
}

UINT16 add_points(POINTS_TYPE arg_points_type, INT16 arg_points) BANKED{
    current_level_points[arg_points_type].points += arg_points;
    current_points = current_points + arg_points;
    return current_points;
}

UINT16 get_points(void) BANKED{
    return current_points;
}

Sprite* spawn_points(POINTS_TYPE arg_points_type, INT16 arg_points, UINT16 arg_x, UINT16 arg_y) BANKED{
    add_points(arg_points_type, arg_points);
    Sprite* s_points = SpriteManagerAdd(SpritePoints, arg_x, arg_y);
    struct PointsData* points_data = (struct PointsData*) s_points->custom_data;
    points_data->points = arg_points;
    points_data->configured = 1u;
    return s_points;
}