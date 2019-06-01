/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:51:04
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 11:39:08
 * @ Description:
 */

#ifndef SX127X_H
#define SX127X_H

#include "sx127x_platform.h"

#include <stdint.h>

typedef enum spreading_factor {
    SF_7 = 7,
    SF_8 = 8,
    SF_9 = 9,
    SF_10 = 10,
    SF_11 = 11,
    SF_12 = 12
} spreading_factor_t;

typedef enum band_width {
    BW_125_KHz = 7,
    BW_250_KHz = 8,
    BW_500_KHz = 9
} band_width_t;

typedef enum coding_rate {
    CR_4_5 = 1,
    CR_4_6 = 2,
    CR_4_7 = 3,
    CR_4_8 = 4
} coding_rate_t;

typedef struct sx127x_radio_settings {
    spreading_factor_t spreading_factor;
} sx127x_radio_settings_t;

typedef struct dev {
    spi_t* spi;
    sx127x_radio_settings_t settings;

} sx127x_dev_t;

uint8_t sx127x_alloc(sx127x_dev_t** dev, spi_t* spi);
uint8_t sx127x_free(sx127x_dev_t** dev);

uint8_t sx127x_init(sx127x_dev_t* dev);

uint8_t sx127x_get_version(sx127x_dev_t* dev);

#endif /* SX127X_H */
