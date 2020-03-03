# sx127x

Библиотека управления приемопередатчиком sx1276/7/8/9

## Пример использования

```C
#include <stdio.h>
#include <string.h>
#include <sx127x.h>

uint32_t sx127x_spi_transmit(uint8_t* buffer, uint32_t size);
uint32_t sx127x_spi_transmit_receive(uint8_t* tx_buffer, uint32_t tx_size, uint8_t* rx_buffer, uint32_t rx_size);
void sx127x_delay(uint32_t delay);
void sx127x_reset_control(bool state);
void sx127x_rx_callback(uint8_t* buffer, uint8_t size);
void sx127x_tx_callback(void);
void sx127x_rx_timeout_callback(void);
void sx127x_rx_crc_error_callback(void);

sx127x_spi_t sx127x_spi = {
    .transmit = &sx127x_spi_transmit,
    .transmit_receive = &sx127x_spi_transmit_receive,
};

sx127x_callbacks_t sx127x_callbacks = {
    .rx_done = &sx127x_rx_callback,
    .tx_done = &sx127x_tx_callback,
    .rx_timeout = &sx127x_rx_timeout_callback,
    .rx_crc_error = &sx127x_rx_crc_error_callback,
};

sx127x_common_t sx127x_common = {
    .delay = &sx127x_delay,
    .reset_control = &sx127x_reset_control,
};

sx127x_dev_t sx1278_phy = {
    .spi = &sx127x_spi,
    .callbacks = &sx127x_callbacks,
    .common = &sx127x_common,
};

int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

    sx127x_radio_settings_t settings = {
        .modulation = LORA,
        .pa_select = PA_BOOST,
        .power = 0x0a,
        .spreading_factor = SF_10,
        .band_width = BW_125_KHz,
        .coding_rate = CR_4_5,
        .payload_crc_on = true,
        .preamble_length = 10,
        .frequency = 433000000,
        .sync_word = 0x6c,
    };

    sx127x_init(&sx1278_phy, &settings);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    sx127x_receive_continuous_it(&sx1278_phy);
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }

    /* USER CODE END 3 */
}

uint32_t sx127x_spi_transmit(uint8_t* buffer, uint32_t size)
{
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi2, buffer, size, 0x1000);

    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
        ;

    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

    return 0;
}

uint32_t sx127x_spi_transmit_receive(uint8_t* tx_buffer, uint32_t tx_size, uint8_t* rx_buffer, uint32_t rx_size)
{
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi2, tx_buffer, tx_size, 0x1000);

    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
        ;

    HAL_SPI_Receive(&hspi2, rx_buffer, rx_size, 0x1000);

    while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
        ;

    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

    return 0;
}

void sx127x_delay(uint32_t delay)
{
    HAL_Delay(delay);
}

void sx127x_reset_control(bool state)
{
    HAL_GPIO_WritePin(SX_RESET_GPIO_Port, SX_RESET_Pin,
        (state) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void sx127x_rx_callback(uint8_t* buffer, uint8_t size)
{
    printf("RX DONE. DATA: %s | RSSI: %d | SNR: %d\r\n", buffer, sx127x_get_last_packet_rssi(&sx1278_phy), sx127x_get_last_packet_snr(&sx1278_phy));
}

void sx127x_tx_callback()
{
    printf("TX DONE\r\n");
}

void sx127x_rx_timeout_callback()
{
    printf("RX TIMEOUT\r\n");
}

void sx127x_rx_crc_error_callback()
{
    printf("RX CRC ERROR\r\n");
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
    case DIO0_Pin:
        sx127x_dio_0_callback(sx1278_phy);
        break;
    case DIO1_Pin:
        sx127x_dio_1_callback(sx1278_phy);
        break;
    case DIO3_Pin:
        sx127x_dio_3_callback(sx1278_phy);
        break;
    default:
        break;
    }
}
```
