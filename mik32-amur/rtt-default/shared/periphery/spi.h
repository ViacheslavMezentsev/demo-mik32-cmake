/**
 * @file
 * Модуль SPI
 * 
 * Модуль контроллера SPI обеспечивает взаимодействие с различными периферийными устройствами такими, 
 * как блоки памяти, датчики температуры, датчики давления, аналоговые преобразователи, 
 * часы реального времени, дисплеи и любые SD карты, поддерживающие последовательный режим обмена.
 * 
 * В режиме ведущего устройства, модуль контроллера SPI управляет последовательным сигналом синхронизации 
 * и сигналом выбора ведомого устройства с поддержкой режима несколько ведущий устройств. 
 * Последовательный сигнал синхронизации формируется из сигнала синхронизации подсистемы. Контроллер инициирует 
 * передачу сообщений, используя для этого до 4-х выходных сигналов выбора ведомого, 
 * которые могут быть внешне расширены. Контроллер считывает и записывает данные из ведомых устройств, 
 * используя 32 битный регистр данных чтения/записи. 
 * В режиме ведомого устройства, модуль контроллера SPI принимает сигнал синхронизации от внешнего устройства 
 * и использует его в качестве сигнала SPI_Ref_Clk для синхронизации захвата данных. 
 * 
 * В режиме ведомого устройства в контроллере реализован программируемый механизм распознавания начала передачи, 
 * при котором контроллер активируется удержанием сигнал выбора ведомого SS. FIFO для чтения и записи обеспечивают 
 * буферизацию между интерфейсом вводы/вывода SPI и ПО модуля, взаимодействующего с ним через интерфейс APB. 
 * Буферы FIFO используются как в режиме ведомого, так и в режиме ведущего устройства.
 */

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#define SPI_CONFIG_OFFSET        	         0x00
#define SPI_STATUS_OFFSET        	         0x04
#define SPI_IEN_OFFSET           	         0x08 
#define SPI_IDIS_OFFSET          	         0x0C 
#define SPI_IMASK_OFFSET         	         0x10 
#define SPI_ENABLE_OFFSET        	         0x14
#define SPI_DELAY_OFFSET         	         0x18
#define SPI_TXD_OFFSET           	         0x1C
#define SPI_RXD_OFFSET           	         0x20
#define SPI_SIC_OFFSET           	         0x24
#define SPI_THRESHOLD_OFFSET     	         0x28
#define SPI_MODULE_ID_OFFSET     	         0xFC

//  Регистр CONFIG модуля контроллера SPI

#define SPI_CONFIG_MANUAL_CS_S		              14
/**
 * Выбор режима управления сигналом выбора ведомого:
 * «1» – ручной режим;
 * «0» – автоматический режим
 */
#define SPI_CONFIG_MANUAL_CS_M		              (1 << SPI_CONFIG_MANUAL_CS_S)

#define SPI_CONFIG_CS_S				                  10
/**
 * Выбор ведомых устройств:
 * «xxx0» – выбрано устройство 4;
 * «xx01» – выбрано устройство 1;
 * «x011» – выбрано устройство 2;
 * «0111» – выбрано устройство 3;
 * «1111» – ведомые устройства не выбраны
 */
#define SPI_CONFIG_CS_M				                  (0xF << SPI_CONFIG_CS_S)
#define SPI_CONFIG_CS_0_M			                  (0xE << SPI_CONFIG_CS_S)
#define SPI_CONFIG_CS_1_M			                  (0xD << SPI_CONFIG_CS_S)
#define SPI_CONFIG_CS_2_M			                  (0xB << SPI_CONFIG_CS_S)
#define SPI_CONFIG_CS_3_M			                  (0x7 << SPI_CONFIG_CS_S)
#define SPI_CONFIG_CS_NONE_M		                (0xF << SPI_CONFIG_CS_S)

#define SPI_CONFIG_PERI_SEL_S		                9
/**
 * Выбор периферийного деэнкодера:
 * «1» – использование внешнего деэнкодера;
 * «0» – выбор только 1 из 4 устройств
 */
#define SPI_CONFIG_PERI_SEL_M		                (1 << SPI_CONFIG_PERI_SEL_S)

#define SPI_CONFIG_REF_CLK_S		                8
/**
 * Выбор опорной тактовой частоты:
 * «1» – не поддерживается;
 * «0» – используется опорная частота SPI
 */
#define SPI_CONFIG_REF_CLK_M		                (1 << SPI_CONFIG_REF_CLK_S)

#define SPI_CONFIG_BAUD_RATE_DIV_S		          3
/**
 * Управляет скоростью передачи данных, задает коэффициент деления частоты spi_ref_clk:
 * «0» – деление на 2;
 * «1» – деление на 4;
 * «2» – деление на 8;
 * «3» – деление на 16;
 * «4» – деление на 32;
 * «5» – деление на 64;
 * «6» – деление на 128;
 * «7» – деление на 256
 */
#define SPI_CONFIG_BAUD_RATE_DIV_M	            (0x7 << SPI_CONFIG_BAUD_RATE_DIV_S) 
#define SPI_CONFIG_BAUD_RATE_DIV_2_M	          (0x0 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_4_M	          (0x1 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_8_M	          (0x2 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_16_M	          (0x3 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_32_M	          (0x4 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_64_M	          (0x5 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_128_M	        (0x6 << SPI_CONFIG_BAUD_RATE_DIV_S)
#define SPI_CONFIG_BAUD_RATE_DIV_256_M	        (0x7 << SPI_CONFIG_BAUD_RATE_DIV_S)

#define SPI_MAXIMUM_BAUD_RATE_DIV		            256

#define SPI_CONFIG_CLK_PH_S				              2
/**
 * Выбор фазы тактового сигнала:
 * «1» – тактовая частота SPI неактивна вне слова;
 * «0» – тактовая частота SPI активна вне слова
 */
#define SPI_CONFIG_CLK_PH_M				              (1 << SPI_CONFIG_CLK_PH_S)

#define SPI_CONFIG_CLK_POL_S			              1
/**
 * Выбор полярности тактового сигнала вне слова:
 * «1» – тактовый сигнал удерживается на высоком уровне;
 * «0» – тактовый сигнал удерживается на низком уровне
 */
#define SPI_CONFIG_CLK_POL_M			              (1 << SPI_CONFIG_CLK_POL_S)

#define SPI_CONFIG_MODE_SEL_S			              0
/**
 * Выбор режима:
 * «1» – SPI в режиме ведущего устройства;
 * «0» – SPI в режиме ведомого устройства
 */
#define SPI_CONFIG_MODE_SEL_M			              (1 << SPI_CONFIG_MODE_SEL_S)
#define SPI_CONFIG_MASTER_M				              (1 << SPI_CONFIG_MODE_SEL_S)
#define SPI_CONFIG_SLAVE_M				              (0 << SPI_CONFIG_MODE_SEL_S)

//  Регистр ENABLE модуля контроллера SPI

#define SPI_ENABLE_CLEAR_RX_FIFO_S              3
/// Запись 1 при SPI_EN = 0 очищает буфер RX_FIFO.
#define SPI_ENABLE_CLEAR_RX_FIFO_M              (1 << SPI_ENABLE_CLEAR_RX_FIFO_S)

#define SPI_ENABLE_CLEAR_TX_FIFO_S              2
/// Запись 1 при SPI_EN = 0 очищает буфер TX_FIFO.
#define SPI_ENABLE_CLEAR_TX_FIFO_M              (1 << SPI_ENABLE_CLEAR_TX_FIFO_S)

#define SPI_ENABLE_S		                        0
/**
 * «1» – включение модуля SPI;
 * «0» – выключение модуля SPI
 */
#define SPI_ENABLE_M		                        (1 << SPI_ENABLE_S)

//  Регистр DELAY модуля контроллера SPI

#define SPI_DELAY_BTWN_S                        16
/**
 * Задержка в периодах опорного тактового сигнала или внешнего тактового сигнала ext_clk 
 * между снятием сигнала выбора одного ведомого устройства 
 * и установкой сигнала выбора другого ведомого устройства
 */
#define SPI_DELAY_BTWN_M                        (0xFF << SPI_DELAY_BTWN_S)

#define SPI_DELAY_BTWN(V)                       (((V) << SPI_DELAY_BTWN_S) & SPI_DELAY_BTWN_M)

#define SPI_DELAY_AFTER_S                       8
/**
 * Задержка в периодах опорного тактового сигнала или внешнего тактового сигнала 
 * ext_clk между последним битом текущего слова и первым битом следующего слова
 */
#define SPI_DELAY_AFTER_M                       (0xFF << SPI_DELAY_AFTER_S)

#define SPI_DELAY_AFTER(V)                      (((V) << SPI_DELAY_AFTER_S) & SPI_DELAY_AFTER_M)

#define SPI_DELAY_INIT_S                        0
/**
 * Дополнительная задержка в периодах опорного тактового сигнала или внешнего тактового сигнала ext_clk между 
 * установкой сигнала n_ss_out в «0» и передачей первого бита
 */
#define SPI_DELAY_INIT_M                        (0xFF << SPI_DELAY_INIT_S)

#define SPI_DELAY_INIT(V)                       (((V) << SPI_DELAY_INIT_S) & SPI_DELAY_INIT_M)

//  Регистр INT_STATUS модуля контроллера SPI

#define SPI_INT_STATUS_SPI_ACTIVE_S             15
/**
 * Статус сеанса передачи:
 * «1» – контроллер SPI в процессе обмена;
 * «0» – контроллер SPI в состоянии ожидания обмена
 */
#define SPI_INT_STATUS_SPI_ACTIVE_M             (1 << SPI_INT_STATUS_SPI_ACTIVE_S)

#define SPI_INT_STATUS_TX_FIFO_UNDERFLOW_S      6
/**
 * Регистр TX FIFO опустошен:
 * «1» – определение опустошения;
 * «0» – опустошение не детектируется
 */
#define SPI_INT_STATUS_TX_FIFO_UNDERFLOW_M      (1 << SPI_INT_STATUS_TX_FIFO_UNDERFLOW_S)

#define SPI_INT_STATUS_RX_FIFO_FULL_S		        5
/**
 * Регистр RX_FIFO заполнен:
 * «1» – FIFO заполнен;
 * «0» – FIFO не заполнен
 */
#define SPI_INT_STATUS_RX_FIFO_FULL_M		        (1 << SPI_INT_STATUS_RX_FIFO_FULL_S)

#define SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_S	    4
/**
 * Регистр RX_FIFO не пустой:
 * «1» – в FIFO есть хотя бы один байт;
 * «0» – FIFO пустой
 */
#define SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_M	    (1 << SPI_INT_STATUS_RX_FIFO_NOT_EMPTY_S)

#define SPI_INT_STATUS_TX_FIFO_FULL_S		        3
/**
 * Регистр TX_FIFO заполнен:
 * «1» – FIFO заполнен;
 * «0» – FIFO не заполен
 */
#define SPI_INT_STATUS_TX_FIFO_FULL_M		        (1 << SPI_INT_STATUS_TX_FIFO_FULL_S)

#define SPI_INT_STATUS_TX_FIFO_NOT_FULL_S	      2
/**
 * Регистр TX_FIFO не заполнен:
 * «1» – регистр заполнен меньше чем THRESHOLD;
 * «0» – регистр заполнен до значение THRESHOLD или больше
 */
#define SPI_INT_STATUS_TX_FIFO_NOT_FULL_M	      (1 << SPI_INT_STATUS_TX_FIFO_NOT_FULL_S)

#define SPI_INT_STATUS_MODE_FAIL_S			        1
/**
 * Напряжение на выводе n_ss_in не соответствую режиму работы SPI:
 * «1» – если n_ss_in имеет низкий уровень в режиме ведомого устройства;
 * «0» – корректная работа
 */
#define SPI_INT_STATUS_MODE_FAIL_M			        (1 << SPI_INT_STATUS_MODE_FAIL_S)

#define SPI_INT_STATUS_RX_OVERFLOW_S		        0
/**
 * Прерывание при переполнении RX_FIFO, значение сбрасывается при чтении:
 * «l» – переполнение RX_FIFO;
 * «0» – нет переполнения
 */
#define SPI_INT_STATUS_RX_OVERFLOW_M		        (1 << SPI_INT_STATUS_RX_OVERFLOW_S)


//TODO: Добавлено описание. Регистр INT_ENABLE модуля контроллера SPI

/// Запись «1» устанавливает маску прерывания (разрешает прерывание) при опустошении TX_FIFO
#define SPI_INT_ENABLE_TX_FIFO_UNDERFLOW          (1 << 6)
/// Запись «1» устанавливает маску прерывания (разрешает прерывание) при заполнении RX_FIFO
#define SPI_INT_ENABLE_RX_FIFO_FULL               (1 << 5)
/// Запись «1» устанавливает маску прерывания (разрешает прерывание), когда RX_FIFO не пустой
#define SPI_INT_ENABLE_RX_FIFO_NOT_EMPTY          (1 << 4)
/// Запись «1» устанавливает маску прерывания (разрешает прерывание), когда TX_FIFO заполнен
#define SPI_INT_ENABLE_TX_FIFO_FULL               (1 << 3)
/// Запись «1» устанавливает маску прерывания (разрешает прерывание) когда TX_FIFO не заполнен
#define SPI_INT_ENABLE_TX_FIFO_NOT_FULL           (1 << 2)
/// Запись «1» устанавливает маску прерывания (разрешает прерывание) при нарушении режима
#define SPI_INT_ENABLE_MODE_FAIL                  (1 << 1)
/// Запись «1» устанавливает маску прерывания (разрешает прерывание) при переполнении при прием
#define SPI_INT_ENABLE_RX_OVERFLOW                (1 << 0)

//TODO: Добавлено описание. Регистр INT_DISABLE модуля контроллера SPI

/// Запись «1» сбрасывает маску прерывания (запрещает прерывание) при опустошении TX_FIFO
#define SPI_INT_DISABLE_TX_FIFO_UNDERFLOW         (1 << 6)
/// Запись «1» сбрасывает маску прерывания (запрещает прерывание) при заполнении RX_FIFO
#define SPI_INT_DISABLE_RX_FIFO_FULL              (1 << 5)
/// Запись «1» сбрасывает маску прерывания (запрещает прерывание), когда RX_FIFO не пустой
#define SPI_INT_DISABLE_RX_FIFO_NOT_EMPTY         (1 << 4)
/// Запись «1» сбрасывает маску прерывания (запрещает прерывание), когда TX_FIFO заполнен
#define SPI_INT_DISABLE_TX_FIFO_FULL              (1 << 3)
/// Запись «1» сбрасывает маску прерывания (запрещает прерывание), когда TX_FIFO не заполнен
#define SPI_INT_DISABLE_TX_FIFO_NOT_FULL          (1 << 2)
/// Запись «1» сбрасывает маску прерывания (запрещает прерывание) при нарушении режима
#define SPI_INT_DISABLE_MODE_FAIL                 (1 << 1)
/// Запись «1» сбрасывает маску прерывания (запрещает прерывание) при переполнении при приеме
#define SPI_INT_DISABLE_RX_OVERFLOW               (1 << 0)

//TODO: Добавлено описание. Регистр INT_MASK модуля контроллера SPI

/**
 * Текущее состояние маски прерывания при опустошении TX_FIFO:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_TX_FIFO_UNDERFLOW         (1 << 6)
/**
 * Текущее состояние маски прерывания при заполнении RX_FIFO:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_RX_FIFO_FULL              (1 << 5)
/**
 * Текущее состояние маски прерывания, когда RX_FIFO не пустой:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_RX_FIFO_NOT_EMPTY         (1 << 4)
/**
 * Текущее состояние маски прерывания, когда TX_FIFO заполнен:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_TX_FIFO_FULL              (1 << 3)
/**
 * Текущее состояние маски прерывания, когда TX_FIFO не заполнен:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_TX_FIFO_NOT_FULL          (1 << 2)
/**
 * Текущее состояние маски прерывания при нарушении режима:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_MODE_FAIL                 (1 << 1)
/**
 * Текущее состояние маски прерывания при переполнении при приеме:
 * «0» – прерывание запрещено;
 * «1» – прерывание разрешено
 */
#define SPI_INT_MASK_RX_OVERFLOW               (1 << 0)

/**
 * Регистр TXDATA модуля контроллера SPI
 * [WO]: TX_FIFO_DATA [7:0] - Данные для TX_FIFO.
 */

/**
 *  Регистр RXDATA модуля контроллера SPI
 * [RO]: RX_FIFO_DATA [7:0] - Данные из RX_FIFO.
 */

/** 
 * Регистр SIC модуля контроллера SPI
 * [RW]: SLAVE_IDLE_COUN [7:0] Модуль SPI в режиме ведомого устройства начинает 
 * передачу только когда тактовый сигнал sclk_in (внешнего ведущего устройства) 
 * не изменяется в течение количества периодов опорного тактового сигнала SPI 
 * заданного в этом поле или когда модуль SPI не активен
*/

/**
 * Регистр TX_thres_reg0 модуля контроллера SPI
 * [RW]: THRESHOLD_OF_TX_FIFO [31:0] - Задает уровень, при котором TX_FIFO считается 
 * не заполненным и формируется прерывание
 */

/**
 * Регистр ID модуля контроллера SPI
 * [RO]: ID [31:0] - Идентификационный номер модуля
 */

#ifndef __ASSEMBLER__
    #include <inttypes.h> 
    /// @brief Структура регистров модуля SPI
    typedef struct
    {
      volatile uint32_t CONFIG;                     ///< Регистр конфигурации
      volatile uint32_t INT_STATUS;                 ///< Регистр статуса прерываний
      volatile uint32_t INT_ENABLE;                 ///< Регистр разрешения прерываний
      volatile uint32_t INT_DISABLE;                ///< Регистр запрета прерываний
      volatile uint32_t INT_MASK;                   ///< Регистр текущих масок прерываний
      volatile uint32_t ENABLE;                     ///< Регистр включения/выключения модуля
      volatile uint32_t DELAY;                      ///< Регистр задержек
      volatile uint32_t TXDATA;                     ///< Регистр передаваемых данных
      volatile uint32_t RXDATA;                     ///< Регистр принимаемых данных
      volatile uint32_t SIC;                        ///< Регистр счетчика отстанова ведомого устройства
      volatile uint32_t TX_THR;                     ///< Регистр пороговых значений TX_FIFO
      volatile uint32_t reserved[0X34];             ///< Резерв
      volatile uint32_t ID;                         ///< Идентификационный номер модуля
    }SPI_TypeDef;
#endif


#endif // SPI_H_INCLUDED
