/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:51:04
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 16:16:38
 * @ Description:
 */

#ifndef SX127X_H
#define SX127X_H

#include "sx127x_platform.h"

#include <stdint.h>

typedef void (*sx127x_rx_callback_fn_t)(uint8_t*, uint8_t);
typedef void (*sx127x_callback_fn_t)(void);

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

typedef enum device_mode {
    MODE_SLEEP = 0,
    MODE_STDBY = 1,
    MODE_FSTX = 2,
    MODE_TX = 3,
    MODE_FSRX = 4,
    MODE_RXCONTINUOUS = 5,
    MODE_RXSINGLE = 6,
    MODE_CAD = 7
} device_mode_t;

typedef struct sx127x_callbacks {
    sx127x_callback_fn_t tx_done;
    sx127x_rx_callback_fn_t rx_done;
    sx127x_callback_fn_t rx_timeout;
    sx127x_callback_fn_t rx_crc_error;
} sx127x_callbacks_t;

typedef struct sx127x_radio_settings {
    device_mode_t mode;
    spreading_factor_t spreading_factor;
} sx127x_radio_settings_t;

typedef struct dev {
    spi_t* spi;
    sx127x_radio_settings_t settings;
    sx127x_callbacks_t* callbacks;
} sx127x_dev_t;

uint8_t sx127x_alloc(sx127x_dev_t** dev, spi_t* spi, sx127x_callbacks_t* callbacks);
uint8_t sx127x_free(sx127x_dev_t** dev);

void sx127x_dio_0_callback(sx127x_dev_t* dev);
void sx127x_dio_1_callback(sx127x_dev_t* dev);
void sx127x_dio_2_callback(sx127x_dev_t* dev);
void sx127x_dio_3_callback(sx127x_dev_t* dev);
void sx127x_dio_4_callback(sx127x_dev_t* dev);
void sx127x_dio_5_callback(sx127x_dev_t* dev);

uint8_t sx127x_init(sx127x_dev_t* dev);

uint8_t sx127x_get_version(sx127x_dev_t* dev);

uint8_t sx127x_set_sleep(sx127x_dev_t* dev);
uint8_t sx127x_set_standby(sx127x_dev_t* dev);

uint8_t sx127x_transmit(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size, uint32_t delay);
uint8_t sx127x_transmit_it(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size);

uint8_t sx127x_receive_single(sx127x_dev_t* dev, uint8_t* buffer, uint8_t* size);
uint8_t sx127x_receive_single_it(sx127x_dev_t* dev);

uint8_t sx127x_receive_continuous(sx127x_dev_t* dev, uint8_t* buffer, uint8_t* size);
uint8_t sx127x_receive_continuous_it(sx127x_dev_t* dev);

#endif /* SX127X_H */
