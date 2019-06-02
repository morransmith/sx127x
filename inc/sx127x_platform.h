/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 10:35:09
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-02 10:07:16
 * @ Description:
 */

#ifndef SX127X_PLATFORM_H
#define SX127X_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t (*spi_transmit_fn)(uint8_t*, uint32_t);
typedef uint32_t (*spi_receive_fn)(uint8_t*, uint32_t);
typedef uint32_t (*spi_transmit_receive_fn)(uint8_t*, uint32_t, uint8_t*, uint32_t);
typedef void (*delay_fn_t)(uint32_t);
typedef void (*reset_control_tn_t)(bool);

typedef struct spi {
    spi_transmit_fn transmit;
    spi_receive_fn receive;
    spi_transmit_receive_fn transmit_receive;
} spi_t;

typedef struct common {
    delay_fn_t delay;
    reset_control_tn_t reset_control;
} common_t;

#endif /* SX127X_PLATFORM_H */
