#include <stdio.h>
#include "pico/stdlib.h"
#include "ST7735_TFT.h"
#include "hardware/spi.h"
#include "hw.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "hardware/structs/scb.h"

#include "UI.h"

#define S2 6
#define S3 20
#define S4 7
#define S5 21

void init_hw()
{
    stdio_init_all();
    spi_init(SPI_PORT, 10000000);
    gpio_set_function(SPI_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX, GPIO_FUNC_SPI);
    tft_spi_init();
}

void dormant(uint16_t WAKEPIN)
{
    // Enable wake on GPIO
    gpio_set_dormant_irq_enabled(WAKEPIN, GPIO_IRQ_EDGE_RISE, true);
    // Enter dormant
    xosc_dormant();
}

int main()
{

    gpio_init(S2);
    gpio_set_dir(S2, GPIO_IN);

    stdio_init_all();
    init_hw();

    TFT_GreenTab_Initialize();

    gpio_init(10);
    gpio_set_dir(10, 1);
    gpio_put(10, 1);
    fillScreen(ST7735_BLACK);

    while (true)
    {
        sleep_ms(100);
        fillScreen(ST7735_BLACK);
        drawText(0, 0, "System Booted", ST7735_WHITE, ST7735_BLACK, 1);
        gpio_put(10, 1);

        if (gpio_get(S2))
        {
            fillScreen(ST7735_BLACK);
            fillRect(0, 0, 5, 10, ST7735_RED);

            for (uint16_t i = 1000; i != 0; i--)
            {
                char buffer[16];
                snprintf(buffer, sizeof(buffer), "%d",i);
                drawText(5, 0, buffer, ST7735_WHITE, ST7735_BLACK, 1);

                if(!gpio_get(S2)) break;
            }
            if (gpio_get(S2))
            {
                fillScreen(ST7735_BLACK);
                gpio_put(10, 0);

                dormant(S2);
            }
        }
    }
}
