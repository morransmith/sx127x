/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:51:05
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 16:39:37
 * @ Description:
 */

#include <sx127x.h>
#include <sx127x_io.h>
#include <sx127x_private.h>
#include <sx127x_registers.h>

#include <stdio.h>
#include <stdlib.h>

uint8_t sx127x_alloc(sx127x_dev_t** dev, spi_t* spi, sx127x_callbacks_t* callbacks)
{
    *dev = (sx127x_dev_t*)malloc(sizeof(sx127x_dev_t));
    if (!dev)
        return -1;

    (*dev)->spi = spi;
    (*dev)->callbacks = callbacks;

    return 0;
}

uint8_t sx127x_free(sx127x_dev_t** dev)
{
    if (!*dev)
        return -1;

    free(*dev);

    return 0;
}

void sx127x_dio_0_callback(sx127x_dev_t* dev)
{
    uint8_t irq = sx127x_get_irq_flags(dev);

    switch (dev->settings.mode) {
    case MODE_TX:
        if (!(irq & FlagTxDone))
            break;

        dev->callbacks->tx_done();

        sx127x_clear_irq_flags(dev, FlagTxDone);
        break;
    case MODE_RXCONTINUOUS:
    case MODE_RXSINGLE: {
        if (!(irq & FlagRxDone))
            break;

        uint8_t size = sx127x_get_last_packet_size(dev);
        uint8_t buffer[size];

        sx127x_read_fifo(dev, buffer, size);

        dev->callbacks->rx_done(buffer, size);

        sx127x_clear_irq_flags(dev, FlagRxDone);
    } break;
    default:
        break;
    }
}

void sx127x_dio_1_callback(sx127x_dev_t* dev)
{
    if (dev->settings.mode != MODE_RXSINGLE)
        return;

    if (!(sx127x_get_irq_flags(dev) & FlagRxTimeout))
        return;

    dev->callbacks->rx_timeout();

    sx127x_clear_irq_flags(dev, FlagRxTimeout);
}

void sx127x_dio_2_callback(sx127x_dev_t* dev)
{
}

void sx127x_dio_3_callback(sx127x_dev_t* dev)
{
    if (!(dev->settings.mode == MODE_RXSINGLE || dev->settings.mode == MODE_RXCONTINUOUS))
        return;

    if (!(sx127x_get_irq_flags(dev) & FlagPayloadCrcError))
        return;

    dev->callbacks->rx_crc_error();

    sx127x_clear_irq_flags(dev, FlagPayloadCrcError);
}

void sx127x_dio_4_callback(sx127x_dev_t* dev)
{
}

void sx127x_dio_5_callback(sx127x_dev_t* dev)
{
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

uint8_t sx127x_transmit(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size, uint32_t delay)
{
    // TODO: Отключение прерывания DIO TxDone

    if (sx127x_clear_irq_flags(dev, FlagTxDone) != 0) // На всякий случай очистра прерывания TxDone
        return -1;

    if (sx127x_set_irq_flags_mask(dev, FlagTxDone) != 0) // Включение прерывания по окончании отправки
        return -1;

    if (sx127x_set_standby(dev) != 0) // FIFO не доступен в режиме SLEEP
        return -1;

    if (sx127x_write_fifo(dev, buffer, size) != 0) // Загрузка FIFO
        return -1;

    if (sx127x_set_tx(dev) != 0) // Начало отправки
        return -1;

    while (!(sx127x_get_irq_flags(dev) & FlagTxDone)) {
        // TODO: delay

        if (!delay--)
            return -1;
    }

    return 0;
}

uint8_t sx127x_transmit_it(sx127x_dev_t* dev, uint8_t* buffer, uint8_t size)
{
    // TODO: Включение прерывания DIO TxDone

    if (sx127x_clear_irq_flags(dev, FlagTxDone) != 0) // На всякий случай очистра прерывания TxDone
        return -1;

    if (sx127x_set_irq_flags_mask(dev, FlagTxDone) != 0) // Включение прерывания по окончании отправки
        return -1;

    if (sx127x_set_standby(dev) != 0) // FIFO не доступен в режиме SLEEP
        return -1;

    if (sx127x_write_fifo(dev, buffer, size) != 0) // Загрузка FIFO
        return -1;

    if (sx127x_set_tx(dev) != 0) // Начало отправки
        return -1;

    return 0;
}

uint8_t sx127x_receive_single(sx127x_dev_t* dev, uint8_t* buffer, uint8_t* size)
{
    // TODO: отключение прерываний DIO

    if (sx127x_clear_irq_flags(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // На всякий случай очистра прерывания TxDone
        return -1;

    if (sx127x_set_irq_flags_mask(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // Включение прерывания по окончании отправки
        return -1;

    if (sx127x_set_fifo_rx_pointer(dev, 0) != 0)
        return -1;

    if (sx127x_set_rx_single(dev) != 0)
        return -1;

    while (1) {
        uint8_t irq = sx127x_get_irq_flags(dev);

        if (irq & FlagRxDone) {
            // TODO: check crc valid

            *size = sx127x_get_last_packet_size(dev);
            if (sx127x_read_fifo(dev, buffer, *size) != 0)
                return -1;

            if (sx127x_clear_irq_flags(dev, FlagRxDone) != 0)
                return -1;

            return 0;
        } else if (irq & FlagRxTimeout) {
            if (sx127x_clear_irq_flags(dev, FlagRxTimeout) != 0)
                return -1;

            return 2;
        }
    }
}

uint8_t sx127x_receive_single_it(sx127x_dev_t* dev)
{
    // TODO: включение прерываний DIO

    if (sx127x_clear_irq_flags(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // На всякий случай очистра прерывания TxDone
        return -1;

    if (sx127x_set_irq_flags_mask(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // Включение прерывания по окончании отправки
        return -1;

    if (sx127x_set_fifo_rx_pointer(dev, 0) != 0)
        return -1;

    if (sx127x_set_rx_single(dev) != 0)
        return -1;

    return 0;
}

uint8_t sx127x_receive_continuous(sx127x_dev_t* dev, uint8_t* buffer, uint8_t* size)
{
    if (dev->settings.mode != MODE_RXCONTINUOUS) {

        // TODO: отключение прерываний DIO

        if (sx127x_clear_irq_flags(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // На всякий случай очистра прерывания TxDone
            return -1;

        if (sx127x_set_irq_flags_mask(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // Включение прерывания по окончании отправки
            return -1;

        if (sx127x_set_fifo_rx_pointer(dev, 0) != 0)
            return -1;

        if (sx127x_set_rx_continuos(dev) != 0)
            return -1;
    }

    while (1) {
        uint8_t irq = sx127x_get_irq_flags(dev);

        if (irq & FlagRxDone) {
            // TODO: check crc valid

            *size = sx127x_get_last_packet_size(dev);
            if (sx127x_read_fifo(dev, buffer, *size) != 0)
                return -1;

            if (sx127x_clear_irq_flags(dev, FlagRxDone) != 0)
                return -1;

            return 0;
        } else if (irq & FlagRxTimeout) {
            if (sx127x_clear_irq_flags(dev, FlagRxTimeout) != 0)
                return -1;

            return 2;
        }
    }
}

uint8_t sx127x_receive_continuous_it(sx127x_dev_t* dev)
{
    // TODO: включение прерываний DIO

    if (sx127x_clear_irq_flags(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // На всякий случай очистра прерывания TxDone
        return -1;

    if (sx127x_set_irq_flags_mask(dev, FlagPayloadCrcError | FlagRxDone | FlagRxTimeout) != 0) // Включение прерывания по окончании отправки
        return -1;

    if (sx127x_set_fifo_rx_pointer(dev, 0) != 0)
        return -1;

    if (sx127x_set_rx_continuos(dev) != 0)
        return -1;

    return 0;
}
