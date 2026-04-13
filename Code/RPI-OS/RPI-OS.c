#include <stdio.h>
#include "pico/stdlib.h"
#include "ST7735_TFT.h"
#include "hardware/spi.h"
#include "hw.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "hardware/structs/scb.h"
#include "hardware/watchdog.h"

#include "UI.h"

#include "minmea.h"

#include "hardware/uart.h"

#define S2 6
#define S3 20
#define S4 7
#define S5 21

#define GPS_UART uart0
#define BAUD_RATE 9600
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

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

bool read_nmea_line(char *buf, size_t max_len)
{
    static size_t pos = 0;

    while (uart_is_readable(GPS_UART))
    {
        char c = uart_getc(GPS_UART);

        if (c == '\n')
        {
            buf[pos] = '\0';
            pos = 0;
            return true; // complete line ready
        }
        else if (c != '\r')
        {
            if (pos < max_len - 1)
            {
                buf[pos++] = c;
            }
            else
            {
                pos = 0; // line too long, discard
            }
        }
    }
    return false; // line not complete yet
}

bool parse_gps_time(const char *line, int *hours, int *minutes, int *seconds)
{
    if (minmea_sentence_id(line, false) != MINMEA_SENTENCE_RMC)
        return false;

    struct minmea_sentence_rmc frame;
    if (!minmea_parse_rmc(&frame, line) || !frame.valid)
        return false;

    *hours = frame.time.hours;
    *minutes = frame.time.minutes;
    *seconds = frame.time.seconds;
    return true;
}

// Returns true and fills out sat count if the line contained a valid GSV sentence
bool parse_gps_satellites(const char *line, int *satellites) {
    if (minmea_sentence_id(line, false) != MINMEA_SENTENCE_GSV)
        return false;

    struct minmea_sentence_gsv frame;
    if (!minmea_parse_gsv(&frame, line))
        return false;

    *satellites = frame.total_sats;
    return true;
}

int main()
{

    gpio_init(S2);
    gpio_set_dir(S2, GPIO_IN);
    gpio_init(S4);
    gpio_set_dir(S4, GPIO_IN);

    stdio_init_all();
    init_hw();

    TFT_GreenTab_Initialize();

    gpio_init(10);
    gpio_set_dir(10, 1);
    gpio_put(10, 1);
    fillScreen(ST7735_BLACK);

    uart_init(GPS_UART, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

    while (true)
    {
        sleep_ms(100);
        // fillScreen(ST7735_BLACK);
        // drawText(0, 0, "System Booted", ST7735_WHITE, ST7735_BLACK, 1);
        gpio_put(10, 1);

        DrawThickRect(1, 1, 126, 158, 4, ST7735_BLUE);

        char line[160];
        if (read_nmea_line(line, sizeof(line)))
        {
            drawText(10, 10, line, ST7735_WHITE, ST7735_BLACK, 1);
        }

        int h, m, s;
        if (parse_gps_time(line, &h, &m, &s))
        {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "Time: %02d:%02d:%02d\n", h, m, s);
            drawText(5, 100, buffer, ST7735_WHITE, ST7735_BLACK, 1);
        }
        int sats;
        if (parse_gps_satellites(line, &sats))
        {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "Sattelites: %02d\n", sats);
            drawText(5, 110, buffer, ST7735_WHITE, ST7735_BLACK, 1);
        }

        if (gpio_get(S2))
        {
            fillScreen(ST7735_BLACK);
            fillRect(0, 0, 5, 10, ST7735_RED);

            for (uint16_t i = 1000; i != 0; i--)
            {
                char buffer[16];
                snprintf(buffer, sizeof(buffer), "%d", i);
                drawText(5, 0, buffer, ST7735_WHITE, ST7735_BLACK, 1);

                if (!gpio_get(S2))
                    break;
            }
            if (gpio_get(S2))
            {
                fillScreen(ST7735_BLACK);
                gpio_put(10, 0);

                dormant(S2);
            }
        }

        if (gpio_get(S4))
        {
            fillScreen(ST7735_BLACK);
            fillRect(123, 0, 5, 10, ST7735_RED);

            for (uint16_t i = 1000; i != 0; i--)
            {
                char buffer[16];
                snprintf(buffer, sizeof(buffer), "%d", i);
                drawText(100, 0, buffer, ST7735_WHITE, ST7735_BLACK, 1);

                if (!gpio_get(S4))
                    break;
            }
            if (gpio_get(S4))
            {
                fillScreen(ST7735_BLACK);
                gpio_put(10, 0);
                watchdog_reboot(0, 0, 1);
            }
        }
    }
}
