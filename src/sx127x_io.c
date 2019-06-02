/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 10:26:27
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-02 11:36:14
 * @ Description:
 */

#include <string.h>

#include <sx127x_io.h>
#include <sx127x_registers.h>

void sx127x_write_register(spi_t* dev, uint8_t reg, uint8_t val)
{
    uint8_t buffer[] = { reg | 0x80, val };
    dev->transmit(buffer, 2);
}

uint8_t sx127x_read_register(spi_t* dev, uint8_t reg)
{
    uint8_t result = 0xFF;
    dev->transmit_receive(&reg, 1, &result, 1);
    return result;
}

uint8_t sx127x_write_burst(spi_t* dev, uint8_t reg, uint8_t* buffer, uint8_t size)
{
    if (size == 0)
        return -1;

    uint8_t tmp[size + 1];
    tmp[0] = reg | 0x80;

    memmove(tmp + 1, buffer, (uint32_t)size);

    dev->transmit(tmp, (uint32_t)size + 1);

    return 0;
}

uint8_t sx127x_read_burst(spi_t* dev, uint8_t reg, uint8_t* buffer, uint8_t size)
{
    if (size == 0)
        return -1;

    dev->transmit_receive(&reg, 1, buffer, (uint32_t)size);

    return 0;
}
