//==================================================================================================
//
// 	File Name		Drv_UART.c
//
//	CPU Type		STM32F103C8T6
//	Builder			STM32CUBE_IDE
//
//	Coding			V.VU	ASTI DANANG
//
//	Outline
//
//
//	History			Ver.0.01	2025.02.18 V.Vu	New
//
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
#include <stdio.h>
#include "stm32f103c8t6.h"
#include "Drv_UART.h"
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
static const uint32_t AHBPrescTable[8] = {2, 4, 8, 16, 64, 128, 256, 512};
static const uint32_t APBPrescTable[4] = {2, 4, 8, 16};
//==================================================================================================
//	Local ROM
//==================================================================================================

//==================================================================================================
//	Local Function Prototype
//==================================================================================================
static uint32_t u4_UART_BaudRate_Cal(uint32_t BaudRate, uint32_t UARTClk);
static uint32_t u4_UART_SystemClk(UART_Handle_t* pUARTHandle);
static void UART_TxISR_8bit(UART_Handle_t* pUARTHandle);
static void UART_TxISR_9bit(UART_Handle_t* pUARTHandle);
static void UART_RxISR_8bit(UART_Handle_t* pUARTHandle);
static void UART_RxISR_9bit(UART_Handle_t* pUARTHandle);
void UART_ApplicationEventCallback(UART_Handle_t* pUARTHandle);
//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_PeriClockControl
//	Name:			-
//	Function:		Enables or disables the clock for the specified UART peripheral
//
//	Argument:		pUARTx - Pointer to UART peripheral
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_PeriClockControl(UART_Handle_t* pUARTHandle, uint8_t EnOrDi)
{
    if (EnOrDi == ENABLE)
    {
        if (pUARTHandle->pUARTx == USART1)
        {
            USART1_CLOCK_ENABLE();
        }
        else if (pUARTHandle->pUARTx == USART2)
        {
            USART2_CLOCK_ENABLE();
        }
        else if (pUARTHandle->pUARTx == USART3)
        {
            USART3_CLOCK_ENABLE();
        }
    }
    else
    {
        if (pUARTHandle->pUARTx == USART1)
        {
            USART1_CLOCK_DISABLE();
        }
        else if (pUARTHandle->pUARTx == USART2)
        {
            USART2_CLOCK_DISABLE();
        }
        else if (pUARTHandle->pUARTx == USART3)
        {
            USART3_CLOCK_DISABLE();
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_Init
//	Name:			-
//	Function:		Init UART with config from USER
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_Init(UART_Handle_t* pUARTHandle)
{
    // Enable clock for the UART peripheral
    UART_PeriClockControl(pUARTHandle, ENABLE);
    // Configure the UART peripheral

    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_UE);  // Enable the UART peripheral
    pUARTHandle->pUARTx->BRR = u4_UART_BaudRate_Cal(pUARTHandle->UART_Config.BaudRate,
                                                    u4_UART_SystemClk(pUARTHandle));        // Set baud rate
    pUARTHandle->pUARTx->CR1 |= (pUARTHandle->UART_Config.DataFormat << USART_CR1_M);       // Set word length
    pUARTHandle->pUARTx->CR1 |= (pUARTHandle->UART_Config.StopbitLen << USART_CR2_STOP);    // Set stop bits
    pUARTHandle->pUARTx->CR1 |= (pUARTHandle->UART_Config.ParityControl << USART_CR1_PCE);  // Set parity control
    pUARTHandle->pUARTx->CR1 |= (pUARTHandle->UART_Config.ParityMode << USART_CR1_PS);      // Set parity mode

    if (pUARTHandle->UART_Config.UARTMode == UART_MODE_TX)
    {
        pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_TE);  // Enable transmitter
    }
    else if (pUARTHandle->UART_Config.UARTMode == UART_MODE_RX)
    {
        pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_RE);  // Enable receiver
    }
    else if (pUARTHandle->UART_Config.UARTMode == UART_MODE_TXRX)
    {
        pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_TE);  // Enable transmitter
        pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_RE);  // Enable receiver
    }

    if (pUARTHandle->UART_Config.HardwareFlowCtrl == UART_HARDWARE_CTS_ENABLE)
    {
        pUARTHandle->pUARTx->CR3 |= (1 << USART_CR3_CTSE);  // Enable CTS flow control
    }
    else if (pUARTHandle->UART_Config.HardwareFlowCtrl == UART_HARDWARE_RTS_ENABLE)
    {
        pUARTHandle->pUARTx->CR3 |= (1 << USART_CR3_RTSE);  // Enable RTS flow control
    }
    else if (pUARTHandle->UART_Config.HardwareFlowCtrl == UART_HARDWARE_CTS_RTS_ENABLE)
    {
        pUARTHandle->pUARTx->CR3 |= (1 << USART_CR3_CTSE);  // Enable CTS flow control
        pUARTHandle->pUARTx->CR3 |= (1 << USART_CR3_RTSE);  // Enable RTS flow control
    }
    else if (pUARTHandle->UART_Config.HardwareFlowCtrl == UART_HARDWARE_FLOW_CTRL_DISABLE)
    {
        pUARTHandle->pUARTx->CR3 &= ~(1 << USART_CR3_CTSE);  // Enable CTS flow control
        pUARTHandle->pUARTx->CR3 &= ~(1 << USART_CR3_RTSE);  // Enable RTS flow control
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			u4_UART_SystemClk
//	Name:			-
//	Function:		Get clock for UART peripheral
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static uint32_t u4_UART_SystemClk(UART_Handle_t* pUARTHandle)
{
    uint32_t SystemClk;
    uint32_t UartClk;
    uint32_t Temp;
    uint16_t AHB_Prescaler;
    uint16_t APBx_Prescaler;
    AHB_Prescaler = (uint16_t)0;
    APBx_Prescaler = (uint16_t)0;
    SystemClk = (uint32_t)0;
    UartClk = (uint32_t)0;
    Temp = (uint32_t)0;

    // 1. Get system clock of AHB bus
    // Assuming the system clock source is HSI (High-Speed Internal) or HSE (High-Speed External)
    uint32_t clk_src = (RCC->CFGR >> 2) & 0x3;  // Bits 2 and 3 determine the system clock source
    if (clk_src == SYSCLK_SRC_HSI)
    {
        // HSI oscillator used as system clock
        SystemClk = HSI_CLOCK_SPEED;  // HSI frequency is 8 MHz
    }
    else if (clk_src == SYSCLK_SRC_HSE)
    {
        // HSE oscillator used as system clock
        while (1)
        {
        }
    }
    else if (clk_src == SYSCLK_SRC_PLL)
    {
        while (1)
        {
        }
    }
    // 2. Get AHB prescaler
    Temp = (RCC->CFGR >> 4) & 0x0F;  // Bits 4 to 7 determine the AHB prescaler
    if (Temp < 8U)
    {
        AHB_Prescaler = 1;
    }
    else
    {
        AHB_Prescaler = AHBPrescTable[(RCC->CFGR >> 4U) & 0x07U];
    }

    if (pUARTHandle->pUARTx == USART1)
    {
        // 3. Get APB1 Prescaler
        Temp = (RCC->CFGR >> 8U) & 0x07U;  // Bits 4 to 7 determine the AHB prescaler
        if (Temp < 4U)
        {
            APBx_Prescaler = 1;
        }
        else
        {
            APBx_Prescaler = APBPrescTable[(RCC->CFGR >> 8U) & 0x03U];
        }
    }
    else
    {
        // 3. Get APB2 Prescaler
        Temp = (RCC->CFGR >> 11U) & 0x07U;  // Bits 4 to 7 determine the AHB prescaler
        if (Temp < 4U)
        {
            APBx_Prescaler = 1;
        }
        else
        {
            APBx_Prescaler = APBPrescTable[(RCC->CFGR >> 11U) & 0x03U];
        }
    }
    UartClk = SystemClk / (AHB_Prescaler * APBx_Prescaler);
    return UartClk;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			u4_UART_BaudRate_Cal
//	Name:			-
//	Function:		Calcualtion UART_BRR register value based on baud rate and clock frequency
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static uint32_t u4_UART_BaudRate_Cal(uint32_t BaudRate, uint32_t UARTClk)
{
    uint32_t u4_UsartDiv = 0;
    uint32_t u4_UsartDivMantissa = 0;
    uint32_t u4_UsartDivFraction = 0;

    // Calculate USARTDIV
    u4_UsartDiv = UARTClk / (BaudRate * 16);

    // Extract Mantissa and Fraction
    u4_UsartDivMantissa = u4_UsartDiv;
    u4_UsartDivFraction = (u4_UsartDiv - u4_UsartDivMantissa) * 16;

    // Handle rounding issues when Fraction overflows (>= 16)
    if (u4_UsartDivFraction >= 16)
    {
        u4_UsartDivMantissa += 1;
        u4_UsartDivFraction = 0;
    }

    // Combine Mantissa and Fraction into USART_BRR format
    return (u4_UsartDivMantissa << 4) | (u4_UsartDivFraction & 0xF);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_DeInit
//	Name:			-
//	Function:		Deinit UART peripheral
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_DeInit(UART_Handle_t* pUARTHandle)
{
    // Disable the UART peripheral
    pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_UE);  // Disable the UART peripheral
    // Reset the UART peripheral registers to default values
    pUARTHandle->pUARTx->BRR = 0;  // Reset baud rate register
    pUARTHandle->pUARTx->CR1 = 0;  // Reset control register 1
    pUARTHandle->pUARTx->CR2 = 0;  // Reset control register 2
    pUARTHandle->pUARTx->CR3 = 0;  // Reset control register 3
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_SendData
//	Name:			-
//	Function:		Send data using UART peripheral
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_SendData(UART_Handle_t* pUARTHandle, uint8_t* pTxBuffer, uint32_t Len)
{
    uint16_t* pData;
    if (pUARTHandle == NULL || pTxBuffer == NULL || Len == 0)
    {
        return;
    }
    if (pUARTHandle->TxState == UART_TX_BUSY_STATE)
    {
        return;
    }

    pUARTHandle->pTxBuffer = pTxBuffer;
    pUARTHandle->TxLen = Len;
    pUARTHandle->TxState = UART_TX_BUSY_STATE;

    // Enable the UART peripheral
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_UE);  // Enable the UART peripheral
    // Enable TE
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_TE);  // Enable transmitter
    // 1. Write data into USART_DR register wait all data len is sent
    while (pUARTHandle->TxLen > 0)
    {
        // Wait until TXE flag is set in SR register
        while (!(pUARTHandle->pUARTx->SR & (1 << USART_SR_TXE)))
        {
        }
        // Write data to the data register
        if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_9BIT)
        {
            pData = (uint16_t*)(pUARTHandle->pTxBuffer);
            pUARTHandle->pUARTx->DR = (uint32_t)(*pData & (0x01FF));  // 9-bit data
            pUARTHandle->TxLen--;
            pData++;
        }
        else if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_8BIT)
        {
            pUARTHandle->pUARTx->DR = (uint32_t)(*pUARTHandle->pTxBuffer);
            pUARTHandle->TxLen--;
            if (pUARTHandle->TxLen > 0)
            {
                pUARTHandle->pTxBuffer++;
            }
        }

        if (pUARTHandle->TxLen > 0)
        {
            // Wait until TC flag is set in SR register to indicate that all data sent successfully
            while (!(pUARTHandle->pUARTx->SR & (1 << USART_SR_TC)))
            {
            }
        }
    }

    // Clear
    pUARTHandle->pTxBuffer = NULL;
    pUARTHandle->TxLen = 0;
    pUARTHandle->TxState = UART_READY_STATE;
    // Clear TC flag in SR register
    pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_TC);  // Clear TC flag
    // Clear TXE flag in SR register
    pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_TXE);  // Clear TXE flag
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_ReceiveData
//	Name:			-
//	Function:		Receive data from UART peripheral
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_ReceiveData(UART_Handle_t* pUARTHandle, uint8_t* pRxBuffer, uint32_t Len)
{
    uint16_t* pData;
    if (pUARTHandle == NULL || pRxBuffer == NULL || Len == 0)
    {
        return;
    }
    if (pUARTHandle->RxState == UART_RX_BUSY_STATE)
    {
        return;
    }

    pUARTHandle->pRxBuffer = pRxBuffer;
    pUARTHandle->RxLen = Len;
    pUARTHandle->RxState = UART_RX_BUSY_STATE;

    // Enable the UART peripheral
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_UE);  // Enable the UART peripheral
    // Enable RE
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_RE);  // Enable transmitter

    // 1. Write data into USART_DR register wait all data len is sent
    while (pUARTHandle->RxLen > 0)
    {
        // Wait until TXE flag is set in SR register
        while (!(pUARTHandle->pUARTx->SR & (1 << USART_SR_RXNE)))
        {
        }
        // Write data to the data register
        if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_9BIT)
        {
            pData = (uint16_t*)(pUARTHandle->pRxBuffer);
            *pData = (*((uint16_t*)&pUARTHandle->pUARTx->DR) & 0x1FFU);  // 9-bit data
            pUARTHandle->RxLen--;
            pData++;
        }
        else if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_8BIT)
        {
            (*pUARTHandle->pRxBuffer) = *((uint8_t*)&pUARTHandle->pUARTx->DR);
            pUARTHandle->RxLen--;
            if (pUARTHandle->RxLen > 0)
            {
                pUARTHandle->pRxBuffer++;
            }
        }
    }

    // Clear
    pUARTHandle->pRxBuffer = pRxBuffer;
    pUARTHandle->TxLen = Len;
    pUARTHandle->RxState = UART_RX_BUSY_STATE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_SendDataIT
//	Name:			-
//	Function:		Send data using interrupt
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_SendDataIT(UART_Handle_t* pUARTHandle, uint8_t* pTxBuffer, uint32_t Len)
{
    //	uint16_t* pData;
    if (pUARTHandle == NULL || pTxBuffer == NULL || Len == 0)
    {
        return;
    }
    if (pUARTHandle->TxState == UART_TX_BUSY_STATE)
    {
        return;
    }

    pUARTHandle->pTxBuffer = pTxBuffer;
    pUARTHandle->TxLen = Len;
    pUARTHandle->TxState = UART_TX_BUSY_STATE;

    // Check data frame format and set the appropriate ISR
    if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_8BIT)
    {
        pUARTHandle->TxISR = UART_TxISR_8bit;
    }
    else if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_9BIT)
    {
        pUARTHandle->TxISR = UART_TxISR_9bit;
    }
    // Enable the UART peripheral
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_UE);  // Enable the UART peripheral
    // Enable TE
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_TE);  // Enable transmitter
    // Enable TXE Interrupt
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_TXEIE);  // Enable transmitter
    // Enable TC Interrupt
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_TCIE);  // Enable transmitter
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_ReceiveDataIT
//	Name:			-
//	Function:		Receive data in interrupt mode
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_ReceiveDataIT(UART_Handle_t* pUARTHandle, uint8_t* pRxBuffer, uint32_t Len)
{
    if (pUARTHandle == NULL || pRxBuffer == NULL || Len == 0)
    {
        return;
    }
    if (pUARTHandle->RxState == UART_RX_BUSY_STATE)
    {
        return;
    }

    pUARTHandle->pRxBuffer = pRxBuffer;
    pUARTHandle->RxLen = Len;
    pUARTHandle->RxState = UART_RX_BUSY_STATE;

    // Check data frame format and set the appropriate ISR
    if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_8BIT)
    {
        pUARTHandle->RxISR = UART_RxISR_8bit;
    }
    else if (pUARTHandle->UART_Config.DataFormat == UART_DATA_LEN_9BIT)
    {
        pUARTHandle->RxISR = UART_RxISR_9bit;
    }
    // Enable the UART peripheral
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_UE);  // Enable the UART peripheral
    // Enable RE
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_RE);  // Enable transmitter
    // Enable TXE Interrupt
    pUARTHandle->pUARTx->CR1 |= (1 << USART_CR1_RXNEIE);  // Enable transmitter
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_IRQInterruptConfig
//	Name:			-
//	Function:		Set the interrupt configuration for the specified UART peripheral
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
    volatile uint32_t* pReg = NULL;
    pReg = (EnOrDi == ENABLE) ? NVIC_ISER0 : NVIC_ICER0;
    pReg += (IRQNumber >> 5);
    *pReg |= (1UL << (IRQNumber & 0x1F));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_IRQPriorityConfig
//	Name:			-
//	Function:		Set the priority for the specified UART interrupt
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    if (IRQPriority > 255)
    {
        return;
    }

    volatile uint32_t* pReg = NULL;
    pReg = NVIC_IPR0;
    pReg += (IRQNumber >> 2);
    *pReg &= ~(0xFFUL << ((IRQNumber & 0x03) * 8));
    *pReg |= (IRQPriority << (((IRQNumber & 0x03) * 8) + 4));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_IRQHandling
//	Name:			-
//	Function:		Handle UART interrupts
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void UART_IRQHandling(UART_Handle_t* pHandle)
{
    // If TXE flag is set, call the Tx ISR function
    if (pHandle->pUARTx->SR & (1 << USART_SR_TXE) && pHandle->pUARTx->CR1 & (1 << USART_CR1_TXEIE))
    {
        pHandle->TxISR(pHandle);
    }
    // If TC flag is set, call the Tx ISR function
    if (pHandle->pUARTx->SR & (1 << USART_SR_TC) && pHandle->pUARTx->CR1 & (1 << USART_CR1_TCIE))
    {
        pHandle->TxISR(pHandle);
    }
    // If RXNE flag is set, call the Rx ISR function
    if (pHandle->pUARTx->SR & (1 << USART_SR_RXNE) && pHandle->pUARTx->CR1 & (1 << USART_CR1_RXNEIE))
    {
        pHandle->RxISR(pHandle);
    }
    // If ORE flag is set, call the Rx ISR function
    if (pHandle->pUARTx->SR & (1 << USART_SR_ORE) && pHandle->pUARTx->CR1 & (1 << USART_CR1_RXNEIE))
    {
        pHandle->RxISR(pHandle);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_TxISR_8bit
//	Name:			-
//	Function:		Transmit data in 8-bit mode
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////

static void UART_TxISR_8bit(UART_Handle_t* pUARTHandle)
{
    // Check Txbff is empty
    if (pUARTHandle->TxLen > 0)
    {
        pUARTHandle->pUARTx->DR = (uint32_t)(*pUARTHandle->pTxBuffer);
        pUARTHandle->TxLen--;
        if (pUARTHandle->TxLen > 0)
        {
            pUARTHandle->pTxBuffer++;
        }
    }

    // Check all data is sent
    // If TxLen is 0, Check the TC flag (Transmission complete) and set the Tx state to ready
    if (pUARTHandle->TxLen == 0)
    {
        if (pUARTHandle->pUARTx->SR & (1 << USART_SR_TC))
        {
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_TXEIE);  // Disable TXE interrupt
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_TCIE);   // Disable TC interrupt
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_TE);     // Disable the Transmitter
            pUARTHandle->TxState = UART_READY_STATE;              // Set Tx state to ready
            pUARTHandle->pTxBuffer = NULL;                        // Clear Tx buffer pointer
            pUARTHandle->TxLen = 0;                               // Clear Tx length
            pUARTHandle->TxISR = NULL;                            // Clear Tx ISR function pointer
            // Clear TC flag in SR register
            pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_TC);  // Clear TC flag
            // Clear TXE flag in SR register
            pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_TXE);  // Clear TXE flag

            // Call the application event callback function
            UART_ApplicationEventCallback(pUARTHandle);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_TxISR_9bit
//	Name:			-
//	Function:		Transmit data in 9-bit mode
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void UART_TxISR_9bit(UART_Handle_t* pUARTHandle)
{
    // Check Txbff is empty
    if (pUARTHandle->TxLen > 0)
    {
        uint16_t* pData = (uint16_t*)(pUARTHandle->pTxBuffer);
        pUARTHandle->pUARTx->DR = (uint32_t)(*pData & 0x01FF);  // Transmit 9-bit data
        pUARTHandle->TxLen--;
        if (pUARTHandle->TxLen > 0)
        {
            pUARTHandle->pTxBuffer += 2;  // Increment by 2 bytes for 9-bit data
        }
    }

    // Check all data is sent
    // If TxLen is 0, Check the TC flag (Transmission complete) and set the Tx state to ready
    if (pUARTHandle->TxLen == 0)
    {
        if (pUARTHandle->pUARTx->SR & (1 << USART_SR_TC))
        {
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_TXEIE);  // Disable TXE interrupt
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_TCIE);   // Disable TC interrupt
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_TE);     // Disable the Transmitter
            pUARTHandle->TxState = UART_READY_STATE;              // Set Tx state to ready
            pUARTHandle->pTxBuffer = NULL;                        // Clear Tx buffer pointer
            pUARTHandle->TxLen = 0;                               // Clear Tx length
            pUARTHandle->TxISR = NULL;                            // Clear Tx ISR function pointer
            // Clear TC flag in SR register
            pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_TC);  // Clear TC flag
            // Clear TXE flag in SR register
            pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_TXE);  // Clear TXE flag

            // Call the application event callback function
            UART_ApplicationEventCallback(pUARTHandle);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_RxISR_8bit
//	Name:			-
//	Function:		Receive data in 8-bit mode
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void UART_RxISR_8bit(UART_Handle_t* pUARTHandle)
{
    // Check Txbff is empty
    if (pUARTHandle->RxLen > 0)
    {
        *pUARTHandle->pRxBuffer = *((uint8_t*)&pUARTHandle->pUARTx->DR);  // Read data from the data register
        pUARTHandle->RxLen--;
        if (pUARTHandle->RxLen > 0)
        {
            pUARTHandle->pRxBuffer++;
        }
    }

    // Check all data is sent
    // If TxLen is 0, Check the TC flag (Transmission complete) and set the Tx state to ready
    if (pUARTHandle->RxLen == 0)
    {
        pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);  // Disable RXE interrupt
        pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_RE);      // Disable the Transmitter
        pUARTHandle->RxState = UART_READY_STATE;               // Set Tx state to ready
        pUARTHandle->pRxBuffer = NULL;                         // Clear Tx buffer pointer
        pUARTHandle->RxLen = 0;                                // Clear Tx length
        pUARTHandle->RxISR = NULL;                             // Clear Tx ISR function pointer
        // Clear RXNE flag in SR register
        pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_RXNE);  // Clear TXE flag
        // Call the application event callback function
        UART_ApplicationEventCallback(pUARTHandle);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_RxISR_9bit
//	Name:			-
//	Function:		Receive data in 9-bit mode
//
//	Argument:		pUARTHandle - Pointer to UART Handle
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void UART_RxISR_9bit(UART_Handle_t* pUARTHandle)
{
    {
        // Check if Rx buffer is not empty
        if (pUARTHandle->RxLen > 0)
        {
            uint16_t* pData = (uint16_t*)(pUARTHandle->pRxBuffer);
            *pData = (uint16_t)(pUARTHandle->pUARTx->DR & 0x01FF);  // Read 9-bit data from the data register
            pUARTHandle->RxLen--;
            if (pUARTHandle->RxLen > 0)
            {
                pUARTHandle->pRxBuffer += 2;  // Increment by 2 bytes for 9-bit data
            }
        }

        // Check if all data is received
        if (pUARTHandle->RxLen == 0)
        {
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);  // Disable RXNE interrupt
            pUARTHandle->pUARTx->CR1 &= ~(1 << USART_CR1_RE);      // Disable the Receiver
            pUARTHandle->RxState = UART_READY_STATE;               // Set Rx state to ready
            pUARTHandle->pRxBuffer = NULL;                         // Clear Rx buffer pointer
            pUARTHandle->RxLen = 0;                                // Clear Rx length
            pUARTHandle->RxISR = NULL;                             // Clear Rx ISR function pointer
            // Clear RXNE flag in SR register
            pUARTHandle->pUARTx->SR &= ~(1 << USART_SR_RXNE);  // Clear RXNE flag

            // Call the application event callback function
            UART_ApplicationEventCallback(pUARTHandle);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			UART_ApplicationEventCallback
//	Name:			-
//	Function:		Callback function to handle UART application events
//
//	Argument:		pUARTHandle - Pointer to UART handle structure
//
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		This is a weak implementation. The user application may override this function.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
__attribute__((weak)) void UART_ApplicationEventCallback(UART_Handle_t* pUARTHandle)
{
    // This is a weak implementation. The user application may override this function.
}
