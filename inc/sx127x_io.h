/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:59:33
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 11:57:50
 * @ Description:
 */

#ifndef SX127X_IO_H
#define SX127X_IO_H

#include "sx127x_platform.h"

#include <stdint.h>

void sx127x_write_register(sx127x_spi_t* dev, uint8_t reg, uint8_t val);
uint8_t sx127x_read_register(sx127x_spi_t* dev, uint8_t reg);

uint8_t sx127x_write_burst(sx127x_spi_t* dev, uint8_t reg, uint8_t* buffer, uint8_t size);
uint8_t sx127x_read_burst(sx127x_spi_t* dev, uint8_t reg, uint8_t* buffer, uint8_t size);

#endif /* SX127X_IO_H */
