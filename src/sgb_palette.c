#pragma bank 10

#include <gb/gb.h>
#include <gb/sgb.h>

#include "sgb_palette.h"


void set_sgb_palette_title() __banked {	
	/*struct {
        UINT8 command;
        UINT16 pal1[4];
        UINT16 pal2[3];
        UINT8 padding;
    } SGB_PALETTE2_PACKET = {
        .command = (SGB_PAL_12 << 3) | 1,
        .pal1 = {myRGB(255, 255, 255), myRGB(88, 216, 84), myRGB(0, 168, 0),  myRGB(0, 0, 0)}, //SGB_STATUS_RED},
        .pal2 = {SGB_STATUS_LIGHT, myRGB(247, 142, 14), myRGB(0, 0, 0)}, //myRGB(228, 92, 16)
        .padding = 0 
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SECONDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 1u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
            0b01010101,0b10101010,0b10101010,0b10101010,
            0b10101010,0b10101010}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_THIRDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 2u,
        .ndata = 20u,
		.style = 0,
   		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
            0b01010101,0b01010101,0b10101010,0b10101010,
            0b10101010,0b10101010}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FOURTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 3u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIFTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 4u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SIXTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 5u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SEVENTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 6u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
    sgb_transfer((void *)&SGB_PALETTE2_PACKET);
    //sgb_transfer((void *)&SGB_PALETTE_FIRSTLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SECONDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_THIRDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FOURTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FIFTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SIXTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SEVENTHLINE_PACKET);
    */
}

void reset_sgb_palette_title() __banked {	
	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIRSTLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 0u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SECONDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 1u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_THIRDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 2u,
        .ndata = 20u,
		.style = 0,
   		.ds = {0,0,0,0,0,0,0,0,0,0}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FOURTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 3u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIFTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 4u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SIXTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 5u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,
        0,0,0,0,
        0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SEVENTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 6u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,
        0,0,0,0,
        0,0}
    };
    sgb_transfer((void *)&SGB_PALETTE_FIRSTLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SECONDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_THIRDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FOURTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FIFTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SIXTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SEVENTHLINE_PACKET);
}

void set_sgb_palette_statusbar() __banked {	
	struct {
        UINT8 command;
        UINT16 pal1[4];
        UINT16 pal2[3];
        UINT8 padding;
    } SGB_PALETTE2_PACKET = {
        .command = (SGB_PAL_12 << 3) | 1,
        .pal1 = {SGB_TUTORIAL_LIGHTER, SGB_AREAROME_LIGHT, SGB_TUTORIAL_LIGHTER, SGB_AREAROME_DARKER},
        .pal2 = {SGB_AREAALPS_LIGHTER, SGB_AREAALPS_LIGHT, SGB_AREAROME_DARKER},
        .padding = 0 
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_0 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 13u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
        0b01010101,0b01010101,0b01010101,0b01010101,
        0b01010101,0b01010101}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_1 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 14u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
        0b01010101,0b01010101,0b01010101,0b01010101,
        0b01010101,0b01010101}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_2 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 15u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
        0b01010101,0b01010101,0b01010101,0b01010101,
        0b01010101,0b01010101}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_3 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 16u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
    
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_4 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 17u,
        .ndata = 20u,
		.style = 0,
		.ds =  {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
    sgb_transfer((void *)&SGB_PALETTE2_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_0);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_1);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_2);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_3);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_4);
}

void reset_sgb_palette_statusbar() __banked{	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_0 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 13u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000}
    };
    struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_1 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 14u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000}
    };
    struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_2 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 15u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_3 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 16u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_4 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 17u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_0);	
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_1);	
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_2);	
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_3);	
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_4);	
}

void set_sgb_palette(UINT16 c0, UINT16 c1, UINT16 c2, UINT16 c3) __banked {
    struct {
        UINT8 command;
        UINT16 pal0[4];
        UINT16 pal1[3];
        UINT8 padding;
    } SGB_PALETTE_PACKET = {
        .command = (SGB_PAL_01 << 3) | 1,
        .pal0 = {c0, c1, c2, c3},
        .pal1 = {0, 0, 0},
        .padding = 0 
    };
    sgb_transfer((void *)&SGB_PALETTE_PACKET);
}


void set_sgb_palette_credit_studioloading() __banked{
    set_sgb_palette(SGB_CREDIT_STUDIOLOADING_LIGHTER, SGB_CREDIT_STUDIOLOADING_LIGHT, SGB_CREDIT_STUDIOLOADING_DARK, SGB_CREDIT_STUDIOLOADING_DARKER);
}

void set_sgb_palette_tutorial() __banked{
    set_sgb_palette(SGB_TUTORIAL_LIGHTER, SGB_TUTORIAL_LIGHT, SGB_TUTORIAL_DARK, SGB_TUTORIAL_DARKER);
}

void set_sgb_palette_credit_viatriumphalis() __banked{
    set_sgb_palette(SGB_CREDIT_VIATRIUMPHALIS_LIGHTER, SGB_CREDIT_VIATRIUMPHALIS_LIGHT, SGB_CREDIT_VIATRIUMPHALIS_DARK, SGB_CREDIT_VIATRIUMPHALIS_DARKER);
}

void set_sgb_palette_credit_titlescreen() __banked{
    set_sgb_palette(SGB_CREDIT_TITLESCREEN_LIGHTER, SGB_CREDIT_TITLESCREEN_LIGHT, SGB_CREDIT_TITLESCREEN_DARK, SGB_CREDIT_TITLESCREEN_DARKER);
}

void set_sgb_palette_arearome() __banked{
    set_sgb_palette(SGB_AREAROME_LIGHTER, SGB_AREAROME_LIGHT, SGB_AREAROME_DARK, SGB_AREAROME_DARKER);
}
void set_sgb_palette_areaalps() __banked{
    set_sgb_palette(SGB_AREAALPS_LIGHTER, SGB_AREAALPS_LIGHT, SGB_AREAALPS_DARK, SGB_AREAALPS_DARKER);
}
void set_sgb_palette_areasea() __banked{
    set_sgb_palette(SGB_AREASEA_LIGHTER, SGB_AREASEA_LIGHT, SGB_AREASEA_DARK, SGB_AREASEA_DARKER);
}
void set_sgb_palette_areagreece() __banked{
    set_sgb_palette(SGB_AREAGREECE_LIGHTER, SGB_AREAGREECE_LIGHT, SGB_AREAGREECE_DARK, SGB_AREAGREECE_DARKER);
}
void set_sgb_palette_areadesert() __banked{
    set_sgb_palette(SGB_AREADESERT_LIGHTER, SGB_AREADESERT_LIGHT, SGB_AREADESERT_DARK, SGB_AREADESERT_DARKER);
}