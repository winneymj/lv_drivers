/**
  * @file ST7789.c
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
#include "ST7789.h"
#if USE_ST7789

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/
#define ST7789_CMD_MODE     0
#define ST7789_DATA_MODE    1

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
//static void st7789_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
static void st7789_command(uint8_t cmd);
static void st7789_data(uint8_t data);
static void st7789_set_addr_win(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);


/**********************
 *  STATIC VARIABLES
 **********************/

// static uint16_t lcd_fb[ST7789_FB_SIZE];
//static uint8_t pagemap[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

// static struct st7789_function st7789_cfg_script[] = {
// 	{ ST7789_START, ST7789_START},
// 	{ ST7789_CMD, ST7789_SWRESET},
// 	{ ST7789_DELAY, 150},
// 	{ ST7789_CMD, ST7789_SLPOUT},
// 	{ ST7789_DELAY, 500},
// 	{ ST7789_CMD, ST7789_INVOFF},
// 	{ ST7789_CMD, ST7789_MADCTL},
// 	{ ST7789_DATA, 0xA0},
// 	{ ST7789_CMD, ST7789_COLMOD},
// 	{ ST7789_DATA, 0x05},
// 	{ ST7789_CMD, ST7789_PORCTRL},
// 	{ ST7789_DATA, 0x0C},
// 	{ ST7789_DATA, 0x0C},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x33},
// 	{ ST7789_DATA, 0x33},
// 	{ ST7789_CMD, ST7789_GCTRL},
// 	{ ST7789_DATA, 0x35},
// 	{ ST7789_CMD, ST7789_VDVVRHEN},
// 	{ ST7789_DATA, 0x01},
// 	{ ST7789_DATA, 0xFF},
// 	{ ST7789_CMD, ST7789_VRHS},
// 	{ ST7789_DATA, 0x17},
// 	{ ST7789_CMD, ST7789_VDVSET},
// 	{ ST7789_DATA, 0x20},
// 	{ ST7789_CMD, ST7789_VCOMS},
// 	{ ST7789_DATA, 0x17},
// 	{ ST7789_CMD, ST7789_VCMOFSET},
// 	{ ST7789_DATA, 0x20},
// 	{ ST7789_CMD, ST7789_PWCTRL1},
// 	{ ST7789_DATA, 0xA4},
// 	{ ST7789_DATA, 0xA1},
// 	{ ST7789_CMD, ST7789_PVGAMCTRL},
// 	{ ST7789_DATA, 0xD0},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x14},
// 	{ ST7789_DATA, 0x15},
// 	{ ST7789_DATA, 0x13},
// 	{ ST7789_DATA, 0x2C},
// 	{ ST7789_DATA, 0x42},
// 	{ ST7789_DATA, 0x43},
// 	{ ST7789_DATA, 0x4E},
// 	{ ST7789_DATA, 0x09},
// 	{ ST7789_DATA, 0x16},
// 	{ ST7789_DATA, 0x14},
// 	{ ST7789_DATA, 0x18},
// 	{ ST7789_DATA, 0x21},
// 	{ ST7789_CMD, ST7789_NVGAMCTRL},
// 	{ ST7789_DATA, 0xD0},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x14},
// 	{ ST7789_DATA, 0x15},
// 	{ ST7789_DATA, 0x13},
// 	{ ST7789_DATA, 0x0B},
// 	{ ST7789_DATA, 0x43},
// 	{ ST7789_DATA, 0x55},
// 	{ ST7789_DATA, 0x53},
// 	{ ST7789_DATA, 0x0C},
// 	{ ST7789_DATA, 0x17},
// 	{ ST7789_DATA, 0x14},
// 	{ ST7789_DATA, 0x23},
// 	{ ST7789_DATA, 0x20},
// 	{ ST7789_CMD, ST7789_CASET},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x01},
// 	{ ST7789_DATA, 0x3F},
// 	{ ST7789_CMD, ST7789_RASET},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0x00},
// 	{ ST7789_DATA, 0xEF},
// 	{ ST7789_CMD, ST7789_DISPON},
// 	{ ST7789_DELAY, 100},
// 	{ ST7789_END, ST7789_END},
// };

static struct st7789_function st7789_cfg_script[] = {
	{ ST7789_START, ST7789_START},
	{ ST7789_CMD, ST7789_SWRESET},
	{ ST7789_DELAY, 150},
	{ ST7789_CMD, ST7789_SLPOUT},
	{ ST7789_DELAY, 255},
	{ ST7789_CMD, ST7789_COLMOD},
	{ ST7789_DATA, 0x55},
	{ ST7789_DELAY, 10},
	{ ST7789_CMD, ST7789_MADCTL},
	{ ST7789_DATA, 0x00},
	{ ST7789_CMD, ST7789_CASET},
	{ ST7789_DATA, 0x00},
	{ ST7789_DATA, ST7789_XSTART},
	{ ST7789_DATA, (ST7789_HOR_RES + ST7789_XSTART) >> 8},
	{ ST7789_DATA, (ST7789_HOR_RES + ST7789_XSTART) & 0xff},
	{ ST7789_CMD, ST7789_RASET},
	{ ST7789_DATA, 0x00},
	{ ST7789_DATA, ST7789_YSTART},
	{ ST7789_DATA, (ST7789_VER_RES + ST7789_YSTART) >> 8},
	{ ST7789_DATA, (ST7789_VER_RES + ST7789_YSTART) & 0xff},
	{ ST7789_CMD, ST7789_INVON},
	{ ST7789_DELAY, 10},
	{ ST7789_CMD, ST7789_NORON},
	{ ST7789_DELAY, 10},
	{ ST7789_CMD, ST7789_DISPON},
	{ ST7789_DELAY, 255},
	{ ST7789_END, ST7789_END},
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Write a command to the ST7789
 * @param cmd the command
 */
static void st7789_command(uint8_t cmd)
{
    LV_DRV_DISP_CMD_DATA(ST7789_CMD_MODE);
    LV_DRV_DISP_SPI_WR_BYTE(cmd);
}

/**
 * Write data to the ST7789
 * @param data the data
 */
static void st7789_data(uint8_t data)
{
    LV_DRV_DISP_CMD_DATA(ST7789_DATA_MODE);
    LV_DRV_DISP_SPI_WR_BYTE(data);
}

static int st7789_data_array(uint8_t *buf, uint32_t len)
{
	uint8_t *pt = buf;

	for (uint32_t lp = 0; lp < len; lp++, pt++)
	{
		LV_DRV_DISP_SPI_WR_BYTE(*pt);
	} 
	return 0;
}

static int st7789_databuf(uint32_t len, uint8_t *buf)
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
static void st7789_hard_reset( void )
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
static void st7789_run_cfg_script(void)
{
	int i = 0;
	int end_script = 0;

	do {
		switch (st7789_cfg_script[i].cmd)
		{
			case ST7789_START:
				break;
			case ST7789_CMD:
				st7789_command( st7789_cfg_script[i].data & 0xFF );
				break;
			case ST7789_DATA:
				st7789_data( st7789_cfg_script[i].data & 0xFF );
				break;
			case ST7789_DELAY:
        LV_DRV_DELAY_MS(st7789_cfg_script[i].data);
				break;
			case ST7789_END:
				end_script = 1;
		}
		i++;
	} while (!end_script);
}

void st7789_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // Rudimentary clipping
  if((x >= ST7789_HOR_RES) || (y >= ST7789_VER_RES)) return;
  if((y+h-1) >= ST7789_VER_RES) h = ST7789_VER_RES - y;

	LV_DRV_DISP_SPI_CS(0); // Listen to us

  st7789_set_addr_win(x, y, x, y + h - 1);

  uint8_t hi = color >> 8, lo = color;
    
  while (h--) {
    st7789_data(hi);
    st7789_data(lo);
  }

	LV_DRV_DISP_SPI_CS(1);
}

void st7789_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Rudimentary clipping
  if((x >= ST7789_HOR_RES) || (y >= ST7789_VER_RES)) return;
  if((x+w-1) >= ST7789_HOR_RES)  w = ST7789_HOR_RES - x;

	LV_DRV_DISP_SPI_CS(0); // Listen to us

  st7789_set_addr_win(x, y, x + w - 1, y);

  uint8_t hi = color >> 8, lo = color;

  while (w--) {
    st7789_data(hi);
    st7789_data(lo);
  }

	LV_DRV_DISP_SPI_CS(1);
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t st7789_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void st7789_invertDisplay(bool i)
{
  st7789_command(i ? ST7789_INVON : ST7789_INVOFF);
}

void st7789_drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) ||(x >= ST7789_HOR_RES) || (y < 0) || (y >= ST7789_VER_RES)) return;

	LV_DRV_DISP_SPI_CS(0); // Listen to us

  st7789_set_addr_win(x, y, x + 1, y + 1);

  st7789_data(color >> 8);
  st7789_data(color);

	LV_DRV_DISP_SPI_CS(1);
}

void st7789_fillScreen(uint16_t color) {
	st7789_fillRect(0, 0,  ST7789_HOR_RES, ST7789_VER_RES, color);
}

// fill a rectangle
void st7789_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= ST7789_HOR_RES) || (y >= ST7789_VER_RES)) return;
  if((x + w - 1) >= ST7789_HOR_RES)  w = ST7789_HOR_RES  - x;
  if((y + h - 1) >= ST7789_VER_RES) h = ST7789_VER_RES - y;

  LV_DRV_DISP_SPI_CS(0);  // Listen to us

  st7789_set_addr_win(x, y, x + w - 1, y + h - 1);

	uint8_t hi = color >> 8, lo = color;
    
  for (y = h; y > 0; y--)
  {
    for (x = w; x > 0; x--)
    {
      // LV_DRV_DISP_SPI_WR_ARRAY((const char *)&color, 2);
      st7789_data(hi);
      st7789_data(lo);
    }
  }
  LV_DRV_DISP_SPI_CS(1);
}

void st7789_setRotation(uint8_t m) {

  st7789_command(ST7789_MADCTL);
  m %= 4; // can't be higher than 3
  switch (m) {
   case 0:
     st7789_data(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);

    //  _xstart = _colstart;
    //  _ystart = _rowstart;
     break;
   case 1:
     st7789_data(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);

    //  _ystart = _colstart;
    //  _xstart = _rowstart;
     break;
  case 2:
     st7789_data(ST7789_MADCTL_RGB);
 
    //  _xstart = _colstart;
    //  _ystart = _rowstart;
     break;

   case 3:
     st7789_data(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);

    //  _ystart = _colstart;
    //  _xstart = _rowstart;
     break;
  }
}

/**
 * Initialize the ST7789
 */
int st7789_init(void)
{
  LV_DRV_DISP_SPI_FREQ(ST7789_SPI_BAUD);
  LV_DRV_DISP_SPI_MODE(ST7789_SPI_BITS, ST7789_SPI_MODE);

	st7789_hard_reset();

	st7789_run_cfg_script();

	st7789_fillScreen(0x0000); // BLACK

	return 0;
}

static void st7789_set_addr_win(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  uint16_t x_start = x0 + ST7789_XSTART, x_end = x1 + ST7789_XSTART;
  uint16_t y_start = y0 + ST7789_YSTART, y_end = y1 + ST7789_YSTART;

  st7789_command(ST7789_CASET); // Column addr set
  st7789_data(x_start >> 8);
  st7789_data(x_start & 0xFF);     // XSTART 
  st7789_data(x_end >> 8);
  st7789_data(x_end & 0xFF);     // XEND

  st7789_command(ST7789_RASET); // Row addr set
  st7789_data(y_start >> 8);
  st7789_data(y_start & 0xFF);     // YSTART
  st7789_data(y_end >> 8);
  st7789_data(y_end & 0xFF);     // YEND

  st7789_command(ST7789_RAMWR);
}

void st7789_flush(struct _disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t *color_p)
{
  LV_DRV_DISP_SPI_CS(0); // Listen to us

  st7789_set_addr_win(area->x1, area->y1, area->x2, area->y2);
  int32_t len = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * 2;
  LV_DRV_DISP_SPI_WR_ARRAY((uint8_t *)color_p, len);

  LV_DRV_DISP_SPI_CS(1);

  // int32_t x, y;
  // for (y = area->y1; y <= area->y2; y++) {
  //     for (x = area->x1; x <= area->x2; x++) {
  //         st7789_drawPixel(x, y, color_p->full);  /* Put a pixel to the display.*/
  //         color_p++;
  //     }
  // }

  lv_disp_flush_ready(disp_drv);         /* Indicate you are ready with the flushing*/
}

// void st7789_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
// {
//      /*Return if the area is out the screen*/
//     if(x2 < 0) return;
//     if(y2 < 0) return;
//     if(x1 > ST7789_HOR_RES - 1) return;
//     if(y1 > ST7789_VER_RES - 1) return;

//     /*Truncate the area to the screen*/
//     int32_t act_x1 = x1 < 0 ? 0 : x1;
//     int32_t act_y1 = y1 < 0 ? 0 : y1;
//     int32_t act_x2 = x2 > ST7789_HOR_RES - 1 ? ST7789_HOR_RES - 1 : x2;
//     int32_t act_y2 = y2 > ST7789_VER_RES - 1 ? ST7789_VER_RES - 1 : y2;

//     st7789_set_addr_win(act_x1, act_x2, act_y1, act_y2);
//     /*Set the first row in */

//     //uint16_t color16;
//     uint32_t size = (act_x2 - act_x1 + 1) * (act_y2 - act_y1 + 1);
//     uint32_t i;

// // // #if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 1
// // //  	memcpy((uint8_t *)lcd_fb,(uint8_t *)color_p,size*sizeof(lv_color_t));
// // // #else
// // // #error "LV_COLOR_DEPTH not supported"
// // // #endif

//     st7789_command(ST7789_RAMWR);

//     st7789_databuf((size * 2), (uint8_t *)lcd_fb );

//     lv_flush_ready();
// }

#ifdef notdef

void st7789_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
     /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > ST7789_HOR_RES - 1) return;
    if(y1 > ST7789_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > ST7789_HOR_RES - 1 ? ST7789_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > ST7789_VER_RES - 1 ? ST7789_VER_RES - 1 : y2;

    int32_t x, y;
    uint8_t white = lv_color_to1(color);

    /*Refresh frame buffer*/
    for(y= act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if (white != 0) {
                lcd_fb[x+ (y/8)*ST7789_HOR_RES] |= (1 << (7-(y%8)));
            } else {
                lcd_fb[x+ (y/8)*ST7789_HOR_RES] &= ~( 1 << (7-(y%8)));
            }
        }
    }

    st7789_sync(act_x1, act_y1, act_x2, act_y2);
}

void st7789_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t * color_p)
{
     /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > ST7789_HOR_RES - 1) return;
    if(y1 > ST7789_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > ST7789_HOR_RES - 1 ? ST7789_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > ST7789_VER_RES - 1 ? ST7789_VER_RES - 1 : y2;

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
                lcd_fb[x+ (y/8)*ST7789_HOR_RES] &= ~( 1 << (7-(y%8)));
            } else {
                lcd_fb[x+ (y/8)*ST7789_HOR_RES] |= (1 << (7-(y%8)));
            }
            color_p ++;
        }

        color_p += x2 - act_x2; /*Next row*/
    }

    st7789_sync(act_x1, act_y1, act_x2, act_y2);
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
static void st7789_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	st7789_command( ST7735_RAMWR );

	st7789_databuf( ST7789_FB_SIZE*2, (uint8_t *)lcd_fb );
}
#endif
#endif