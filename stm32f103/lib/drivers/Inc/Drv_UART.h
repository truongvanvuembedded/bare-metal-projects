//==================================================================================================
//
// 	File Name		Drv_UART.h
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
#ifndef INC_UART_H_
#define INC_UART_H_

//==================================================================================================
//	Header File
//==================================================================================================
#include "stm32f103c8t6.h"
//==================================================================================================
//	Local define
//==================================================================================================

/* 
 * @ UART_mode
 */
#define UART_MODE_TX	(0U)  // Transmit mode
#define UART_MODE_RX	(1U)  // Receive mode
#define UART_MODE_TXRX	(2U)  // Transmit and Receive mode
/* 
 * @ UART_hardware_flow_control
 */
#define UART_HARDWARE_FLOW_CTRL_DISABLE	(0U)  // Hardware flow control disable
#define UART_HARDWARE_CTS_ENABLE		(1U)  // Hardware CTS enable
#define UART_HARDWARE_RTS_ENABLE		(2U)  // Hardware RTS enable
#define UART_HARDWARE_CTS_RTS_ENABLE	(3U)  // Hardware CTS and RTS enable
/* 
 * @ UART_data_format
 */
#define UART_DATA_LEN_8BIT	(0U)  // 8-bit data length
#define UART_DATA_LEN_9BIT	(1U)  // 9-bit data length
/* 
 * @ UART_parity_control
 */
#define UART_PARITY_DISABLE	(0U)  // Parity control disable
#define UART_PARITY_ENABLE	(1U)  // Parity control enable

/* 
 * @ UART_parity_mode
 */
#define UART_PARITY_EVEN_MODE (0U)	// Parity Even mode
#define UART_PARITY_ODD_MODE  (1U)	// Parity Odd mode
/* 
 * @ UART_Stopbit_num
 */
 #define UART_STOP_BIT_1  (0U)	// 1 stop bit
 #define UART_STOP_BIT_05 (1U)	// 0.5 stop bit
 #define UART_STOP_BIT_2  (2U)	// 2 stop bit
 #define UART_STOP_BIT_15 (3U)	// 1.5 stop bit

 /* 
  * @ BaudRate
  */
#define UART_BAUDRATE_2400		(2400U)	// 2400 baud rate
#define UART_BAUDRATE_9600		(9600U)	// 9600 baud rate
#define UART_BAUDRATE_115200	(115200U)	// 115200 baud rate

 /* 
  * @ State of UART
  */
#define UART_READY_STATE		(0U)
#define UART_TX_BUSY_STATE		(1U)
#define UART_RX_BUSY_STATE		(2U)

/* 
 * @ UART_Interrupt_Index
 */
#define UART_IRQ_INDEX_USART1  (37U)  // Interrupt index for USART1
#define UART_IRQ_INDEX_USART2  (38U)  // Interrupt index for USART2
#define UART_IRQ_INDEX_USART3  (39U)  // Interrupt index for USART3
//==================================================================================================
//	Struct Template
//==================================================================================================
typedef struct
{
	uint8_t UARTMode;		/*!< Specifies the UART mode. This parameter can be a value of @ref UART_mode */
	uint8_t DataFormat;	   	/*!< Specifies the data format. This parameter can be a value of @ref UART_data_format */
	uint8_t ParityControl; 	/*!< Specifies the pariry control. This parameter can be a value of @ref UART_parity_control */
	uint8_t ParityMode; 	/*!< Specifies the pariry mode. This parameter can be a value of @ref UART_parity_mode */
	uint8_t StopbitLen; 	/*!< Specifies the number of stop bit. This parameter can be a value of @ref UART_Stopbit_num */
	uint32_t BaudRate;		/*!< Specifies the number of BaudRate. This parameter can be a value of @ref BaudRate */
	uint8_t HardwareFlowCtrl; /*!< Specifies the hardware flow control. This parameter can be a value of @ref UART_hardware_flow_control */
} UART_Config_t;

typedef struct UART_Handle_t
{
	UART_Config_t UART_Config;  // Holds UART configuration settings
	USART_RegDef_t *pUARTx;     // Holds the base address of UART peripheral
	uint8_t *pTxBuffer;        // To store the app. Tx buffer address
	uint8_t *pRxBuffer;        // To store the app. Rx buffer address
	uint32_t TxLen;            // To store Tx length
	uint32_t RxLen;            // To store Rx length
	uint8_t TxState;          // To store Tx communication state
	uint8_t RxState;          // To store Rx communication state
	void (*TxISR)(struct UART_Handle_t *pUARTHandle); // To store Tx ISR function pointer
	void (*RxISR)(struct UART_Handle_t *pUARTHandle); // To store Rx ISR function pointer
} UART_Handle_t;
//==================================================================================================
//	Function prototype declaration
//==================================================================================================
/* UART Clock Control Functions */
void UART_PeriClockControl(UART_Handle_t *pUARTHandle, uint8_t EnOrDi);

/* UART Initialization and Deinitialization Functions */
void UART_Init(UART_Handle_t *pUARTHandle);
void UART_DeInit(UART_Handle_t *pUARTHandle);

/* UART Data Send and Receive Functions */
void UART_SendData(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void UART_ReceiveData(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);
/* UART Data Send and Receive Functions using Interrupts */
void UART_SendDataIT(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void UART_ReceiveDataIT(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);
/* UART IRQ (Interrupt) Configuration and ISR Handling Functions */
void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void UART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void UART_IRQHandling(UART_Handle_t *pHandle);

#endif /* INC_UART_H_ */