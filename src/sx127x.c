/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:51:05
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 12:40:37
 * @ Description:
 */

#include <sx127x.h>
#include <sx127x_io.h>
#include <sx127x_private.h>
#include <sx127x_registers.h>

#include <stdio.h>
#include <stdlib.h>

uint8_t sx127x_alloc(sx127x_dev_t** dev, spi_t* spi)
{
    *dev = (sx127x_dev_t*)malloc(sizeof(sx127x_dev_t));
    if (!dev)
        return -1;

    (*dev)->spi = spi;

    return 0;
}

uint8_t sx127x_free(sx127x_dev_t** dev)
{
    if (!*dev)
        return -1;

    free(*dev);

    return 0;
}

uint8_t sx127x_init(sx127x_dev_t* dev)
{
    printf("hello from init\n");

    sx127x_get_version(dev);

    return 0;
}

uint8_t sx127x_get_version(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegVersion);
}

uint8_t sx127x_set_sleep(sx127x_dev_t* dev)
{
    uint8_t reg = sx127x_read_register(dev->spi, RegOpMode);
    sx127x_write_register(dev->spi, RegOpMode, (reg & 0xF8) | MODE_SLEEP);
    dev->settings.mode = MODE_SLEEP;
    return 0;
}

uint8_t sx127x_set_standby(sx127x_dev_t* dev)
{
    uint8_t reg = sx127x_read_register(dev->spi, RegOpMode);
    sx127x_write_register(dev->spi, RegOpMode, (reg & 0xF8) | MODE_STDBY);
    dev->settings.mode = MODE_STDBY;
    return 0;
}

uint8_t sx127x_transmit(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
    // TODO: Отключение прерывания DIO TxDone

    if (sx127x_set_standby(dev) != 0) // FIFO не доступен в режиме SLEEP
        return -1;

    if (sx127x_write_fifo(dev, buffer, size) != 0) // Загрузка FIFO
        return -1;

    if (sx127x_set_tx(dev) != 0) // Начало отправки
        return -1;

    // TODO: Проверка статуса отправки

    return 0;
}

uint8_t sx127x_transmit_it(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
    // TODO: Включение прерывания DIO TxDone

    if (sx127x_set_standby(dev) != 0) // FIFO не доступен в режиме SLEEP
        return -1;

    if (sx127x_write_fifo(dev, buffer, size) != 0) // Загрузка FIFO
        return -1;

    if (sx127x_set_tx(dev) != 0) // Начало отправки
        return -1;

    // TODO: Проверка состояния модема
}

uint8_t sx127x_receive_single(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
}

uint8_t sx127x_receive_single_it(sx127x_dev_t* dev)
{
}

uint8_t sx127x_receive_continuous(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
}

uint8_t sx127x_receive_continuous_it(sx127x_dev_t* dev)
{
}
