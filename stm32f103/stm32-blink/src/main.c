#include <stdint.h>
#include "Drv_GPIO.h"

void delay() {
    for (volatile int i = 0; i < 250000; i++);
}

int main(void) {
    // Enable GPIOB clock
    GPIO_ClockControl( GPIOB, ENABLE );

    // Set PB2 as output (MODE=10, CNF=00)
    GPIO_Handle_t pin_B2 = {
        .pGPIOx = GPIOB,
        .GPIO_PinConfig = {
            .GPIO_PinMode = GPIO_MODE_OUTPUT_PP,
            .GPIO_PinNumber = GPIO_PIN_2,
            .GPIO_PinPuPdControl = GPIO_PUPD_NO_PULL,
            .GPIO_PinSpeed = GPIO_SPEED_LOW
        }
    };
    /* Init GPIO */
    GPIO_Init( &pin_B2 );

    while (1) {
        GPIO_ToggleOutputPin( GPIOB, GPIO_PIN_2 );
        delay();
    }
}