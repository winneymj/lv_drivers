/**
 * @file ST7789.h
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

#ifndef ST7789_H
#define ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_ST7789

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// #include "../../lv_drv_conf.h"
// #if USE_ST7789 != 0

// #include <stdint.h>
// #include "lvgl/lv_misc/lv_color.h"

/*********************
 *      DEFINES
 *********************/

/* ST7789 Commands */
#define ST7789_NOP		0x0
#define ST7789_SWRESET	0x01
#define ST7735_RDDID	0x04
#define ST7735_RDDST	0x09
#define ST7735_SLPIN	0x10
#define ST7789_SLPOUT	0x11
#define ST7735_PTLON	0x12
#define ST7789_NORON	0x13
#define ST7789_INVOFF	0x20
#define ST7789_INVON	0x21
#define ST7735_DISPOFF	0x28
#define ST7789_DISPON	0x29
#define ST7789_CASET	0x2A
#define ST7789_RASET	0x2B
#define ST7789_RAMWR	0x2C
#define ST7735_RAMRD	0x2E
#define ST7789_COLMOD	0x3A
#define ST7789_MADCTL	0x36
#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00
#define ST7735_FRMCTR1	0xB1
#define ST7789_PORCTRL	0xB2
#define ST7735_FRMCTR3	0xB3
#define ST7735_INVCTR	0xB4
#define ST7735_DISSET5	0xB6
#define ST7789_GCTRL	0xB7
#define ST7789_VCOMS	0xBB
#define ST7735_PWCTR1	0xC0
#define ST7735_PWCTR2	0xC1
#define ST7789_VDVVRHEN	0xC2
#define ST7789_VRHS		0xC3
#define ST7789_VDVSET	0xC4
#define ST7789_VCMOFSET	0xC5
#define ST7789_PWCTRL1	0xD0
#define ST7735_RDID1	0xDA
#define ST7735_RDID2	0xDB
#define ST7735_RDID3	0xDC
#define ST7735_RDID4	0xDD
#define ST7789_PVGAMCTRL	0xE0
#define ST7789_NVGAMCTRL	0xE1
#define ST7735_PWCTR6	0xFC

#define ST7789_FB_SIZE  (ST7789_HOR_RES * ST7789_VER_RES)

/**********************
 *      TYPEDEFS
 **********************/

/* Init script function */
struct st7789_function {
	uint16_t cmd;
	uint16_t data;
};

/* Init script commands */
enum st7789_cmd {
	ST7789_START,
	ST7789_END,
	ST7789_CMD,
	ST7789_DATA,
	ST7789_DELAY
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

int st7789_init(void);
void st7789_flush(struct _disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void st7789_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void st7789_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t * color_p);
void st7789_setRotation(uint8_t m);
void st7789_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void st7789_fillScreen(uint16_t color);
uint16_t st7789_Color565(uint8_t r, uint8_t g, uint8_t b);
void st7789_invertDisplay(bool i);
void st7789_drawPixel(int16_t x, int16_t y, uint16_t color);






/**********************
 *      MACROS
 **********************/

#endif /* USE_ST7789 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ST7789_H */