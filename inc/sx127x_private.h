/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 11:51:19
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 19:52:07
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

uint8_t sx127x_get_fifo_pointer(sx127x_dev_t* dev);
uint8_t sx127x_get_fifo_tx_pointer(sx127x_dev_t* dev);
uint8_t sx127x_get_fifo_rx_pointer(sx127x_dev_t* dev);
uint8_t sx127x_get_fifo_rx_current_pointer(sx127x_dev_t* dev);

uint8_t sx127x_get_last_packet_size(sx127x_dev_t* dev);

uint8_t sx127x_get_irq_flags(sx127x_dev_t* dev);
uint8_t sx127x_get_irq_flags_mask(sx127x_dev_t* dev);

uint16_t sx127x_get_dio_config(sx127x_dev_t* dev);

uint8_t sx127x_get_modem_status(sx127x_dev_t* dev);

#endif /* SX127X_PRIVATE_H */
