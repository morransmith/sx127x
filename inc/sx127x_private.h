/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 11:51:19
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-02 09:33:32
 * @ Description:
 */

#ifndef SX127X_PRIVATE_H
#define SX127X_PRIVATE_H

#include "sx127x.h"

uint8_t sx127x_set_fifo_pointer(sx127x_dev_t* dev, uint8_t pointer);
uint8_t sx127x_set_fifo_tx_pointer(sx127x_dev_t* dev, uint8_t pointer);
uint8_t sx127x_set_fifo_rx_pointer(sx127x_dev_t* dev, uint8_t pointer);

uint8_t sx127x_set_payload_size(sx127x_dev_t* dev, uint8_t size);

uint8_t sx127x_write_fifo(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size);
uint8_t sx127x_read_fifo(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size);

uint8_t sx127x_set_tx(sx127x_dev_t* dev);
uint8_t sx127x_set_rx_single(sx127x_dev_t* dev);
uint8_t sx127x_set_rx_continuos(sx127x_dev_t* dev);

uint8_t sx127x_clear_irq_flags(sx127x_dev_t* dev, uint8_t flags);
uint8_t sx127x_set_irq_flags_mask(sx127x_dev_t* dev, uint8_t mask);

uint8_t sx127x_set_dio_config(sx127x_dev_t* dev, uint16_t config);

uint8_t sx127x_set_modulation_mode(sx127x_dev_t* dev, modulation_t modulation);

uint8_t sx127x_set_pa_select(sx127x_dev_t* dev, pa_select_t pa_select);

uint8_t sx127x_get_fifo_pointer(sx127x_dev_t* dev);
uint8_t sx127x_get_fifo_tx_pointer(sx127x_dev_t* dev);
uint8_t sx127x_get_fifo_rx_pointer(sx127x_dev_t* dev);
uint8_t sx127x_get_fifo_rx_current_pointer(sx127x_dev_t* dev);

uint8_t sx127x_get_last_packet_size(sx127x_dev_t* dev);

uint8_t sx127x_get_irq_flags(sx127x_dev_t* dev);
uint8_t sx127x_get_irq_flags_mask(sx127x_dev_t* dev);

uint16_t sx127x_get_dio_config(sx127x_dev_t* dev);

uint8_t sx127x_get_crc_valid(sx127x_dev_t* dev);

uint8_t sx127x_get_modem_status(sx127x_dev_t* dev);

modulation_t sx127x_get_modulation_mode(sx127x_dev_t* dev);

device_mode_t sx127x_get_mode(sx127x_dev_t* dev);

pa_select_t sx127x_get_pa_select(sx127x_dev_t* dev);

uint8_t sx127x_get_power(sx127x_dev_t* dev);

spreading_factor_t sx127x_get_spreading_factor(sx127x_dev_t* dev);

band_width_t sx127x_get_band_width(sx127x_dev_t* dev);

coding_rate_t sx127x_get_coding_rate(sx127x_dev_t* dev);

bool sx127x_get_payload_crc_on(sx127x_dev_t* dev);

uint16_t sx127x_get_preamble_length(sx127x_dev_t* dev);

uint32_t sx127x_get_frequency(sx127x_dev_t* dev);

uint8_t sx127x_get_sync_word(sx127x_dev_t* dev);

#endif /* SX127X_PRIVATE_H */
