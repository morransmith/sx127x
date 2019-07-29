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

typedef uint32_t (*sx127x_spi_transmit_fn)(uint8_t*, uint32_t);
typedef uint32_t (*sx127x_spi_transmit_receive_fn)(uint8_t*, uint32_t, uint8_t*, uint32_t);
typedef void (*sx127x_delay_fn_t)(uint32_t);
typedef void (*sx127x_reset_control_tn_t)(bool);

typedef struct sx127x_spi {
    sx127x_spi_transmit_fn transmit;
    sx127x_spi_transmit_receive_fn transmit_receive;
} sx127x_spi_t;

typedef struct sx127x_common {
    sx127x_delay_fn_t delay;
    sx127x_reset_control_tn_t reset_control;
} sx127x_common_t;

#endif /* SX127X_PLATFORM_H */
