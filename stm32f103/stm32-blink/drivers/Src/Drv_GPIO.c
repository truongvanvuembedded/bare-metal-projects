//==================================================================================================
//
// 	File Name		Drv_GPIO.c
//
//	CPU Type		STM32F103C8T6
//	Builder			STM32CUBE_IDE
//					
//	Coding			V.VU    ASTI DANANG
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
#include "Drv_GPIO.h"
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

//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			GPIO_ClockControl
//	Name:			GPIO_ClockControl
//	Function:		Enables or disables the clock for the specified GPIO port
//
//	Argument:		pGPIOx - Pointer to GPIO port
//					EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_ClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi) 
{
	if (EnOrDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_CLOCK_ENABLE();
		} else if (pGPIOx == GPIOB) {
			GPIOB_CLOCK_ENABLE();
		} else if (pGPIOx == GPIOC) {
			GPIOC_CLOCK_ENABLE();
		} else if (pGPIOx == GPIOD) {
			GPIOD_CLOCK_ENABLE();
		} else if (pGPIOx == GPIOE) {
			GPIOE_CLOCK_ENABLE();
		} else if (pGPIOx == GPIOF) {
			GPIOF_CLOCK_ENABLE();
		} else if (pGPIOx == GPIOG) {
			GPIOG_CLOCK_ENABLE();
		}
	} else if (EnOrDi == DISABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_CLOCK_DISABLE();
		} else if (pGPIOx == GPIOB) {
			GPIOB_CLOCK_DISABLE();
		} else if (pGPIOx == GPIOC) {
			GPIOC_CLOCK_DISABLE();
		} else if (pGPIOx == GPIOD) {
			GPIOD_CLOCK_DISABLE();
		} else if (pGPIOx == GPIOE) {
			GPIOE_CLOCK_DISABLE();
		} else if (pGPIOx == GPIOF) {
			GPIOF_CLOCK_DISABLE();
		} else if (pGPIOx == GPIOG) {
			GPIOG_CLOCK_DISABLE();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			GPIO_Init
//	Name:			GPIO_Init
//	Function:		Initializes the specified GPIO pin(s) based on configuration
//
//	Argument:		pGPIOHandle - Pointer to GPIO handle structure
//	Return value:	-
//	Create:			2025.02.18	V.Vu	New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) 
{
	if (pGPIOHandle == NULL) 
	{
		return; // Avoid NULL pointer dereference
	}
	
	uint32_t temp;
	volatile uint32_t* pReg;
	
	// Enable clock for the corresponding GPIO port
	GPIO_ClockControl(pGPIOHandle->pGPIOx, ENABLE);
	
	// Configure the mode of the GPIO pin
	temp = (uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinMode;
	pReg = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber < GPIO_PIN_8) 
		   ? &pGPIOHandle->pGPIOx->CRL 
		   : &pGPIOHandle->pGPIOx->CRH;
	
	*pReg &= ~(0x0F << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x07) * 4));
	*pReg |= (temp << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x07) * 4));
	
	// Configure the speed of the GPIO pin if it is an output or alternate function mode
	if (temp == GPIO_MODE_OUTPUT_PP || temp == GPIO_MODE_OUTPUT_OD || 
		temp == GPIO_MODE_AF_PP || temp == GPIO_MODE_AF_OD) 
	{
		uint32_t speed = (uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed;
		*pReg &= ~(0x03 << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x07) * 4));
		*pReg |= (speed << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x07) * 4));
	}
	
	// Configure input mode with pull-up/pull-down resistors
	if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & GPIO_MODE_IT) != 0) 
	{
		*pReg &= ~(0x0F << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x07) * 4));
		*pReg |= (GPIO_MODE_INPUT_PULL << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x07) * 4));
		
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RISING || 
			pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RISING_FALLING) 
		{
			EXTI->RTSR |= (1UL << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FALLING || 
			pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RISING_FALLING) 
		{
			EXTI->FTSR |= (1UL << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		EXTI->IMR |= (1UL << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	
	// Configure pull-up/pull-down settings
	temp = (uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl;
	if (temp != GPIO_PUPD_NO_PULL) 
	{
		pReg = (temp == GPIO_PUPD_PULL_UP) 
			   ? &pGPIOHandle->pGPIOx->BSRR 
			   : &pGPIOHandle->pGPIOx->BRR;
		*pReg = (1UL << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	
	// Configure alternate function (AF) for certain modes
    temp = (uint32_t) pGPIOHandle->GPIO_PinConfig.GPIO_PinMode;
	if (temp == GPIO_MODE_AF_PP || temp == GPIO_MODE_AF_OD ||
		temp == GPIO_MODE_IT_RISING || temp == GPIO_MODE_IT_FALLING || temp == GPIO_MODE_IT_RISING_FALLING) 
	{
		AFIO_CLOCK_ENABLE();
		
		if (pGPIOHandle->pGPIOx == GPIOA) temp = AFIO_EXTICR_PORT_PA;
		else if (pGPIOHandle->pGPIOx == GPIOB) temp = AFIO_EXTICR_PORT_PB;
		else if (pGPIOHandle->pGPIOx == GPIOC) temp = AFIO_EXTICR_PORT_PC;
		else if (pGPIOHandle->pGPIOx == GPIOD) temp = AFIO_EXTICR_PORT_PD;
		else if (pGPIOHandle->pGPIOx == GPIOE) temp = AFIO_EXTICR_PORT_PE;
		else if (pGPIOHandle->pGPIOx == GPIOF) temp = AFIO_EXTICR_PORT_PF;
		else if (pGPIOHandle->pGPIOx == GPIOG) temp = AFIO_EXTICR_PORT_PG;
		
		pReg = &AFIO->EXTICR[pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber >> 2];
		*pReg &= ~(0x0F << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x03) * 4));
		*pReg |= (temp << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber & 0x03) * 4));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			GPIO_DeInit
//	Name:			GPIO_DeInit
//	Function:		Resets the specified GPIO port to its default state
//
//	Argument:		pGPIOx - Pointer to GPIO port
//	Return value:	-
//	Create:			2025.02.18	V.Vu	New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
	{
		RCC->APB2RSTR |= (1 << 2);
		RCC->APB2RSTR &= ~(1 << 2);
	}
	else if (pGPIOx == GPIOB)
	{
		RCC->APB2RSTR |= (1 << 3);
		RCC->APB2RSTR &= ~(1 << 3);
	}
	else if (pGPIOx == GPIOC)
	{
		RCC->APB2RSTR |= (1 << 4);
		RCC->APB2RSTR &= ~(1 << 4);
	}
	else if (pGPIOx == GPIOD)
	{
		RCC->APB2RSTR |= (1 << 5);
		RCC->APB2RSTR &= ~(1 << 5);
	}
	else if (pGPIOx == GPIOE)
	{
		RCC->APB2RSTR |= (1 << 6);
		RCC->APB2RSTR &= ~(1 << 6);
	}
}
	////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:		GPIO_ReadFromInputPin
//	Name:		GPIO_ReadFromInputPin
//	Function:	Reads the value from the specified GPIO input pin
//
//	Argument:	pGPIOx - Pointer to GPIO port
//				PinNumber - Pin number to read
//	Return value:	uint8_t - Value of the pin (0 or 1)
//	Create:		2025.02.18 V.Vu  New
//	Change:		-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint32_t idr;
	idr = ((pGPIOx->IDR >> PinNumber) & 0x01UL);
	return (uint8_t)idr;
}
	////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:		GPIO_ReadFromInputPort
//	Name:		GPIO_ReadFromInputPort
//	Function:	Reads the value from the entire GPIO port
//
//	Argument:	pGPIOx - Pointer to GPIO port
//	Return value:	uint16_t - Value of the port
//	Create:		2025.02.18 V.Vu  New
//	Change:		-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t idr;
	idr = (uint16_t) ((pGPIOx->IDR) & (uint32_t)(0xFFFF));
	return idr;
}
	////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:		GPIO_WriteToOutputPin
//	Name:		GPIO_WriteToOutputPin
//	Function:	Writes a value to the specified GPIO output pin
//
//	Argument:	pGPIOx - Pointer to GPIO port
//				PinNumber - Pin number to write
//				Value - Value to write (0 or 1)
//	Return value:	-
//	Create:		2025.02.18 V.Vu  New
//	Change:		-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if (Value == ENABLE)
	{
		pGPIOx->BSRR = (1UL << PinNumber);
	}
	else
	{
		pGPIOx->BSRR = ((1UL << PinNumber) << 16);
	}
}
	////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:		GPIO_WriteToOutputPort
//	Name:		GPIO_WriteToOutputPort
//	Function:	Writes a value to the entire GPIO port
//
//	Argument:	pGPIOx - Pointer to GPIO port
//				Value - Value to write to the port
//	Return value:	-
//	Create:		2025.02.18 V.Vu  New
//	Change:		-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = (uint32_t)Value;
}
	////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:		GPIO_ToggleOutputPin
//	Name:		GPIO_ToggleOutputPin
//	Function:	Toggles the state of the specified GPIO output pin
//
//	Argument:	pGPIOx - Pointer to GPIO port
//				PinNumber - Pin number to toggle
//	Return value:	-
//	Create:		2025.02.18 V.Vu  New
//	Change:		-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint32_t odr;
	odr = pGPIOx->ODR;
	odr = ((odr & (1 << PinNumber)) << 16) | (~odr & (1 << PinNumber));
	pGPIOx->BSRR = odr;
}
	////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:		GPIO_IRQInterruptConfig
//	Name:		GPIO_IRQInterruptConfig
//	Function:	Configures the specified IRQ number for the GPIO interrupt
//
//	Argument:	IRQNumber - IRQ number to configure
//				EnOrDi - Enable or Disable (1 or 0)
//	Return value:	-
//	Create:		2025.02.18 V.Vu  New
//	Change:		-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
	volatile uint32_t* pReg = NULL;
	pReg = (EnOrDi == ENABLE) ? NVIC_ISER0 : NVIC_ICER0 ;
	pReg += (IRQNumber >> 5);
	*pReg |= (1UL << (IRQNumber & 0x1F));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Tag:			GPIO_IRQPriorityConfig
//	Name:			GPIO_IRQPriorityConfig
//	Function:		Configures the priority for the specified IRQ number
//
//	Argument:		IRQNumber - IRQ number to configure
//				IRQPriority - Priority level
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
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
//	Tag:			GPIO_IRQHandling
//	Name:			GPIO_IRQHandling
//	Function:		Handles the interrupt for the specified GPIO pin
//
//	Argument:		PinNumber - Pin number that caused the interrupt
//	Return value:	-
//	Create:			2025.02.18 V.Vu  New
//	Change:			-
//	Remarks:		-
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_IRQHandling(uint8_t PinNumber)
{
	uint32_t temp;
	temp = EXTI->PR;

	if (((temp >> PinNumber) & 0X01UL) == 1)
	{
		EXTI->PR |= (1UL << PinNumber);
	}
}