/* $Id$ */
/* board-specific CPU attached signals */
//
// Проект HF Dream Receiver (КВ приёмник мечты)
// автор Гена Завидовский mgs2001@mail.ru
// UA1ARN
//

// Трансивер с DSP обработкой "Аист" на процессоре
// rmainunit_v5km0.pcb STM32H743IIT6, TFT 4.3", 2xUSB, SD-CARD, NAU8822L и FPGA EP4CE22E22I7N

#ifndef ARM_STM32MP1_CPUSTYLE_STORCH_V9_H_INCLUDED
#define ARM_STM32MP1_CPUSTYLE_STORCH_V9_H_INCLUDED 1

#define WITHSPI16BIT	1	/* возможно использование 16-ти битных слов при обмене по SPI */
#define WITHSPI32BIT	1	/* возможно использование 32-ти битных слов при обмене по SPI */
#define WITHSPIHW 		1	/* Использование аппаратного контроллера SPI */
//#define WITHSPIHWDMA 	1	/* Использование DMA при обмене по SPI */
//#define WITHSPISW 	1	/* Использование программного управления SPI. Нельзя убирать эту строку - требуется явное отключение из-за конфликта с I2C */
////*#define WITHLTDCHW		1	/* Наличие контроллера дисплея с framebuffer-ом */
//#define WITHDMA2DHW		1	/* Использование DMA2D для формирования изображений	- у STM32MP1 его нет */
#define WITHSDRAMHW	1	/* В процессоре есть внешняя память */
//#define WIHSPIDFHW	1	/* обслуживание DATA FLASH */

//#define WITHTWIHW 	1	/* Использование аппаратного контроллера TWI (I2C) */
//#define WITHTWISW 	1	/* Использование программного контроллера TWI (I2C) */
#if WITHINTEGRATEDDSP
	#define WITHI2SHW	1	/* Использование I2S - аудиокодек на I2S2 и I2S2_alt или I2S2 и I2S3	*/
	#define WITHSAI1HW	1	/* Использование SAI1 - FPGA или IF codec	*/
	//#define WITHSAI2HW	1	/* Использование SAI2 - FPGA или IF codec	*/
#endif /* WITHINTEGRATEDDSP */

//#define WITHCPUDACHW	1	/* использование встроенного в процессор DAC */
//#define WITHCPUADCHW 	1	/* использование встроенного в процессор ADC */

//#define WITHSDHCHW	1		/* Hardware SD HOST CONTROLLER */
//#define WITHSDHCHW4BIT	1	/* Hardware SD HOST CONTROLLER в 4-bit bus width */

//#define WITHUART1HW	1	/* PA9, PA10 Используется периферийный контроллер последовательного порта #1 */
#define WITHUART2HW	1	/* PA2, PA3 Используется периферийный контроллер последовательного порта #2 */
//#define WITHCAT_USART2		1
#define WITHDEBUG_USART2	1
#define WITHNMEA_USART2		1	/* порт подключения GPS/GLONASS */

#define WITHUSBHW	1	/* Используется встроенная в процессор поддержка USB */
#if WITHUSBHW
	//#define WITHUSBDEV_VBUSSENSE	1	/* используется предопределенный вывод VBUS_SENSE */
	//#define WITHUSBDEV_HSDESC	1	/* Требуется формировать дескрипторы как для HIGH SPEED */
	//#define WITHUSBDEV_HIGHSPEEDULPI	1
	//#define WITHUSBDEV_HIGHSPEEDPHYC	1

	/* For H7 exist: Legacy defines */
	//#define USB_OTG_HS                   USB1_OTG_HS
	//#define USB_OTG_FS                   USB2_OTG_FS

	#define WITHUSBHW_DEVICE	USB_OTG_HS	/* на этом устройстве поддерживается функциональность DEVUCE	*/
	//#define WITHUSBHW_HOST		USB_OTG_FS


	#define WITHCAT_CDC		1	/* использовать виртуальный последовательный порт на USB соединении */
	#define WITHMODEM_CDC	1

	#if WITHINTEGRATEDDSP

		#define WITHUSBUAC		1	/* использовать виртуальную звуковую плату на USB соединении */
		#define WITHUSBUACIN2		1	/* формируются три канала передачи звука */
		//#define WITHUABUACOUTAUDIO48MONO	1	/* для уменьшения размера буферов в endpoints */
	#endif /* WITHINTEGRATEDDSP */

	#define WITHUSBCDC		1	/* ACM использовать виртуальный последовательный порт на USB соединении */
	//#define WITHUSBCDCEEM	1	/* EEM использовать Ethernet Emulation Model на USB соединении */
	//#define WITHUSBCDCECM	1	/* ECM использовать Ethernet Control Model на USB соединении */
	//#define WITHUSBRNDIS	1	/* RNDIS использовать Remote NDIS на USB соединении */
	//#define WITHUSBHID	1	/* HID использовать Human Interface Device на USB соединении */
	//#define WITHUSBDFU	1	/* DFU USB Device Firmware Upgrade support */

#endif /* WITHUSBHW */

#define LS020_RS_INITIALIZE() \
	do { \
		arm_hardware_piof_outputs2m(LS020_RS, LS020_RS); \
		arm_hardware_pioe_outputs((1U << 0), 0 * (1U << 0));		/* PE0 - enable backlight */ \
	} while (0)

#define LS020_RESET_INITIALIZE() \
	do { \
		arm_hardware_piof_outputs2m(LS020_RESET, LS020_RESET); \
		arm_hardware_pioe_outputs((1U << 0), 0 * (1U << 0));		/* PE0 - enable backlight */ \
	} while (0)

#define LS020_RS_SET(v) do { \
		if ((v) != 0) LS020_RS_PORT_S(LS020_RS); \
		else  LS020_RS_PORT_C(LS020_RS); \
	} while (0)

#define LS020_RESET_SET(v) do { \
		if ((v) != 0) LS020_RESET_PORT_S(LS020_RESET); \
		else  LS020_RESET_PORT_C(LS020_RESET); \
	} while (0)

#if LCDMODE_SPI_NA
	// эти контроллеры требуют только RS

	#define LS020_RS_PORT_S(v)		do { GPIOF->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RS_PORT_C(v)		do { GPIOF->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RS			(1u << 3)			// PF3 D7 signal

#elif LCDMODE_SPI_RN
	// эти контроллеры требуют только RESET

	#define LS020_RESET_PORT_S(v)		do { GPIOF->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RESET_PORT_C(v)		do { GPIOF->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RESET			(1u << 2)			// PF2 D6 signal in HD44780 socket

#elif LCDMODE_SPI_RA
	// Эти контроллеры требуют RESET и RS
	// LCDMODE_UC1608

	#define LS020_RS_PORT_S(v)		do { GPIOF->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RS_PORT_C(v)		do { GPIOF->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RS			(1u << 3)			// PF3 D7 signal

	#define LS020_RESET_PORT_S(v)		do { GPIOF->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RESET_PORT_C(v)		do { GPIOF->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RESET			(1u << 2)			// PF2 D6 signal in HD44780 socket

#elif LCDMODE_HD44780 && (LCDMODE_SPI == 0)

	#error Unsupported LCDMODE_HD44780

#endif

#if WITHENCODER

	// Выводы подключения енкодера #1
	#define ENCODER_INPUT_PORT	(GPIOH->IDR) 
	#define ENCODER_BITA		(1u << 4)		// PH4
	#define ENCODER_BITB		(1u << 5)		// PH5

	// Выводы подключения енкодера #2
	#define ENCODER2_INPUT_PORT	(GPIOH->IDR) 
	#define ENCODER2_BITA		(1u << 2)		// PH2
	#define ENCODER2_BITB		(1u << 3)		// PH3


	#define ENCODER_BITS		(ENCODER_BITA | ENCODER_BITB)
	#define ENCODER2_BITS		(ENCODER2_BITA | ENCODER2_BITB)

	#define ENCODER_INITIALIZE() \
		do { \
			arm_hardware_pioh_inputs(ENCODER_BITS); \
			arm_hardware_pioh_updown(ENCODER_BITS, 0); \
			arm_hardware_pioh_onchangeinterrupt(ENCODER_BITS, ENCODER_BITS, ENCODER_BITS, ARM_OVERREALTIME_PRIORITY); \
			arm_hardware_pioh_inputs(ENCODER2_BITS); \
			arm_hardware_pioh_updown(ENCODER2_BITS, 0); \
			arm_hardware_pioh_onchangeinterrupt(0 * ENCODER2_BITS, ENCODER2_BITS, ENCODER2_BITS, ARM_OVERREALTIME_PRIORITY); \
		} while (0)

#endif

#if WITHI2SHW
	// Инициализируются I2S2 и I2S3
	#define I2S2HW_INITIALIZE() do { \
		arm_hardware_piob_altfn2(1uL << 12,	AF_SPI2); /* PB12 I2S2_WS	*/ \
		arm_hardware_piob_altfn2(1uL << 10,	AF_SPI2); /* PB10 I2S2_CK	*/ \
		arm_hardware_pioc_altfn2(1uL << 3,	AF_SPI2); /* PC3 I2S2_SD - передача */ \
		arm_hardware_pioa_altfn2(1uL << 15,	AF_SPI3); /* PA15 I2S3_WS	*/ \
		arm_hardware_piob_altfn2(1uL << 3,	AF_SPI3); /* PB3 I2S3_CK	*/ \
		arm_hardware_piob_altfn2(1uL << 2,	7 /* AF_7 */); /* PB2 I2S3_SD, - приём от кодека */ \
	} while (0)
#endif /* WITHSAI1HW */

#if WITHSAI1HW
	#define SAI1HW_INITIALIZE()	do { \
		/*arm_hardware_pioe_altfn20(1uL << 2, AF_SAI); */	/* PE2 - SAI1_MCK_A - 12.288 MHz	*/ \
		arm_hardware_pioe_altfn2(1uL << 4,	AF_SAI);			/* PE4 - SAI1_FS_A	- 48 kHz	*/ \
		arm_hardware_pioe_altfn20(1uL << 5,	AF_SAI);			/* PE5 - SAI1_SCK_A	*/ \
		arm_hardware_pioe_altfn2(1uL << 6,	AF_SAI);			/* PE6 - SAI1_SD_A	(i2s data to codec)	*/ \
		arm_hardware_pioe_altfn2(1uL << 3,	AF_SAI);			/* PE3 - SAI1_SD_B	(i2s data from codec)	*/ \
	} while (0)
#endif /* WITHSAI1HW */

#if WITHSAI2HW
	/* 
	Поскольку блок SAI2 инициализируется как SLAVE с синхронизацией от SAI1,
	из внешних сигналов требуется только SAI2_SD_A
	*/
	#define SAI2HW_INITIALIZE()	do { \
		/* arm_hardware_pioe_altfn20(1uL << 0, AF_SAI2); */	/* PE0 - SAI2_MCK_A - 12.288 MHz	*/ \
		/* arm_hardware_piod_altfn2(1uL << 12, AF_SAI2); */	/* PD12 - SAI2_FS_A	- 48 kHz	*/ \
		/* arm_hardware_piod_altfn20(1uL << 13, AF_SAI2); */	/* PD13 - SAI2_SCK_A	*/ \
		/* arm_hardware_piod_altfn2(1uL << 11, AF_SAI2); */	/* PD11 - SAI2_SD_A	(i2s data to codec)	*/ \
		/* arm_hardware_pioe_altfn2(1uL << 11, AF_SAI2);	*/ /* PE11 - SAI2_SD_B	(i2s data from codec)	*/ \
	} while (0)
#endif /* WITHSAI1HW */

/* Распределение битов в ARM контроллерах */

#if (WITHCAT && WITHCAT_USART2)
	// CAT data lites
	// RXD at PA10, TXD at PA9

	// CAT control lines
	//#define FROMCAT_TARGET_PIN_RTS		(GPIOA->IDR) 
	//#define FROMCAT_BIT_RTS				(1u << 11)	/* PA11 сигнал RTS от FT232RL	*/

	/* манипуляция от порта RS-232, сигнал PPS от GPS/GLONASS/GALILEO модуля */
	//#define FROMCAT_TARGET_PIN_DTR		(GPIOA->IDR)
	//#define FROMCAT_BIT_DTR				(1u << 12)	/* PA12 сигнал DTR от FT232RL	*/

	/* манипуляция от порта RS-232 */
	#define FROMCAT_DTR_INITIALIZE() \
		do { \
		} while (0)

	/* переход на передачу от порта RS-232 */
	#define FROMCAT_RTS_INITIALIZE() \
		do { \
		} while (0)

#endif /* (WITHCAT && WITHCAT_USART2) */

#if (WITHCAT && WITHCAT_CDC)

	// CAT data lites
	// RXD at PA10, TXD at PA9

	// CAT control lines
	//#define FROMCAT_TARGET_PIN_RTS		(GPIOA->IDR) // was PINA 
	//#define FROMCAT_BIT_RTS				(1u << 11)	/* сигнал RTS от FT232RL	*/

	/* манипуляция от порта RS-232, сигнал PPS от GPS/GLONASS/GALILEO модуля */
	//#define FROMCAT_TARGET_PIN_DTR		(GPIOA->IDR) // was PINA 
	//#define FROMCAT_BIT_DTR				(1u << 12)	/* сигнал DTR от FT232RL	*/

	/* манипуляция от виртуального CDC порта */
	#define FROMCAT_DTR_INITIALIZE() \
		do { \
		} while (0)

	/* переход на передачу от виртуального CDC порта*/
	#define FROMCAT_RTS_INITIALIZE() \
		do { \
		} while (0)

#endif /* (WITHCAT && WITHCAT_CDC) */

#if WITHSDHCHW
	#if WITHSDHCHW4BIT
		#define HARDWARE_SDIO_INITIALIZE()	do { \
			arm_hardware_piod_altfn50(1uL << 2, AF_SDIO);	/* PD2 - SDIO_CMD	*/ \
			arm_hardware_pioc_altfn50(1uL << 12, AF_SDIO);	/* PC12 - SDIO_CK	*/ \
			arm_hardware_pioc_altfn50(1uL << 8, AF_SDIO);	/* PC8 - SDIO_D0	*/ \
			arm_hardware_pioc_altfn50(1uL << 9, AF_SDIO);	/* PC9 - SDIO_D1	*/ \
			arm_hardware_pioc_altfn50(1uL << 10, AF_SDIO);	/* PC10 - SDIO_D2	*/ \
			arm_hardware_pioc_altfn50(1uL << 11, AF_SDIO);	/* PC11 - SDIO_D3	*/ \
		} while (0)
		/* отключить процессор от SD карты - чтобы при выполнении power cycle не возникало фантомное питание через сигналы управления. */
		#define HARDWARE_SDIO_HANGOFF()	do { \
			arm_hardware_piod_inputs(1uL << 2);	/* PD2 - SDIO_CMD	*/ \
			arm_hardware_pioc_inputs(1uL << 12);	/* PC12 - SDIO_CK	*/ \
			arm_hardware_pioc_inputs(1uL << 8);	/* PC8 - SDIO_D0	*/ \
			arm_hardware_pioc_inputs(1uL << 9);	/* PC9 - SDIO_D1	*/ \
			arm_hardware_pioc_inputs(1uL << 10);	/* PC10 - SDIO_D2	*/ \
			arm_hardware_pioc_inputs(1uL << 11);	/* PC11 - SDIO_D3	*/ \
			arm_hardware_piod_updown(0, 1uL << 2);	/* PD2 - SDIO_CMD	*/ \
			arm_hardware_pioc_updown(0, 1uL << 12);	/* PC12 - SDIO_CK	*/ \
			arm_hardware_pioc_updown(0, 1uL << 8);	/* PC8 - SDIO_D0	*/ \
			arm_hardware_pioc_updown(0, 1uL << 9);	/* PC9 - SDIO_D1	*/ \
			arm_hardware_pioc_updown(0, 1uL << 10);	/* PC10 - SDIO_D2	*/ \
			arm_hardware_pioc_updown(0, 1uL << 11);	/* PC11 - SDIO_D3	*/ \
		} while (0)
	#else /* WITHSDHCHW4BIT */
		#define HARDWARE_SDIO_INITIALIZE()	do { \
			arm_hardware_piod_altfn50(1uL << 2, AF_SDIO);	/* PD2 - SDIO_CMD	*/ \
			arm_hardware_pioc_altfn50(1uL << 12, AF_SDIO);	/* PC12 - SDIO_CK	*/ \
			arm_hardware_pioc_altfn50(1uL << 8, AF_SDIO);	/* PC8 - SDIO_D0	*/ \
		} while (0)
		/* отключить процессор от SD карты - чтобы при выполнении power cycle не возникало фантомное питание через сигналы управления. */
		#define HARDWARE_SDIO_HANGOFF()	do { \
			arm_hardware_piod_inputs(1uL << 2);	/* PD2 - SDIO_CMD	*/ \
			arm_hardware_pioc_inputs(1uL << 12);	/* PC12 - SDIO_CK	*/ \
			arm_hardware_pioc_inputs(1uL << 8);	/* PC8 - SDIO_D0	*/ \
			arm_hardware_piod_updown(0, 1uL << 2);	/* PD2 - SDIO_CMD	*/ \
			arm_hardware_pioc_updown(0, 1uL << 12);	/* PC12 - SDIO_CK	*/ \
			arm_hardware_pioc_updown(0, 1uL << 8);	/* PC8 - SDIO_D0	*/ \
		} while (0)
	#endif /* WITHSDHCHW4BIT */

	#define HARDWARE_SDIO_WP_BIT	(1U << 8)	/* PG8 - SDIO_WP */
	#define HARDWARE_SDIO_CD_BIT	(1U << 7)	/* PG7 - SDIO_SENSE */

	#define HARDWARE_SDIOSENSE_INITIALIZE()	do { \
			arm_hardware_piog_inputs(HARDWARE_SDIO_WP_BIT); /* PD1 - SDIO_WP */ \
			arm_hardware_piog_updown(HARDWARE_SDIO_WP_BIT, 0); \
			arm_hardware_piog_inputs(HARDWARE_SDIO_CD_BIT); /* PD0 - SDIO_SENSE */ \
			arm_hardware_piog_updown(HARDWARE_SDIO_CD_BIT, 0); \
	} while (0)

	#define HARDWARE_SDIOSENSE_CD() ((GPIOG->IDR & HARDWARE_SDIO_CD_BIT) == 0)	/* получить состояние датчика CARD PRESENT */
	#define HARDWARE_SDIOSENSE_WP() ((GPIOG->IDR & HARDWARE_SDIO_WP_BIT) != 0)	/* получить состояние датчика CARD WRITE PROTECT */

	#define HARDWARE_SDIOPOWER_C(v)	do { GPIOC->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define HARDWARE_SDIOPOWER_S(v)	do { GPIOC->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define HARDWARE_SDIOPOWER_BIT (1u << 7)	/* PC7 */
	/* если питание SD CARD управляется прямо с процессора */
	#define HARDWARE_SDIOPOWER_INITIALIZE()	do { \
		arm_hardware_pioc_outputs2m(HARDWARE_SDIOPOWER_BIT, HARDWARE_SDIOPOWER_BIT); /* питание выключено */ \
		} while (0)
	/* parameter on not zero for powering SD CARD */
	#define HARDWARE_SDIOPOWER_SET(on)	do { \
		if ((on) != 0) \
			HARDWARE_SDIOPOWER_C(HARDWARE_SDIOPOWER_BIT); \
		else \
			HARDWARE_SDIOPOWER_S(HARDWARE_SDIOPOWER_BIT); \
	} while (0)

#endif /* WITHSDHCHW */

#if WITHTX

	// txpath outputs not used
	////#define TXPATH_TARGET_PORT_S(v)		do { GPIOD->BSRR = BSRR_S(v); __DSB(); } while (0)
	////#define TXPATH_TARGET_PORT_C(v)		do { GPIOD->BSRR = BSRR_C(v); __DSB(); } while (0)
	// 
	#define TXGFV_RX		(1u << 4)
	#define TXGFV_TRANS		0			// переход между режимами приёма и передачи
	#define TXGFV_TX_SSB	(1u << 0)
	#define TXGFV_TX_CW		(1u << 1)
	#define TXGFV_TX_AM		(1u << 2)
	#define TXGFV_TX_NFM	(1u << 3)

	#define TXPATH_INITIALIZE() \
		do { \
		} while (0)


	// +++
	// TXDISABLE input - PD10
	#define TXDISABLE_TARGET_PIN				(GPIOD->IDR)
	#define TXDISABLE_BIT_TXDISABLE				(1U << 10)		// PD10 - TX INHIBIT
	// получить бит запрета передачи (от усилителя мощности)
	#define HARDWARE_GET_TXDISABLE() ((TXDISABLE_TARGET_PIN & TXDISABLE_BIT_TXDISABLE) != 0)
	#define TXDISABLE_INITIALIZE() \
		do { \
			arm_hardware_piod_inputs(TXDISABLE_BIT_TXDISABLE); \
			arm_hardware_piod_updown(0, TXDISABLE_BIT_TXDISABLE); \
		} while (0)
	// ---

	// +++
	// PTT input - PD13
	// PTT2 input - PD0
	#define PTT_TARGET_PIN				(GPIOD->IDR)
	#define PTT_BIT_PTT					(1uL << 13)		// PD13 - PTT
	#define PTT2_TARGET_PIN				(GPIOD->IDR)
	#define PTT2_BIT_PTT				(1uL << 8)		// PD8 - PTT2
	// получить бит запроса оператором перехода на пердачу
	#define HARDWARE_GET_PTT() ((PTT_TARGET_PIN & PTT_BIT_PTT) == 0 || (PTT2_TARGET_PIN & PTT2_BIT_PTT) == 0)
	#define PTT_INITIALIZE() \
		do { \
			arm_hardware_piod_inputs(PTT_BIT_PTT); \
			arm_hardware_piod_updown(PTT_BIT_PTT, 0); \
			arm_hardware_piod_inputs(PTT2_BIT_PTT); \
			arm_hardware_piod_updown(PTT2_BIT_PTT, 0); \
		} while (0)
	// ---
	// TUNE input - PD9
	#define TUNE_TARGET_PIN				(GPIOD->IDR)
	#define TUNE_BIT_TUNE					(1U << 9)		// PD9
	#define HARDWARE_GET_TUNE() ((TUNE_TARGET_PIN & TUNE_BIT_TUNE) == 0)
	#define TUNE_INITIALIZE() \
		do { \
			arm_hardware_piod_inputs(TUNE_BIT_TUNE); \
			arm_hardware_piod_updown(TUNE_BIT_TUNE, 0); \
		} while (0)

#else /* WITHTX */

	#define TXDISABLE_INITIALIZE() \
		do { \
		} while (0)
	#define PTT_INITIALIZE() \
		do { \
		} while (0)
	#define TUNE_INITIALIZE() \
		do { \
		} while (0)
#endif /* WITHTX */

#if WITHELKEY
	// Electronic key inputs
	#define ELKEY_TARGET_PIN			(GPIOD->IDR)
	#define ELKEY_BIT_LEFT				(1uL << 11)		// PD11
	#define ELKEY_BIT_RIGHT				(1uL << 12)		// PD12

	#define ELKEY_INITIALIZE() \
		do { \
			arm_hardware_piod_inputs(ELKEY_BIT_LEFT | ELKEY_BIT_RIGHT); \
			arm_hardware_piod_updown(ELKEY_BIT_LEFT | ELKEY_BIT_RIGHT, 0); \
		} while (0)

#endif /* WITHELKEY */

// IOUPDATE = PA15
//#define SPI_IOUPDATE_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
//#define SPI_IOUPDATE_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
//#define SPI_IOUPDATE_BIT		(1uL << 15)	// * PA15

#if WITHSPIHW || WITHSPISW
	// Набор определений для работы без внешнего дешифратора
	#define SPI_ALLCS_PORT_S(v)	do { GPIOE->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_ALLCS_PORT_C(v)	do { GPIOE->BSRR = BSRR_C(v); __DSB(); } while (0)

	#define targetext1		(0 * 1uL << 15)	// PG15 ext1 on front panel
	#define targetxad2		(0 * 1uL << 14)	// PG14 ext2(not connected now)
	#define targetnvram		(1uL << 2)		// PE2 nvmem FM25L16B
	#define targetctl1		(0 * 1uL << 3)	// PG7 board control registers chain
	#define targetcodec1	(1uL << 15)		// PE15 on-board codec1 NAU8822L
	#define targetfpga1		(0 * 1uL << 1)	// PG1 FPGA control registers CS1
	#define targetfpga2		(0 * 1uL << 0)	// PG0 FPGA control registers CS2

	// Здесь должны быть перечислены все биты формирования CS в устройстве.
	#define SPI_ALLCS_BITS ( \
		targetext1	| 	/* PG15 ext1 on front panel */ \
		targetxad2	|	/* PA100W on-board ADC (not connected on this board) */ \
		targetnvram	| 	/* PG4 nvmem FM25L16B */ \
		targetctl1	| 	/* PG3 board control registers chain */ \
		targetcodec1	| 	/* PG2 on-board codec1 NAU8822L */ \
		targetfpga1	| 	/* PG1 FPGA control registers CS1 */ \
		targetfpga2	| 	/* PG0 FPGA control registers CS2 targetfir1 */ \
		0)

	#define targetlcd	targetext1 	/* LCD over SPI line devices control */ 
	#define targetuc1608 targetext1	/* LCD with positive chip select signal	*/

	#define SPI_ALLCS_BITSNEG 0		// Выходы, активные при "1"

	//#define SPI_NAEN_PORT_S(v)	do { GPIOE->BSRR = BSRR_S(v); __DSB(); } while (0)
	//#define SPI_NAEN_PORT_C(v)	do { GPIOE->BSRR = BSRR_C(v); __DSB(); } while (0)

	//#define SPI_NAEN_BIT (1u << 7)		// * PE7 used

	/* инициализация лиий выбора периферийных микросхем */
	#define SPI_ALLCS_INITIALIZE() \
		do { \
			arm_hardware_pioe_outputs(SPI_ALLCS_BITS, SPI_ALLCS_BITS ^ SPI_ALLCS_BITSNEG); \
		} while (0)

	// MOSI & SCK port
	#define SPI_TARGET_SCLK_PORT_C(v)	do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define SPI_TARGET_SCLK_PORT_S(v)	do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define	SPI_SCLK_BIT			(1uL << 3)	// * PB3 бит, через который идет синхронизация SPI

	#define SPI_TARGET_MOSI_PORT_C(v)	do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define SPI_TARGET_MOSI_PORT_S(v)	do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define	SPI_MOSI_BIT			(1uL << 5)	// * PB5 бит, через который идет вывод (или ввод в случае двунаправленного SPI).

	#define SPI_TARGET_MISO_PIN		(GPIOB->IDR)
	#define	SPI_MISO_BIT			(1uL << 4)	// * PB4 бит, через который идет ввод с SPI.

	#define SPIIO_INITIALIZE() do { \
			arm_hardware_piob_outputs(SPI_SCLK_BIT, SPI_SCLK_BIT); /* PB3 */ \
			arm_hardware_piob_outputs(SPI_MOSI_BIT, SPI_MOSI_BIT); /* PB5 */ \
			arm_hardware_piob_inputs(SPI_MISO_BIT); /* PB4 */ \
		} while (0)
	#define HARDWARE_SPI_CONNECT() do { \
			arm_hardware_piob_altfn20(SPI_MOSI_BIT | SPI_MISO_BIT, AF_SPI1); /* В этих процессорах и входы и выходы перекдючаются на ALT FN */ \
			arm_hardware_piob_altfn20(SPI_SCLK_BIT, AF_SPI1); /* В этих процессорах и входы и выходы перекдючаются на ALT FN */ \
		} while (0)
	#define HARDWARE_SPI_DISCONNECT() do { \
			arm_hardware_piob_outputs(SPI_SCLK_BIT, SPI_SCLK_BIT); \
			arm_hardware_piob_outputs(SPI_MOSI_BIT, SPI_MOSI_BIT); \
			arm_hardware_piob_inputs(SPI_MISO_BIT); \
		} while (0)
	#define HARDWARE_SPI_CONNECT_MOSI() do { \
			arm_hardware_piob_altfn20(SPI_MOSI_BIT, AF_SPI1);	/* PIO disable for MOSI bit (SD CARD read support) */ \
		} while (0)
	#define HARDWARE_SPI_DISCONNECT_MOSI() do { \
			arm_hardware_piob_outputs(SPI_MOSI_BIT, SPI_MOSI_BIT);	/* PIO enable for MOSI bit (SD CARD read support)  */ \
		} while (0)

#endif /* WITHSPIHW || WITHSPISW */

#define HARDWARE_USART1_INITIALIZE() do { \
		arm_hardware_pioa_altfn2((1uL << 9) | (1uL << 10), AF_USART1); /* PA9: TX DATA line (2 MHz), PA10: RX data line */ \
		arm_hardware_pioa_updown((1uL << 10), 0);	/* PA10: pull-up RX data */ \
	} while (0)
#define HARDWARE_USART2_INITIALIZE() do { \
		arm_hardware_pioa_altfn2((1uL << 2) | (1uL << 3), AF_USART2); /* PA2: TX DATA line (2 MHz), PA3: RX data line */ \
		arm_hardware_pioa_updown((1uL << 3), 0);	/* PA3: pull-up RX data */ \
	} while (0)

#if WITHKEYBOARD
	/* PF0: pull-up second encoder button */
	#define KBD_MASK (1U << 0)	// PF0
	#define KBD_TARGET_PIN (GPIOF->IDR)

	#define HARDWARE_KBD_INITIALIZE() do { \
			arm_hardware_piof_inputs(KBD_MASK); \
			arm_hardware_piof_updown(KBD_MASK, 0);	/* PF10: pull-up second encoder button */ \
		} while (0)

#else /* WITHKEYBOARD */

	#define HARDWARE_KBD_INITIALIZE() do { \
		} while (0)

#endif /* WITHKEYBOARD */

#if 1 // WITHTWISW
	#define TARGET_TWI_TWCK_PORT_C(v) do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_TWI_TWCK_PORT_S(v) do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_TWI_TWD_PORT_C(v) do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_TWI_TWD_PORT_S(v) do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_TWI_TWCK_PIN		(GPIOB->IDR)
	#define TARGET_TWI_TWD_PIN		(GPIOB->IDR)
	#define TARGET_TWI_TWCK		(1u << 6)		// PB6 SCL
	#define TARGET_TWI_TWD		(1u << 7)		// PB7 SDA

	// Инициализация битов портов ввода-вывода для программной реализации I2C
	#define	TWISOFT_INITIALIZE() do { \
			arm_hardware_piob_opendrain(TARGET_TWI_TWCK | TARGET_TWI_TWD, TARGET_TWI_TWCK | TARGET_TWI_TWD); \
		} while (0) 
	// Инициализация битов портов ввода-вывода для аппаратной реализации I2C
	// присоединение выводов к периферийному устройству
	#define	TWIHARD_INITIALIZE() do { \
			arm_hardware_piob_periphopendrain_altfn2(TARGET_TWI_TWCK | TARGET_TWI_TWD, AF_I2C1);	/* AF=4 */ \
		} while (0) 


#endif // WITHTWISW

#if WITHFPGAWAIT_AS || WITHFPGALOAD_PS

	/* outputs */
	#define FPGA_NCONFIG_PORT_S(v)	do { GPIOE->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define FPGA_NCONFIG_PORT_C(v)	do { GPIOE->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define FPGA_NCONFIG_BIT		(1UL << 11)	/* PE11 bit conneced to nCONFIG pin ALTERA FPGA */

	/* inputs */
	#define FPGA_CONF_DONE_INPUT	(GPIOE->IDR)
	#define FPGA_CONF_DONE_BIT		(1UL << 12)	/* PE12 bit conneced to CONF_DONE pin ALTERA FPGA */

	#define FPGA_NSTATUS_INPUT		(GPIOE->IDR)
	#define FPGA_NSTATUS_BIT		(1UL << 15)	/* PE15 bit conneced to NSTATUS pin ALTERA FPGA */

	#define FPGA_INIT_DONE_INPUT	(GPIOE->IDR)
	#define FPGA_INIT_DONE_BIT		(1UL << 10)	/* PE10 bit conneced to INIT_DONE pin ALTERA FPGA */

	/* Инициадизация выводов GPIO процессора для получения состояния и управлением загрузкой FPGA */
	#define HARDWARE_FPGA_LOADER_INITIALIZE() do { \
			arm_hardware_pioe_outputs(FPGA_NCONFIG_BIT, FPGA_NCONFIG_BIT); \
			arm_hardware_pioe_inputs(FPGA_NSTATUS_BIT); \
			arm_hardware_pioe_inputs(FPGA_CONF_DONE_BIT); \
			arm_hardware_pioe_inputs(FPGA_INIT_DONE_BIT); \
		} while (0)

	/* Проверяем, проинициализировалась ли FPGA (вошла в user mode). */
	/*
		After the option bit to enable INIT_DONE is programmed into the device (during the first
		frame of configuration data), the INIT_DONE pin goes low.
		When initialization is complete, the INIT_DONE pin is released and pulled high. 
		This low-to-high transition signals that the device has entered user mode.
	*/
	#define HARDWARE_FPGA_IS_USER_MODE() ((FPGA_INIT_DONE_INPUT & FPGA_INIT_DONE_BIT) != 0)

#endif /* WITHFPGAWAIT_AS || WITHFPGALOAD_PS */

#if WITHDSPEXTFIR
	// Биты доступа к массиву коэффициентов FIR фильтра в FPGA
	#define TARGET_FPGA_FIR_CS_PORT_C(v)	do { GPIOE->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_FPGA_FIR_CS_PORT_S(v)	do { GPIOE->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_FPGA_FIR_CS_BIT (1uL << 7)	/* PE7 - fir CS */

	#define TARGET_FPGA_FIR1_WE_PORT_C(v)	do { GPIOH->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_FPGA_FIR1_WE_PORT_S(v)	do { GPIOH->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_FPGA_FIR1_WE_BIT (1uL << 6)	/* PH6 - fir1 WE */

	#define TARGET_FPGA_FIR2_WE_PORT_C(v)	do { GPIOH->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_FPGA_FIR2_WE_PORT_S(v)	do { GPIOH->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_FPGA_FIR2_WE_BIT (1uL << 7)	/* PH7 - fir2 WE */

	#define TARGET_FPGA_FIR_INITIALIZE() do { \
			arm_hardware_pioh_outputs(TARGET_FPGA_FIR1_WE_BIT, TARGET_FPGA_FIR1_WE_BIT); \
			arm_hardware_pioh_outputs(TARGET_FPGA_FIR2_WE_BIT, TARGET_FPGA_FIR2_WE_BIT); \
			arm_hardware_pioe_outputs(TARGET_FPGA_FIR_CS_BIT, TARGET_FPGA_FIR_CS_BIT); \
		} while (0)
#endif /* WITHDSPEXTFIR */

#if 1
	/* получение состояния переполнения АЦП */
	#define TARGET_FPGA_OVF_INPUT		(GPIOF->IDR)
	#define TARGET_FPGA_OVF_BIT			(1u << 12)	// PF12
	#define TARGET_FPGA_OVF_GET			((TARGET_FPGA_OVF_INPUT & TARGET_FPGA_OVF_BIT) == 0)	// 1 - overflow active
	#define TARGET_FPGA_OVF_INITIALIZE() do { \
				arm_hardware_piof_inputs(TARGET_FPGA_OVF_BIT); \
			} while (0)
#endif

#if WITHCPUDACHW
	/* включить нужные каналы */
	#define HARDWARE_DAC_INITIALIZE() do { \
			DAC1->CR = DAC_CR_EN1; /* DAC1 enable */ \
		} while (0)
	#define HARDWARE_DAC_ALC(v) do { /* вывод 12-битного значения на ЦАП - канал 1 */ \
			DAC1->DHR12R1 = (v); /* DAC1 set value */ \
		} while (0)

#else /* WITHCPUDACHW */
	#define HARDWARE_DAC_INITIALIZE() do { \
		} while (0)

#endif /* WITHCPUDACHW */

#if WITHCPUADCHW
	#define HARDWARE_ADC_INITIALIZE(ainmask) do { \
			arm_hardware_pioa_analoginput(((ainmask) >> 0) & 0xff);	/* ADC12_IN0..ADC12_IN7 */ \
			arm_hardware_piob_analoginput(((ainmask) >> 8) & 0x03);	/* ADC12_IN8..ADC12_IN0 */ \
			arm_hardware_pioc_analoginput(((ainmask) >> 10) & 0x3f);	/* ADC12_IN10..ADC12_IN15 */ \
		} while (0)
#endif /* WITHCPUADCHW */

#if WITHUSBHW
	#define TARGET_USBFS_VBUSON_PORT_C(v)	do { GPIOC->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_USBFS_VBUSON_PORT_S(v)	do { GPIOC->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_USBFS_VBUSON_BIT (1uL << 6)	// PC6 - нулём включение питания для device
	/**USB_OTG_FS GPIO Configuration    
	PA9     ------> USB_OTG_FS_VBUS
	PA10     ------> USB_OTG_FS_ID
	PA11     ------> USB_OTG_FS_DM
	PA12     ------> USB_OTG_FS_DP 
	*/
	#define	USBD_FS_INITIALIZE() do { \
		} while (0)

	#define TARGET_USBFS_VBUSON_SET(on)	do { \
		if ((on) != 0) \
			TARGET_USBFS_VBUSON_PORT_C(TARGET_USBFS_VBUSON_BIT); \
		else \
			TARGET_USBFS_VBUSON_PORT_S(TARGET_USBFS_VBUSON_BIT); \
	} while (0)

	/**USB_OTG_HS GPIO Configuration    
	PB13     ------> USB_OTG_HS_VBUS
	PB14     ------> USB_OTG_HS_DM
	PB15     ------> USB_OTG_HS_DP 
	*/
	#define	USBD_HS_FS_INITIALIZE() do { \
		arm_hardware_pioa_altfn50(/*(1uL << 10) | */ (1uL << 11) | (1uL << 12), AF_OTGFS);			/* PA10, PA11, PA12 - USB_OTG_FS	*/ \
		arm_hardware_pioa_inputs(1uL << 9);		/* PA9 - USB_OTG_FS_VBUS */ \
		} while (0)

	#define	USBD_HS_ULPI_INITIALIZE() do { \
		} while (0)
#endif /* WITHUSBHW */

	#define	HARDWARE_BL_INITIALIZE() do { \
		/* step-up backlight converter */ \
		} while (0)

#if WITHDCDCFREQCTL
	#define	HARDWARE_DCDC_INITIALIZE() do { \
		arm_hardware_piof_altfn2((1U << 6), AF_TIM1); /* TIM16_CH1 - PF6 */ \
		hardware_blfreq_initialize(); \
		} while (0)
#else /* WITHDCDCFREQCTL */
	#define	HARDWARE_DCDC_INITIALIZE() do { \
		} while (0)
#endif /* WITHDCDCFREQCTL */

	/* установка яркости и включение/выключение преобразователя подсветки */
	#define HARDWARE_BL_SET(en, level) do { \
		const portholder_t enmask = (1U << 1); /* PF1 */ \
		const portholder_t opins = (1U << 3) | (1U << 2); /* PF3:PF2 */ \
		const portholder_t initialstate = (~ (level) & 0x03) << 2; \
		GPIOF->BSRR = \
			((en) ? BSRR_S(enmask) : BSRR_C(enmask)) | /* backlight control on/off */ \
			BSRR_S((initialstate) & (opins)) | /* set bits */ \
			BSRR_C(~ (initialstate) & (opins)) | /* reset bits */ \
			0; \
		__DSB(); \
	} while (0)

#if LCDMODE_LTDC
	enum
	{
		GPIO_AF_LTDC = 14,  /* LCD-TFT Alternate Function mapping */
		GPIO_AF_LTDC9 = 9  /* LCD-TFT Alternate Function mapping */
	};
	/* demode values: 0: static signal, 1: DE controlled */
	#define HARDWARE_LTDC_INITIALIZE(demode) do { \
		/* Synchronisation signals */ \
		arm_hardware_pioi_altfn20((1U << 9), GPIO_AF_LTDC);		/* VSYNC */ \
		arm_hardware_pioi_altfn20((1U << 10), GPIO_AF_LTDC);	/* HSYNC */ \
		arm_hardware_pioe_altfn20((1U << 14), GPIO_AF_LTDC);	/* CLK */ \
		/* Control */ \
		arm_hardware_pioe_altfn20((demode != 0) * (1U << 13), GPIO_AF_LTDC);	/* DE */ \
		arm_hardware_pioe_outputs((demode == 0) * (1U << 13), 0 * (1U << 13));	/* DE=0 (DISP, pin 31) */ \
		/* RED */ \
		arm_hardware_pioh_altfn20((1U << 9), GPIO_AF_LTDC);		/* R3 */ \
		arm_hardware_pioh_altfn20((1U << 10), GPIO_AF_LTDC);	/* R4 */ \
		arm_hardware_pioh_altfn20((1U << 11), GPIO_AF_LTDC);	/* R5 */ \
		arm_hardware_pioh_altfn20((1U << 12), GPIO_AF_LTDC);	/* R6 */ \
		arm_hardware_piog_altfn20((1U << 6), GPIO_AF_LTDC);		/* R7 */ \
		/* GREEN */ \
		arm_hardware_pioh_altfn20((1U << 13), GPIO_AF_LTDC);	/* G2 */ \
		arm_hardware_pioh_altfn20((1U << 14), GPIO_AF_LTDC);	/* G3 */ \
		arm_hardware_pioh_altfn20((1U << 15), GPIO_AF_LTDC);	/* G4 */ \
		arm_hardware_pioi_altfn20((1U << 0), GPIO_AF_LTDC);		/* G5 */ \
		arm_hardware_pioi_altfn20((1U << 1), GPIO_AF_LTDC);		/* G6 */ \
		arm_hardware_pioi_altfn20((1U << 2), GPIO_AF_LTDC);		/* G7 */ \
		/* BLUE */ \
		arm_hardware_piog_altfn20((1U << 11), GPIO_AF_LTDC);	/* B3 */ \
		arm_hardware_piog_altfn20((1U << 12), GPIO_AF_LTDC9);	/* B4 */ \
		arm_hardware_pioi_altfn20((1U << 5), GPIO_AF_LTDC);		/* B5 */ \
		arm_hardware_pioi_altfn20((1U << 6), GPIO_AF_LTDC);		/* B6 */ \
		arm_hardware_pioi_altfn20((1U << 7), GPIO_AF_LTDC);		/* B7 */ \
	} while (0)

	/* управление состоянием сигнала DISP панели */
	/* demode values: 0: static signal, 1: DE controlled */
	#define HARDWARE_LTDC_SET_DISP(demode, state) do { \
		const uint32_t DEmask = (1U << 13); /* PE13 */ \
		if (demode != 0) break; \
		/* const uint32_t VSYNC = (1U << 9); */ /* PI9 */ \
		/* while ((GPIOI->IDR & VSYNC) != 0) ; */ /* схема синхронизации стоит на плате дисплея. дождаться 0 */ \
		/* while ((GPIOI->IDR & VSYNC) == 0) ; */ /* дождаться 1 */ \
		arm_hardware_pioe_outputs(DEmask, ((state) != 0) * DEmask);	/* DE=DISP, pin 31 - можно менять только при VSYNC=1 */ \
	} while (0)
	/* управление состоянием сигнала MODE 7" панели */
	#define HARDWARE_LTDC_SET_MODE(state) do { \
		const uint32_t mask = (1U << 4); /* PF4 */ \
		arm_hardware_piof_outputs(mask, (state != 0) * mask);	/* PF4 MODE=state */ \
	} while (0)
#endif /* LCDMODE_LTDC */

#if LCDMODE_LQ043T3DX02K
	#define WITHLCDBACKLIGHT	1	// Имеется управление подсветкой дисплея
	#define WITHLCDBACKLIGHTMIN	0	// Нижний предел регулировки (показываетый на дисплее)
	#define WITHLCDBACKLIGHTMAX	4	// Верхний предел регулировки (показываетый на дисплее)
	#define WITHKBDBACKLIGHT	1	// Имеется управление подсветкой клавиатуры
#else
	#define WITHLCDBACKLIGHT	1	// Имеется управление подсветкой дисплея
	#define WITHLCDBACKLIGHTMIN	0	// Нижний предел регулировки (показываетый на дисплее)
	#define WITHLCDBACKLIGHTMAX	3	// Верхний предел регулировки (показываетый на дисплее)
	#define WITHKBDBACKLIGHT	1	// Имеется управление подсветкой клавиатуры
#endif

	/* макроопределение, которое должно включить в себя все инициализации */
	#define	HARDWARE_INITIALIZE() do { \
		HARDWARE_KBD_INITIALIZE(); \
		HARDWARE_DAC_INITIALIZE(); \
		HARDWARE_BL_INITIALIZE(); \
		/* HARDWARE_DCDC_INITIALIZE(); */ \
		TXDISABLE_INITIALIZE(); \
		TUNE_INITIALIZE(); \
		} while (0)


		// Bootloader parameters
		#define BOOTLOADER_APPAREA DRAM_MEM_BASE	/* адрес ОЗУ, куда перемещать application */
		#define BOOTLOADER_APPFULL (1024uL * 2048)	// 2MB

		#define BOOTLOADER_SELFBASE QSPI_MEM_BASE	/* адрес где лежит во FLASH образ application */
		#define BOOTLOADER_SELFSIZE (1024uL * 128)	// 128

		#define BOOTLOADER_APPBASE (BOOTLOADER_SELFBASE + BOOTLOADER_SELFSIZE)	/* адрес где лежит во FLASH образ application */
		#define BOOTLOADER_APPSIZE (BOOTLOADER_APPFULL - BOOTLOADER_SELFSIZE)	// 2048 - 128

		#define BOOTLOADER_PAGESIZE (1024uL * 4)	// W25Q32FV with 4 KB pages

		#define USBD_DFU_RAM_XFER_SIZE 1024
		#define USBD_DFU_FLASH_XFER_SIZE 256	// match to (Q)SPI FLASH MEMORY page size
		#define USBD_DFU_FLASHNAME "W25Q32FV"

#endif /* ARM_STM32MP1_CPUSTYLE_STORCH_V9_H_INCLUDED */
