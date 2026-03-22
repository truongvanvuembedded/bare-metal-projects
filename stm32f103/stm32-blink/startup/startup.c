
//==================================================================================================
// File Name       : startup_stm32f103.c
// CPU Type        : STM32F103C(T6)
// Project Name    : stm32-blink
// Description     : Startup code and vector table for STM32F103
// History         : Ver 0.01 22/03/2026 V.Vu
//==================================================================================================
/**
 ******************************************************************************
 * @file      startup_stm32f103c8tx.c
 * @author    Auto-generated from .s file
 * @brief     STM32F103C8Tx device vector table for GCC toolchain.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the initial PC == Reset_Handler,
 *                - Set the vector table entries with the exceptions ISR address
 *                - Branches to main in the C library (which eventually
 *                  calls main()).
 ******************************************************************************
 */
#include <stdint.h>

/* Linker script symbols for memory sections */
extern uint32_t _sidata; /* start of .data in flash */
extern uint32_t _sdata;  /* start of .data in RAM */
extern uint32_t _edata;  /* end of .data in RAM */
extern uint32_t _sbss;   /* start of .bss in RAM */
extern uint32_t _ebss;   /* end of .bss in RAM */
extern uint32_t _estack; /* top of stack */

extern void main(void);
extern void SystemInit(void);
// extern void __libc_init_array(void);

/* Cortex-M3 Exception and IRQ handlers (weak aliases to Default_Handler) */
#define WEAK_DEFAULT __attribute__((weak, alias("Default_Handler")))

/* Cortex-M3 core exception handlers */
void Reset_Handler(void);
void NMI_Handler(void) WEAK_DEFAULT;
void HardFault_Handler(void) WEAK_DEFAULT;
void MemManage_Handler(void) WEAK_DEFAULT;
void BusFault_Handler(void) WEAK_DEFAULT;
void UsageFault_Handler(void) WEAK_DEFAULT;
void SVC_Handler(void) WEAK_DEFAULT;
void DebugMon_Handler(void) WEAK_DEFAULT;
void PendSV_Handler(void) WEAK_DEFAULT;
void SysTick_Handler(void) WEAK_DEFAULT;

/* STM32F103 peripheral IRQ handlers (weak) */
void WWDG_IRQHandler(void) WEAK_DEFAULT;            /* Window Watchdog interrupt */
void PVD_IRQHandler(void) WEAK_DEFAULT;             /* PVD through EXTI line detection interrupt */
void TAMPER_IRQHandler(void) WEAK_DEFAULT;          /* Tamper interrupt */
void RTC_IRQHandler(void) WEAK_DEFAULT;             /* RTC global interrupt */
void FLASH_IRQHandler(void) WEAK_DEFAULT;           /* Flash global interrupt */
void RCC_IRQHandler(void) WEAK_DEFAULT;             /* RCC global interrupt */
void EXTI0_IRQHandler(void) WEAK_DEFAULT;           /* EXTI Line0 interrupt */
void EXTI1_IRQHandler(void) WEAK_DEFAULT;           /* EXTI Line1 interrupt */
void EXTI2_IRQHandler(void) WEAK_DEFAULT;           /* EXTI Line2 interrupt */
void EXTI3_IRQHandler(void) WEAK_DEFAULT;           /* EXTI Line3 interrupt */
void EXTI4_IRQHandler(void) WEAK_DEFAULT;           /* EXTI Line4 interrupt */
void DMA1_Channel1_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel1 global interrupt */
void DMA1_Channel2_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel2 global interrupt */
void DMA1_Channel3_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel3 global interrupt */
void DMA1_Channel4_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel4 global interrupt */
void DMA1_Channel5_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel5 global interrupt */
void DMA1_Channel6_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel6 global interrupt */
void DMA1_Channel7_IRQHandler(void) WEAK_DEFAULT;   /* DMA1 Channel7 global interrupt */
void ADC1_2_IRQHandler(void) WEAK_DEFAULT;          /* ADC1 and ADC2 global interrupt */
void USB_HP_CAN_TX_IRQHandler(void) WEAK_DEFAULT;   /* USB High Priority or CAN TX interrupts */
void USB_LP_CAN_RX0_IRQHandler(void) WEAK_DEFAULT;  /* USB Low Priority or CAN RX0 interrupts */
void CAN_RX1_IRQHandler(void) WEAK_DEFAULT;         /* CAN RX1 interrupt */
void CAN_SCE_IRQHandler(void) WEAK_DEFAULT;         /* CAN SCE interrupt */
void EXTI9_5_IRQHandler(void) WEAK_DEFAULT;         /* EXTI Line[9:5] interrupts */
void TIM1_BRK_IRQHandler(void) WEAK_DEFAULT;        /* TIM1 Break interrupt */
void TIM1_UP_IRQHandler(void) WEAK_DEFAULT;         /* TIM1 Update interrupt */
void TIM1_TRG_COM_IRQHandler(void) WEAK_DEFAULT;    /* TIM1 Trigger and Commutation interrupts */
void TIM1_CC_IRQHandler(void) WEAK_DEFAULT;         /* TIM1 Capture Compare interrupt */
void TIM2_IRQHandler(void) WEAK_DEFAULT;            /* TIM2 global interrupt */
void TIM3_IRQHandler(void) WEAK_DEFAULT;            /* TIM3 global interrupt */
void TIM4_IRQHandler(void) WEAK_DEFAULT;            /* TIM4 global interrupt */
void I2C1_EV_IRQHandler(void) WEAK_DEFAULT;         /* I2C1 event interrupt */
void I2C1_ER_IRQHandler(void) WEAK_DEFAULT;         /* I2C1 error interrupt */
void I2C2_EV_IRQHandler(void) WEAK_DEFAULT;         /* I2C2 event interrupt */
void I2C2_ER_IRQHandler(void) WEAK_DEFAULT;         /* I2C2 error interrupt */
void SPI1_IRQHandler(void) WEAK_DEFAULT;            /* SPI1 global interrupt */
void SPI2_IRQHandler(void) WEAK_DEFAULT;            /* SPI2 global interrupt */
void USART1_IRQHandler(void) WEAK_DEFAULT;          /* USART1 global interrupt */
void USART2_IRQHandler(void) WEAK_DEFAULT;          /* USART2 global interrupt */
void USART3_IRQHandler(void) WEAK_DEFAULT;          /* USART3 global interrupt */
void EXTI15_10_IRQHandler(void) WEAK_DEFAULT;       /* EXTI Line[15:10] interrupts */
void RTCAlarm_IRQHandler(void) WEAK_DEFAULT;        /* RTC Alarms through EXTI line interrupt */
void TIM8_BRK_IRQHandler(void) WEAK_DEFAULT;        /* TIM8 Break interrupt */
void TIM8_UP_IRQHandler(void) WEAK_DEFAULT;         /* TIM8 Update interrupt */
void TIM8_TRG_COM_IRQHandler(void) WEAK_DEFAULT;    /* TIM8 Trigger and Commutation interrupts */
void TIM8_CC_IRQHandler(void) WEAK_DEFAULT;         /* TIM8 Capture Compare interrupt */
void ADC3_IRQHandler(void) WEAK_DEFAULT;            /* ADC3 global interrupt */
void FSMC_IRQHandler(void) WEAK_DEFAULT;            /* FSMC global interrupt */
void SDIO_IRQHandler(void) WEAK_DEFAULT;            /* SDIO global interrupt */
void TIM5_IRQHandler(void) WEAK_DEFAULT;            /* TIM5 global interrupt */
void SPI3_IRQHandler(void) WEAK_DEFAULT;            /* SPI3 global interrupt */
void UART4_IRQHandler(void) WEAK_DEFAULT;           /* UART4 global interrupt */
void UART5_IRQHandler(void) WEAK_DEFAULT;           /* UART5 global interrupt */
void TIM6_IRQHandler(void) WEAK_DEFAULT;            /* TIM6 global interrupt */
void TIM7_IRQHandler(void) WEAK_DEFAULT;            /* TIM7 global interrupt */
void DMA2_Channel1_IRQHandler(void) WEAK_DEFAULT;   /* DMA2 Channel1 global interrupt */
void DMA2_Channel2_IRQHandler(void) WEAK_DEFAULT;   /* DMA2 Channel2 global interrupt */
void DMA2_Channel3_IRQHandler(void) WEAK_DEFAULT;   /* DMA2 Channel3 global interrupt */
void DMA2_Channel4_5_IRQHandler(void) WEAK_DEFAULT; /* DMA2 Channel4 and Channel5 global interrupt */

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
*/
void Default_Handler(void)
{
    while (1)
        ;
}

/* Vector table */
void (*const g_pfnVectors[])(void) __attribute__((section(".isr_vector"))) = {
    (void (*)(void))(&_estack), /* Initial stack pointer */
    Reset_Handler,              /* Reset handler */
    NMI_Handler,                /* NMI handler */
    HardFault_Handler,          /* Hard fault handler */
    MemManage_Handler,          /* MPU fault handler */
    BusFault_Handler,           /* Bus fault handler */
    UsageFault_Handler,         /* Usage fault handler */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    SVC_Handler,                /* SVCall handler */
    DebugMon_Handler,           /* Debug monitor handler */
    0,                          /* Reserved */
    PendSV_Handler,             /* PendSV handler */
    SysTick_Handler,            /* SysTick handler */

    /* STM32F103 peripheral IRQs */
    WWDG_IRQHandler,           /* Window Watchdog interrupt */
    PVD_IRQHandler,            /* PVD through EXTI line detection interrupt */
    TAMPER_IRQHandler,         /* Tamper interrupt */
    RTC_IRQHandler,            /* RTC global interrupt */
    FLASH_IRQHandler,          /* Flash global interrupt */
    RCC_IRQHandler,            /* RCC global interrupt */
    EXTI0_IRQHandler,          /* EXTI Line0 interrupt */
    EXTI1_IRQHandler,          /* EXTI Line1 interrupt */
    EXTI2_IRQHandler,          /* EXTI Line2 interrupt */
    EXTI3_IRQHandler,          /* EXTI Line3 interrupt */
    EXTI4_IRQHandler,          /* EXTI Line4 interrupt */
    DMA1_Channel1_IRQHandler,  /* DMA1 Channel1 global interrupt */
    DMA1_Channel2_IRQHandler,  /* DMA1 Channel2 global interrupt */
    DMA1_Channel3_IRQHandler,  /* DMA1 Channel3 global interrupt */
    DMA1_Channel4_IRQHandler,  /* DMA1 Channel4 global interrupt */
    DMA1_Channel5_IRQHandler,  /* DMA1 Channel5 global interrupt */
    DMA1_Channel6_IRQHandler,  /* DMA1 Channel6 global interrupt */
    DMA1_Channel7_IRQHandler,  /* DMA1 Channel7 global interrupt */
    ADC1_2_IRQHandler,         /* ADC1 and ADC2 global interrupt */
    USB_HP_CAN_TX_IRQHandler,  /* USB High Priority or CAN TX interrupts */
    USB_LP_CAN_RX0_IRQHandler, /* USB Low Priority or CAN RX0 interrupts */
    CAN_RX1_IRQHandler,        /* CAN RX1 interrupt */
    CAN_SCE_IRQHandler,        /* CAN SCE interrupt */
    EXTI9_5_IRQHandler,        /* EXTI Line[9:5] interrupts */
    TIM1_BRK_IRQHandler,       /* TIM1 Break interrupt */
    TIM1_UP_IRQHandler,        /* TIM1 Update interrupt */
    TIM1_TRG_COM_IRQHandler,   /* TIM1 Trigger and Commutation interrupts */
    TIM1_CC_IRQHandler,        /* TIM1 Capture Compare interrupt */
    TIM2_IRQHandler,           /* TIM2 global interrupt */
    TIM3_IRQHandler,           /* TIM3 global interrupt */
    TIM4_IRQHandler,           /* TIM4 global interrupt */
    I2C1_EV_IRQHandler,        /* I2C1 event interrupt */
    I2C1_ER_IRQHandler,        /* I2C1 error interrupt */
    I2C2_EV_IRQHandler,        /* I2C2 event interrupt */
    I2C2_ER_IRQHandler,        /* I2C2 error interrupt */
    SPI1_IRQHandler,           /* SPI1 global interrupt */
    SPI2_IRQHandler,           /* SPI2 global interrupt */
    USART1_IRQHandler,         /* USART1 global interrupt */
    USART2_IRQHandler,         /* USART2 global interrupt */
    USART3_IRQHandler,         /* USART3 global interrupt */
    EXTI15_10_IRQHandler,      /* EXTI Line[15:10] interrupts */
    RTCAlarm_IRQHandler,       /* RTC Alarms through EXTI line interrupt */
    0,                         /* Reserved */
    TIM8_BRK_IRQHandler,       /* TIM8 Break interrupt */
    TIM8_UP_IRQHandler,        /* TIM8 Update interrupt */
    TIM8_TRG_COM_IRQHandler,   /* TIM8 Trigger and Commutation interrupts */
    TIM8_CC_IRQHandler,        /* TIM8 Capture Compare interrupt */
    ADC3_IRQHandler,           /* ADC3 global interrupt */
    FSMC_IRQHandler,           /* FSMC global interrupt */
    SDIO_IRQHandler,           /* SDIO global interrupt */
    TIM5_IRQHandler,           /* TIM5 global interrupt */
    SPI3_IRQHandler,           /* SPI3 global interrupt */
    UART4_IRQHandler,          /* UART4 global interrupt */
    UART5_IRQHandler,          /* UART5 global interrupt */
    TIM6_IRQHandler,           /* TIM6 global interrupt */
    TIM7_IRQHandler,           /* TIM7 global interrupt */
    DMA2_Channel1_IRQHandler,  /* DMA2 Channel1 global interrupt */
    DMA2_Channel2_IRQHandler,  /* DMA2 Channel2 global interrupt */
    DMA2_Channel3_IRQHandler,  /* DMA2 Channel3 global interrupt */
    DMA2_Channel4_5_IRQHandler /* DMA2 Channel4 and DMA2 Channel5 global interrupt */
};

/* Reset handler: setup stack, copy data, clear BSS, call main */
void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Initialize system */
    SystemInit();

    /* Copy initialized data from flash to RAM */
    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    /* Zero-fill the .bss section */
    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    /* Call static constructors (C library) */
    // __libc_init_array();

    /* Call main application */
    main();

    /* If main returns, loop forever */
    while (1)
        ;
}