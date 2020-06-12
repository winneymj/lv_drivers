/**
 * @file GC9A01.h
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

#ifndef GC9A01_H
#define GC9A01_H

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

#if USE_GC9A01

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// #include "../../lv_drv_conf.h"
// #if USE_GC9A01 != 0

// #include <stdint.h>
// #include "lvgl/lv_misc/lv_color.h"

/*********************
 *      DEFINES
 *********************/

/* GC9A01 Commands */
#define GC9A01_NOP		0x0
#define GC9A01_SWRESET	0x01
#define ST7735_RDDID	0x04
#define ST7735_RDDST	0x09
#define ST7735_SLPIN	0x10
#define GC9A01_SLPOUT	0x11
#define ST7735_PTLON	0x12
#define GC9A01_NORON	0x13
#define GC9A01_INVOFF	0x20
#define GC9A01_INVON	0x21
#define ST7735_DISPOFF	0x28
#define GC9A01_DISPON	0x29
#define GC9A01_CASET	0x2A
#define GC9A01_RASET	0x2B
#define GC9A01_RAMWR	0x2C
#define ST7735_RAMRD	0x2E
#define GC9A01_COLMOD	0x3A
#define GC9A01_MADCTL	0x36
#define GC9A01_MADCTL_MY  0x80
#define GC9A01_MADCTL_MX  0x40
#define GC9A01_MADCTL_MV  0x20
#define GC9A01_MADCTL_ML  0x10
#define GC9A01_MADCTL_RGB 0x00
#define ST7735_FRMCTR1	0xB1
#define GC9A01_PORCTRL	0xB2
#define ST7735_FRMCTR3	0xB3
#define ST7735_INVCTR	0xB4
#define ST7735_DISSET5	0xB6
#define GC9A01_GCTRL	0xB7
#define GC9A01_VCOMS	0xBB
#define ST7735_PWCTR1	0xC0
#define ST7735_PWCTR2	0xC1
#define GC9A01_VDVVRHEN	0xC2
#define GC9A01_VRHS		0xC3
#define GC9A01_VDVSET	0xC4
#define GC9A01_VCMOFSET	0xC5
#define GC9A01_PWCTRL1	0xD0
#define ST7735_RDID1	0xDA
#define ST7735_RDID2	0xDB
#define ST7735_RDID3	0xDC
#define ST7735_RDID4	0xDD
#define GC9A01_PVGAMCTRL	0xE0
#define GC9A01_NVGAMCTRL	0xE1
#define ST7735_PWCTR6	0xFC

#define GC9A01_FB_SIZE  (GC9A01_HOR_RES * GC9A01_VER_RES)

/**********************
 *      TYPEDEFS
 **********************/

/* Init script function */
struct GC9A01_function {
	uint16_t cmd;
	uint16_t data;
};

/* Init script commands */
enum GC9A01_cmd {
	GC9A01_START,
	GC9A01_END,
	GC9A01_CMD,
	GC9A01_DATA,
	GC9A01_DELAY
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

int GC9A01_init(void);
void GC9A01_flush(struct _disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void GC9A01_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void GC9A01_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t * color_p);
void GC9A01_setRotation(uint8_t m);
void GC9A01_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GC9A01_fillScreen(uint16_t color);
uint16_t GC9A01_Color565(uint8_t r, uint8_t g, uint8_t b);
void GC9A01_invertDisplay(bool i);
void GC9A01_drawPixel(int16_t x, int16_t y, uint16_t color);
void GC9A01_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void GC9A01_drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);

/**********************
 *      MACROS
 **********************/

#endif /* USE_GC9A01 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GC9A01_H */