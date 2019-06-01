/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 11:52:39
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 12:35:35
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

    if (sx127x_set_payload_size(dev, 0) != 0) // Установка размера пакета данных
        return -1;

    return sx127x_write_burst(dev->spi, RegFifo, buffer, size); // Загрузка буфера в FIFO
}

uint8_t sx127x_read_fifo(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{

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