/**
  * @file GC9A01.c
  *
  * Copyright 2019 OPEN-EYES S.r.l.
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  **/


/*********************
 *      INCLUDES
 *********************/
#include "GC9A01.h"
#if USE_GC9A01

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/
#define GC9A01_CMD_MODE     0
#define GC9A01_DATA_MODE    1

#define CMD_DISPLAY_OFF         0xAE
#define CMD_DISPLAY_ON          0xAF

#define CMD_SET_DISP_START_LINE 0x40
#define CMD_SET_PAGE            0xB0

#define CMD_SET_COLUMN_UPPER    0x10
#define CMD_SET_COLUMN_LOWER    0x00

#define CMD_SET_ADC_NORMAL      0xA0
#define CMD_SET_ADC_REVERSE     0xA1

#define CMD_SET_DISP_NORMAL     0xA6
#define CMD_SET_DISP_REVERSE    0xA7

#define CMD_SET_ALLPTS_NORMAL   0xA4
#define CMD_SET_ALLPTS_ON       0xA5
#define CMD_SET_BIAS_9          0xA2
#define CMD_SET_BIAS_7          0xA3

#define CMD_RMW                 0xE0
#define CMD_RMW_CLEAR           0xEE
#define CMD_INTERNAL_RESET      0xE2
#define CMD_SET_COM_NORMAL      0xC0
#define CMD_SET_COM_REVERSE     0xC8
#define CMD_SET_POWER_CONTROL   0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST    0x81
#define CMD_SET_VOLUME_SECOND   0x00
#define CMD_SET_STATIC_OFF      0xAC
#define CMD_SET_STATIC_ON       0xAD
#define CMD_SET_STATIC_REG      0x00
#define CMD_SET_BOOSTER_FIRST   0xF8
#define CMD_SET_BOOSTER_234     0x00
#define CMD_SET_BOOSTER_5       0x01
#define CMD_SET_BOOSTER_6       0x03
#define CMD_NOP                 0xE3
#define CMD_TEST                0xF0

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
//static void GC9A01_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
static void GC9A01_command(uint8_t cmd);
static void GC9A01_data(uint8_t data);
static void GC9A01_set_addr_win(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);


/**********************
 *  STATIC VARIABLES
 **********************/

// static uint16_t lcd_fb[GC9A01_FB_SIZE];
//static uint8_t pagemap[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

// static struct GC9A01_function GC9A01_cfg_script[] = {
// 	{ GC9A01_START, GC9A01_START},
// 	{ GC9A01_CMD, GC9A01_SWRESET},
// 	{ GC9A01_DELAY, 150},
// 	{ GC9A01_CMD, GC9A01_SLPOUT},
// 	{ GC9A01_DELAY, 500},
// 	{ GC9A01_CMD, GC9A01_INVOFF},
// 	{ GC9A01_CMD, GC9A01_MADCTL},
// 	{ GC9A01_DATA, 0xA0},
// 	{ GC9A01_CMD, GC9A01_COLMOD},
// 	{ GC9A01_DATA, 0x05},
// 	{ GC9A01_CMD, GC9A01_PORCTRL},
// 	{ GC9A01_DATA, 0x0C},
// 	{ GC9A01_DATA, 0x0C},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x33},
// 	{ GC9A01_DATA, 0x33},
// 	{ GC9A01_CMD, GC9A01_GCTRL},
// 	{ GC9A01_DATA, 0x35},
// 	{ GC9A01_CMD, GC9A01_VDVVRHEN},
// 	{ GC9A01_DATA, 0x01},
// 	{ GC9A01_DATA, 0xFF},
// 	{ GC9A01_CMD, GC9A01_VRHS},
// 	{ GC9A01_DATA, 0x17},
// 	{ GC9A01_CMD, GC9A01_VDVSET},
// 	{ GC9A01_DATA, 0x20},
// 	{ GC9A01_CMD, GC9A01_VCOMS},
// 	{ GC9A01_DATA, 0x17},
// 	{ GC9A01_CMD, GC9A01_VCMOFSET},
// 	{ GC9A01_DATA, 0x20},
// 	{ GC9A01_CMD, GC9A01_PWCTRL1},
// 	{ GC9A01_DATA, 0xA4},
// 	{ GC9A01_DATA, 0xA1},
// 	{ GC9A01_CMD, GC9A01_PVGAMCTRL},
// 	{ GC9A01_DATA, 0xD0},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x14},
// 	{ GC9A01_DATA, 0x15},
// 	{ GC9A01_DATA, 0x13},
// 	{ GC9A01_DATA, 0x2C},
// 	{ GC9A01_DATA, 0x42},
// 	{ GC9A01_DATA, 0x43},
// 	{ GC9A01_DATA, 0x4E},
// 	{ GC9A01_DATA, 0x09},
// 	{ GC9A01_DATA, 0x16},
// 	{ GC9A01_DATA, 0x14},
// 	{ GC9A01_DATA, 0x18},
// 	{ GC9A01_DATA, 0x21},
// 	{ GC9A01_CMD, GC9A01_NVGAMCTRL},
// 	{ GC9A01_DATA, 0xD0},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x14},
// 	{ GC9A01_DATA, 0x15},
// 	{ GC9A01_DATA, 0x13},
// 	{ GC9A01_DATA, 0x0B},
// 	{ GC9A01_DATA, 0x43},
// 	{ GC9A01_DATA, 0x55},
// 	{ GC9A01_DATA, 0x53},
// 	{ GC9A01_DATA, 0x0C},
// 	{ GC9A01_DATA, 0x17},
// 	{ GC9A01_DATA, 0x14},
// 	{ GC9A01_DATA, 0x23},
// 	{ GC9A01_DATA, 0x20},
// 	{ GC9A01_CMD, GC9A01_CASET},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x01},
// 	{ GC9A01_DATA, 0x3F},
// 	{ GC9A01_CMD, GC9A01_RASET},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0x00},
// 	{ GC9A01_DATA, 0xEF},
// 	{ GC9A01_CMD, GC9A01_DISPON},
// 	{ GC9A01_DELAY, 100},
// 	{ GC9A01_END, GC9A01_END},
// };

static struct GC9A01_function GC9A01_cfg_script[] = {
	{ GC9A01_START, GC9A01_START},
	{ GC9A01_CMD, GC9A01_SWRESET},
	{ GC9A01_DELAY, 150},
	{ GC9A01_CMD, GC9A01_SLPOUT},
	{ GC9A01_DELAY, 255},
	{ GC9A01_CMD, GC9A01_COLMOD},
	{ GC9A01_DATA, 0x55},
	{ GC9A01_DELAY, 10},
	{ GC9A01_CMD, GC9A01_MADCTL},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_CMD, GC9A01_CASET},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, GC9A01_XSTART},
	{ GC9A01_DATA, (GC9A01_HOR_RES + GC9A01_XSTART) >> 8},
	{ GC9A01_DATA, (GC9A01_HOR_RES + GC9A01_XSTART) & 0xff},
	{ GC9A01_CMD, GC9A01_RASET},
	{ GC9A01_DATA, 0x00},
	{ GC9A01_DATA, GC9A01_YSTART},
	{ GC9A01_DATA, (GC9A01_VER_RES + GC9A01_YSTART) >> 8},
	{ GC9A01_DATA, (GC9A01_VER_RES + GC9A01_YSTART) & 0xff},
	{ GC9A01_CMD, GC9A01_INVON},
	{ GC9A01_DELAY, 10},
	{ GC9A01_CMD, GC9A01_NORON},
	{ GC9A01_DELAY, 10},
	{ GC9A01_CMD, GC9A01_DISPON},
	{ GC9A01_DELAY, 255},
	{ GC9A01_END, GC9A01_END},
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Write a command to the GC9A01
 * @param cmd the command
 */
static void GC9A01_command(uint8_t cmd)
{
    LV_DRV_DISP_CMD_DATA(GC9A01_CMD_MODE);
    LV_DRV_DISP_SPI_WR_BYTE(cmd);
}

/**
 * Write data to the GC9A01
 * @param data the data
 */
static void GC9A01_data(uint8_t data)
{
    LV_DRV_DISP_CMD_DATA(GC9A01_DATA_MODE);
    LV_DRV_DISP_SPI_WR_BYTE(data);
}

static int GC9A01_data_array(uint8_t *buf, uint32_t len)
{
	uint8_t *pt = buf;

	for (uint32_t lp = 0; lp < len; lp++, pt++)
	{
		LV_DRV_DISP_SPI_WR_BYTE(*pt);
	} 
	return 0;
}

static int GC9A01_databuf(uint32_t len, uint8_t *buf)
{
	uint32_t byte_left = len;
	uint8_t *pt = buf;

	while (byte_left)
	{
		if (byte_left > 64)
		{
			LV_DRV_DISP_SPI_WR_ARRAY(pt, 64);
			byte_left = byte_left - 64;
			pt = pt + 64;
		}
		else
		{
			LV_DRV_DISP_SPI_WR_ARRAY(pt, byte_left);
			byte_left=0;
		}
	}

	return 0;
}

// hard reset of the tft controller
// ----------------------------------------------------------
static void GC9A01_hard_reset( void )
{
    LV_DRV_DISP_SPI_CS(0); // Low to listen to us

    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(50);
    LV_DRV_DISP_RST(0);
    LV_DRV_DELAY_MS(50);
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(50);
}

// Configuration of the tft controller
// ----------------------------------------------------------
static void GC9A01_run_cfg_script(void)
{
	int i = 0;
	int end_script = 0;

	do {
		switch (GC9A01_cfg_script[i].cmd)
		{
			case GC9A01_START:
				break;
			case GC9A01_CMD:
				GC9A01_command( GC9A01_cfg_script[i].data & 0xFF );
				break;
			case GC9A01_DATA:
				GC9A01_data( GC9A01_cfg_script[i].data & 0xFF );
				break;
			case GC9A01_DELAY:
        LV_DRV_DELAY_MS(GC9A01_cfg_script[i].data);
				break;
			case GC9A01_END:
				end_script = 1;
		}
		i++;
	} while (!end_script);
}

void GC9A01_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // Rudimentary clipping
  if((x >= GC9A01_HOR_RES) || (y >= GC9A01_VER_RES)) return;
  if((y+h-1) >= GC9A01_VER_RES) h = GC9A01_VER_RES - y;

	LV_DRV_DISP_SPI_CS(0); // Listen to us

  GC9A01_set_addr_win(x, y, x, y + h - 1);

  uint8_t hi = color >> 8, lo = color;
    
  while (h--) {
    GC9A01_data(hi);
    GC9A01_data(lo);
  }

	LV_DRV_DISP_SPI_CS(1);
}

void GC9A01_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Rudimentary clipping
  if((x >= GC9A01_HOR_RES) || (y >= GC9A01_VER_RES)) return;
  if((x+w-1) >= GC9A01_HOR_RES)  w = GC9A01_HOR_RES - x;

	LV_DRV_DISP_SPI_CS(0); // Listen to us

  GC9A01_set_addr_win(x, y, x + w - 1, y);

  uint8_t hi = color >> 8, lo = color;

  while (w--) {
    GC9A01_data(hi);
    GC9A01_data(lo);
  }

	LV_DRV_DISP_SPI_CS(1);
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t GC9A01_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void GC9A01_invertDisplay(bool i)
{
  GC9A01_command(i ? GC9A01_INVON : GC9A01_INVOFF);
}

void GC9A01_drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) ||(x >= GC9A01_HOR_RES) || (y < 0) || (y >= GC9A01_VER_RES)) return;

	LV_DRV_DISP_SPI_CS(0); // Listen to us

  GC9A01_set_addr_win(x, y, x + 1, y + 1);

  GC9A01_data(color >> 8);
  GC9A01_data(color);

	LV_DRV_DISP_SPI_CS(1);
}

void GC9A01_fillScreen(uint16_t color) {
	GC9A01_fillRect(0, 0,  GC9A01_HOR_RES, GC9A01_VER_RES, color);
}

// fill a rectangle
void GC9A01_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= GC9A01_HOR_RES) || (y >= GC9A01_VER_RES)) return;
  if((x + w - 1) >= GC9A01_HOR_RES)  w = GC9A01_HOR_RES  - x;
  if((y + h - 1) >= GC9A01_VER_RES) h = GC9A01_VER_RES - y;

  LV_DRV_DISP_SPI_CS(0);  // Listen to us

  GC9A01_set_addr_win(x, y, x + w - 1, y + h - 1);

	uint8_t hi = color >> 8, lo = color;
    
  for (y = h; y > 0; y--)
  {
    for (x = w; x > 0; x--)
    {
      // LV_DRV_DISP_SPI_WR_ARRAY((const char *)&color, 2);
      GC9A01_data(hi);
      GC9A01_data(lo);
    }
  }
  LV_DRV_DISP_SPI_CS(1);
}

void GC9A01_setRotation(uint8_t m) {

  GC9A01_command(GC9A01_MADCTL);
  m %= 4; // can't be higher than 3
  switch (m) {
   case 0:
     GC9A01_data(GC9A01_MADCTL_MX | GC9A01_MADCTL_MY | GC9A01_MADCTL_RGB);

    //  _xstart = _colstart;
    //  _ystart = _rowstart;
     break;
   case 1:
     GC9A01_data(GC9A01_MADCTL_MY | GC9A01_MADCTL_MV | GC9A01_MADCTL_RGB);

    //  _ystart = _colstart;
    //  _xstart = _rowstart;
     break;
  case 2:
     GC9A01_data(GC9A01_MADCTL_RGB);
 
    //  _xstart = _colstart;
    //  _ystart = _rowstart;
     break;

   case 3:
     GC9A01_data(GC9A01_MADCTL_MX | GC9A01_MADCTL_MV | GC9A01_MADCTL_RGB);

    //  _ystart = _colstart;
    //  _xstart = _rowstart;
     break;
  }
}

/**
 * Initialize the GC9A01
 */
int GC9A01_init(void)
{
  LV_DRV_DISP_SPI_FREQ(GC9A01_SPI_BAUD);
  LV_DRV_DISP_SPI_MODE(GC9A01_SPI_BITS, GC9A01_SPI_MODE);

	GC9A01_hard_reset();

	GC9A01_run_cfg_script();

	GC9A01_fillScreen(0x0000); // BLACK

	return 0;
}

static void GC9A01_set_addr_win(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  uint16_t x_start = x0 + GC9A01_XSTART, x_end = x1 + GC9A01_XSTART;
  uint16_t y_start = y0 + GC9A01_YSTART, y_end = y1 + GC9A01_YSTART;

  GC9A01_command(GC9A01_CASET); // Column addr set
  GC9A01_data(x_start >> 8);
  GC9A01_data(x_start & 0xFF);     // XSTART 
  GC9A01_data(x_end >> 8);
  GC9A01_data(x_end & 0xFF);     // XEND

  GC9A01_command(GC9A01_RASET); // Row addr set
  GC9A01_data(y_start >> 8);
  GC9A01_data(y_start & 0xFF);     // YSTART
  GC9A01_data(y_end >> 8);
  GC9A01_data(y_end & 0xFF);     // YEND

  GC9A01_command(GC9A01_RAMWR);
}

void GC9A01_flush(struct _disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t *color_p)
{
  LV_DRV_DISP_SPI_CS(0); // Listen to us

  GC9A01_set_addr_win(area->x1, area->y1, area->x2, area->y2);
  int32_t len = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * 2;
  LV_DRV_DISP_SPI_WR_ARRAY((uint8_t *)color_p, len);

  LV_DRV_DISP_SPI_CS(1);

  // int32_t x, y;
  // for (y = area->y1; y <= area->y2; y++) {
  //     for (x = area->x1; x <= area->x2; x++) {
  //         GC9A01_drawPixel(x, y, color_p->full);  /* Put a pixel to the display.*/
  //         color_p++;
  //     }
  // }

  lv_disp_flush_ready(disp_drv);         /* Indicate you are ready with the flushing*/
}

// void GC9A01_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
// {
//      /*Return if the area is out the screen*/
//     if(x2 < 0) return;
//     if(y2 < 0) return;
//     if(x1 > GC9A01_HOR_RES - 1) return;
//     if(y1 > GC9A01_VER_RES - 1) return;

//     /*Truncate the area to the screen*/
//     int32_t act_x1 = x1 < 0 ? 0 : x1;
//     int32_t act_y1 = y1 < 0 ? 0 : y1;
//     int32_t act_x2 = x2 > GC9A01_HOR_RES - 1 ? GC9A01_HOR_RES - 1 : x2;
//     int32_t act_y2 = y2 > GC9A01_VER_RES - 1 ? GC9A01_VER_RES - 1 : y2;

//     GC9A01_set_addr_win(act_x1, act_x2, act_y1, act_y2);
//     /*Set the first row in */

//     //uint16_t color16;
//     uint32_t size = (act_x2 - act_x1 + 1) * (act_y2 - act_y1 + 1);
//     uint32_t i;

// // // #if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 1
// // //  	memcpy((uint8_t *)lcd_fb,(uint8_t *)color_p,size*sizeof(lv_color_t));
// // // #else
// // // #error "LV_COLOR_DEPTH not supported"
// // // #endif

//     GC9A01_command(GC9A01_RAMWR);

//     GC9A01_databuf((size * 2), (uint8_t *)lcd_fb );

//     lv_flush_ready();
// }

#ifdef notdef

void GC9A01_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
     /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > GC9A01_HOR_RES - 1) return;
    if(y1 > GC9A01_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > GC9A01_HOR_RES - 1 ? GC9A01_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > GC9A01_VER_RES - 1 ? GC9A01_VER_RES - 1 : y2;

    int32_t x, y;
    uint8_t white = lv_color_to1(color);

    /*Refresh frame buffer*/
    for(y= act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if (white != 0) {
                lcd_fb[x+ (y/8)*GC9A01_HOR_RES] |= (1 << (7-(y%8)));
            } else {
                lcd_fb[x+ (y/8)*GC9A01_HOR_RES] &= ~( 1 << (7-(y%8)));
            }
        }
    }

    GC9A01_sync(act_x1, act_y1, act_x2, act_y2);
}

void GC9A01_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t * color_p)
{
     /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > GC9A01_HOR_RES - 1) return;
    if(y1 > GC9A01_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > GC9A01_HOR_RES - 1 ? GC9A01_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > GC9A01_VER_RES - 1 ? GC9A01_VER_RES - 1 : y2;

    int32_t x, y;


    //Set the rectangular area
     ssd1963_cmd(0x002A);
     ssd1963_data(act_x1 >> 8);
     ssd1963_data(0x00FF & act_x1);
     ssd1963_data(act_x2 >> 8);
     ssd1963_data(0x00FF & act_x2);

     ssd1963_cmd(0x002B);
     ssd1963_data(act_y1 >> 8);
     ssd1963_data(0x00FF & act_y1);
     ssd1963_data(act_y2 >> 8);
     ssd1963_data(0x00FF & act_y2);

    /*Set the first row in */

    /*Refresh frame buffer*/
    for(y= act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if (lv_color_to1(*color_p) != 0) {
                lcd_fb[x+ (y/8)*GC9A01_HOR_RES] &= ~( 1 << (7-(y%8)));
            } else {
                lcd_fb[x+ (y/8)*GC9A01_HOR_RES] |= (1 << (7-(y%8)));
            }
            color_p ++;
        }

        color_p += x2 - act_x2; /*Next row*/
    }

    GC9A01_sync(act_x1, act_y1, act_x2, act_y2);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
 * Flush a specific part of the buffer to the display
 * @param x1 left coordinate of the area to flush
 * @param y1 top coordinate of the area to flush
 * @param x2 right coordinate of the area to flush
 * @param y2 bottom coordinate of the area to flush
 */
static void GC9A01_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	GC9A01_command( ST7735_RAMWR );

	GC9A01_databuf( GC9A01_FB_SIZE*2, (uint8_t *)lcd_fb );
}
#endif
#endif