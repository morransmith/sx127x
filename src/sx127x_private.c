/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 11:52:39
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 20:04:23
 * @ Description:
 */

#include <sx127x_io.h>
#include <sx127x_private.h>
#include <sx127x_registers.h>

uint8_t sx127x_set_fifo_pointer(sx127x_dev_t* dev, uint8_t pointer)
{
    sx127x_write_register(dev->spi, RegFifoAddrPtr, pointer);
    return 0;
}

uint8_t sx127x_set_fifo_tx_pointer(sx127x_dev_t* dev, uint8_t pointer)
{
    sx127x_write_register(dev->spi, RegFifoTxBaseAddr, pointer);
    return 0;
}

uint8_t sx127x_set_fifo_rx_pointer(sx127x_dev_t* dev, uint8_t pointer)
{
    sx127x_write_register(dev->spi, RegFifoRxBaseAddr, pointer);
    return 0;
}

uint8_t sx127x_set_payload_size(sx127x_dev_t* dev, uint8_t size)
{
    sx127x_write_register(dev->spi, RegPayloadLength, size);
    return 0;
}

uint8_t sx127x_write_fifo(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
    if (!size) // Размер не может быть 0
        return -1;

    if (sx127x_set_fifo_tx_pointer(dev, 0) != 0) // Установка указателя TX FIFO
        return -1;

    if (sx127x_set_fifo_pointer(dev, 0) != 0) // Установка текущего указателя FIFO
        return -1;

    if (sx127x_set_payload_size(dev, 5) != 0) // Установка размера пакета данных
        return -1;

    return sx127x_write_burst(dev->spi, RegFifo, buffer, size); // Загрузка буфера в FIFO
}

uint8_t sx127x_read_fifo(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
    uint8_t pointer = sx127x_get_fifo_rx_current_pointer(dev);

    if (sx127x_set_fifo_pointer(dev, pointer) != 0) // Установка текущего указателя FIFO
        return -1;

    return sx127x_read_burst(dev->spi, RegFifo, buffer, size);
}

uint8_t sx127x_set_tx(sx127x_dev_t* dev)
{
    uint8_t reg = sx127x_read_register(dev->spi, RegOpMode);
    sx127x_write_register(dev->spi, RegOpMode, (reg & 0xF8) | MODE_TX);
    dev->settings.mode = MODE_TX;
    return 0;
}

uint8_t sx127x_set_rx_single(sx127x_dev_t* dev)
{
    uint8_t reg = sx127x_read_register(dev->spi, RegOpMode);
    sx127x_write_register(dev->spi, RegOpMode, (reg & 0xF8) | MODE_RXSINGLE);
    dev->settings.mode = MODE_RXSINGLE;
    return 0;
}

uint8_t sx127x_set_rx_continuos(sx127x_dev_t* dev)
{
    uint8_t reg = sx127x_read_register(dev->spi, RegOpMode);
    sx127x_write_register(dev->spi, RegOpMode, (reg & 0xF8) | MODE_RXCONTINUOUS);
    dev->settings.mode = MODE_RXCONTINUOUS;
    return 0;
}

uint8_t sx127x_clear_irq_flags(sx127x_dev_t* dev, uint8_t flags)
{
    sx127x_write_register(dev->spi, RegIrqFlags, flags);
    return 0;
}

uint8_t sx127x_set_irq_flags_mask(sx127x_dev_t* dev, uint8_t mask)
{
    sx127x_write_register(dev->spi, RegIrqFlagsMask, mask ^ 0xff);
    return 0;
}

uint8_t sx127x_set_dio_config(sx127x_dev_t* dev, uint16_t config)
{
    uint8_t data[2] = { (uint8_t)(config >> 8), (uint8_t)config };
    sx127x_write_burst(dev->spi, RegDioMapping1, data, 2);
    return 0;
}

uint8_t sx127x_get_fifo_pointer(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegFifoAddrPtr);
}

uint8_t sx127x_get_fifo_tx_pointer(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegFifoTxBaseAddr);
}

uint8_t sx127x_get_fifo_rx_pointer(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegFifoRxBaseAddr);
}

uint8_t sx127x_get_fifo_rx_current_pointer(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegFifoRxCurrentAddr);
}

uint8_t sx127x_get_last_packet_size(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegRxNbBytes);
}

uint8_t sx127x_get_irq_flags(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegIrqFlags);
}

uint8_t sx127x_get_irq_flags_mask(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegIrqFlagsMask);
}

uint16_t sx127x_get_dio_config(sx127x_dev_t* dev)
{
    uint8_t buffer[2];
    sx127x_read_burst(dev->spi, RegDioMapping1, buffer, 2);
    return (((uint16_t)(buffer[0]) << 8) | ((uint16_t)(buffer[1]) & 0xff)) & 0xfff0; // regdiomapping1 & regdiomapping2[4:7]
}

uint8_t sx127x_get_modem_status(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegModemStat);
}