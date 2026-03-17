//==================================================================================================
//
// 	File Name		Drv_SPI.h
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
#ifndef INC_SPI_H_
#define INC_SPI_H_

//==================================================================================================
//	Header File
//==================================================================================================
#include "stm32f103c8t6.h"
//==================================================================================================
//	Local define
//==================================================================================================
// Possible values from @SPI_DeviceMode
#define SPI_DEVICE_MODE_MASTER      (1U)
#define SPI_DEVICE_MODE_SLAVE       (0U)
// Possible values from @SPI_BusConfig
#define SPI_BUS_CONFIG_FD           (0U)  // 2-line unidirectional data mode selected
#define SPI_BUS_CONFIG_HD           (1U)  // 1-line bidirectional data mode selected
#define SPI_BUS_CONFIG_SI_RXONLY    (2U)  
// Possible values from @SPI_SclkSpeed
#define SPI_SCLK_SPEED_DIV2         (0U)
#define SPI_SCLK_SPEED_DIV4         (1U)
#define SPI_SCLK_SPEED_DIV8         (2U)
#define SPI_SCLK_SPEED_DIV16        (3U)
#define SPI_SCLK_SPEED_DIV32        (4U)
#define SPI_SCLK_SPEED_DIV64        (5U)
#define SPI_SCLK_SPEED_DIV128       (6U)
#define SPI_SCLK_SPEED_DIV256       (7U)
// Possible values from @SPI_DFF
#define SPI_DFF_8BITS               (0U)
#define SPI_DFF_16BITS              (1U)
// Possible values from @SPI_CPOL
#define SPI_CPOL_HIGH               (1U)
#define SPI_CPOL_LOW                (0U)
// Possible values from @SPI_CPHA
#define SPI_CPHA_HIGH               (1U)
#define SPI_CPHA_LOW                (0U)
// Possible values from @SPI_SSM
#define SPI_SSM_SOFTWARE            (1U)
#define SPI_SSM_HARDWARE            (0U)
// Possible values from @SPI_FirstBit
#define SPI_FIRSTBIT_MSB            (0U)
#define SPI_FIRSTBIT_LSB            (1U)
// Possible values from @SPI_State
#define EM_SPI_STATE_READY          (0U)
#define EM_SPI_STATE_TX_BUSY        (1U)
#define EM_SPI_STATE_RX_BUSY        (2U)

// Possible values for SPI IRQ Numbers
#define IRQ_NO_SPI1                (35U)
#define IRQ_NO_SPI2                (36U)
#define IRQ_NO_SPI3                (51U)
//==================================================================================================
//	Struct Template
//==================================================================================================
typedef struct
{
	uint8_t SPI_DeviceMode;       // Possible values from @SPI_DeviceMode
	uint8_t SPI_BusConfig;        // Possible values from @SPI_BusConfig
	uint8_t SPI_SclkSpeed;        // Possible values from @SPI_SclkSpeed
	uint8_t SPI_DFF;              // Possible values from @SPI_DFF
	uint8_t SPI_CPOL;             // Possible values from @SPI_CPOL
	uint8_t SPI_CPHA;             // Possible values from @SPI_CPHA
	uint8_t SPI_SSM;              // Possible values from @SPI_SSM
	uint8_t SPI_FirstBit;           // Possible values from @SPI_FirstBit
} SPI_Config_t;

typedef struct SPI_Handle_t
{
	SPI_RegDef_t *pSPIx;          // This holds the base address of the SPIx(x:0,1,2) peripheral
	SPI_Config_t SPIConfig;       // This holds SPI configuration settings
	uint8_t *pTxBuffer;           // To store the app. Tx buffer address
	uint8_t *pRxBuffer;           // To store the app. Rx buffer address
	uint32_t TxLen;               // To store Tx length
	uint32_t RxLen;               // To store Rx length
	uint8_t State;              // To store Tx communication state
	void (*TxISR)(struct SPI_Handle_t *pSPIHandle); // To store Tx ISR function pointer
	void (*RxISR)(struct SPI_Handle_t *pSPIHandle); // To store Rx ISR function pointer
} SPI_Handle_t;
//==================================================================================================
//	Function prototype declaration
//==================================================================================================
/* SPI Clock Control Functions */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/* SPI Initialization and Deinitialization Functions */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/* SPI Data Send and Receive Functions */
void SPI_SendData(SPI_Handle_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_Handle_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);
/* SPI Data Send and Receive Functions using Interrupts */
void SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);
/* SPI IRQ (Interrupt) Configuration and ISR Handling Functions */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

#endif /* INC_SPI_H_ */