/**
 * @file
 * Блок управления питанием и тактированием доменов (PowerManager, PM)
 * 
 * Блок управления питанием и тактированием доменов обеспечивает управление тактовыми сигналами 
 * (установка коэффициентов делителей для шин, отключение тактирования отдельных устройств). 
 */

#ifndef PM_H_INCLUDED
#define PM_H_INCLUDED

#define PM_DIV_AHB_OFFSET                   0x00
#define PM_DIV_APB_M_OFFSET                 0x04
#define PM_DIV_APB_P_OFFSET                 0x08
#define PM_CLK_AHB_SET_OFFSET               0x0C
#define PM_CLK_AHB_CLEAR_OFFSET             0x10
#define PM_CLK_APB_M_SET_OFFSET             0x14
#define PM_CLK_APB_M_CLEAR_OFFSET           0x18
#define PM_CLK_APB_P_SET_OFFSET             0x1C
#define PM_CLK_APB_P_CLEAR_OFFSET           0x20
#define PM_AHB_CLK_MUX_OFFSET               0x24
#define PM_WDT_CLK_MUX_OFFSET               0x28
#define PM_CPU_RTC_CLK_MUX_OFFSET           0x2C
#define PM_TIMER_CFG_OFFSET                 0x30
#define PM_FREQ_MASK_OFFSET                 0x34
#define PM_FREQ_STATUS_OFFSET               0x38
#define PM_SLEEP_MODE_OFFSET                0x3C

// Индексы таймеров, подключенных к выводам мультиплексора блока управления питанием (PM)

#define PM_TIMER32_0_INDEX                  0
#define PM_TIMER32_1_INDEX                  1
#define PM_TIMER32_2_INDEX                  2
#define PM_TIMER16_0_INDEX                  3
#define PM_TIMER16_1_INDEX                  4
#define PM_TIMER16_2_INDEX                  5

// Перечень значений регистра настройки источника тактирования системы (AHB_MUX)

#define PM_AHB_CLK_MUX_S             0
/**
 * Выбор источника тактирования сторожевого таймера:
 * 0 – внешний OSC32M;
 * 1 – внутренний HSI32M;
 * 2 – внешний OSC32K;
 * 3 – внутренний LSI32К;
 */
#define PM_AHB_CLK_MUX_M            (0x3 << PM_AHB_CLK_MUX_S)

#define PM_AHB_CLK_MUX_OSC32M_M     (0x0 << PM_AHB_CLK_MUX_S)
#define PM_AHB_CLK_MUX_HSI32M_M     (0x1 << PM_AHB_CLK_MUX_S)
#define PM_AHB_CLK_MUX_OSC32K_M     (0x2 << PM_AHB_CLK_MUX_S)
#define PM_AHB_CLK_MUX_LSI32K_M     (0x3 << PM_AHB_CLK_MUX_S)

#define PM_AHB_FORCE_MUX_S           2
/**
 * Запрет на принудительное переключение с выбранного источника тактирования при пропадании тактирования
 * 0 – разрешение автоматической смены частоты;
 * 1 – запрет автоматической смены частоты;
 */
#define PM_AHB_FORCE_MUX_M          (1 << PM_AHB_FORCE_MUX_S)

#define PM_AHB_FORCE_MUX_FIXED      (1 << PM_AHB_FORCE_MUX_S)
#define PM_AHB_FORCE_MUX_UNFIXED    (0 << PM_AHB_FORCE_MUX_S)


// Регистр выбора источника тактирования сторожевого таймера (WDT_CLK_MUX )

#define PM_WDT_CLK_MUX_S            0
/**
 * Выбор источника тактирования сторожевого таймера:
 * 0 – внешний OSC32M;
 * 1 – внутренний HSI32M;
 * 2 – внешний OSC32K;
 * 3 – внутренний LSI32К;
 */
#define PM_WDT_CLK_MUX_M            (3 << PM_WDT_CLK_MUX_S)

#define PM_WDT_CLK_MUX_OSC32M_M     (0 << PM_WDT_CLK_MUX_S)
#define PM_WDT_CLK_MUX_HSI32M_M     (1 << PM_WDT_CLK_MUX_S)
#define PM_WDT_CLK_MUX_OSC32K_M     (2 << PM_WDT_CLK_MUX_S)
#define PM_WDT_CLK_MUX_LSI32K_M     (3 << PM_WDT_CLK_MUX_S)

// Регистр выбора источника тактирования RTC в составе ядра (CPU_RTC_CLK)

#define PM_CPU_RTC_CLK_MUX_S             0
/**
 * Выбор источника тактирования RTC в составе ядра:
 * 0 – внешний OSC32K;
 * 1 – внутренний LSI32К;
 */
#define PM_CPU_RTC_CLK_MUX_M            (1 << PM_CPU_RTC_CLK_MUX_S)

#define PM_CPU_RTC_CLK_MUX_OSC32K_M     (0 << PM_CPU_RTC_CLK_MUX_S)
#define PM_CPU_RTC_CLK_MUX_LSI32K_M     (1 << PM_CPU_RTC_CLK_MUX_S)

// Регистр выбора источника тактирования для таймеров (TIMER_CFG)

#define PM_TIMER_CFG_MUX_TIMER32_0_S    0
#define PM_TIMER_CFG_MUX_TIMER32_1_S    3
#define PM_TIMER_CFG_MUX_TIMER32_2_S    6
/// Тактирование Timer32_x от системной частоты (sys_clk)
#define PM_TIMER_CFG_MUX_TIMER32_TIM1_SYS_CLK_M(timer_index)  (0b00 << (timer_index))
/// TODO: Уточнить стр. 287 (значение совпадает с тактированием от OSC32K)!!! Тактирование Timer32_x от частоты шины AHB (hclk)
#define PM_TIMER_CFG_MUX_TIMER32_TIM1_HCLK_M(timer_index)     (0b01 << (timer_index))
/// TODO: Уточнить стр. 287 (значение совпадает с тактированием от шины AHB)!!! Тактирование Timer32_x от внешнего осциллятора OSC32K
#define PM_TIMER_CFG_MUX_TIMER32_TIM2_OSC32K_M(timer_index)   (0b01 << (timer_index))
/// TODO: Уточнить стр. 278 (MUX_TIM32_0[1] = 1)!!! Тактирование Timer32_x от встроенного осциллятора LSI32K
#define PM_TIMER_CFG_MUX_TIMER32_TIM2_LSI32K_M(timer_index)   (0b10 << (timer_index))

#define PM_TIMER_CFG_MUX_TIMER16_0_S    9
#define PM_TIMER_CFG_MUX_TIMER16_1_S    12
#define PM_TIMER_CFG_MUX_TIMER16_2_S    15
/// Тактирование Timer16_x от системной частоты (sys_clk)
#define PM_TIMER_CFG_MUX_TIMER16_SYS_CLK_M(timer_index)     (0x0 << (timer_index))
/// Тактирование Timer16_x от частоты шины AHB (hclk)
#define PM_TIMER_CFG_MUX_TIMER16_HCLK_M(timer_index)        (0x1 << (timer_index))
/// Тактирование Timer16_x от внешнего осциллятора OSC32M
#define PM_TIMER_CFG_MUX_TIMER16_OSC32M_M(timer_index)      (0x2 << (timer_index))
/// Тактирование Timer16_x от встроенного осциллятора HSI32M
#define PM_TIMER_CFG_MUX_TIMER16_HSI32M_M(timer_index)      (0x3 << (timer_index))
/// Тактирование Timer16_x от внешнего осциллятора OSC32K
#define PM_TIMER_CFG_MUX_TIMER16_OSC32K_M(timer_index)      (0x4 << (timer_index))
/// Тактирование Timer16_x от встроенного осциллятора HSI32K
#define PM_TIMER_CFG_MUX_TIMER16_LSI32K_M(timer_index)      (0x5 << (timer_index))

#define PM_TIMER_CFG_MUX_TIMER_M(timer_index)               (0x7 << (timer_index))

// Регистр настройки прерываний монитора частоты (FREQ_MASK)

#define PM_FREQ_MASK_FORCE_DIV_S        4
/// TODO: НЕДОКУМЕНТИРОВАННО!!! Неопределенное поведение.
#define PM_FREQ_MASK_FORCE_DIV_M        (1 << PM_FREQ_MASK_FORCE_DIV_S)

#define PM_FREQ_MASK_OSC32M_S           3
/// Разрешение прерывания при пропадании частоты внешнего осциллятора 32 МГц
#define PM_FREQ_MASK_OSC32M_M           (1 << PM_FREQ_MASK_OSC32M_S)

#define PM_FREQ_MASK_HSI32M_S           2
/// Разрешение прерывания при при пропадании частоты HSI32M
#define PM_FREQ_MASK_HSI32M_M           (1 << PM_FREQ_MASK_HSI32M_S)

#define PM_FREQ_MASK_OSC32K_S           1
/// Разрешение прерывания при при пропадании частоты внешнего осциллятора 32 кГц
#define PM_FREQ_MASK_OSC32K_M           (1 << PM_FREQ_MASK_OSC32K_S)

#define PM_FREQ_MASK_LSI32K_S           0
/// Разрешение прерывания при при пропадании частоты LSI32K
#define PM_FREQ_MASK_LSI32K_M           (1 << PM_FREQ_MASK_LSI32K_S)

// Маски статуса монитора частоты

#define PM_FREQ_STATUS_OSC32M_S         3
// Статус частоты внешнего осциллятора OSC32M
#define PM_FREQ_STATUS_OSC32M_M         (1 << PM_FREQ_STATUS_OSC32M_S)

#define PM_FREQ_STATUS_HSI32M_S         2
/// Статус частоты внутреннего осциллятора LSI32M
#define PM_FREQ_STATUS_HSI32M_M         (1 << PM_FREQ_STATUS_HSI32M_S)

#define PM_FREQ_STATUS_OSC32K_S         1
// Статус частоты внешнего осциллятора OSC32K
#define PM_FREQ_STATUS_OSC32K_M         (1 << PM_FREQ_STATUS_OSC32K_S)

#define PM_FREQ_STATUS_LSI32K_S         0
/// Статус частоты внутреннего осциллятора LSI32K
#define PM_FREQ_STATUS_LSI32K_M         (1 << PM_FREQ_STATUS_LSI32K_S)

// Перечень значений регистра включения спящего режима (SLEEP_MODE)

#define PM_SLEEP_MODE_S             0
/// Переход в спящий режим одновременно ЭСППЗУ (EEPROM), ОЗУ (RAM) и контроллера SPIFI
#define PM_SLEEP_MODE_M             (7 << PM_SLEEP_MODE_S)

#define PM_SLEEP_MODE_EEPROM_S      0
/// Переход в спящий режим только ЭСППЗУ (EEPROM)
#define PM_SLEEP_MODE_EEPROM_M      (1 << PM_SLEEP_MODE_EEPROM_S)

#define PM_SLEEP_MODE_RAM_S         1
/// Переход в спящий режим только ОЗУ (RAM)
#define PM_SLEEP_MODE_RAM_M         (1 << PM_SLEEP_MODE_RAM_S)

#define PM_SLEEP_MODE_SPIFI_S       2
/// Переход в спящий режим только контроллера SPIFI
#define PM_SLEEP_MODE_SPIFI_M       (1 << PM_SLEEP_MODE_SPIFI_S)

// Регистр включения/выключения тактирования устройств на шине AHB (CLK_AHB_SET / CLK_AHB_CLEAR)

#define PM_CLK_AHB_CPU_S              0
/// Включение тактирования ядра МК
#define PM_CLK_AHB_CPU_M              (1 << PM_CLK_AHB_CPU_S)

#define PM_CLK_AHB_EEPROM_S           1
/// Включение тактирования ЭСППЗУ (EEPROM)
#define PM_CLK_AHB_EEPROM_M           (1 << PM_CLK_AHB_EEPROM_S)

#define PM_CLK_AHB_RAM_S              2
/// Включение тактирования ОЗУ (RAM)
#define PM_CLK_AHB_RAM_M              (1 << PM_CLK_AHB_RAM_S)

#define PM_CLK_AHB_SPIFI_S            3
/// Включение тактирования контроллера SPIFI
#define PM_CLK_AHB_SPIFI_M            (1 << PM_CLK_AHB_SPIFI_S)

#define PM_CLK_AHB_TCB_S              4
/// Включение тактирования контроллера TCB
#define PM_CLK_AHB_TCB_M              (1 << PM_CLK_AHB_TCB_S)

#define PM_CLK_AHB_DMA_S              5
/// Включение тактирования контроллера ПДП (DMA)
#define PM_CLK_AHB_DMA_M              (1 << PM_CLK_AHB_DMA_S)

#define PM_CLK_AHB_CRYPTO_S           6
/// Включение тактирования ускорителя криптографии
#define PM_CLK_AHB_CRYPTO_M           (1 << PM_CLK_AHB_CRYPTO_S)

#define PM_CLK_AHB_CRC32_S            7
/// Включение тактирования CRC32
#define PM_CLK_AHB_CRC32_M            (1 << PM_CLK_AHB_CRC32_S)

// Перечень значений для включения/выключения тактирования устройств на шине APB_M (CLK_APB_M_SET / CLK_APB_M_CLEAR)

#define PM_CLK_APB_M_PM_S               0
/// Включить тактирование блока управления питанием (PM)
#define PM_CLK_APB_M_PM_M               (1 << PM_CLK_APB_M_PM_S)

#define PM_CLK_APB_M_EPIC_S             1
/// Включить тактирование контроллера прерываний EPIC
#define PM_CLK_APB_M_EPIC_M             (1 << PM_CLK_APB_M_EPIC_S)

#define PM_CLK_APB_M_TIMER32_0_S        2
/// Включить тактирование Timer32_0
#define PM_CLK_APB_M_TIMER32_0_M        (1 << PM_CLK_APB_M_TIMER32_0_S)

#define PM_CLK_APB_M_PAD_CONFIG_S       3
/// Включить тактирование контроллера выводов (PAD)
#define PM_CLK_APB_M_PAD_CONFIG_M       (1 << PM_CLK_APB_M_PAD_CONFIG_S)

#define PM_CLK_APB_M_WDT_BUS_S          4
/// Включить тактирование сторожевого таймера шины (WDT)
#define PM_CLK_APB_M_WDT_BUS_M          (1 << PM_CLK_APB_M_WDT_BUS_S)

#define PM_CLK_APB_M_OTP_CONTROLLER_S   5
/// Включить тактирование контроллера однократно программируемой ПЗУ (OTP)
#define PM_CLK_APB_M_OTP_CONTROLLER_M   (1 << PM_CLK_APB_M_OTP_CONTROLLER_S)

#define PM_CLK_APB_M_PVD_CONTROL_S      6
/// Включить тактирование монитора питания системного домена (PVD)
#define PM_CLK_APB_M_PVD_CONTROL_M      (1 << PM_CLK_APB_M_PVD_CONTROL_S)

#define PM_CLK_APB_M_WU_S               7
/// Включить тактирование блока пробуждения (WakeUP) батарейного домена
#define PM_CLK_APB_M_WU_M               (1 << PM_CLK_APB_M_WU_S)

#define PM_CLK_APB_M_RTC_S              8
/// Включить тактирование часов реального времени (RTC) батарейного домена
#define PM_CLK_APB_M_RTC_M              (1 << PM_CLK_APB_M_RTC_S)

// Перечень значений для включения/выключения тактирования устройств на шине APB_P (CLK_APB_P_SET / CLK_APB_P_CLEAR)

#define PM_CLK_APB_P_WDT_S          0
/// Включить тактирование сторожевого таймера (WDT)
#define PM_CLK_APB_P_WDT_M          (1 << PM_CLK_APB_P_WDT_S)

#define PM_CLK_APB_P_UART_0_S       1
/// Включить тактирование контроллера USART_0
#define PM_CLK_APB_P_UART_0_M       (1 << PM_CLK_APB_P_UART_0_S)

#define PM_CLK_APB_P_UART_1_S       2
/// Включить тактирование контроллера USART_1
#define PM_CLK_APB_P_UART_1_M       (1 << PM_CLK_APB_P_UART_1_S)

#define PM_CLK_APB_P_TIMER16_0_S    3
/// Включить тактирование Timer16_0
#define PM_CLK_APB_P_TIMER16_0_M    (1 << PM_CLK_APB_P_TIMER16_0_S)

#define PM_CLK_APB_P_TIMER16_1_S    4
/// Включить тактирование Timer16_1
#define PM_CLK_APB_P_TIMER16_1_M    (1 << PM_CLK_APB_P_TIMER16_1_S)

#define PM_CLK_APB_P_TIMER16_2_S    5
/// Включить тактирование Timer16_2
#define PM_CLK_APB_P_TIMER16_2_M    (1 << PM_CLK_APB_P_TIMER16_2_S)

#define PM_CLK_APB_P_TIMER32_1_S    6
/// Включить тактирование Timer32_1
#define PM_CLK_APB_P_TIMER32_1_M    (1 << PM_CLK_APB_P_TIMER32_1_S)

#define PM_CLK_APB_P_TIMER32_2_S    7
/// Включить тактирование Timer32_1
#define PM_CLK_APB_P_TIMER32_2_M    (1 << PM_CLK_APB_P_TIMER32_2_S)

#define PM_CLK_APB_P_SPI_0_S        8
/// Включить тактирование контроллера SPI_0
#define PM_CLK_APB_P_SPI_0_M        (1 << PM_CLK_APB_P_SPI_0_S)

#define PM_CLK_APB_P_SPI_1_S        9
/// Включить тактирование контроллера SPI_1
#define PM_CLK_APB_P_SPI_1_M        (1 << PM_CLK_APB_P_SPI_1_S)

#define PM_CLK_APB_P_I2C_0_S        10
/// Включить тактирование контроллера I2C_0
#define PM_CLK_APB_P_I2C_0_M        (1 << PM_CLK_APB_P_I2C_0_S)

#define PM_CLK_APB_P_I2C_1_S        11
/// Включить тактирование контроллера I2C_1
#define PM_CLK_APB_P_I2C_1_M        (1 << PM_CLK_APB_P_I2C_1_S)

#define PM_CLK_APB_P_GPIO_0_S       12
/// Включить тактирование контроллера GPIO_0
#define PM_CLK_APB_P_GPIO_0_M       (1 << PM_CLK_APB_P_GPIO_0_S)

#define PM_CLK_APB_P_GPIO_1_S       13
/// Включить тактирование контроллера GPIO_1
#define PM_CLK_APB_P_GPIO_1_M       (1 << PM_CLK_APB_P_GPIO_1_S)

#define PM_CLK_APB_P_GPIO_2_S       14
/// Включить тактирование контроллера GPIO_2
#define PM_CLK_APB_P_GPIO_2_M       (1 << PM_CLK_APB_P_GPIO_2_S)

#define PM_CLK_APB_P_ANALOG_REGS_S  15
/// Включить тактирование аналоговых блоков АЦП, ЦАП, мониторов I0C и питания 
#define PM_CLK_APB_P_ANALOG_REGS_M  (1 << PM_CLK_APB_P_ANALOG_REGS_S)

#define PM_CLK_APB_P_GPIO_IRQ_S     16
/// Включить тактирование схемы формирования прерываний GPIO
#define PM_CLK_APB_P_GPIO_IRQ_M     (1 << PM_CLK_APB_P_GPIO_IRQ_S)

#ifndef __ASSEMBLER__
    #include <inttypes.h> 
    /// Структура регистров блока управления питанием и тактированием 
    typedef struct
    {   
        volatile uint32_t DIV_AHB;              ///< Регистр настройки делителя шины AHB. Частота шины AHB (FAHB) рассчитывается как FSYS / (DIV_AHB+1) 
        volatile uint32_t DIV_APB_M;            ///< Регистр настройки делителя шины APB_M. Частота шины APB_M (FAPB_M) рассчитывается как FAPB / (DIV_APB_M+1)
        volatile uint32_t DIV_APB_P;            ///< Регистр настройки делителя шины APB_P. Частота шины APB_P (FAPB_P) рассчитывается как FAPB / (DIV_APB_P+1)
        volatile uint32_t CLK_AHB_SET;          ///< Регистр включения тактированием устройств на шине AHB. Запись "1" включает тактирование соответствующего устройства.
        volatile uint32_t CLK_AHB_CLEAR;        ///< Регистр выключения тактированием устройств на шине AHB. Запись "1" выключает тактирование соответствующего устройства.
        volatile uint32_t CLK_APB_M_SET;        ///< Регистр включения тактированием устройств на шине APB_M. Запись "1" включает тактирование соответствующего устройства.
        volatile uint32_t CLK_APB_M_CLEAR;      ///< Регистр выключения тактированием устройств на шине APB_M. Запись "1" выключает тактирование соответствующего устройства.
        volatile uint32_t CLK_APB_P_SET;        ///< Регистр включения тактированием устройств на шине APB_P. Запись "1" включает тактирование соответствующего устройства.
        volatile uint32_t CLK_APB_P_CLEAR;      ///< Регистр выключения тактированием устройств на шине APB_P. Запись "1" выключает тактирование соответствующего устройства.
        volatile uint32_t AHB_CLK_MUX;          ///< Регистр настройки источника тактирования системы. 
        volatile uint32_t WDT_CLK_MUX;          ///< Регистр выбора источника тактирования сторожевого таймера. 
        volatile uint32_t CPU_RTC_CLK_MUX;      ///< Регистр выбора источника тактирования RTC в составе ядра.
        // FIXME: переименовать в TIMER_CFG_MUX
        volatile uint32_t TIMER_CFG;            ///< Регистр выбора источника тактирования таймеров.
        volatile uint32_t FREQ_MASK;            ///< Регистр настройки прерываний монитора частоты. 
        volatile uint32_t FREQ_STATUS;          ///< Регистр статуса монитора частоты.
        volatile uint32_t SLEEP_MODE;           ///< Регистр включения спящего режима. Запись "1" переводит МК в спящий режим.
    } PM_TypeDef;
#endif

#endif 

