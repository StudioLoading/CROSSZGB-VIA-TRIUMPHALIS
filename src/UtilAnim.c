#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "custom_datas.h"

IMPORT_TILES(worldmapt);
IMPORT_TILES(worldmapt00);//ROME
IMPORT_TILES(worldmapt01);//ALPS
IMPORT_TILES(worldmapt02);//SEE
IMPORT_TILES(worldmapt03);//GREECE
IMPORT_TILES(worldmapt04);//DESERT
IMPORT_TILES(worldmapt05);//EGYPT

IMPORT_TILES(arearomet99);
IMPORT_TILES(arearomet00);
IMPORT_TILES(arearomet01);
IMPORT_TILES(arearomet02);

IMPORT_TILES(areaalpst99);
IMPORT_TILES(areaalpst00);
IMPORT_TILES(areaalpst01);
IMPORT_TILES(areaalpst02);
IMPORT_TILES(areaalpst03);

IMPORT_TILES(areaseat99);
IMPORT_TILES(areaseat00);
IMPORT_TILES(areaseat01);
IMPORT_TILES(areaseat02);
IMPORT_TILES(areaseat03);

IMPORT_TILES(areagreecet99);
IMPORT_TILES(areagreecet00);
IMPORT_TILES(areagreecet01);
IMPORT_TILES(areagreecet02);
IMPORT_TILES(areagreecet03);

IMPORT_TILES(areadesertt);
IMPORT_TILES(areadesertt00);
IMPORT_TILES(areadesertt01);
IMPORT_TILES(areadesertt02);
IMPORT_TILES(areadesertt99);

IMPORT_TILES(areaegyptt99);
IMPORT_TILES(areaegyptt00);
IMPORT_TILES(areaegyptt01);
IMPORT_TILES(areaegyptt02);

IMPORT_TILES(mapegyptt);
IMPORT_TILES(mapegyptt1);
IMPORT_TILES(mapegyptt2);
IMPORT_TILES(mapegyptt3);
IMPORT_TILES(mapegyptt4);
IMPORT_TILES(mapegyptt5);

IMPORT_TILES(configmapt);
IMPORT_TILES(configmapt00elm);
IMPORT_TILES(configmapt00wheel);
IMPORT_TILES(configmapt00briglie);
IMPORT_TILES(configmapt00whip);

void set_banked_bkg_data(UINT8 first_tile, UINT8 nb_tiles, struct TilesInfo* t, UINT8 bank) NONBANKED {
    uint8_t save = _current_bank;
    SWITCH_ROM(bank);
    set_bkg_data(first_tile, nb_tiles, t->data+((16u) * first_tile));
	SWITCH_ROM(save);
}

void Anim_worldmap_0() BANKED{
    set_banked_bkg_data(0, 123u, &worldmapt, BANK(worldmapt));
}

void Anim_worldmap_1(AREA arg_current_area) BANKED{
    switch(arg_current_area){
        case AREA_ROME:
            set_banked_bkg_data(0, 123u, &worldmapt00, BANK(worldmapt00));
        break;
        case AREA_ALPS:
            set_banked_bkg_data(0, 123u, &worldmapt01, BANK(worldmapt01));
        break;
        case AREA_SEA:
            set_banked_bkg_data(0, 123u, &worldmapt02, BANK(worldmapt02));
        break;
        case AREA_GREECE:
            set_banked_bkg_data(0, 123u, &worldmapt03, BANK(worldmapt03));
        break;
        case AREA_DESERT:
            set_banked_bkg_data(0, 123u, &worldmapt04, BANK(worldmapt04));
        break;
        case AREA_EGYPT:
            set_banked_bkg_data(0, 123u, &worldmapt05, BANK(worldmapt05));
        break;
    }
}

void Anim_arearome_0() BANKED{
    set_banked_bkg_data(1, 73u, &arearomet99, BANK(arearomet99));
}
void Anim_arearome_1() BANKED{
    set_banked_bkg_data(1, 73u, &arearomet00, BANK(arearomet00));
}
void Anim_arearome_2() BANKED{
    set_banked_bkg_data(1, 73u, &arearomet01, BANK(arearomet01));
}
void Anim_arearome_3() BANKED{
    set_banked_bkg_data(1, 73u, &arearomet02, BANK(arearomet02));
}


void Anim_areaalps_0() BANKED{
    set_banked_bkg_data(1, 83u, &areaalpst99, BANK(areaalpst99));
}
void Anim_areaalps_1() BANKED{
    set_banked_bkg_data(1, 83u, &areaalpst00, BANK(areaalpst00));
}
void Anim_areaalps_2() BANKED{
    set_banked_bkg_data(1, 83u, &areaalpst01, BANK(areaalpst01));
}
void Anim_areaalps_3() BANKED{
    set_banked_bkg_data(1, 83u, &areaalpst02, BANK(areaalpst02));
}
void Anim_areaalps_4() BANKED{
    set_banked_bkg_data(1, 83u, &areaalpst03, BANK(areaalpst03));
}

void Anim_areasea_0() BANKED{
    set_banked_bkg_data(1, 111u, &areaseat99, BANK(areaseat99));
}
void Anim_areasea_1() BANKED{
    set_banked_bkg_data(1, 111u, &areaseat00, BANK(areaseat00));
}
void Anim_areasea_2() BANKED{
    set_banked_bkg_data(1, 111u, &areaseat01, BANK(areaseat01));
}
void Anim_areasea_3() BANKED{
    set_banked_bkg_data(1, 111u, &areaseat02, BANK(areaseat02));
}
void Anim_areasea_4() BANKED{
    set_banked_bkg_data(1, 111u, &areaseat03, BANK(areaseat03));
}

void Anim_areagreece_0() BANKED{
    set_banked_bkg_data(1, 123u, &areagreecet99, BANK(areagreecet99));
}
void Anim_areagreece_1() BANKED{
    set_banked_bkg_data(1, 123u, &areagreecet00, BANK(areagreecet00));
}
void Anim_areagreece_2() BANKED{
    set_banked_bkg_data(1, 123u, &areagreecet01, BANK(areagreecet01));
}
void Anim_areagreece_3() BANKED{
    set_banked_bkg_data(1, 123u, &areagreecet02, BANK(areagreecet02));
}
void Anim_areagreece_4() BANKED{
    set_banked_bkg_data(1, 123u, &areagreecet03, BANK(areagreecet03));
}


void Anim_areadesert_0() BANKED{
    set_banked_bkg_data(1, 95u, &areadesertt, BANK(areadesertt));
}
void Anim_areadesert_1() BANKED{
    set_banked_bkg_data(1, 95u, &areadesertt00, BANK(areadesertt00));
}
void Anim_areadesert_2() BANKED{
    set_banked_bkg_data(1, 95u, &areadesertt01, BANK(areadesertt01));
}
void Anim_areadesert_3() BANKED{
    set_banked_bkg_data(1, 95u, &areadesertt02, BANK(areadesertt02));
}

void Anim_areaegypt_0() BANKED{
    set_banked_bkg_data(1, 95u, &areaegyptt99, BANK(areaegyptt99));
}
void Anim_areaegypt_1() BANKED{
    set_banked_bkg_data(1, 95u, &areaegyptt00, BANK(areaegyptt00));
}
void Anim_areaegypt_2() BANKED{
    set_banked_bkg_data(1, 95u, &areaegyptt01, BANK(areaegyptt01));
}
void Anim_areaegypt_3() BANKED{
    set_banked_bkg_data(1, 95u, &areaegyptt02, BANK(areaegyptt02));
}

void Anim_mapegypt_0() BANKED{
    set_banked_bkg_data(32, 73u, &mapegyptt, BANK(mapegyptt));
}
void Anim_mapegypt_1() BANKED{
    set_banked_bkg_data(32, 73u, &mapegyptt1, BANK(mapegyptt1));
}
void Anim_mapegypt_2() BANKED{
    set_banked_bkg_data(32, 73u, &mapegyptt2, BANK(mapegyptt2));
}
void Anim_mapegypt_3() BANKED{
    set_banked_bkg_data(32, 73u, &mapegyptt3, BANK(mapegyptt3));
}
void Anim_mapegypt_4() BANKED{
    set_banked_bkg_data(32, 73u, &mapegyptt4, BANK(mapegyptt4));
}
void Anim_mapegypt_5() BANKED{
    set_banked_bkg_data(32, 73u, &mapegyptt5, BANK(mapegyptt5));
}

void Anim_config_no() BANKED{
    set_banked_bkg_data(0, 167u, &configmapt, BANK(configmapt));
}
void Anim_config_elm() BANKED{
    set_banked_bkg_data(0, 167u, &configmapt00elm, BANK(configmapt00elm));
}
void Anim_config_wheel() BANKED{
    set_banked_bkg_data(0, 167u, &configmapt00wheel, BANK(configmapt00wheel));
}
void Anim_config_reins() BANKED{
    set_banked_bkg_data(0, 167u, &configmapt00briglie, BANK(configmapt00briglie));
}
void Anim_config_whip() BANKED{
    set_banked_bkg_data(0, 167u, &configmapt00whip, BANK(configmapt00whip));
}