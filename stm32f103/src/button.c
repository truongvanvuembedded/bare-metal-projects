//==================================================================================================
//
//	File Name		: button.c
//	CPU Type		: Stm32f103ct6
//	Project Name	: stm32-button
//
//	Description		: Entry point for button project
//
//	History			: Ver 0.01 04/04/2026 V.Vu
//
//==================================================================================================
//==================================================================================================
//	Compile Option
//==================================================================================================

//==================================================================================================
//	#pragma section
//==================================================================================================

//==================================================================================================
//	Local Compile Option
//==================================================================================================

//==================================================================================================
//	Header File
//==================================================================================================
#include <stdint.h>
#include "system_tick.h"
#include "Drv_GPIO.h"
#include "button.h"
//==================================================================================================
//	Local define
//==================================================================================================

//==================================================================================================
//	Local define I/O
//==================================================================================================

//==================================================================================================
//	Local Struct Template
//==================================================================================================

//==================================================================================================
//	Local RAM
//==================================================================================================

//==================================================================================================
//	Local ROM
//==================================================================================================

//==================================================================================================
//	Local Function Prototype
//==================================================================================================
static inline void __disable_irq(void);
static inline void __enable_irq(void);
/* Button */
static void bt_key_init(void);
static U1 u1_btn_read(void);
static void btn_callback(void* arg);
//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Name    :   SystemInit
//  Function:   Initialize system hardware after reset
//              - Configure clock sources and PLL
//              - Initialize Flash latency and wait states
//              - Set up vector table base address (if needed)
//
//  Argument:   None
//  Return    :   None
//  Created   :   22/03/2026 V.Vu
//  Changed   :   -
//  Remarks   :   Called automatically before main()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemInit(void)
{
    /* Init system tick */
    system_tick_init();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name	:	main
//	Function:	Entry pointer after reset
//
//	Argument:	None
//	Return	:	None
//	Create	:	22/03/2026 V.Vu
//	Change	:	-
//	Remarks	:
//
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    // Enable GPIOB clock
    GPIO_ClockControl(GPIOB, ENABLE);

    // Set PB2 as output (MODE=10, CNF=00)
    GPIO_Handle_t pin_B2 = {.pGPIOx = GPIOB,
                            .GPIO_PinConfig = {.GPIO_PinMode = GPIO_MODE_OUTPUT_PP,
                                               .GPIO_PinNumber = GPIO_PIN_2,
                                               .GPIO_PinPuPdControl = GPIO_PUPD_NO_PULL,
                                               .GPIO_PinSpeed = GPIO_SPEED_LOW}};
    /* Init GPIO */
    GPIO_Init(&pin_B2);

    /* Init button lib */
    ST_BUTTON st_Btn;
    u1_Button_Init(&st_Btn, U1HI, bt_key_init, u1_btn_read, btn_callback);
    Button_Enable(&st_Btn);

    while (1)
    {
        Button_Timer_Polling(&st_Btn);
        /* Cycle 10ms */
        delay_ms(10);
    }
}
/**
 * @brief  Disable all interrupts (Global) using Assembly.
 */
static inline void __disable_irq(void)
{
    __asm volatile("cpsid i" : : : "memory");
}

/**
 * @brief  Enable all interrupts (Global) using Assembly.
 */
static inline void __enable_irq(void)
{
    __asm volatile("cpsie i" : : : "memory");
}
static void bt_key_init(void)
{
    // Enable GPIOA clock
    GPIO_ClockControl(GPIOA, ENABLE);

    // Set PB2 as output (MODE=10, CNF=00)
    GPIO_Handle_t pin_A0 = {
        .pGPIOx = GPIOA,
        .GPIO_PinConfig = {.GPIO_PinMode = GPIO_MODE_INPUT_PULL, .GPIO_PinNumber = GPIO_PIN_0, .GPIO_PinPuPdControl = GPIO_PUPD_PULL_DOWN}};
    /* Init GPIO */
    GPIO_Init(&pin_A0);
}
static U1 u1_btn_read(void)
{
    return GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0);
}
static void btn_callback(void* arg)
{
    ST_BUTTON* apst_Btn = (ST_BUTTON*)arg;
    if (apst_Btn->u1_Status == U1_BUTTON_SW_STATE_SHORT_RELEASE_PRESS)
    {
        GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_2);
    }
}
/**
 * @brief  Standard character output function for printf redirection.
 * @param  character: The character to be sent.
 */
void _putchar(char character)
{
}