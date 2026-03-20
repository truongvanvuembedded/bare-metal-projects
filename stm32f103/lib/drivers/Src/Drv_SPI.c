//==================================================================================================
//
// 	File Name		Drv_SPI.c
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
#include "Drv_SPI.h"
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
static void SPI_TxISR_16bit(SPI_Handle_t *pSPIHandle);
static void SPI_TxISR_8bit(SPI_Handle_t *pSPIHandle);
static void SPI_RxISR_16bit(SPI_Handle_t *pSPIHandle);
static void SPI_RxISR_8bit(SPI_Handle_t *pSPIHandle);
static void SPI_Enable(SPI_RegDef_t *pSPIx);
static void SPI_Disable(SPI_RegDef_t *pSPIx);
static void SPI_EnableIT(SPI_RegDef_t *pSPIx, uint32_t ITFlag);
static void SPI_DisableIT(SPI_RegDef_t *pSPIx, uint32_t ITFlag);
static void SPI_WaitFlagSet(SPI_RegDef_t *pSPIx, uint32_t Flag);
static void SPI_WaitFlagReset(SPI_RegDef_t *pSPIx, uint32_t Flag);

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle);
//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_PeriClockControl
//	Name:			-
//	Function:		Enables or disables the clock for the specified SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_CLOCK_ENABLE();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_CLOCK_ENABLE();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_CLOCK_ENABLE();
		}
	}
	else
	{
		if (pSPIx == SPI1)
		{
			SPI1_CLOCK_DISABLE();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_CLOCK_DISABLE();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_CLOCK_DISABLE();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_Init
//	Name:			-
//	Function:		Initializes the SPI peripheral
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// Enable clock for the SPI peripheral
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
	// Configure the SPI_CR1 register
	uint32_t tempreg = 0UL;

	// 1. Configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	// 2. Configure the bus configuration
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		// BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// BIDI mode should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SI_RXONLY)
	{
		// BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		// RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the SPI serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	// 4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// 5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	// 6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	// 7. Configure the SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	// Config to avoid MODF error
	if (pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_SOFTWARE)
	{
		// Set SSI bit to avoid MODF error
		tempreg |= (1 << SPI_CR1_SSI);
	}
	pSPIHandle->pSPIx->CR1 = tempreg;
	
	if (pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_HARDWARE)
	{
		// Enable SSOE bit to avoid MODF error
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_DeInit
//	Name:			-
//	Function:		Deinitializes the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	// Implementation here
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_SendData
//	Name:			-
//	Function:		Sends data through the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					pTxBuffer - Pointer to transmission buffer
//					Len - Length of data to be sent
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_SendData(SPI_Handle_t *pSPIx_handle, uint8_t *pBuffer, uint32_t Len)
{
	// Check SPI Null or not
	if (pSPIx_handle == NULL || pBuffer == NULL || Len == 0)
	{
		return;
	}
	// Check State of SPI busy or not
	// Check len of data Tx 
	if (pSPIx_handle->State == EM_SPI_STATE_TX_BUSY)
	{
		return;
	}
	// Init parameter
	pSPIx_handle->pTxBuffer = pBuffer;
	pSPIx_handle->TxLen = Len;
	pSPIx_handle->pRxBuffer = NULL;
	pSPIx_handle->RxLen = (uint32_t)0;
	pSPIx_handle->State = EM_SPI_STATE_TX_BUSY;

	// Enable SPI peripheral
	SPI_Enable(pSPIx_handle->pSPIx);

	// 1.Check SPI DFF is 8-bit or 16-bit
	if (pSPIx_handle->SPIConfig.SPI_DFF == SPI_DFF_8BITS)
	{
		// 2. Check SPI mode is slave or master
		if ((pSPIx_handle->SPIConfig.SPI_DeviceMode == SPI_DEVICE_MODE_SLAVE) || (Len == (uint32_t)1))
		{
			pSPIx_handle->pSPIx->DR = (uint32_t) *pSPIx_handle->pTxBuffer;
			pSPIx_handle->pTxBuffer++;
			pSPIx_handle->TxLen--;
		}
		while (pSPIx_handle->TxLen > 0)
		{
			// Wait until TXE is set
			if (pSPIx_handle->pSPIx->SR & (1UL << SPI_SR_TXE))
			{
				pSPIx_handle->pSPIx->DR = (uint32_t) *((uint8_t *)pSPIx_handle->pTxBuffer);
				pSPIx_handle->pTxBuffer++;
				pSPIx_handle->TxLen--;
			}
			// countin timer
		}
	}
	else if (pSPIx_handle->SPIConfig.SPI_DFF == SPI_DFF_16BITS)
	{
		// 2. Check SPI mode is slave or master
		if ((pSPIx_handle->SPIConfig.SPI_DeviceMode == SPI_DEVICE_MODE_SLAVE) || (Len == (uint32_t)1))
		{
			uint16_t data = *((uint16_t *)pSPIx_handle->pTxBuffer);
			data = (data >> 8) | (data << 8); // Swap bytes

			pSPIx_handle->pSPIx->DR = (uint32_t)data;
			pSPIx_handle->pTxBuffer += sizeof(uint16_t);
			pSPIx_handle->TxLen -= (pSPIx_handle->TxLen == 1) ? sizeof(uint8_t) : sizeof(uint16_t);
		}
		while (pSPIx_handle->TxLen > 0)
		{
			// Wait until TXE is set
			if (pSPIx_handle->pSPIx->SR & (1UL << SPI_SR_TXE))
			{
				uint16_t data = *((uint16_t *)pSPIx_handle->pTxBuffer);
				data = (data >> 8) | (data << 8); // Swap bytes
				
				pSPIx_handle->pSPIx->DR = (uint32_t)data;
				pSPIx_handle->pTxBuffer += sizeof(uint16_t);
				pSPIx_handle->TxLen -= (pSPIx_handle->TxLen == 1) ? sizeof(uint8_t) : sizeof(uint16_t);
			}
		}
	}

	// Init parameter
	pSPIx_handle->pTxBuffer = NULL;
	pSPIx_handle->TxLen = (uint32_t)0;
	pSPIx_handle->State = EM_SPI_STATE_READY;

	// Check end of transmission
	SPI_WaitFlagSet(pSPIx_handle->pSPIx, SPI_SR_TXE);
	SPI_WaitFlagReset(pSPIx_handle->pSPIx, SPI_SR_BSY);
	// Disable SPI peripheral
	SPI_Disable(pSPIx_handle->pSPIx);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_ReceiveData
//	Name:			-
//	Function:		Receives data through the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					pRxBuffer - Pointer to reception buffer
//					Len - Length of data to be received
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_ReceiveData(SPI_Handle_t *pSPIx_handle, uint8_t *pRxBuffer, uint32_t Len)
{
	// Check SPI Null or not
	if (pSPIx_handle == NULL)
	{
		return;
	}
	// Check State of SPI busy or not
	// Check len of data Tx 
	if (pSPIx_handle->State == EM_SPI_STATE_RX_BUSY || Len == 0)
	{
		return;
	}
	// Init parameter
	pSPIx_handle->pTxBuffer = NULL;
	pSPIx_handle->TxLen = (uint32_t)0;
	pSPIx_handle->pRxBuffer = pRxBuffer;
	pSPIx_handle->RxLen = Len;
	pSPIx_handle->State = EM_SPI_STATE_RX_BUSY;

	// Enable SPI peripheral
	SPI_Enable(pSPIx_handle->pSPIx);

	// 1.Check SPI DFF is 8-bit or 16-bit
	if (pSPIx_handle->SPIConfig.SPI_DFF == SPI_DFF_8BITS)
	{
		while (pSPIx_handle->RxLen > 0)
		{
			// Check RXNE is set or not
			if (pSPIx_handle->pSPIx->SR & (1UL << SPI_SR_RXNE))
			{
				*pSPIx_handle->pRxBuffer = (uint8_t)pSPIx_handle->pSPIx->DR;
				pSPIx_handle->pRxBuffer++;
				pSPIx_handle->RxLen--;
			}
			// If RXNE is not set,Begin counting timer.
		}
	}
	else if (pSPIx_handle->SPIConfig.SPI_DFF == SPI_DFF_16BITS)
	{
		while (pSPIx_handle->RxLen > (uint32_t)0)
		{
			// Check RXNE is set or not
			if (pSPIx_handle->pSPIx->SR & (1UL << SPI_SR_RXNE))
			{
				*pSPIx_handle->pRxBuffer = (uint16_t)pSPIx_handle->pSPIx->DR;
				pSPIx_handle->pRxBuffer += sizeof(uint16_t);
				pSPIx_handle->RxLen -= sizeof(uint16_t);
			}
			// If RXNE is not set,Begin counting timer.
		}
	}

	// Init parameter
	pSPIx_handle->pRxBuffer = NULL;
	pSPIx_handle->RxLen = (uint32_t)0;
	pSPIx_handle->State = EM_SPI_STATE_READY;
	// Check end of transmission
	SPI_WaitFlagSet(pSPIx_handle->pSPIx, SPI_SR_RXNE);
	SPI_WaitFlagReset(pSPIx_handle->pSPIx, SPI_SR_BSY);

	// Disable SPI peripheral
	SPI_Disable(pSPIx_handle->pSPIx);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_SendDataIT
//	Name:			-
//	Function:		This function handles sending data using SPI in interrupt mode.
//
//	Argument:		pSPIHandle - Pointer to the SPI handle structure
//					pTxBuffer - Pointer to the transmission buffer
//					Len - Length of the data to be sent
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	// Check if the SPI handle or Tx buffer is NULL or if the length is less than 1
	if (pSPIHandle == NULL || pTxBuffer == NULL || Len == 0)
	{
		return;
	}
	// Check if the SPI is busy or not
	if (pSPIHandle->State == EM_SPI_STATE_TX_BUSY)
	{
		return;
	}

	// Initialize the handle parameters
	pSPIHandle->pTxBuffer = pTxBuffer;
	pSPIHandle->TxLen = Len;
	pSPIHandle->State = EM_SPI_STATE_TX_BUSY;

	// Init Rx buffer and Rx length
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;

	// Check data frame format and set the appropriate ISR
	if (pSPIHandle->SPIConfig.SPI_DFF == SPI_DFF_8BITS)
	{
		pSPIHandle->TxISR = SPI_TxISR_8bit;
	}
	else if (pSPIHandle->SPIConfig.SPI_DFF == SPI_DFF_16BITS)
	{
		pSPIHandle->TxISR = SPI_TxISR_16bit;
	}
	// Enable SPI peripheral
	SPI_Enable(pSPIHandle->pSPIx);

	// Enable the TXEIE control bit to get an interrupt whenever TXE flag is set in SR
	SPI_EnableIT(pSPIHandle->pSPIx, SPI_CR2_TXEIE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_ReceiveDataIT
//	Name:			-
//	Function:		This function handles receiving data using SPI in interrupt mode.
//
//	Argument:		pSPIHandle - Pointer to the SPI handle structure
//					pRxBuffer - Pointer to the reception buffer
//					Len - Length of the data to be received
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	// Check if the SPI handle or Tx buffer is NULL or if the length is less than 1
	if (pSPIHandle == NULL || pRxBuffer == NULL || Len == 0)
	{
		return;
	}
	// Check if the SPI is busy or not
	if (pSPIHandle->State == EM_SPI_STATE_RX_BUSY)
	{
		return;
	}

	// Initialize the handle parameters
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;

	// Init Rx buffer and Rx length
	pSPIHandle->pRxBuffer = pRxBuffer;
	pSPIHandle->RxLen = Len;
	pSPIHandle->State = EM_SPI_STATE_RX_BUSY;

	// Check data frame format and set the RxISR function
	if (pSPIHandle->SPIConfig.SPI_DFF == SPI_DFF_8BITS)
	{
		pSPIHandle->RxISR = SPI_RxISR_8bit;
	}
	else if (pSPIHandle->SPIConfig.SPI_DFF == SPI_DFF_16BITS)
	{
		pSPIHandle->RxISR = SPI_RxISR_16bit;
	}

	// Enable SPI peripheral
	SPI_Enable(pSPIHandle->pSPIx);

	// Enable the TXEIE control bit to get an interrupt whenever TXE flag is set in SR
	SPI_EnableIT(pSPIHandle->pSPIx, SPI_CR2_RXNEIE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_IRQInterruptConfig
//	Name:			-
//	Function:		Configures the SPI interrupt
//
//	Argument:		IRQNumber - Interrupt request number
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
	volatile uint32_t* pReg = NULL;
	pReg = (EnOrDi == ENABLE) ? NVIC_ISER0 : NVIC_ICER0 ;
	pReg += (IRQNumber >> 5);
	*pReg |= (1UL << (IRQNumber & 0x1F));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_IRQPriorityConfig
//	Name:			-
//	Function:		Configures the priority of the SPI interrupt
//
//	Argument:		IRQNumber - Interrupt request number
//					IRQPriority - Priority of the interrupt
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
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
//	Tag:			SPI_IRQHandling
//	Name:			-
//	Function:		Handles the SPI interrupt
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	if ((((pSPIHandle->pSPIx->CR2) & (1UL << SPI_CR2_TXEIE)) != 0) && ((pSPIHandle->pSPIx->SR & (1UL << SPI_SR_TXE)) != 0))
	{
		if (pSPIHandle->TxISR != NULL)
		{
			pSPIHandle->TxISR(pSPIHandle);
		}
	}

	if (((pSPIHandle->pSPIx->CR2 & (1UL << SPI_CR2_RXNEIE)) != 0) && ((pSPIHandle->pSPIx->SR & (1UL << SPI_SR_RXNE)) != 0))
	{
		if (pSPIHandle->RxISR != NULL)
		{
			pSPIHandle->RxISR(pSPIHandle);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_TxISR_8bit
//	Name:			-
//	Function:		Handles the SPI transmit interrupt
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_TxISR_8bit(SPI_Handle_t *pSPIHandle)
{
	*(volatile uint8_t*)&pSPIHandle->pSPIx->DR = *((uint8_t *)pSPIHandle->pTxBuffer);
	pSPIHandle->pTxBuffer++;
	pSPIHandle->TxLen--;
	if (pSPIHandle->TxLen == 0)
	{
		// Wait TSE flag is set
		SPI_WaitFlagSet(pSPIHandle->pSPIx, SPI_SR_TXE);
		// Check end of transmission
		SPI_WaitFlagReset(pSPIHandle->pSPIx, SPI_SR_BSY);
		// Disable TXEIE
		SPI_DisableIT(pSPIHandle->pSPIx, SPI_CR2_TXEIE);
		// Disable SPI peripheral
		SPI_Disable(pSPIHandle->pSPIx);
		// Reset the TxISR function and State
		pSPIHandle->TxISR = NULL;
		pSPIHandle->State = EM_SPI_STATE_READY;
		pSPIHandle->pTxBuffer = NULL;
		// Call the application callback
		SPI_ApplicationEventCallback(pSPIHandle);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_TxISR_16bit
//	Name:			-
//	Function:		Handles the SPI transmit interrupt
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_TxISR_16bit(SPI_Handle_t *pSPIHandle)
{
	uint16_t data = *((uint16_t*)pSPIHandle->pTxBuffer);
	data = (data >> 8) | (data << 8); // Swap bytes

	*(uint16_t*)&pSPIHandle->pSPIx->DR = data;
	pSPIHandle->pTxBuffer += sizeof(uint16_t);
	pSPIHandle->TxLen -= (pSPIHandle->TxLen == 1) ? sizeof(uint8_t) : sizeof(uint16_t);
	if (pSPIHandle->TxLen == 0)
	{
		// Wait TSE flag is set
		SPI_WaitFlagSet(pSPIHandle->pSPIx, SPI_SR_TXE);
		// Check end of transmission
		SPI_WaitFlagReset(pSPIHandle->pSPIx, SPI_SR_BSY);
		// Disable TXEIE
		SPI_DisableIT(pSPIHandle->pSPIx, SPI_CR2_TXEIE);
		// Disable SPI peripheral
		SPI_Disable(pSPIHandle->pSPIx);
		// Reset the TxISR function and State
		pSPIHandle->TxISR = NULL;
		pSPIHandle->State = EM_SPI_STATE_READY;
		pSPIHandle->pTxBuffer = NULL;
		// Call the application callback
		SPI_ApplicationEventCallback(pSPIHandle);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_RxISR_8bit
//	Name:			-
//	Function:		Handles the SPI receive interrupt
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_RxISR_8bit(SPI_Handle_t *pSPIHandle)
{
	*pSPIHandle->pRxBuffer = (uint8_t)pSPIHandle->pSPIx->DR;
	pSPIHandle->pRxBuffer++;
	pSPIHandle->RxLen--;
	if (pSPIHandle->RxLen == 0)
	{
		// Check end of transmission
		SPI_WaitFlagReset(pSPIHandle->pSPIx, SPI_SR_BSY);
		// Disable RXNEIE
		SPI_DisableIT(pSPIHandle->pSPIx, SPI_CR2_RXNEIE);
		// Disable SPI peripheral
		SPI_Disable(pSPIHandle->pSPIx);
		// Reset the RxISR function and State
		pSPIHandle->RxISR = NULL;
		pSPIHandle->State = EM_SPI_STATE_READY;
		// Call the application callback
		SPI_ApplicationEventCallback(pSPIHandle);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_RxISR_16bit
//	Name:			-
//	Function:		Handles the SPI receive interrupt
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_RxISR_16bit(SPI_Handle_t *pSPIHandle)
{
	*(uint16_t*)pSPIHandle->pRxBuffer = (*(uint16_t*)&pSPIHandle->pSPIx->DR & (uint16_t)0x00FFUL);
	pSPIHandle->pRxBuffer += sizeof(uint16_t);
	pSPIHandle->RxLen -= sizeof(uint16_t);
	if (pSPIHandle->RxLen == 0)
	{
		// Check end of transmission
		SPI_WaitFlagReset(pSPIHandle->pSPIx, SPI_SR_BSY);
		// Disable RXNEIE
		SPI_DisableIT(pSPIHandle->pSPIx, SPI_CR2_RXNEIE);
		// Disable SPI peripheral
		SPI_Disable(pSPIHandle->pSPIx);
		// Reset the RxISR function and State
		pSPIHandle->RxISR = NULL;
		pSPIHandle->State = EM_SPI_STATE_READY;
		// Call the application callback
		SPI_ApplicationEventCallback(pSPIHandle);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_Enable
//	Name:			-
//	Function:		Enables the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_Enable(SPI_RegDef_t *pSPIx)
{
	if (!(pSPIx->CR1 & (1UL << SPI_CR1_SPE)))
	{
		pSPIx->CR1 |= (1UL << SPI_CR1_SPE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_Disable
//	Name:			-
//	Function:		Disables the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_Disable(SPI_RegDef_t *pSPIx)
{
	if (pSPIx->CR1 & (1UL << SPI_CR1_SPE))
	{
		pSPIx->CR1 &= ~(1UL << SPI_CR1_SPE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_EnableIT
//	Name:			-
//	Function:		Enables the specified interrupt in the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					ITFlag - Interrupt flag to be enabled
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_EnableIT(SPI_RegDef_t *pSPIx, uint32_t ITFlag)
{
	if (!(pSPIx->CR2 & (1UL << ITFlag)))
	{
		pSPIx->CR2 |= (1UL << ITFlag);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_DisableIT
//	Name:			-
//	Function:		Disables the specified interrupt in the SPI peripheral
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					ITFlag - Interrupt flag to be disabled
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_DisableIT(SPI_RegDef_t *pSPIx, uint32_t ITFlag)
{
	if (pSPIx->CR2 & (1UL << ITFlag))
	{
		pSPIx->CR2 &= ~(1UL << ITFlag);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_WaitFlagSet
//	Name:			-
//	Function:		Waits until the specified flag is set in the SPI status register
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					Flag - Flag to wait for
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_WaitFlagSet(SPI_RegDef_t *pSPIx, uint32_t Flag)
{
	while (!(pSPIx->SR & (1UL << Flag)));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_WaitFlagReset
//	Name:			-
//	Function:		Waits until the specified flag is reset in the SPI status register
//
//	Argument:		pSPIx - Pointer to SPI peripheral
//					Flag - Flag to wait for
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SPI_WaitFlagReset(SPI_RegDef_t *pSPIx, uint32_t Flag)
{
	while (pSPIx->SR & (1UL << Flag));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			SPI_ApplicationEventCallback
//	Name:			-
//	Function:		Callback function to handle SPI application events
//
//	Argument:		pSPIHandle - Pointer to SPI handle structure
//					
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		This is a weak implementation. The user application may override this function.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle)
{
	// This is a weak implementation. The user application may override this function.
}
