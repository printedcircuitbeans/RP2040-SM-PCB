#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
        gpio_init(10);
    gpio_set_dir(10,1);

    gpio_put(10,1);
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}