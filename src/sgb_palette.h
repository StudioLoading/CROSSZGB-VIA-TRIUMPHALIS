#include <gb/gb.h>

#define myRGB(r, g, b) (((UINT16)((b) >> 3) << 10) | ((UINT16)((g) >> 3) << 5) | ((UINT16)((r) >> 3) << 0))

#define SGB_CREDIT_STUDIOLOADING_LIGHTER 	myRGB(255, 255, 255)
#define SGB_CREDIT_STUDIOLOADING_LIGHT 	myRGB(0, 0, 0)
#define SGB_CREDIT_STUDIOLOADING_DARK 	myRGB(0, 0, 0)
#define SGB_CREDIT_STUDIOLOADING_DARKER 	myRGB(0, 0, 0)

#define SGB_CREDIT_VIATRIUMPHALIS_LIGHTER 	myRGB(255, 255, 168)
#define SGB_CREDIT_VIATRIUMPHALIS_LIGHT 	      myRGB(255, 168, 0)
#define SGB_CREDIT_VIATRIUMPHALIS_DARK 	      myRGB(168, 0, 16)
#define SGB_CREDIT_VIATRIUMPHALIS_DARKER 	      myRGB(80, 0, 0)

#define SGB_CREDIT_TITLESCREEN_LIGHTER 	      myRGB(255, 255, 168)
#define SGB_CREDIT_TITLESCREEN_LIGHT 	      myRGB(255, 168, 0)
#define SGB_CREDIT_TITLESCREEN_DARK 	      myRGB(168, 0, 16)
#define SGB_CREDIT_TITLESCREEN_DARKER 	      myRGB(0, 0, 0)

#define SGB_TUTORIAL_LIGHTER                    myRGB(250, 250, 250)
#define SGB_TUTORIAL_LIGHT                    myRGB(255, 255, 168)
#define SGB_TUTORIAL_DARK                    myRGB(255, 168, 0)
#define SGB_TUTORIAL_DARKER                    myRGB(168, 0, 16)

#define SGB_AREAROME_LIGHTER 	  myRGB(255, 255, 168)
#define SGB_AREAROME_LIGHT 	      myRGB(255, 168, 0)
#define SGB_AREAROME_DARK 	      myRGB(168, 0, 16)
#define SGB_AREAROME_DARKER 	  myRGB(0, 0, 0)

#define SGB_AREAALPS_LIGHTER 	  myRGB(182, 206, 207)
#define SGB_AREAALPS_LIGHT 	      myRGB(104, 136, 252)
#define SGB_AREAALPS_DARK 	      myRGB(0, 64, 88)
#define SGB_AREAALPS_DARKER 	  myRGB(80, 48, 0)

#define SGB_AREASEA_LIGHTER 	  myRGB(112, 192, 240)
#define SGB_AREASEA_LIGHT 	      myRGB(0, 176, 16)
#define SGB_AREASEA_DARK 	      myRGB(136, 96, 0)
#define SGB_AREASEA_DARKER        myRGB(0, 0, 0)

#define SGB_AREAGREECE_LIGHTER 	  myRGB(255, 255, 255)
#define SGB_AREAGREECE_LIGHT 	  myRGB(136, 232, 240)
#define SGB_AREAGREECE_DARK 	  myRGB(0, 80, 204)
#define SGB_AREAGREECE_DARKER     myRGB(0, 0, 0)

#define SGB_AREADESERT_LIGHTER 	  myRGB(255, 255, 255)
#define SGB_AREADESERT_LIGHT 	  myRGB(240, 224, 152)
#define SGB_AREADESERT_DARK 	  myRGB(195, 116, 0)
#define SGB_AREADESERT_DARKER     myRGB(0, 0, 0)

void set_sgb_palette_credit_studioloading() __banked;
void set_sgb_palette_credit_viatriumphalis() __banked;
void set_sgb_palette_credit_titlescreen() __banked;
void set_sgb_palette_arearome() __banked;
void set_sgb_palette_areaalps() __banked;
void set_sgb_palette_areasea() __banked;
void set_sgb_palette_areagreece() __banked;
void set_sgb_palette_areadesert() __banked;

void set_sgb_palette_overworldsw() __banked;
void set_sgb_palette_2() __banked;
void set_sgb_palette_inventory() __banked;

void set_sgb_palette_title() __banked;
void reset_sgb_palette_title() __banked;
void set_sgb_palette_statusbar() __banked;
void reset_sgb_palette_statusbar() __banked;
