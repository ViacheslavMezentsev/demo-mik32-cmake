/**
 * @file
 * Контроллер прямого доступа к памяти (ПДП, DMA)
 *
 * Специализированный контроллер прямого доступа к памяти (ПДП) реализует передачу
 * данных между ведомыми устройствами на коммутационной матрице без участия микроконтроллера,
 * поддерживая аппаратное адресное обращение к доменам MIK32, участвующими в обмене данными.
 * Контроллер ПДП имеет 4 независимых каналов, обеспечивающих работу в режимах память-память,
 * периферия-периферия, память-периферия.
 * Каждый из каналов ПДП имеет программируемый уровень приоритетов обработки запросов, задаваемый пользователем.
 */

#ifndef DMA_MC_H_INCLUDED
#define DMA_MC_H_INCLUDED

#define DMA_CHANNEL_COUNT 4

#define DMA_CH1_DST_OFFSET                  0x00
#define DMA_CH1_SRC_OFFSET                  0x04
#define DMA_CH1_LEN_OFFSET                  0x08
#define DMA_CH1_CFG_OFFSET                  0x0c
#define DMA_CH2_DST_OFFSET                  0x10
#define DMA_CH2_SRC_OFFSET                  0x14
#define DMA_CH2_LEN_OFFSET                  0x18
#define DMA_CH2_CFG_OFFSET                  0x1c
#define DMA_CH3_DST_OFFSET                  0x20
#define DMA_CH3_SRC_OFFSET                  0x24
#define DMA_CH3_LEN_OFFSET                  0x28
#define DMA_CH3_CFG_OFFSET                  0x2c
#define DMA_CH4_DST_OFFSET                  0x30
#define DMA_CH4_SRC_OFFSET                  0x34
#define DMA_CH4_LEN_OFFSET                  0x38
#define DMA_CH4_CFG_OFFSET                  0x3c
#define DMA_CONFIG_STATUS_OFFSET            0x40

// Линии запросов периферийных устройств, используемые в контроллере DMA

#define DMA_UART_0_INDEX            0
#define DMA_UART_1_INDEX            1
#define DMA_CRYPTO_INDEX            2
#define DMA_SPI_0_INDEX             3
#define DMA_SPI_1_INDEX             4
#define DMA_I2C_0_INDEX             5
#define DMA_I2C_1_INDEX             6
#define DMA_SPIFI_INDEX             7
#define DMA_TIMER32_1_INDEX         8
#define DMA_TIMER32_2_INDEX         9
#define DMA_DAC0_INDEX              10
#define DMA_DAC1_INDEX              11
#define DMA_TIMER32_0_INDEX         12

 // Регистр CHx_CFG (CH1..CH4) – предназначен для управления и получения статуса канала

/**
 * Разрешение работы канала:
 * «1» – инициализация работы канала;
 * «0» – принудительная остановка
 */
#define DMA_CH_CFG_ENABLE_S                 0
#define DMA_CH_CFG_ENABLE_M                 (1 << DMA_CH_CFG_ENABLE_S)
#define DMA_CH_CFG_DISABLE_M                (0 << DMA_CH_CFG_ENABLE_S)

/**
 * Приоритет канала:
 * «00» – низкий;
 * «01» – средний;
 * «10» – высокий;
 * «11» – приоритетный (очень высокий) 
 */
#define DMA_CH_CFG_PRIOR_S                  1
#define DMA_CH_CFG_PRIOR_M                  (0x3 << DMA_CH_CFG_PRIOR_S)
#define DMA_CH_CFG_PRIOR_LOW_M              (0b00 << DMA_CH_CFG_PRIOR_S)
#define DMA_CH_CFG_PRIOR_MEDIUM_M           (0b01 << DMA_CH_CFG_PRIOR_S)
#define DMA_CH_CFG_PRIOR_HIGH_M             (0b10 << DMA_CH_CFG_PRIOR_S)
#define DMA_CH_CFG_PRIOR_VERY_HIGH_M        (0b11 << DMA_CH_CFG_PRIOR_S)

/**
 * Режим адреса источника:
 * «0» – периферия;
 * «1» – память
 */
#define DMA_CH_CFG_READ_MODE_S              3
#define DMA_CH_CFG_READ_MODE_MEMORY_M       (1 << DMA_CH_CFG_READ_MODE_S)
#define DMA_CH_CFG_READ_MODE_PERIPHERY_M    (0 << DMA_CH_CFG_READ_MODE_S)

/**
 * Режим адреса назначения:
 * «0» – периферия;
 * «1» – память
 */
#define DMA_CH_CFG_WRITE_MODE_S             4
#define DMA_CH_CFG_WRITE_MODE_MEMORY_M      (1 << DMA_CH_CFG_WRITE_MODE_S)
#define DMA_CH_CFG_WRITE_MODE_PERIPHERY_M   (0 << DMA_CH_CFG_WRITE_MODE_S)

/**
 * Инкремент адреса источника:
 * «0» – нет инкремента;
 * «1» – есть инкремент
 */
#define DMA_CH_CFG_READ_INCREMENT_S         5
#define DMA_CH_CFG_READ_INCREMENT_M         (1 << DMA_CH_CFG_READ_INCREMENT_S)
#define DMA_CH_CFG_READ_NO_INCREMENT_M      (0 << DMA_CH_CFG_READ_INCREMENT_S)

/**
 * Инкремент адреса назначения:
 * «0» – нет инкремента;
 * «1» – есть инкремент
 */
#define DMA_CH_CFG_WRITE_INCREMENT_S        6
#define DMA_CH_CFG_WRITE_INCREMENT_M        (1 << DMA_CH_CFG_WRITE_INCREMENT_S)
#define DMA_CH_CFG_WRITE_NO_INCREMENT_M     (0 << DMA_CH_CFG_WRITE_INCREMENT_S)

/**
 * Разрядность адреса источника:
 * «00» – байт;
 * «01» – полуслово;
 * «10» – слово;
 * «11» - резерв
 * @warning должно быть кратно LEN
 */
#define DMA_CH_CFG_READ_SIZE_S              7
#define DMA_CH_CFG_READ_SIZE_BYTE_M         (0b00 << DMA_CH_CFG_READ_SIZE_S)  // байт
#define DMA_CH_CFG_READ_SIZE_2BYTE_M        (0b01 << DMA_CH_CFG_READ_SIZE_S) // полуслово
#define DMA_CH_CFG_READ_SIZE_4BYTE_M        (0b10 << DMA_CH_CFG_READ_SIZE_S) // слово
#define DMA_CH_CFG_READ_SIZE_REZ_M          (0b11 << DMA_CH_CFG_READ_SIZE_S)   // резерв

/**
 * Разрядность адреса назначения:
 * «00» – байт;
 * «01» – полуслово;
 * «10» – слово;
 * «11» - резерв
 * @warning должно быть кратно LEN
 */
#define DMA_CH_CFG_WRITE_SIZE_S             9
#define DMA_CH_CFG_WRITE_SIZE_BYTE_M        (0b00 << DMA_CH_CFG_WRITE_SIZE_S)  // байт
#define DMA_CH_CFG_WRITE_SIZE_2BYTE_M       (0b01 << DMA_CH_CFG_WRITE_SIZE_S) // полуслово
#define DMA_CH_CFG_WRITE_SIZE_4BYTE_M       (0b10 << DMA_CH_CFG_WRITE_SIZE_S) // слово
#define DMA_CH_CFG_WRITE_SIZE_REZ_M         (0b11 << DMA_CH_CFG_WRITE_SIZE_S)   // резерв

/**
 * Количество байт в пакете. Определяется как 2^READ_BURST_SIZE
 * @warning должно быть кратно READ_SIZE
 */
#define DMA_CH_CFG_READ_BURST_SIZE_S        11  // Кол-во байт пакетной передачи:   2^Read_burst_size
/**
 * Количество байт в пакете. Определяется как 2^WRITE_BURST_SIZE
 * @warning должно быть кратно WRITE_SIZE
 */
#define DMA_CH_CFG_WRITE_BURST_SIZE_S       14 // Кол-во байт пакетной передачи:   2^Write_burst_size

/**
 * Выбор периферийной линии источника
 */
#define DMA_CH_CFG_READ_REQUEST_S           17 // выбор канала чтения
#define DMA_CH_CFG_READ_REQUEST_M           (0xF << DMA_CH_CFG_READ_REQUEST_S)
#define DMA_CH_CFG_READ_REQUEST(v)          (((v) << DMA_CH_CFG_READ_REQUEST_S) & DMA_CH_CFG_READ_REQUEST_M)

/**
 * Выбор периферийной линии назначения
 */
#define DMA_CH_CFG_WRITE_REQUEST_S          21 // выбор канала записи
#define DMA_CH_CFG_WRITE_REQUEST_M          (0xF << DMA_CH_CFG_WRITE_REQUEST_S)
#define DMA_CH_CFG_WRITE_REQUEST(v)         (((v) << DMA_CH_CFG_WRITE_REQUEST_S) & DMA_CH_CFG_WRITE_REQUEST_M)

/**
 * Разрешение работы логики с откликом для адресата источника:
 * 0 – запрещено;
 * 1 – разрешено.
 */
#define DMA_CH_CFG_READ_ACK_EN_S            25
#define DMA_CH_CFG_READ_ACK_EN_M            (1 << DMA_CH_CFG_READ_ACK_EN_S)

/**
 * Разрешение работы логики с откликом для адресата назначения:
 * 0 – запрещено;
 * 1 – разрешено.
 */
#define DMA_CH_CFG_WRITE_ACK_EN_S           26
#define DMA_CH_CFG_WRITE_ACK_EN_M           (1 << DMA_CH_CFG_WRITE_ACK_EN_S)

/**
 * Разрешение формирования прерывания по завершении работы канала:
 * 0 – прерывание не формируется;
 * 1 – прерывание формируется.
 */
#define DMA_CH_CFG_IRQ_EN_S                 27
#define DMA_CH_CFG_IRQ_EN_M                 (1 << DMA_CH_CFG_IRQ_EN_S)

/* При чтении с CONFIG_STATUS.CURRENT_VALUE = 0 */
#define DMA_CH_CFG_BUS_ERROR_READ_S         0
#define DMA_CH_CFG_BUS_ERROR_READ_M         (1 << DMA_STATUS_BUS_ERROR_READ_S)
#define DMA_CH_CFG_BUS_ERROR_WRITE_S        1
#define DMA_CH_CFG_BUS_ERROR_WRITE_M        (1 << DMA_STATUS_BUS_ERROR_WRITE_S)
#define DMA_CH_CFG_STATE_S                  2
#define DMA_CH_CFG_STATE_M                  (1 << DMA_STATUS_STATE_S)
#define DMA_CH_CFG_STATE_READ               (0b01 << DMA_STATUS_STATE_S)
#define DMA_CH_CFG_STATE_WRITE              (0b10 << DMA_STATUS_STATE_S)

#define DMA_CHANNEL_M                       ((1 << DMA_CHANNEL_COUNT) - 1)

/**
 * Регистр CONFIG_STATUS обеспечивает управление прерываниями каналов и контроллера в целом. 
 * Функции отдельных полей регистра отличаются при выполнении операции чтения и записи. О
 */

#define DMA_STATUS_READY_S              0
/**
 * Статус состояния каналов:
 * «1» - готов к работе;
 * «0» - занят.
 */
#define DMA_STATUS_READY_M              (DMA_CHANNEL_M << DMA_STATUS_READY_S)
#define DMA_STATUS_READY(i)             ((1 << (DMA_STATUS_READY_S + (i))) & DMA_STATUS_READY_M)

#define DMA_STATUS_CHANNEL_IRQ_S        1 * DMA_CHANNEL_COUNT

/**
 * Статус прерываний:
 * «1» - есть прерывания;
 * «0» - нет прерываний.
 */
#define DMA_STATUS_CHANNEL_IRQ_M        (DMA_CHANNEL_M << DMA_STATUS_CHANNEL_IRQ_S)

#define DMA_STATUS_CHANNEL_BUS_ERROR_S  2 * DMA_CHANNEL_COUNT
/**
 * Статус состояния каналов при ошибках на шине:
 * «1» - есть ошибка;
 * «0» - нет ошибки
 */
#define DMA_STATUS_CHANNEL_BUS_ERROR_M  (DMA_CHANNEL_M << DMA_STATUS_CHANNEL_BUS_ERROR_S)

#define DMA_CONFIG_CLEAR_LOCAL_IRQ_S    0
#define DMA_CONFIG_CLEAR_LOCAL_IRQ_M    (DMA_CHANNEL_M << DMA_CONFIG_CLEAR_LOCAL_IRQ_S)
/**
 * Очистка локального прерывания: 
 * «1» – снятие запроса на прерывание.
 */
#define DMA_CONFIG_CLEAR_LOCAL_IRQ(i)   ((1 << (DMA_CONFIG_CLEAR_LOCAL_IRQ_S + (i))) & DMA_CONFIG_CLEAR_LOCAL_IRQ_M)

#define DMA_CONFIG_CLEAR_GLOBAL_IRQ_S   (DMA_CHANNEL_COUNT + 0)   // [4]
/**
 * Очистка глобального прерывания:
 * «1» – снятие запроса на прерывание.
 */
#define DMA_CONFIG_CLEAR_GLOBAL_IRQ_M   (1 << DMA_CONFIG_CLEAR_GLOBAL_IRQ_S)

#define DMA_CONFIG_CLEAR_ERROR_IRQ_S    (DMA_CHANNEL_COUNT + 1)    // [5]
/**
 * Очистка прерывания ошибки:
 * «1» – снятие запроса на прерывание.
 */
#define DMA_CONFIG_CLEAR_ERROR_IRQ_M    (1 << DMA_CONFIG_CLEAR_ERROR_IRQ_S)

#define DMA_CONFIG_GLOBAL_IRQ_ENA_S     (DMA_CHANNEL_COUNT + 2) // [6]
/**
 * Разрешение формирования глобального прерывания:
 * «1» – разрешено;
 * «0» – запрещено
 */
#define DMA_CONFIG_GLOBAL_IRQ_ENA_M     (1 << DMA_CONFIG_GLOBAL_IRQ_ENA_S)

#define DMA_CONFIG_ERROR_IRQ_ENA_S      (DMA_CHANNEL_COUNT + 3)  // [7]
/**
 * Разрешение формирования прерывания при ошибке:
 * «1» – разрешено;
 * «0» – запрещено
 */
#define DMA_CONFIG_ERROR_IRQ_ENA_M      (1 << DMA_CONFIG_ERROR_IRQ_ENA_S)

#define DMA_CONFIG_CURRENT_VALUE_S      (DMA_CHANNEL_COUNT + 4)  // [8]
/**
 * Разрешение чтения текущего статуса канала:
 * «0» – разрешено (текущие значения);
 * «1» – запрещено (значения при настройке)
 */
#define DMA_CONFIG_CURRENT_VALUE_M      (1 << DMA_CONFIG_CURRENT_VALUE_S)

#ifndef __ASSEMBLER__
#include <inttypes.h>
/// @brief Структура регистров настройки параметров и управления каналом
typedef struct
{
    volatile uint32_t   DST;    ///< [RW]: регистр адреса назначения канала. Биты [31:0] хранят адрес назначения.
    volatile uint32_t   SRC;    ///< [RW]: регистр адреса источника канала. Биты [31:0] хранят адрес источника.
    volatile uint32_t   LEN;    ///< [RW]: регистр передаваемых данных  канала. Биты [31:0] количество байт пересылки вычисляется как LEN+1.
    volatile uint32_t   CFG;    ///< [RW]: регистр управления конфигурацией канала. Биты [27:0] кодируют настройки конфигурации канала.
} DMA_CHANNEL_TypeDef;

/// @brief Структура регистров контроллера прямого доступа к памяти
typedef struct
{
    DMA_CHANNEL_TypeDef CHANNELS[DMA_CHANNEL_COUNT];    ///< [RW]: массив регистров каналов ПДП
    volatile uint32_t   CONFIG_STATUS;                  ///< [RW]: регистр прерываний и настройки контроллера
} DMA_CONFIG_TypeDef;
#endif

#endif //
