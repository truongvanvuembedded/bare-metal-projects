//==================================================================================================
//
// 	File Name		Drv_I2C.h
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
#ifndef INC_DRV_I2C_H_
#define INC_DRV_I2C_H_

//==================================================================================================
//	Header File
//==================================================================================================
#include "stm32f103c8t6.h"
#include <Drv_I2C.h>
//==================================================================================================
//	Local define
//==================================================================================================
/**
 * @file Drv_I2C.h
 * @brief This file contains the definitions for I2C clock speeds.
 */

#define I2C_SCL_SPEED_SM      (0U)  // Standard mode (100 kHz)
#define I2C_SCL_SPEED_FM      (1U)  // Fast mode (400 kHz)

/**
 * @brief I2C Fast Mode Duty Cycle.
 * 
 * These macros define the duty cycle for I2C fast mode I2C_duty_cycle_in_fast_mode.
 */
#define I2C_FM_DUTY_2         (0U)  // Duty cycle 2
#define I2C_FM_DUTY_16_9      (1U)  // Duty cycle 16/9
/**
 * @brief I2C Clock Speed Configuration.
 * 
 * These macros define the clock speed configuration for I2C.
 */
#define I2C_CLOCK_SM_100KHZ   (100000U)  // Standard mode (100 kHz)
#define I2C_CLOCK_FM_400KHZ   (400000U)  // Fast mode (400 kHz)

/**
 * @brief NVIC IRQ Position Definitions for I2C.
 * 
 * These macros define the IRQ positions for I2C in the NVIC.
 */
#define NVIC_IRQ_POS_I2C1_EV   (31U)  // I2C1 Event Interrupt
#define NVIC_IRQ_POS_I2C1_ER   (32U)  // I2C1 Error Interrupt
#define NVIC_IRQ_POS_I2C2_EV   (33U)  // I2C2 Event Interrupt
#define NVIC_IRQ_POS_I2C2_ER   (34U)  // I2C2 Error Interrupt

/**
 * @brief State of bus I2C.
 */
#define I2C_STATE_READY		(0U)
#define I2C_STATE_TX_BUSY	(1U)
#define I2C_STATE_RX_BUSY	(2U)

/**
 * @brief Event create Interrupt for I2C.
 */
#define I2C_EVENT_START_BIT                (0)  // SB - Start bit sent (Master)
#define I2C_EVENT_ADDRESS	               (1)  // ADDR - Address sent (Master) or matched (Slave)
#define I2C_EVENT_10BIT_HEADER_SENT        (2)  // ADD10 - 10-bit header sent (Master)
#define I2C_EVENT_STOP_RECEIVED            (3)  // STOPF - Stop received (Slave)
#define I2C_EVENT_TRANSFER_FINISHED        (4)  // BTF - Data byte transfer finished
#define I2C_EVENT_RX_BUFFER_NOT_EMPTY      (5)  // RxNE - Receive buffer not empty
#define I2C_EVENT_TX_BUFFER_EMPTY          (6)  // TxE - Transmit buffer empty

/**
 * @brief Callback event defines
 */
#define I2C_CALLBACK_TX_COMPLETE           (16) // Transmission complete
#define I2C_CALLBACK_RX_COMPLETE           (17) // Reception complete
#define I2C_CALLBACK_ERROR                 (18) // Error callback
#define I2C_CALLBACK_STOP_DETECTED         (19) // Stop detecd callback

#define I2C_ERROR_BERR  3
#define I2C_ERROR_ARLO  4
#define I2C_ERROR_AF    5
#define I2C_ERROR_OVR   6
#define I2C_ERROR_TIMEOUT 7

//==================================================================================================
//	Struct Template
//==================================================================================================
typedef struct {
	 uint8_t ClockSpeed;       /*!< Specifies the clock frequency.
									  This parameter must be set to a value lower than 400kHz */
	
	  uint8_t DutyCycle;        /*!< Specifies the I2C fast mode duty cycle.
									  This parameter can be a value of @ref I2C_duty_cycle_in_fast_mode */
	
	  uint8_t Address;      /*!< Specifies the first device own address.
									  This parameter can be a 7-bit */
} I2C_Config_t;

typedef struct {
	I2C_RegDef_t *pI2Cx;        // Holds the base address of I2C peripheral
	I2C_Config_t I2C_Config;    // Holds I2C configuration settings
	uint8_t *pTxBuffer;           // To store the app. Tx buffer address
	uint8_t *pRxBuffer;           // To store the app. Rx buffer address
	uint32_t TxLen;               // To store Tx length
	uint32_t RxLen;               // To store Rx length
	uint32_t RxSize;
	uint8_t State;              // To store Tx communication state
	uint8_t Devaddress;     /*!< I2C Target device address                */
	uint8_t StopFlag;
} I2C_Handle_t;
//==================================================================================================
//	Function prototype declaration
//==================================================================================================
/* SPI Clock Control Functions */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

/* I2C Initialization and Deinitialization Functions */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/* I2C Data Send and Receive Functions */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFLag);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFLag);

/* I2C Data Send and Receive Functions with Interrupts */
void I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFlag);
void I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t StopFlag);

/* I2C IRQ (Interrupt) Configuration and ISR Handling Functions */
// Interrupt config
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

#endif /* INC_DRV_I2C_H_ */
