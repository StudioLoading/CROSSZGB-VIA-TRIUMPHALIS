#include "Banks/SetAutoBank.h"

#include "Sprite.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "custom_datas.h"

UINT16 current_points = 0u; // max 65.535

UINT16 add_points(INT8 arg_points) BANKED;
UINT16 get_points() BANKED;
Sprite* spawn_points(UINT8 arg_points, UINT16 arg_x, UINT16 arg_y) BANKED;

UINT16 add_points(INT8 arg_points) BANKED{
    current_points = current_points + arg_points;
    return current_points;
}

UINT16 get_points() BANKED{
    return current_points;
}

Sprite* spawn_points(UINT8 arg_points, UINT16 arg_x, UINT16 arg_y) BANKED{
    add_points(arg_points);
    Sprite* s_points = SpriteManagerAdd(SpritePoints, arg_x, arg_y);
    struct PointsData* points_data = (struct PointsData*) s_points->custom_data;
    points_data->points = arg_points;
    points_data->configured = 1u;
}