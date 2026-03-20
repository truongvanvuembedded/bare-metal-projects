//==================================================================================================
//
// 	File Name		Drv_I2C.c
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
#include "Drv_I2C.h"
//==================================================================================================
//	Local define
//==================================================================================================
#define MASTER_WRITE		(0U)
#define MASTER_READ			(1U)

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
static void I2C_Enable(I2C_RegDef_t *pI2Cx);
static void I2C_Disable(I2C_RegDef_t *pI2Cx);
static void I2C_EnableIT(I2C_RegDef_t *pI2Cx, uint32_t ITFlag);
static void I2C_DisableIT(I2C_RegDef_t *pI2Cx, uint32_t ITFlag);
static void I2C_SR1_WaitFlagSet(I2C_RegDef_t *pI2Cx, uint32_t Flag);
static void I2C_SR1_WaitFlagReset(I2C_RegDef_t *pI2Cx, uint32_t Flag);
static uint8_t I2C_GetFlagStatus(uint32_t Data, uint32_t Flag);
static uint32_t I2C_GetPCLK1Value(void);
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);\
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static uint8_t I2C_MasterAddressWriteRead(uint8_t SlaveAddr, uint8_t ReadOrWrite);
static void I2C_Reset(I2C_RegDef_t *pI2Cx);
static void I2C_SetACKBit(I2C_RegDef_t *pI2Cx);
static void I2C_ResetACKBit(I2C_RegDef_t *pI2Cx);
static void I2C_ClearSBFlag(I2C_RegDef_t *pI2Cx);
static void clearI2CHandle(I2C_Handle_t *pI2CHandle);
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pi2cHandle, uint8_t Event);
//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_PeriClockControl
//	Name:			-
//	Function:		Enables or disables the clock for the specified I2C peripheral
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pI2Cx == I2C1)
		{
			I2C1_CLOCK_ENABLE();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_CLOCK_ENABLE();
		}
	}
	else
	{
		if (pI2Cx == I2C1)
		{
			I2C1_CLOCK_DISABLE();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_CLOCK_DISABLE();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_Init
//	Name:			-
//	Function:		Initializes the I2C peripheral
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t pclk1;
	uint32_t SerialClk;
	uint32_t temp;

	// 1. Enable the clock for the I2C peripheral
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);
	/*Reset I2C*/
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_SWRST);
	pI2CHandle->pI2Cx->CR1 &= ~(1<< I2C_CR1_SWRST);
	// 2. Disable the I2C peripheral
	I2C_Disable(pI2CHandle->pI2Cx);
	// 3. Configure the I2C peripheral
	// 3.2. Confige Clock Speed mode
	pI2CHandle->pI2Cx->CCR |= (((uint32_t)pI2CHandle->I2C_Config.ClockSpeed) << I2C_CCR_FS);
	// 3.3. Configure Own Address
	pI2CHandle->pI2Cx->OAR1 &= ~(1UL << 15);	// 7-bit addressing mode
	pI2CHandle->pI2Cx->OAR1 |= (pI2CHandle->I2C_Config.Address << 1);
	// 3.4. Configure Duty Cycle
	// 3.5. Configure clock speed
	// 3.5.1. Check if the clock speed is standard mode or fast mode
	// 3.5.2. Get the value of the APB clock
	// 3.5.3. Calculate the value of the FREQ field in CR2 register
	// 3.5.4. Calculate the value of of the CCR field in the CCR register
	// 3..5.5 If the clock speed is fast mode, configure the DUTY bit in the CCR register
	
	pclk1 = I2C_GetPCLK1Value();
	if ((pclk1 < 2000000) || (pclk1 > 50000000))
	{
		return;
	}
	pI2CHandle->pI2Cx->CR2 |= (pclk1 / 1000000);

	if (pI2CHandle->I2C_Config.ClockSpeed == I2C_SCL_SPEED_SM)
	{
		SerialClk = I2C_CLOCK_SM_100KHZ;
		// CCR formula of standard mode
		// T_SCL = 2 * CCR * T_PCLK1
		// CCR = T_SCL / (2 * T_PCLK1)
		// CCR = F_PCLK1 / (2 * F_SCL)
		temp = (pclk1 / (2 * SerialClk));
		pI2CHandle->pI2Cx->CCR |= (temp & 0xFFF);
	}
	else
	{
		SerialClk = I2C_CLOCK_FM_400KHZ;
	}

	// Enable PE
	//I2C_Enable(pI2CHandle->pI2Cx);
	// 3.1. Configure ACK control bit
	//pI2CHandle->pI2Cx->CR1 |= (1UL << I2C_CR1_ACK);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_DeInit
//	Name:			-
//	Function:		Deinitializes the I2C peripheral
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_MasterSendData
//	Name:			-
//	Function:		Sends data from the master to the slave
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//					pTxBuffer - Pointer to transmit buffer
//					Len - Length of data to be sent
//					SlaveAddr - Address of the slave
//					StopFLag - Flag indicate Stop create or not (Repeat start for next transition)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFLag)
{
	uint32_t dummy;
	uint8_t Address;
	
	// Check I2C Null or not
	if (pI2CHandle == NULL || pTxBuffer == NULL || Len == 0)
	{
		return;
	}
	// Check State of I2C busy or not
	// Check len of data Tx 
	if (pI2CHandle->State != I2C_STATE_READY)
	{
		return;
	}
	// Init parameter
	pI2CHandle->pTxBuffer = pTxBuffer;
	pI2CHandle->TxLen = Len;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = (uint32_t)0;
	pI2CHandle->State = I2C_STATE_TX_BUSY;

	// Enable PE
	I2C_Enable(pI2CHandle->pI2Cx);

	// 1. Generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	// 2. Confirm that Start generation is completed by checking SB flag in SR1
	// Cleared by software by reading the SR1 register followed by writing the DR register
	I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_SB);
	I2C_ClearSBFlag(pI2CHandle->pI2Cx);												
	// 3. Send ADD of slave with Read/Write bit (Write = 0, Read = 1)
	Address = I2C_MasterAddressWriteRead(SlaveAddr, MASTER_WRITE);
	pI2CHandle->pI2Cx->DR = (uint32_t) Address;										
	I2C_SR1_WaitFlagReset(pI2CHandle->pI2Cx, I2C_SR1_SB);							
	// 4. Wait untill master send Address completed by checking ADDR bit in SR1
	// This bit is cleared by software reading SR1 register followed reading SR2
	I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_ADDR);
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);
	I2C_SR1_WaitFlagReset(pI2CHandle->pI2Cx, I2C_SR1_ADDR);
	// 5. Send data untill len = 0
	while (pI2CHandle->TxLen > 0)
	{
		I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_TXE);
		pI2CHandle->pI2Cx->DR = (uint32_t) *pI2CHandle->pTxBuffer;
		pI2CHandle->pTxBuffer++;
		pI2CHandle->TxLen--;
	}
	// 6. When send all data successfully. Then generate Stop condition by Checking TXE and BTF == 1 before
	I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_TXE);
	I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_BTF);
	// 7. Create Stop condition
	if (StopFLag == ENABLE)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}
	// Release state of I2C
	pI2CHandle->State = I2C_STATE_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_MasterReceiveData
//	Name:			-
//	Function:		Receives data from the slave to the master
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//					pRxBuffer - Pointer to receive buffer
//					Len - Length of data to be received
//					SlaveAddr - Address of the slave
//					StopFLag - Flag indicate Stop create or not (Repeat start for next transition)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFLag)
{
	uint8_t Address;
	// Check if I2C handle, buffer, or length is invalid
	if (pI2CHandle == NULL || pRxBuffer == NULL || Len == 0)
	{
		return;
	}
	if (pI2CHandle->State != I2C_STATE_READY)
	{
		return;
	}
	pI2CHandle->State = I2C_STATE_RX_BUSY;
	pI2CHandle->RxLen = Len;
	pI2CHandle->pRxBuffer = pRxBuffer;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
	
	// Enable PE
	I2C_Enable(pI2CHandle->pI2Cx);
	I2C_SetACKBit(pI2CHandle->pI2Cx);
	// 1. Create start condition -> Wait SB bit set (SCL low Untill SB bit reset) -> Clear SB bit (SCL High) -> Confirm SB bit is reseted
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_SB);
	I2C_ClearSBFlag(pI2CHandle->pI2Cx);
	// 2. Send address (7bit) + 1bit (R/W) -> Wait ADDR flag set (SCL low) -> Clear ADDR (SCL high) -> Confirm ADDR flag is reset
	Address = I2C_MasterAddressWriteRead(SlaveAddr, MASTER_READ);
	pI2CHandle->pI2Cx->DR = (uint32_t) Address;				
	I2C_SR1_WaitFlagReset(pI2CHandle->pI2Cx, I2C_SR1_SB);
	I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_ADDR);
	// If len data == 1 (Clear ACK bit to create NACK in bus before clear ADDR bit)
	if (pI2CHandle->RxLen == 1)
	{
		I2C_ResetACKBit(pI2CHandle->pI2Cx);
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);
		I2C_SR1_WaitFlagReset(pI2CHandle->pI2Cx, I2C_SR1_ADDR);
		if (StopFLag == ENABLE)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}
		I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_RXNE);
		*pI2CHandle->pRxBuffer = *((uint8_t*) &pI2CHandle->pI2Cx->DR);
		pI2CHandle->State = I2C_STATE_READY;
		pI2CHandle->RxLen = 0;
		pI2CHandle->pRxBuffer = NULL;
		return;
	}
	else{
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);
		I2C_SR1_WaitFlagReset(pI2CHandle->pI2Cx, I2C_SR1_ADDR);
		// 3. Wait RxNE flag is set then begin read data in DR untill len == 0 
		while (pI2CHandle->RxLen > 0)
		{
			if (pI2CHandle->RxLen == 2)
			{
				I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_RXNE);
				I2C_ResetACKBit(pI2CHandle->pI2Cx);
				if (StopFLag == ENABLE)
				{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}
				I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_BTF);
				*pI2CHandle->pRxBuffer = *((uint8_t*) &pI2CHandle->pI2Cx->DR);
				pI2CHandle->pRxBuffer++;
				*pI2CHandle->pRxBuffer = *((uint8_t*) &pI2CHandle->pI2Cx->DR);
				break;
			}
			I2C_SR1_WaitFlagSet(pI2CHandle->pI2Cx, I2C_SR1_RXNE);
			*pI2CHandle->pRxBuffer = *((uint8_t*) &pI2CHandle->pI2Cx->DR);
			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;
		}
	}


	pI2CHandle->State = I2C_STATE_READY;
	pI2CHandle->RxLen = 0;
	pI2CHandle->pRxBuffer = NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_MasterSendDataIT
//	Name:			-
//	Function:		Sends data from the master to the slave using interrupts
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//					pTxBuffer - Pointer to transmit buffer
//					Len - Length of data to be sent
//					SlaveAddr - Address of the slave
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFlag)
{
	// Check parameter and State of I2C handle
	if (pI2CHandle == NULL || pTxBuffer == NULL || Len == 0)
	{
		return;
	}
	if (pI2CHandle->State != I2C_STATE_READY)
	{
		return;
	}
	// Assign parameter to pI2CHandle
	pI2CHandle->pTxBuffer = pTxBuffer;
	pI2CHandle->TxLen = Len;
	pI2CHandle->State = I2C_STATE_TX_BUSY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->Devaddress = SlaveAddr;
	pI2CHandle->StopFlag = StopFlag;
	// 2. Enable PE
	I2C_Enable(pI2CHandle->pI2Cx);

	// 3. Create Start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 4. Enable IT flag in CR2 register
	I2C_EnableIT(pI2CHandle->pI2Cx, I2C_CR2_ITBUFEN);
	I2C_EnableIT(pI2CHandle->pI2Cx, I2C_CR2_ITEVTEN);
	I2C_EnableIT(pI2CHandle->pI2Cx, I2C_CR2_ITERREN);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_MasterReceiveDataIT
//	Name:			-
//	Function:		Receives data from the slave to the master using interrupts
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//					pRxBuffer - Pointer to receive buffer
//					Len - Length of data to be received
//					SlaveAddr - Address of the slave
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFlag)
{
	// Check parameter and State of I2C handle
	if (pI2CHandle == NULL || pRxBuffer == NULL || Len == 0)
	{
		return;
	}
	if (pI2CHandle->State != I2C_STATE_READY)
	{
		return;
	}
	// Assign parameter to pI2CHandle
	pI2CHandle->pRxBuffer = pRxBuffer;
	pI2CHandle->RxLen = Len;
	pI2CHandle->State = I2C_STATE_RX_BUSY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
	pI2CHandle->StopFlag = StopFlag;
	pI2CHandle->Devaddress = SlaveAddr;
	// 2. Enable PE
	I2C_Enable(pI2CHandle->pI2Cx);

	// Handle Address Sent event
	I2C_SetACKBit(pI2CHandle->pI2Cx);
	
	// 3. Create Start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 4. Enable IT flag in CR2 register
	I2C_EnableIT(pI2CHandle->pI2Cx, I2C_CR2_ITBUFEN);
	I2C_EnableIT(pI2CHandle->pI2Cx, I2C_CR2_ITEVTEN);
	I2C_EnableIT(pI2CHandle->pI2Cx, I2C_CR2_ITERREN);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_IRQInterruptConfig
//	Name:			-
//	Function:		Configures the I2C interrupt
//
//	Argument:		IRQNumber - Interrupt request number
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
	volatile uint32_t* pReg = NULL;
	pReg = (EnOrDi == ENABLE) ? NVIC_ISER0 : NVIC_ICER0 ;
	pReg += (IRQNumber >> 5);
	*pReg |= (1UL << (IRQNumber & 0x1F));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_IRQPriorityConfig
//	Name:			-
//	Function:		Configures the priority of the I2C interrupt
//
//	Argument:		IRQNumber - Interrupt request number
//					IRQPriority - Priority of the interrupt
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
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
//	Tag:			I2C_EV_IRQHandling
//	Name:			-
//	Function:		Handles the I2C event interrupt
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint8_t ReadWriteFlag;
	uint8_t address;
	uint32_t dummy;

	// Check for SB (Start Bit) event
	if (I2C_GetFlagStatus(pI2CHandle->pI2Cx->SR1, I2C_SR1_SB) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITEVTEN))
	{
		// Clear SB flag
		I2C_ClearSBFlag(pI2CHandle->pI2Cx);
		// Send slave address
		ReadWriteFlag = ((pI2CHandle->State == I2C_STATE_TX_BUSY) ? MASTER_WRITE :  MASTER_READ);
		address = I2C_MasterAddressWriteRead(pI2CHandle->Devaddress, ReadWriteFlag);
		pI2CHandle->pI2Cx->DR = (uint32_t) address;			
	}

	// Check for ADDR (Address Sent) event
	if (I2C_GetFlagStatus(pI2CHandle->pI2Cx->SR1, I2C_SR1_ADDR) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITEVTEN))
	{
		if ((pI2CHandle->RxLen == 1) && (pI2CHandle->State == I2C_STATE_RX_BUSY))
		{
			I2C_ResetACKBit(pI2CHandle->pI2Cx);
			I2C_ClearADDRFlag(pI2CHandle->pI2Cx);
		}
		else{
			I2C_ClearADDRFlag(pI2CHandle->pI2Cx);
		}
	}

	// Check for TXE (Transmit Buffer Empty) event
	if (I2C_GetFlagStatus(pI2CHandle->pI2Cx->SR1, I2C_SR1_TXE) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITEVTEN) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITBUFEN))
	{
		// Handle Transmit Buffer Empty event
		if (pI2CHandle->TxLen > 0)
		{
			*((uint8_t *) &pI2CHandle->pI2Cx->DR) = *pI2CHandle->pTxBuffer;
			pI2CHandle->pTxBuffer++;
			pI2CHandle->TxLen--;
		}
		
	}
	
	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	if(I2C_GetFlagStatus(pI2CHandle->pI2Cx->SR1, I2C_SR1_BTF) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITEVTEN))
	{
		//BTF flag is set
		if(pI2CHandle->State == I2C_STATE_TX_BUSY)
		{
			//make sure that TXE is also set .
			if(pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_TXE) )
			{
				//BTF, TXE = 1
				if(pI2CHandle->TxLen == 0 )
				{
					//1. generate the STOP condition
					if(pI2CHandle->StopFlag == ENABLE)
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					//2. reset all the member elements of the handle structure.
					clearI2CHandle(pI2CHandle);

					//3. notify the application about transmission complete
					I2C_ApplicationEventCallback(pI2CHandle,I2C_CALLBACK_TX_COMPLETE);

				}
			}

		}
	}
	// Check for RXNE (Receive Buffer Not Empty) event
	if (I2C_GetFlagStatus(pI2CHandle->pI2Cx->SR1, I2C_SR1_RXNE) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITEVTEN) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITBUFEN))
	{
		// Handle Transmit Buffer Empty event
		if (pI2CHandle->RxLen == 1)
		{
			*pI2CHandle->pRxBuffer = *((uint8_t *) &pI2CHandle->pI2Cx->DR);
			pI2CHandle->RxLen--;
		}
		
		if (pI2CHandle->RxLen > 1)
		{
			if (pI2CHandle->RxLen == 2)
			{
				I2C_ResetACKBit(pI2CHandle->pI2Cx);
			}
			*pI2CHandle->pRxBuffer = *((uint8_t *) &pI2CHandle->pI2Cx->DR);
			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;
		}

		// Check if Rxlen == 0
		if (pI2CHandle->RxLen == 0)
		{
			if (pI2CHandle->StopFlag)
			{
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}
			clearI2CHandle(pI2CHandle);
			I2C_ApplicationEventCallback(pI2CHandle, I2C_CALLBACK_RX_COMPLETE);
		}
	}
	// Check for STOPF Stop condition is deteced (Slave) events
	if (I2C_GetFlagStatus(pI2CHandle->pI2Cx->SR1, I2C_SR1_STOPF) && I2C_GetFlagStatus(pI2CHandle->pI2Cx->CR2, I2C_CR2_ITEVTEN))
	{
//		dummy = pI2CHandle->pI2Cx->SR1;
//		pI2CHandle->pI2Cx->CR1 |= (uint32_t)0x00;
//		I2C_ApplicationEventCallback(pI2CHandle, I2C_CALLBACK_STOP_DETECTED);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_ER_IRQHandling
//	Name:			-
//	Function:		Handles the I2C error interrupt
//
//	Argument:		pI2CHandle - Pointer to I2C handle structure
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1,temp2;

	//Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);

	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

		//Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

		//Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

		//Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_Enable
//	Name:			-
//	Function:		Enables the I2C peripheral
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_Enable(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_PE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_Disable
//	Name:			-
//	Function:		Disables the I2C peripheral
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_Disable(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_EnableIT
//	Name:			-
//	Function:		Enables the specified I2C interrupt
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//					ITFlag - Interrupt flag to be enabled
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_EnableIT(I2C_RegDef_t *pI2Cx, uint32_t ITFlag)
{
	pI2Cx->CR2 |= (1 << ITFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_DisableIT
//	Name:			-
//	Function:		Disables the specified I2C interrupt
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//					ITFlag - Interrupt flag to be disabled
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_DisableIT(I2C_RegDef_t *pI2Cx, uint32_t ITFlag)
{
	pI2Cx->CR2 &= ~(1 << ITFlag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_GetFlagStatus
//	Name:			-
//	Function:		Checks the status of the specified I2C flag
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//					Flag - Flag to be checked
//	Return value:	FlagStatus - Status of the flag (SET or RESET)
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8_t I2C_GetFlagStatus(uint32_t Data, uint32_t Flag)
{
	if (Data & (1U << Flag))
	{
		return ENABLE;
	}
	return DISABLE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_Reset
//	Name:			-
//	Function:		Resets the I2C peripheral
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C_Reset(I2C_RegDef_t *pI2Cx)
{
	// Set the SWRST bit to reset the I2C peripheral
	pI2Cx->CR1 |= (1 << 15);
	// Clear the SWRST bit to release the I2C peripheral from reset state
	pI2Cx->CR1 &= ~(1 << 15);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_GetPCLK1Value
//	Name:			-
//	Function:		Gets the current clock value of the APB1 bus
//
//	Argument:		-
//	Return value:	PCLK1Value - Clock value of the APB1 bus
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static uint32_t I2C_GetPCLK1Value(void)
{
	// 1. Check system clock source
	RCC_RegDef_t *pRCC = RCC;
	uint32_t SystemClock;
	uint32_t SystemClock_Value;
	uint32_t AHBPrescaler;
	uint32_t APB1Prescaler;
	uint32_t Pclk1Value;

	SystemClock = pRCC->CFGR & 0x3UL;
	if (SystemClock == SYSCLK_SRC_HSI)
	{
		SystemClock_Value = HSI_CLOCK_SPEED;
	}
	else if (SystemClock == SYSCLK_SRC_HSE)
	{
		while (1);
	}
	else if (SystemClock == SYSCLK_SRC_PLL)
	{
		while (1);
	}
	
	// 2. Get the value of the AHB prescaler
	AHBPrescaler = (pRCC->CFGR >> 4) & 0xFUL;
	if (AHBPrescaler < 8UL)
	{
		AHBPrescaler = 1;
	}
	else
	{
		AHBPrescaler = AHBPrescTable[AHBPrescaler - 8UL];
	}
	// 3. Get the value of the APB1 prescaler
	APB1Prescaler = (pRCC->CFGR >> 8) & 0x7UL;
	if (APB1Prescaler < 8UL)
	{
		APB1Prescaler = 1;
	}
	else
	{
		APB1Prescaler = APBPrescTable[APB1Prescaler - 4];
	}
	// 4. Calculate the value of the PCLK1
	Pclk1Value = (SystemClock_Value / AHBPrescaler) / APB1Prescaler;
	return Pclk1Value;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_GenerateStartCondition
//	Name:			-
//	Function:		Generates the start condition for I2C communication
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_GenerateStopCondition
//	Name:			-
//	Function:		Generates the stop condition for I2C communication
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_SR1_WaitFlagSet
//	Name:			-
//	Function:		Waits until the specified I2C flag is set
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//					Flag - Flag to be checked
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_SR1_WaitFlagSet(I2C_RegDef_t *pI2Cx, uint32_t Flag)
{
	while (!(pI2Cx->SR1 & (1 << Flag)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_SR1_WaitFlagReset
//	Name:			-
//	Function:		Waits until the specified I2C flag is reset
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//					Flag - Flag to be checked
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_SR1_WaitFlagReset(I2C_RegDef_t *pI2Cx, uint32_t Flag)
{
	while (pI2Cx->SR1 & (1 << Flag));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_ClearADDRFlag
//	Name:			-
//	Function:		Clears the ADDR flag in the I2C status register
//					Cleared by software by reading the SR1 register followed by writing the DR register
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead;
	dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_MasterAddressWriteRead
//	Name:			-
//	Function:		Combines the address and the Read/Write bit into an 8-bit value
//
//	Argument:		SlaveAddr - Address of the slave
//					ReadOrWrite - Read or Write bit (0 for Write, 1 for Read)
//	Return value:	AddressWithRW - 8-bit value containing the address and the Read/Write bit
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8_t I2C_MasterAddressWriteRead(uint8_t SlaveAddr, uint8_t ReadOrWrite)
{
	return (SlaveAddr << 1U) | (ReadOrWrite & 0x01U);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_SetACKBit
//	Name:			-
//	Function:		Sets the ACK bit in the I2C control register
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_SetACKBit(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_ResetACKBit
//	Name:			-
//	Function:		Resets the ACK bit in the I2C control register
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_ResetACKBit(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_ClearSBFlag
//	Name:			-
//	Function:		Clears the SB (Start Bit) flag in the I2C status register
//					Cleared by software by reading the SR1 register followed by writing to the DR register
//
//	Argument:		pI2Cx - Pointer to I2C peripheral
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
static void I2C_ClearSBFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead;
	dummyRead = pI2Cx->SR1;
	(void)dummyRead;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			I2C_ApplicationEventCallback
//	Name:			-
//	Function:		Callback function to handle i2c application events
//
//	Argument:		pi2cHandle - Pointer to i2c handle structure
//					
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		This is a weak implementation. The user application may override this function.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pi2cHandle, uint8_t Event)
{
	// This is a weak implementation. The user application may override this function.
}

static void clearI2CHandle(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
	pI2CHandle->RxLen = 0;
	pI2CHandle->State = I2C_STATE_READY;
	pI2CHandle->StopFlag = 0;
	pI2CHandle->Devaddress = 0;
}
