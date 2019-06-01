/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 10:35:09
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 10:44:52
 * @ Description:
 */

#ifndef SX127X_PLATFORM_H
#define SX127X_PLATFORM_H

#include <stdint.h>

typedef uint32_t (*spi_transmit_fn)(uint8_t*, uint32_t);
typedef uint32_t (*spi_receive_fn)(uint8_t*, uint32_t);
typedef uint32_t (*spi_transmit_receive_fn)(uint8_t*, uint32_t, uint8_t*, uint32_t);

typedef struct spi {
    spi_transmit_fn transmit;
    spi_receive_fn receive;
    spi_transmit_receive_fn transmit_receive;
} spi_t;

#endif /* SX127X_PLATFORM_H */
