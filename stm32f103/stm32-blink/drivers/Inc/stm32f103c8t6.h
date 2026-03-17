/*
 * stm32f103c8t6.h
 *
 *  Created on: Feb 17, 2025
 *      Author: DELL
 */

#ifndef INC_STM32F103C8T6_H_
#define INC_STM32F103C8T6_H_
#include <stdint.h>
/* 
 * Define 
 */
#define _vo			volatile
#define ENABLE		((uint8_t)1)
#define DISABLE		((uint8_t)0)

#define SYSCLK_SRC_HSI    0x00U  /* HSI selected as system clock */
#define SYSCLK_SRC_HSE    0x01U  /* HSE selected as system clock */
#define SYSCLK_SRC_PLL    0x02U  /* PLL selected as system clock */

#define HSI_CLOCK_SPEED    8000000U   /* HSI clock speed is 8 MHz */
/************************************************* Processor Side *************************************************/
/* 
 * NVIC address
 */
#define NVIC_ISER0	((_vo uint32_t*)0xE000E100)  // Interrupt Set-enable Register 0
#define NVIC_ICER0	((_vo uint32_t*)0xE000E180)  // Interrupt Clear-enable Register 0
#define NVIC_ISPR0	((_vo uint32_t*)0xE000E200)  // Interrupt Set-pending Register 0
#define NVIC_ICPR0	((_vo uint32_t*)0xE000E280)  // Interrupt Clear-pending Register 0
#define NVIC_IABR0	((_vo uint32_t*)0xE000E300)  // Interrupt Active Bit Register 0
#define NVIC_IPR0	((_vo uint32_t*)0xE000E400)  // Interrupt Priority Register 0
#define NVIC_STIR	((_vo uint32_t*)0xE000EF00)  // Software Trigger Interrupt Register

/********************************s***************** Memory, Bus and Peripheral Address base *************************************************/
/* 
 * Memory base addresses 
 */
#define FLASH_BASE_ADDR		0x08000000UL  /* Base address for Flash memory */
#define SRAM_BASE_ADDR		0x20000000UL  /* Base address for SRAM */
#define RAM_BASE_ADDR		SRAM_BASE_ADDR  /* Alias for SRAM base address */

/* 
 * Bus base addresses 
 */
#define APB1_BASE_ADDR	0x40000000UL  /* Base address for APB1 bus */
#define APB2_BASE_ADDR	0x40010000UL  /* Base address for APB2 bus */
#define AHB_BASE_ADDR	0x40018000UL  /* Base address for AHB bus */

/* 
 * Peripheral base addresses on APB1 bus 
 */
#define I2C1_BASE_ADDR      (APB1_BASE_ADDR + 0x5400UL)  /* Base address for I2C1 */
#define I2C2_BASE_ADDR      (APB1_BASE_ADDR + 0x5800UL)  /* Base address for I2C2 */
#define USART2_BASE_ADDR    (APB1_BASE_ADDR + 0x4400UL)  /* Base address for USART2 */
#define USART3_BASE_ADDR    (APB1_BASE_ADDR + 0x4800UL)  /* Base address for USART3 */
#define SPI2_BASE_ADDR      (APB1_BASE_ADDR + 0x3800UL)  /* Base address for SPI2/I2S */
#define SPI3_BASE_ADDR      (APB1_BASE_ADDR + 0x3C00UL)  /* Base address for SPI3/I2S */
#define UART4_BASE_ADDR      (APB1_BASE_ADDR + 0x4C00UL)  /* Base address for USART4 */
#define UART5_BASE_ADDR      (APB1_BASE_ADDR + 0x5000UL)  /* Base address for USART5 */
/* 
 * Peripheral base addresses on APB2 bus 
 */
#define GPIOA_BASE_ADDR     (APB2_BASE_ADDR + 0x0800UL)  /* Base address for GPIOA */
#define GPIOB_BASE_ADDR     (APB2_BASE_ADDR + 0x0C00UL)  /* Base address for GPIOB */
#define GPIOC_BASE_ADDR     (APB2_BASE_ADDR + 0x1000UL)  /* Base address for GPIOC */
#define GPIOD_BASE_ADDR     (APB2_BASE_ADDR + 0x1400UL)  /* Base address for GPIOD */
#define GPIOE_BASE_ADDR     (APB2_BASE_ADDR + 0x1800UL)  /* Base address for GPIOE */
#define GPIOF_BASE_ADDR     (APB2_BASE_ADDR + 0x1C00UL)  /* Base address for GPIOF */
#define GPIOG_BASE_ADDR     (APB2_BASE_ADDR + 0x2000UL)  /* Base address for GPIOG */
#define AFIO_BASE_ADDR      (APB2_BASE_ADDR + 0x0000UL)  /* Base address for AFIO (Alternate Function I/O) */
#define EXTI_BASE_ADDR      (APB2_BASE_ADDR + 0x0400UL)  /* Base address for EXTI */
#define USART1_BASE_ADDR    (APB2_BASE_ADDR + 0x3800UL)  /* Base address for USART1 */
#define SPI1_BASE_ADDR      (APB2_BASE_ADDR + 0x3000UL)  /* Base address for SPI1 */

/* 
 * Peripheral base addresses on AHB bus 
 */
#define RCC_BASE_ADDR	(AHB_BASE_ADDR + 0x9000UL)  /* Base address for Reset and Clock Control (RCC) */

/************************************************* Peripheral Register Structure *************************************************/
/* 
 * GPIO Registers Struct Definition
 */
typedef struct {
    _vo uint32_t CRL;	/* GPIOx Control Register Low */
    _vo uint32_t CRH;	/* GPIOx Control Register High */
    _vo uint32_t IDR;	/* GPIOx Input Data Register */
    _vo uint32_t ODR;	/* GPIOx Output Data Register */
    _vo uint32_t BSRR;	/* GPIOx Bit Set/Reset Register */
    _vo uint32_t BRR;	/* GPIOx Bit Reset Register */
    _vo uint32_t LCKR;	/* GPIOx Lock Register */
} GPIO_RegDef_t;

/* 
 * RCC Registers Struct Definition
 */
typedef struct {
    _vo uint32_t CR;		/* RCC Control Register */
    _vo uint32_t CFGR;		/* RCC Configuration Register */
    _vo uint32_t CIR;		/* RCC Clock Interrupt Register */
    _vo uint32_t APB2RSTR;	/* RCC APB2 Peripheral Reset Register */
    _vo uint32_t APB1RSTR;	/* RCC APB1 Peripheral Reset Register */
    _vo uint32_t AHBENR;	/* RCC AHB Peripheral Clock Enable Register */
    _vo uint32_t APB2ENR;	/* RCC APB2 Peripheral Clock Enable Register */
    _vo uint32_t APB1ENR;	/* RCC APB1 Peripheral Clock Enable Register */
    _vo uint32_t BDCR;		/* RCC Backup Domain Control Register */
    _vo uint32_t CSR;		/* RCC Control/Status Register */
} RCC_RegDef_t;


/* AFIO Pin Configuration Structure */
typedef struct {
    _vo uint32_t EVCR;		// Event Control Register (0x00)
    _vo uint32_t MAPR;		// AF remap and debug I/O configuration register (0x04)
    _vo uint32_t EXTICR[4];	// External interrupt configuration registers (0x08 - 0x14)
    _vo uint32_t MAPR2;		// Additional remap register (0x1C)
} AFIO_TypeDef;

// EXTI structure Definition
typedef struct {
    volatile uint32_t IMR;	// Interrupt Mask Register
    volatile uint32_t EMR;	// Event Mask Register
    volatile uint32_t RTSR;	// Rising Trigger Selection Register
    volatile uint32_t FTSR;	// Falling Trigger Selection Register
    volatile uint32_t SWIER;	// Software Interrupt Event Register
    volatile uint32_t PR;		// Pending Register
} EXTI_TypeDef;

/* 
 * SPI Registers Struct Definition
 */
typedef struct {
    _vo uint32_t CR1;		/* SPI Control Register 1 (0x00) */
    _vo uint32_t CR2;		/* SPI Control Register 2 (0x04) */
    _vo uint32_t SR;		/* SPI Status Register (0x08) */
    _vo uint32_t DR;		/* SPI Data Register (0x0C) */
    _vo uint32_t CRCPR;		/* SPI CRC Polynomial Register (0x10) */
    _vo uint32_t RXCRCR;	/* SPI RX CRC Register (0x14) */
    _vo uint32_t TXCRCR;	/* SPI TX CRC Register (0x18) */
    _vo uint32_t I2SCFGR;	/* SPI_I2S Configuration Register (0x1C) */
    _vo uint32_t I2SPR;		/* SPI_I2S Prescaler Register (0x20) */
} SPI_RegDef_t;

/* 
 * I2C Registers Struct Definition
 */
typedef struct {
    _vo uint32_t CR1;       /* I2C Control Register 1 (0x00) */
    _vo uint32_t CR2;       /* I2C Control Register 2 (0x04) */
    _vo uint32_t OAR1;      /* I2C Own Address Register 1 (0x08) */
    _vo uint32_t OAR2;      /* I2C Own Address Register 2 (0x0C) */
    _vo uint32_t DR;        /* I2C Data Register (0x10) */
    _vo uint32_t SR1;       /* I2C Status Register 1 (0x14) */
    _vo uint32_t SR2;       /* I2C Status Register 2 (0x18) */
    _vo uint32_t CCR;       /* I2C Clock Control Register (0x1C) */
    _vo uint32_t TRISE;     /* I2C TRISE Register (0x20) */
} I2C_RegDef_t;
/* 
 * UART Registers Struct Definition
 */
typedef struct {
    _vo uint32_t SR;       /* UART Status Register (0x00) */
    _vo uint32_t DR;       /* UART Data Register (0x04) */
    _vo uint32_t BRR;      /* UART Baud Rate Register (0x08) */
    _vo uint32_t CR1;      /* UART Control Register 1 (0x0C) */
    _vo uint32_t CR2;      /* UART Control Register 2 (0x10) */
    _vo uint32_t CR3;      /* UART Control Register 3 (0x14) */
    _vo uint32_t GTPR;     /* UART Guard Time and Prescaler Register (0x18) */
} USART_RegDef_t;

/************************************************* Peripheral Definition *************************************************/
/* 
 * GPIO Registers Struct Definition
 */
#define GPIOA	((GPIO_RegDef_t*)GPIOA_BASE_ADDR)  /* Base address for GPIOA */
#define GPIOB	((GPIO_RegDef_t*)GPIOB_BASE_ADDR)  /* Base address for GPIOB */
#define GPIOC	((GPIO_RegDef_t*)GPIOC_BASE_ADDR)  /* Base address for GPIOC */
#define GPIOD	((GPIO_RegDef_t*)GPIOD_BASE_ADDR)  /* Base address for GPIOD */
#define GPIOE	((GPIO_RegDef_t*)GPIOE_BASE_ADDR)  /* Base address for GPIOE */
#define GPIOF	((GPIO_RegDef_t*)GPIOF_BASE_ADDR)  /* Base address for GPIOF */
#define GPIOG	((GPIO_RegDef_t*)GPIOG_BASE_ADDR)  /* Base address for GPIOG */
/* 
 * SPI Registers Struct Definition
 */
#define SPI1	((SPI_RegDef_t*)SPI1_BASE_ADDR)  /* Base address for SPI1 */
#define SPI2	((SPI_RegDef_t*)SPI2_BASE_ADDR)  /* Base address for SPI2 */
#define SPI3	((SPI_RegDef_t*)SPI3_BASE_ADDR)  /* Base address for SPI3 */

/* 
 * I2C Peripheral Definitions
 */
#define I2C1    ((I2C_RegDef_t*)I2C1_BASE_ADDR)  /* Base address for I2C1 */
#define I2C2    ((I2C_RegDef_t*)I2C2_BASE_ADDR)  /* Base address for I2C2 */

/* 
 * USART Peripheral Definitions
 */
#define USART1    ((USART_RegDef_t*)USART1_BASE_ADDR)  /* Base address for USART1 */
#define USART2    ((USART_RegDef_t*)USART2_BASE_ADDR)  /* Base address for USART2 */
#define USART3    ((USART_RegDef_t*)USART3_BASE_ADDR)  /* Base address for USART3 */
#define UART4     ((USART_RegDef_t*)UART4_BASE_ADDR)  /* Base address for UART4 */
#define UART5     ((USART_RegDef_t*)UART5_BASE_ADDR)  /* Base address for UART5 */

/* 
 * RCC Registers Struct Definition
 */
#define RCC	((RCC_RegDef_t*)RCC_BASE_ADDR)  /* Base address for GPIOG */

/* 
 * AFUI Registers Struct Definition
 */
#define AFIO	((AFIO_TypeDef*)AFIO_BASE_ADDR)  /* Base address for GPIOG */

/* 
 * AFUI Registers Struct Definition
 */
#define EXTI	((EXTI_TypeDef *)EXTI_BASE_ADDR)  // Base address for EXTI
/************************************************* Macro Definition *************************************************/
/* 
 * Macro Enable Clock for GPIO
 */
#define GPIOA_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 2))
#define GPIOB_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 3))
#define GPIOC_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 4))
#define GPIOD_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 5))
#define GPIOE_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 6))
#define GPIOF_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 7))
#define GPIOG_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 8))
/* 
 * Macro Disable Clock for GPIO
 */
#define GPIOA_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 2))
#define GPIOB_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 3))
#define GPIOC_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 4))
#define GPIOD_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 5))
#define GPIOE_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 6))
#define GPIOF_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 7))
#define GPIOG_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 8))

/* 
 * Macro Enable Clock for SPI
 */
#define SPI1_CLOCK_ENABLE()	    (RCC->APB2ENR |= (1UL << 12))
#define SPI2_CLOCK_ENABLE()	    (RCC->APB1ENR |= (1UL << 14))
#define SPI3_CLOCK_ENABLE()	    (RCC->APB1ENR |= (1UL << 15))
/* 
 * Macro Disable Clock for SPI
 */
#define SPI1_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 12))
#define SPI2_CLOCK_DISABLE()	(RCC->APB1ENR &= ~(1UL << 14))
#define SPI3_CLOCK_DISABLE()	(RCC->APB1ENR &= ~(1UL << 15))

/* 
 * Macro Enable Clock for I2C
 */
#define I2C1_CLOCK_ENABLE()    (RCC->APB1ENR |= (1UL << 21))
#define I2C2_CLOCK_ENABLE()    (RCC->APB1ENR |= (1UL << 22))
/* 
 * Macro Disable Clock for I2C
 */
#define I2C1_CLOCK_DISABLE()   (RCC->APB1ENR &= ~(1UL << 21))
#define I2C2_CLOCK_DISABLE()   (RCC->APB1ENR &= ~(1UL << 22))

/* 
 * Macro Enable Clock for UART
 */
#define USART1_CLOCK_ENABLE()    (RCC->APB2ENR |= (1UL << 14))
#define USART2_CLOCK_ENABLE()    (RCC->APB1ENR |= (1UL << 17))
#define USART3_CLOCK_ENABLE()    (RCC->APB1ENR |= (1UL << 18))
#define UART4_CLOCK_ENABLE()     (RCC->APB1ENR |= (1UL << 19))
#define UART5_CLOCK_ENABLE()     (RCC->APB1ENR |= (1UL << 20))

/* 
 * Macro Disable Clock for UART
 */
#define USART1_CLOCK_DISABLE()   (RCC->APB2ENR &= ~(1UL << 14))
#define USART2_CLOCK_DISABLE()   (RCC->APB1ENR &= ~(1UL << 17))
#define USART3_CLOCK_DISABLE()   (RCC->APB1ENR &= ~(1UL << 18))
#define UART4_CLOCK_DISABLE()    (RCC->APB1ENR &= ~(1UL << 19))
#define UART5_CLOCK_DISABLE()    (RCC->APB1ENR &= ~(1UL << 20))
/* 
 * Macro Enable Clock for AFIO
 */
#define AFIO_CLOCK_ENABLE()	(RCC->APB2ENR |= (1UL << 0))
/* 
 * Macro Disable Clock for AFIO
 */
#define AFIO_CLOCK_DISABLE()	(RCC->APB2ENR &= ~(1UL << 0))


/*********************************************** Define Bit Map for SPI ***********************************************/
/* Bit position definitions for SPI_CR1 */
#define SPI_CR1_CPHA		(0U)
#define SPI_CR1_CPOL		(1U)
#define SPI_CR1_MSTR		(2U)
#define SPI_CR1_BR			(3U)
#define SPI_CR1_SPE			(6U)
#define SPI_CR1_LSBFIRST	(7U)
#define SPI_CR1_SSI			(8U)
#define SPI_CR1_SSM			(9U)
#define SPI_CR1_RXONLY		(10U)
#define SPI_CR1_DFF			(11U)
#define SPI_CR1_CRCNEXT		(12U)
#define SPI_CR1_CRCEN		(13U)
#define SPI_CR1_BIDIOE		(14U)
#define SPI_CR1_BIDIMODE	(15U)
/* Bit position definitions for SPI_CR2 */
#define SPI_CR2_RXDMAEN		(0U)
#define SPI_CR2_TXDMAEN		(1U)
#define SPI_CR2_SSOE		(2U)
#define SPI_CR2_ERRIE		(5U)
#define SPI_CR2_RXNEIE		(6U)
#define SPI_CR2_TXEIE		(7U)
/* Bit position definitions for SPI_SR */
#define SPI_SR_RXNE		(0U)
#define SPI_SR_TXE		(1U)
#define SPI_SR_CHSIDE	(2U)
#define SPI_SR_UDR		(3U)
#define SPI_SR_CRCERR	(4U)
#define SPI_SR_MODF		(5U)
#define SPI_SR_OVR		(6U)
#define SPI_SR_BSY		(7U)

/*********************************************** Define Bit Map for I2C ***********************************************/
/* Bit position definitions for I2C_CR1 */
#define I2C_CR1_PE            (0U)
#define I2C_CR1_SMBUS         (1U)
#define I2C_CR1_SMBTYPE       (3U)
#define I2C_CR1_ENARP         (4U)
#define I2C_CR1_ENPEC         (5U)
#define I2C_CR1_ENGC          (6U)
#define I2C_CR1_NOSTRETCH     (7U)
#define I2C_CR1_START         (8U)
#define I2C_CR1_STOP          (9U)
#define I2C_CR1_ACK           (10U)
#define I2C_CR1_POS           (11U)
#define I2C_CR1_PEC           (12U)
#define I2C_CR1_ALERT         (13U)
#define I2C_CR1_SWRST         (15U)

/* Bit position definitions for I2C_CR2 */
#define I2C_CR2_FREQ          (0U)
#define I2C_CR2_ITERREN       (8U)
#define I2C_CR2_ITEVTEN       (9U)
#define I2C_CR2_ITBUFEN       (10U)
#define I2C_CR2_DMAEN         (11U)
#define I2C_CR2_LAST          (12U)

/* Bit position definitions for I2C_SR1 */
#define I2C_SR1_SB            (0U)
#define I2C_SR1_ADDR          (1U)
#define I2C_SR1_BTF           (2U)
#define I2C_SR1_ADD10         (3U)
#define I2C_SR1_STOPF         (4U)
#define I2C_SR1_RXNE          (6U)
#define I2C_SR1_TXE           (7U)
#define I2C_SR1_BERR          (8U)
#define I2C_SR1_ARLO          (9U)
#define I2C_SR1_AF            (10U)
#define I2C_SR1_OVR           (11U)
#define I2C_SR1_PECERR        (12U)
#define I2C_SR1_TIMEOUT       (14U)
#define I2C_SR1_SMBALERT      (15U)

/* Bit position definitions for I2C_SR2 */
#define I2C_SR2_MSL           (0U)
#define I2C_SR2_BUSY          (1U)
#define I2C_SR2_TRA           (2U)
#define I2C_SR2_GENCALL       (4U)
#define I2C_SR2_SMBDEFAULT    (5U)
#define I2C_SR2_SMBHOST       (6U)
#define I2C_SR2_DUALF         (7U)
#define I2C_SR2_PEC           (8U)
/* Bit position definitions for I2C_CCR */
#define I2C_CCR_CCR           (0U)
#define I2C_CCR_DUTY          (14U)
#define I2C_CCR_FS            (15U)


/*********************************************** Define Bit Map for UART ***********************************************/
/* Bit position definitions for USART_SR */
#define USART_SR_PE           (0U)
#define USART_SR_FE           (1U)
#define USART_SR_NE           (2U)
#define USART_SR_ORE          (3U)
#define USART_SR_IDLE         (4U)
#define USART_SR_RXNE         (5U)
#define USART_SR_TC           (6U)
#define USART_SR_TXE          (7U)
#define USART_SR_LBD          (8U)
#define USART_SR_CTS          (9U)

/* Bit position definitions for USART_DR */
#define USART_DR           (0U)

/* Bit position definitions for USART_BRR */
#define USART_BRR_DIV_FRACTION (0U)
#define USART_BRR_DIV_MANTISSA (4U)

/* Bit position definitions for USART_CR1 */
#define USART_CR1_SBK         (0U)
#define USART_CR1_RWU         (1U)
#define USART_CR1_RE          (2U)
#define USART_CR1_TE          (3U)
#define USART_CR1_IDLEIE      (4U)
#define USART_CR1_RXNEIE      (5U)
#define USART_CR1_TCIE        (6U)
#define USART_CR1_TXEIE       (7U)
#define USART_CR1_PEIE        (8U)
#define USART_CR1_PS          (9U)
#define USART_CR1_PCE         (10U)
#define USART_CR1_WAKE        (11U)
#define USART_CR1_M           (12U)
#define USART_CR1_UE          (13U)

/* Bit position definitions for USART_CR2 */
#define USART_CR2_ADD         (0U)
#define USART_CR2_LBDL        (5U)
#define USART_CR2_LBDIE       (6U)
#define USART_CR2_LBCL        (8U)
#define USART_CR2_CPHA        (9U)
#define USART_CR2_CPOL        (10U)
#define USART_CR2_CLKEN       (11U)
#define USART_CR2_STOP        (12U)
#define USART_CR2_LINEN       (14U)

/* Bit position definitions for USART_CR3 */
#define USART_CR3_EIE         (0U)
#define USART_CR3_IREN        (1U)
#define USART_CR3_IRLP        (2U)
#define USART_CR3_HDSEL       (3U)
#define USART_CR3_NACK        (4U)
#define USART_CR3_SCEN        (5U)
#define USART_CR3_DMAR        (6U)
#define USART_CR3_DMAT        (7U)
#define USART_CR3_RTSE        (8U)
#define USART_CR3_CTSE        (9U)
#define USART_CR3_CTSIE       (10U)
#define USART_CR3_ONEBIT      (11U)

/* Bit position definitions for USART_GTPR */
#define USART_GTPR_PSC        (0U)
#define USART_GTPR_GT         (8U)

#endif /* INC_STM32F103C8T6_H_ */
