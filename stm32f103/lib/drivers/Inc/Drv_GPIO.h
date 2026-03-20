//==================================================================================================
//
// 	File Name		Drv_GPIO.h
//
//	CPU Type		STM32F103C8T6
//	Builder			
//						
//	Coding			
//
//	Outline			-
//
//	History			
//==================================================================================================
//	Local Compile Option
//==================================================================================================
#ifndef INC_DRV_GPIO_H_
#define INC_DRV_GPIO_H_

//==================================================================================================
//	Header File
//==================================================================================================
#include "stm32f103c8t6.h"
//==================================================================================================
//	Local define
//==================================================================================================
// Input Mode Definitions (MODE[1:0] = 00, CNF[1:0] = c?c gi? tr? t??ng ?ng)
#define GPIO_MODE_INPUT_ANALOG			(0x00U)	// CNF[1:0] = 00, MODE[1:0] = 00(Input Analog)
#define GPIO_MODE_INPUT_FLOATING		(0x04U)	// CNF[1:0] = 01, MODE[1:0] = 00(Floating input)
#define GPIO_MODE_INPUT_PULL			(0x08U)	// CNF[1:0] = 10, MODE[1:0] = 00(Input with pull-up/pull-down)

// Output Mode Definitions (MODE[1:0] > 00)
#define GPIO_MODE_OUTPUT_PP				(0x01U)	// Output mode: General purpose output, Push-pull
#define GPIO_MODE_OUTPUT_OD				(0x05U)	// Output mode: General purpose output, Open-drain

// Alternate Function Mode Definitions (MODE[1:0] > 00)
#define GPIO_MODE_AF_PP					(0x09U)	// Alternate function output, Push-pull
#define GPIO_MODE_AF_OD					(0x0DU)	// Alternate function output, Open-drain

// Speed Modes
#define GPIO_SPEED_LOW					(0x01U)	// Low Speed
#define GPIO_SPEED_MEDIUM				(0x02U)	// Medium Speed
#define GPIO_SPEED_HIGH					(0x03U)	// High Speed

// Pull Modes
#define GPIO_PUPD_NO_PULL				(0x00U)	// No Pull-up, No Pull-down
#define GPIO_PUPD_PULL_UP				(0x01U)	// Pull-up Resistor
#define GPIO_PUPD_PULL_DOWN				(0x02U)	// Pull-down Resistor

// EXTI Modes
#define GPIO_MODE_IT					(0x10U)	// External Interrupt Mode with Rising edge trigger detection
#define GPIO_MODE_IT_RISING				(0x11U)	// External Interrupt Mode with Rising edge trigger detection
#define GPIO_MODE_IT_FALLING			(0x12U)	// External Interrupt Mode with Falling edge trigger detection
#define GPIO_MODE_IT_RISING_FALLING		(0x13U)	// External Interrupt Mode with Rising/Falling edge trigger detection

// GPIO Pin
// Define GPIO pins 0 to 15
#define GPIO_PIN_0						(0U)	// Pin 0
#define GPIO_PIN_1						(1U)	// Pin 1
#define GPIO_PIN_2						(2U)	// Pin 2
#define GPIO_PIN_3						(3U)	// Pin 3
#define GPIO_PIN_4						(4U)	// Pin 4
#define GPIO_PIN_5						(5U)	// Pin 5
#define GPIO_PIN_6						(6U)	// Pin 6
#define GPIO_PIN_7						(7U)	// Pin 7
#define GPIO_PIN_8						(8U)	// Pin 8
#define GPIO_PIN_9						(9U)	// Pin 9
#define GPIO_PIN_10						(10U)	// Pin 10
#define GPIO_PIN_11						(11U)	// Pin 11
#define GPIO_PIN_12						(12U)	// Pin 12
#define GPIO_PIN_13						(13U)	// Pin 13
#define GPIO_PIN_14						(14U)	// Pin 14
#define GPIO_PIN_15						(15U)	// Pin 15

// AFIO Pin
#define AFIO_EXTICR_PORT_PA				(0x0U)	// 0000: PA[x] pin
#define AFIO_EXTICR_PORT_PB				(0x1U)	// 0001: PB[x] pin
#define AFIO_EXTICR_PORT_PC				(0x2U)	// 0010: PC[x] pin
#define AFIO_EXTICR_PORT_PD				(0x3U)	// 0011: PD[x] pin
#define AFIO_EXTICR_PORT_PE				(0x4U)	// 0100: PE[x] pin
#define AFIO_EXTICR_PORT_PF				(0x5U)	// 0101: PF[x] pin
#define AFIO_EXTICR_PORT_PG				(0x6U)	// 0110: PG[x] pin

// EXTI IRQ Number
#define EXTI0_IRQn						(6U)	// EXTI Line 0
#define EXTI1_IRQn						(7U)	// EXTI Line 1
#define EXTI2_IRQn						(8U)	// EXTI Line 2
#define EXTI3_IRQn						(9U)	// EXTI Line 3
#define EXTI4_IRQn						(10U)	// EXTI Line 4
#define EXTI9_5_IRQn					(23U)	// EXTI Lines 5 to 9
#define EXTI15_10_IRQn					(40U)	// EXTI Lines 10 to 15

//==================================================================================================
//	Struct Template
//==================================================================================================
/* GPIO Pin Configuration Structure */
typedef struct {
	uint8_t	GPIO_PinNumber;				/* Specifies the GPIO pin number (0-15) */
	uint8_t	GPIO_PinMode;				/* Specifies the GPIO pin mode (Input, Output, Alternate Function, Analog) */
	uint8_t	GPIO_PinSpeed;				/* Specifies the speed of the GPIO pin (Low, Medium, High, Very High) */
	uint8_t	GPIO_PinPuPdControl;		/* Specifies the Pull-Up or Pull-Down configuration (No Pull, Pull-Up, Pull-Down) */
} GPIO_PinConfig_t;

/* GPIO Handle Structure */
typedef struct {
	GPIO_RegDef_t *pGPIOx;					/* Pointer to the base address of the GPIO port */
	GPIO_PinConfig_t GPIO_PinConfig;	/* GPIO pin configuration settings */
} GPIO_Handle_t;
//==================================================================================================
//	Function prototype declaration
//==================================================================================================

/* GPIO Clock Control Functions */
void GPIO_ClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

/* GPIO Initialization and Deinitialization Functions */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/* GPIO Read Functions */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

/* GPIO Write Functions */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);

/* GPIO Toggle Function */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/* GPIO IRQ (Interrupt) Configuration Functions */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_DRV_GPIO_H_ */