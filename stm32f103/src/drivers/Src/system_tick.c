#include "system_tick.h"
#include <stdint.h>

/* SysTick Base Address: 0xE000E010 */
#define STK_CTRL (*(volatile uint32_t*)0xE000E010)  // Control and Status Register
#define STK_LOAD (*(volatile uint32_t*)0xE000E014)  // Reload Value Register
#define STK_VAL  (*(volatile uint32_t*)0xE000E018)  // Current Value Register

/* Global variable to track milliseconds */
static volatile uint32_t ms_ticks = 0;

/**
 * @brief  Initializes the SysTick timer to trigger an interrupt every 1ms.
 * @note   Assumes SystemCoreClock is 8MHz (Internal HSI).
 */
void system_tick_init(void)
{
    /* 1. Calculate Reload Value for 1ms 
     * Formula: Reload = (Clock Frequency / Target Frequency) - 1
     * (8,000,000 Hz / 1000 Hz) - 1 = 7999
     */
    STK_LOAD = 7999;

    /* 2. Clear current value register by writing any value to it */
    STK_VAL = 0;

    /* 3. Configure SysTick Control Register
     * Bit 0 (ENABLE):    1 -> Start the counter
     * Bit 1 (TICKINT):   1 -> Enable SysTick exception (Interrupt)
     * Bit 2 (CLKSOURCE): 1 -> Use Processor Clock (8MHz)
     * Result: 0b111 = 0x07
     */
    STK_CTRL = 0x07;
}

/**
 * @brief  The SysTick Interrupt Handler. 
 * @note   Must match the name in your startup.c vector table.
 */
void SysTick_Handler(void)
{
    ms_ticks++;
}

/**
 * @brief  Delay function using the SysTick counter.
 * @param  ms: Number of milliseconds to wait.
 */
void delay_ms(uint32_t ms)
{
    uint32_t start_time = ms_ticks;
    /* Wait until the difference reaches the target ms */
    while ((ms_ticks - start_time) < ms)
        ;
}

uint32_t get_tick(void)
{
    return ms_ticks;
}
