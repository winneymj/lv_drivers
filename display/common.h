#ifndef __COMMON_H__
#define __COMMON_H__

#include <mbed.h>

// /* GLOBAL PINSSSSSS */
DigitalOut cmd_data(P0_6);
DigitalOut reset(P0_7);
DigitalOut spi_cs(P0_8);
SPI spi(SPI_PSELMOSI0, NC, SPI_PSELSCK0, NC);
// // SPI spi(PB_5, PB_4, PB_3); // mosi, miso, sclk

// /* FNC PROTOTYPES */
void pin_rst_set(int); // set reset pin to a value
void pin_cmd_set(int); // set cmd dta pin to a value
void spi_cs_set(int); // setting chip select pin to a value
void spi_wr(uint8_t); // write a byte over spi
void spi_wr_mem(uint8_t *, uint32_t);
// void spi_wr_mem(uint32_t, uint32_t); // not correct 
void spi_set_freq(int); // set baudrate
void spi_mode(uint8_t, int8_t);

// /* FNC DEFINITIONS */
void pin_rst_set(int val) {
	reset = val;
}

void pin_cmd_set(int val) {
	cmd_data = val;
}

void spi_cs_set(int val) {
	spi_cs = val;
}

// write a single byte but mbed SPI write doesnt have one for writing
// a single byte so we just write it as a 32 bits integer
void spi_wr(uint8_t data)
{
	spi.write(data);
}

void spi_wr_mem(const char *addr, uint32_t len)
{
  spi.write(addr, len, NULL, 0);
}

// // @TODO need to check if this logic is correct for writing to an address on the spi bus
// void spi_wr_mem(uint32_t addr, uint32_t data) {
// 	spi_cs = 0;
// 	spi.write(addr); // write to address 1st
// 	spi.write(data); // write them bytes
// 	spi_cs = 1;
// }

void spi_mode(uint8_t bits, int8_t mode)
{
	spi.format(bits, mode);
}

void spi_set_freq(int val)
{
	spi.frequency(val);
}


#endif