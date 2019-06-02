/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 11:52:39
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-02 21:55:11
 * @ Description:
 */

#include <sx127x_io.h>
#include <sx127x_private.h>
#include <sx127x_registers.h>

static const float F_STEP = (32000000.0 / 524288.0);

uint8_t sx127x_frequency_to_reg(uint32_t frequency, uint8_t* reg); // reg должен быть размером 3 байта
uint32_t sx127x_reg_to_frequency(uint8_t* reg); // reg должен быть размером 3 байта

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

    if (sx127x_set_payload_size(dev, size) != 0) // Установка размера пакета данных
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

    return sx127x_write_burst(dev->spi, RegDioMapping1, data, 2);
}

uint8_t sx127x_set_modulation_mode(sx127x_dev_t* dev, modulation_t modulation)
{
    sx127x_write_register(dev->spi, RegOpMode, (sx127x_read_register(dev->spi, RegOpMode) & 0x7f) | ((uint8_t)modulation << 7));

    dev->settings.modulation = modulation;

    return 0;
}

uint8_t sx127x_set_pa_select(sx127x_dev_t* dev, pa_select_t pa_select)
{
    sx127x_write_register(dev->spi, RegPaConfig, (sx127x_read_register(dev->spi, RegPaConfig) & 0x7f) | ((uint8_t)pa_select) << 7);

    dev->settings.pa_select = pa_select;

    return 0;
}

uint8_t sx127x_set_power(sx127x_dev_t* dev, uint8_t power)
{
    if (power > 0x0f)
        return -1;

    sx127x_write_register(dev->spi, RegPaConfig, (sx127x_read_register(dev->spi, RegPaConfig) & 0xf0) | 0x70 | power);

    dev->settings.power = power;

    return 0;
}

uint8_t sx127x_set_spreading_factor(sx127x_dev_t* dev, spreading_factor_t spreading_factor)
{
    if (spreading_factor < SF_10)
        sx127x_write_register(dev->spi, RegModemConfig3, sx127x_read_register(dev->spi, RegModemConfig3) & ~(1 << 3));
    else
        sx127x_write_register(dev->spi, RegModemConfig3, sx127x_read_register(dev->spi, RegModemConfig3) | (1 << 3));

    sx127x_write_register(dev->spi, RegModemConfig2, (sx127x_read_register(dev->spi, RegModemConfig2) & 0x0f) | ((uint8_t)spreading_factor << 4));

    dev->settings.spreading_factor = spreading_factor;

    return 0;
}

uint8_t sx127x_set_band_width(sx127x_dev_t* dev, band_width_t band_width)
{
    sx127x_write_register(dev->spi, RegModemConfig1, (sx127x_read_register(dev->spi, RegModemConfig1) & 0x0f) | ((uint8_t)band_width << 4));

    dev->settings.band_width = band_width;

    return 0;
}

uint8_t sx127x_set_coding_rate(sx127x_dev_t* dev, coding_rate_t coding_rate)
{
    sx127x_write_register(dev->spi, RegModemConfig1, (sx127x_read_register(dev->spi, RegModemConfig1) & 0xf1) | ((uint8_t)coding_rate << 1));

    dev->settings.coding_rate = coding_rate;

    return 0;
}

uint8_t sx127x_set_payload_crc_on(sx127x_dev_t* dev, bool crc_on)
{
    sx127x_write_register(dev->spi, RegModemConfig2, (sx127x_read_register(dev->spi, RegModemConfig2) & 0x0fB) | ((uint8_t)crc_on << 2));

    dev->settings.payload_crc_on = crc_on;

    return 0;
}

uint8_t sx127x_set_preamble_length(sx127x_dev_t* dev, uint16_t preamble_length)
{
    uint8_t preamble_val[2] = { (uint8_t)((preamble_length) >> 8), (uint8_t)preamble_length };

    dev->settings.preamble_length = preamble_length;

    return sx127x_write_burst(dev->spi, RegPreambleMsb, preamble_val, 2);
}

uint8_t sx127x_set_frequency(sx127x_dev_t* dev, uint32_t frequency)
{
    if (frequency < 525000000)
        sx127x_write_register(dev->spi, RegOpMode, sx127x_read_register(dev->spi, RegOpMode) | (1 << 3));
    else
        sx127x_write_register(dev->spi, RegOpMode, sx127x_read_register(dev->spi, RegOpMode) & ~(1 << 3));

    uint8_t reg[3];
    if (sx127x_frequency_to_reg(frequency, reg) != 0)
        return -1;

    dev->settings.frequency = frequency;

    return sx127x_write_burst(dev->spi, RegFrMsb, reg, 3);
}

uint8_t sx127x_set_sync_word(sx127x_dev_t* dev, uint8_t sync_word)
{
    sx127x_write_register(dev->spi, RegSyncWord, sync_word);

    dev->settings.sync_word = sync_word;

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

uint8_t sx127x_get_crc_valid(sx127x_dev_t* dev)
{
    if (!(sx127x_read_register(dev->spi, RegHopChannel) & (1 << 6)))
        return 2;

    if (sx127x_get_irq_flags(dev) & FlagPayloadCrcError)
        return 0;

    return 1;
}

uint8_t sx127x_get_modem_status(sx127x_dev_t* dev)
{
    return sx127x_read_register(dev->spi, RegModemStat);
}

modulation_t sx127x_get_modulation_mode(sx127x_dev_t* dev)
{
    dev->settings.modulation = (modulation_t)(sx127x_read_register(dev->spi, RegOpMode) >> 7);

    return dev->settings.modulation;
}

device_mode_t sx127x_get_mode(sx127x_dev_t* dev)
{
    dev->settings.mode = (device_mode_t)(sx127x_read_register(dev->spi, RegOpMode) & 0x7);

    return dev->settings.mode;
}

pa_select_t sx127x_get_pa_select(sx127x_dev_t* dev)
{
    dev->settings.pa_select = (pa_select_t)(sx127x_read_register(dev->spi, RegPaConfig) >> 7);

    return dev->settings.pa_select;
}

uint8_t sx127x_get_power(sx127x_dev_t* dev)
{
    dev->settings.power = sx127x_read_register(dev->spi, RegPaConfig) & 0x07;

    return dev->settings.power;
}

spreading_factor_t sx127x_get_spreading_factor(sx127x_dev_t* dev)
{
    dev->settings.spreading_factor = (spreading_factor_t)(sx127x_read_register(dev->spi, RegModemConfig2) >> 4);

    return dev->settings.spreading_factor;
}

band_width_t sx127x_get_band_width(sx127x_dev_t* dev)
{
    dev->settings.band_width = (band_width_t)(sx127x_read_register(dev->spi, RegModemConfig1) >> 4);

    return dev->settings.band_width;
}

coding_rate_t sx127x_get_coding_rate(sx127x_dev_t* dev)
{
    dev->settings.coding_rate = (coding_rate_t)((sx127x_read_register(dev->spi, RegModemConfig1) >> 1) & 0x07);

    return dev->settings.coding_rate;
}

bool sx127x_get_payload_crc_on(sx127x_dev_t* dev)
{
    dev->settings.payload_crc_on = (sx127x_read_register(dev->spi, RegModemConfig2) & (1 << 2));

    return dev->settings.payload_crc_on;
}

uint16_t sx127x_get_preamble_length(sx127x_dev_t* dev)
{
    uint8_t preamble_length[2];

    sx127x_read_burst(dev->spi, RegPreambleMsb, preamble_length, 2);

    dev->settings.preamble_length = (((uint16_t)preamble_length[0]) << 8) | ((uint16_t)preamble_length[1]);

    return dev->settings.preamble_length;
}

uint32_t sx127x_get_frequency(sx127x_dev_t* dev)
{
    uint8_t frequency[3];

    sx127x_read_burst(dev->spi, RegFrMsb, frequency, 3);

    dev->settings.frequency = sx127x_reg_to_frequency(frequency);

    return dev->settings.frequency;
}

uint8_t sx127x_get_sync_word(sx127x_dev_t* dev)
{
    dev->settings.sync_word = sx127x_read_register(dev->spi, RegSyncWord);

    return dev->settings.sync_word;
}

uint8_t sx127x_frequency_to_reg(uint32_t frequency, uint8_t* reg)
{
    if (frequency < 137000000 || frequency > 1020000000)
        return -1;

    uint32_t result = (uint32_t)((float)frequency / F_STEP);

    reg[0] = (uint8_t)(result >> 16);
    reg[1] = (uint8_t)(result >> 8);
    reg[2] = (uint8_t)(result);

    return 0;
}

uint32_t sx127x_reg_to_frequency(uint8_t* reg)
{
    uint32_t reg_value = (((uint32_t)reg[0]) << 16) | (((uint32_t)reg[1]) << 8) | ((uint32_t)reg[2]);
    return (uint32_t)((float)reg_value * F_STEP);
}
