/**
 * @ Author: Morran Smith
 * @ Create Time: 2019-06-01 09:51:05
 * @ Modified by: Morran Smith
 * @ Modified time: 2019-06-01 11:22:47
 * @ Description:
 */

#include <sx127x.h>
#include <sx127x_io.h>
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