/**
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();

    int i_sec=0;
    while (true) {
        printf("seconds run=%d\n",i_sec++);
        sleep_ms(1000);
    }

    return 0;
}
