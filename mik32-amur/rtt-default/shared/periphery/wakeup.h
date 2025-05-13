/**
 * @file
 * Блок пробуждения (WakeUp)
 * 
 * Блок WakeUp обеспечивает управление питанием системных доменов и источниками тактовых сигналов.
 */

#ifndef WU_H_INCLUDED
#define WU_H_INCLUDED

#define WU_SYS_MASK_OFFSET              0x00
#define WU_SYS_LEVEL_OFFSET             0x04
#define WU_SYS_POWEROFF_OFFSET          0x08
#define WU_POWER_SWITCH_OFFSET          0x0C
#define WU_CLOCKS_BU_OFFSET             0x10
#define WU_CLOCKS_SYS_OFFSET            0x14
#define WU_RTC_CONTROL_OFFSET           0x18
#define WU_STOP_OFFSET                  0x1C

// Перечень значений регистра управления событиями для включения и сброса системного домена (SYS_MASK)

#define WU_MASK_WAKEUP_RTC_S                0
/// Разрешить включение системного домена (из режима СТОП) при срабатывании будильника
#define WU_MASK_WAKEUP_RTC_M                (1 << WU_MASK_WAKEUP_RTC_S)

#define WU_MASK_WAKEUP_EXT_S                1
/// Разрешить включение системного домена (из режима СТОП) при активном уровне внешнего вывода EXT_WU
#define WU_MASK_WAKEUP_EXT_M                (1 << WU_MASK_WAKEUP_EXT_S)

#define WU_MASK_SYS_RESET_LDO_S             2
/// Запретить формирование сброса системного домена при снижении питания (индикация от LDO_SYS)
#define WU_MASK_SYS_RESET_LDO_M             (1 << WU_MASK_SYS_RESET_LDO_S)

#define WU_MASK_SYS_RESET_VOLTMON_S         3
/// Запретить формирование сброса системного домена при снижении питания (индикация от схемы слежения)
#define WU_MASK_SYS_RESET_VOLTMON_M         (1 << WU_MASK_SYS_RESET_VOLTMON_S)

#define WU_MASK_SYS_RESET_BOR_S             4
/// Запретить формирование сброса системного домена при появлении сигнала BOR
#define WU_MASK_SYS_RESET_BOR_M             (1 << WU_MASK_SYS_RESET_BOR_S)

#define WU_MASK_BU_RESET_BOR_S              5
/// Запретить формирование сброса батарейного домена при появлении сигнала BOR
#define WU_MASK_BU_RESET_BOR_M              (1 << WU_MASK_BU_RESET_BOR_S)

// Перечень значений регистра активных уровней событий для включения и сброса системного домена (SYS_LEVEL)

#define WU_SYS_LEVEL_RTC_ALARM_S              0
/// Установить активный уровень срабатывания будильника
#define WU_SYS_LEVEL_RTC_ALARM_M              (0x1 << WU_SYS_LEVEL_RTC_ALARM_S)

#define WU_SYS_LEVEL_EXT_LINE_S               1
/// Установить активный уровень внешнего вывода EXT_WU
#define WU_SYS_LEVEL_EXT_LINE_M               (0x1 << WU_SYS_LEVEL_EXT_LINE_S)

// Перечень значений регистра правления питанием системного домена (SYS_POWEROFF)

#define WU_SYS_POWEROFF_S                  0
/// Отключить питание системного домена
#define WU_SYS_POWEROFF_M                  (1 << WU_SYS_POWEROFF_S)

// Перечень значений регистра управления и статуса схемы слежения за питанием (POWER_SWITCH)

#define WU_POWER_SWITCH_EN_S                0
/// Разрешить принудительное переключение источников питания
#define WU_POWER_SWITCH_EN_M                (1 << WU_POWER_SWITCH_EN_S)

#define WU_POWER_SWITCH_CONTROL_S           1
#define WU_POWER_SWITCH_CONTROL_M           (1 << WU_POWER_SWITCH_CONTROL_S)
/// Установить принудительное использование VCC, как источника питания
#define WU_POWER_SWITCH_CONTROL_VCC_M       (0 << WU_POWER_SWITCH_CONTROL_S)
/// Установить принудительное использование VBAT, как источника питания
#define WU_POWER_SWITCH_CONTROL_VBAT_M      (1 << WU_POWER_SWITCH_CONTROL_S)

#define WU_POWER_SWITCH_BATT_GOOD_S         2
/// Маска для получения из регистра значения состояния резервного (батарейного) источника питания 
#define WU_POWER_SWITCH_BATT_GOOD_M         (1 << WU_POWER_SWITCH_BATT_GOOD_S)

// Перечень значений регистра управления тактированием батарейного домена (CLOCKS_BU)

#define WU_CLOCKS_BU_OSC32K_EN_S           0
/// Включить внешний осциллятор на 32 кГц (OSC32K).
#define WU_CLOCKS_BU_OSC32K_EN_M           (1 << WU_CLOCKS_BU_OSC32K_EN_S)

#define WU_CLOCKS_BU_LSI32K_EN_S           1
/// Включить внутренний осциллятор на 32 кГц (LSI32K)
#define WU_CLOCKS_BU_LSI32K_EN_M           (1 << WU_CLOCKS_BU_LSI32K_EN_S)

#define WU_CLOCKS_BU_ADJ_LSI32K_S          6
#define WU_CLOCKS_BU_ADJ_LSI32K_M          (0xF << WU_CLOCKS_BU_ADJ_LSI32K_S)
/// Установить поправочный коэффициент для внутреннего осциллятора LSI32K
#define WU_CLOCKS_BU_ADJ_LSI32K(v)         (((v) << WU_CLOCKS_BU_ADJ_LSI32K_S) & WU_CLOCKS_BU_ADJ_LSI32K_M)

#define WU_CLOCKS_BU_RTC_CLK_MUX_S         10
/// Разрешить автоматический выбор приоритетного источника тактирования 
#define WU_CLOCKS_BU_RTC_CLK_MUX_M         (0x3 << WU_CLOCKS_BU_RTC_CLK_MUX_S)
/// Установить внутренний осциллятор LSI32K, как приоритетный источник тактирования
#define WU_CLOCKS_BU_RTC_CLK_MUX_LSI32K_M  (0x1 << WU_CLOCKS_BU_RTC_CLK_MUX_S)
/// Установить внешний осциллятор OSC32K, как приоритетный источник тактирования
#define WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M  (0x2 << WU_CLOCKS_BU_RTC_CLK_MUX_S)

#define WU_CLOCKS_BU_OSC32K_SM_S           14
/// Включить режим повышенного потребления для внешнего осциллятора OSC32K
#define WU_CLOCKS_BU_OSC32K_SM_M           (1 << WU_CLOCKS_BU_OSC32K_SM_S)  

// Перечень значений регистра управления тактированием системного домена (CLOCKS_SYS)

#define WU_CLOCKS_SYS_OSC32M_EN_S                   0
/// Включить внешний осциллятор на 32 МГц (OSM32M)
#define WU_CLOCKS_SYS_OSC32M_EN_M                   (1 << WU_CLOCKS_SYS_OSC32M_EN_S)

#define WU_CLOCKS_SYS_HSI32M_EN_S                   1
/// Включить внутренний осциллятор 32 мГц (HSI32M)
#define WU_CLOCKS_SYS_HSI32M_EN_M                   (1 << WU_CLOCKS_SYS_HSI32M_EN_S)

#define WU_CLOCKS_SYS_ADJ_HSI32M_S                  2
#define WU_CLOCKS_SYS_ADJ_HSI32M_M                  (0xFF << WU_CLOCKS_SYS_ADJ_HSI32M_S)
/// Установить поправочные коэффициенты для HSI32M
#define WU_CLOCKS_SYS_ADJ_HSI32M(v)                 (((v) << WU_CLOCKS_SYS_ADJ_HSI32M_S) & WU_CLOCKS_SYS_ADJ_HSI32M_M)

#define WU_CLOCKS_SYS_FORCE_32K_CLK_S               10
/// Разрешить автоматический выбор опорного источника монитора частоты (0b11 и 0b00) 
#define WU_CLOCKS_SYS_FORCE_32K_CLK_M               (0b11 << WU_CLOCKS_SYS_FORCE_32K_CLK_S)
/// Принудительно переключить опорный источник монитора частоты на LSI32K 
#define WU_CLOCKS_SYS_FORCE_32K_CLK_LSI32K_M        (0b01 << WU_CLOCKS_SYS_FORCE_32K_CLK_S)
/// Принудительно переключить опорный источник монитора частоты на OSC32K 
#define WU_CLOCKS_SYS_FORCE_32K_CLK_OSC32K_M        (0b10 << WU_CLOCKS_SYS_FORCE_32K_CLK_S)

// Перечень значений регистра сброса RTC (RTC_CONRTOL)

#define WU_RTC_CONTROL_RESET_S                  0
#define WU_RTC_CONTROL_RESET_SET_M              (0 << WU_RTC_CONTROL_RESET_S)
/// Сбросить RTC
#define WU_RTC_CONTROL_RESET_CLEAR_M            (1 << WU_RTC_CONTROL_RESET_S)

// Перечень значений регистра управления режимом СТОП (STOP)

#define WU_STOP_S                  0
/// Переводит системный домен питания в режим СТОП
#define WU_STOP_M                  (1 << WU_STOP_S)

    
#ifndef __ASSEMBLER__
    #include <inttypes.h> 
    /// @brief Структура регистров блока пробуждения (WakeUp)
    typedef struct
    {   
       
        volatile uint32_t SYS_MASK;             ///< [RW]: Регистр управления событиями для включения и сброса системного домена. Биты [5:0] задают маску обработки событий для включения и сброса системного домена.
        volatile uint32_t SYS_LEVEL;            ///< [RW]: Регистр активных уровней событий для включения и сброса системного домена. Биты [1:0] кодирует уровень срабатывания от будильника и внешнего вывода ext_wu.
        volatile uint32_t SYS_POWEROFF;         ///< [RW]: Регистр управления питанием системного домена. Запись "1" в регистр отключает питание системного домена.
        volatile uint32_t POWER_SWITCH;         ///< [RW]: Регистр управления и статуса схемы слежения за питанием. Биты [2:0] кодируют режимы управления.
        volatile uint32_t CLOCKS_BU;            ///< [RW]: Регистр управления тактированием батарейного домена. Биты [14:0] кодируют режимы управления и источники тактирования.
        volatile uint32_t CLOCKS_SYS;           ///< [RW]: Регистр управления тактированием системного домена. Биты [11:0] кодируют режимы управления тактированием.
        volatile uint32_t RTC_CONTROL;          ///< [RW]: Регистр сброса RTC. Запись "1" в регистр сбрасывает RTC.
        volatile uint32_t STOP;                 ///< [RW]: Регистр управления режимом СТОП. Запись "1" в регистр переводит МК в режим СТОП.
    } WU_TypeDef;
#endif

#endif 

