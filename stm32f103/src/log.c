//==================================================================================================
//
//	File Name		: log.c
//	CPU Type		: Stm32f103ct6
//	Project Name	: stm32-log
//
//	Description		: Entry point for project
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
#include "ring_buffer.h"
#include "printf.h"
#include "system_tick.h"
#include "Drv_UART.h"
#include "Drv_GPIO.h"
//==================================================================================================
//	Local define
//==================================================================================================
#define USART1_CR1              (*(volatile uint32_t *)0x4001380C)
//==================================================================================================
//	Local define I/O
//==================================================================================================

//==================================================================================================
//	Local Struct Template
//==================================================================================================

//==================================================================================================
//	Local RAM
//==================================================================================================
UART_Handle_t UART1Handle;
GPIO_Handle_t UARTPins;
ST_RING_BUFFER st_RingBuf_Send;
ST_RING_BUFFER st_RingBuf_Rev;
U1 u1_BufSend[100];
U1 u1_BufRev[100];
U1 u1_Test;
//==================================================================================================
//	Local ROM
//==================================================================================================

//==================================================================================================
//	Local Function Prototype
//==================================================================================================
static void uart_init(void);
static void uartpin_init(void);
static inline void __disable_irq(void);
static inline void __enable_irq(void);
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
	uartpin_init();
	uart_init();
	/* Init ring buffer */
	RingBuffer_Init(&st_RingBuf_Send, u1_BufSend, sizeof(u1_BufSend));
	RingBuffer_Init(&st_RingBuf_Rev, u1_BufRev, sizeof(u1_BufRev));
    /* Enable receive via IT for test receive data with send data via IT*/
    UART_ReceiveDataIT(&UART1Handle, &u1_Test, 1);
    while (1)
    {
        printf("Hello I'm Truong Van Vu Embedded Software Engineer\n");
        delay_ms(1000);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			uart_init
//	Name:			-
//	Function:		UART peripheral initialization
//
//	Argument:		pUARTx - Pointer to UART peripheral
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void uart_init(void)
{
	// Confige UART1
	UART1Handle.pUARTx = USART1;
	UART1Handle.UART_Config.BaudRate = UART_BAUDRATE_9600;
	UART1Handle.UART_Config.DataFormat = UART_DATA_LEN_8BIT;
	UART1Handle.UART_Config.ParityControl = UART_PARITY_DISABLE;
	UART1Handle.UART_Config.StopbitLen = UART_STOP_BIT_1;
	UART1Handle.UART_Config.UARTMode = UART_MODE_TXRX;
	UART1Handle.UART_Config.HardwareFlowCtrl = UART_HARDWARE_FLOW_CTRL_DISABLE;
	UART_Init(&UART1Handle);

	// Configure the IRQ priority 
	UART_IRQInterruptConfig(UART_IRQ_INDEX_USART1, ENABLE);
	// Confige the interrupt for UART peripheral at the NVIC
	UART_IRQPriorityConfig(UART_IRQ_INDEX_USART1, 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			uartpin_init
//	Name:			-
//	Function:		UART peripheral pin initialization
//
//	Argument:		-
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void uartpin_init(void)
{
	// UART remap to config alternative funtion
	AFIO->MAPR &= ~(1U << 2);
	// Congif UART1 with Tx: A9 and Rx: A10
	UARTPins.pGPIOx = GPIOA;
	UARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_AF_PP;
	UARTPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_NO_PULL;
	UARTPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	// UART1_TX
	UARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_9;
	GPIO_Init(&UARTPins);
    // UART1_RX (A10)
    UARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_10;
    UARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING; // Change to Input for RX
    UARTPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPD_NO_PULL; // Pull-up is safer
    GPIO_Init(&UARTPins);
}
/**
 * @brief  USART1 Global Interrupt Handler for TX and RX.
 * @note   Handles both Transmit (TXE) and Receive (RXNE) events.
 */
void USART1_IRQHandler(void)
{
    /* Register addresses for USART1 */
    volatile uint32_t *SR  = (uint32_t *)0x40013800; // Status Register
    volatile uint32_t *DR  = (uint32_t *)0x40013804; // Data Register
    volatile uint32_t *CR1 = (uint32_t *)0x4001380C; // Control Register 1

    /* --- CASE 1: RECEIVE DATA (RXNE) --- */
    /* Bit 5 (RXNE): 1 -> Data is received and ready to be read */
    if (*SR & (1 << 5)) 
    {
        /* Reading the DR register automatically clears the RXNE flag */
        U1 received_data = (U1)(*DR & 0xFF);

        /* Push received data into the Receive Ring Buffer */
        __disable_irq();
        RingBuffer_Put_Data(&st_RingBuf_Rev, received_data);
        __enable_irq();
    }

    /* --- CASE 2: TRANSMIT DATA (TXE) --- */
    /* Bit 7 (TXE): 1 -> Transmit Data Register is empty */
    if (*SR & (1 << 7)) 
    {
        U1 data_to_send;

        /* Check if the Send Ring Buffer has data to transmit */
        if (u1_RingBuffer_Get_Data(&st_RingBuf_Send, &data_to_send) == 1) 
        {
            /* Writing to DR starts transmission and clears the TXE flag */
            *DR = (uint32_t)data_to_send;
        }
        else 
        {
            /* No more data to send: Disable TXE interrupt to save CPU cycles */
            *CR1 &= ~(1 << 7);
        }
    }
}
/**
 * @brief  Standard character output function for printf redirection.
 * @param  character: The character to be sent.
 */
void _putchar(char character)
{
    /* 1. Push the character into the Ring Buffer */
    /* Ensure st_RingBuf_Send is initialized before calling this */
    __disable_irq();
    RingBuffer_Put_Data(&st_RingBuf_Send, (uint8_t)character);
    __enable_irq();
    /* 2. Enable USART1 Transmit Data Register Empty Interrupt (TXEIE) 
     * Offset for CR1 is 0x0C, TXEIE is bit 7.
     * This triggers the IRQHandler to start processing the buffer.
     */
    USART1_CR1 |= (1 << 7); 
}
/**
 * @brief  Disable all interrupts (Global) using Assembly.
 */
static inline void __disable_irq(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

/**
 * @brief  Enable all interrupts (Global) using Assembly.
 */
static inline void __enable_irq(void) {
    __asm volatile ("cpsie i" : : : "memory");
}