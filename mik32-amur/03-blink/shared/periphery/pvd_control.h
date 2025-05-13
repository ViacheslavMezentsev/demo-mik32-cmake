/**
 * @file
 * Монитор питания
 * 
 * Монитор питания формирует флаги при повышении или понижении питания 
 * относительно заданного порогового значения. Пороговые значения могут 
 * быть запрограммированы в диапазоне ±(4÷12)% от номинального значения. 
 * Шаг установки пороговых значений монитора напряжения питания составляет 
 * 1/16 диапазона контроля. Точность установки пороговых значений – 1/2 шага. 
 * Схема монитора питания работает в широком диапазоне напряжений питания – 
 * от 1,584 до 3,696 В в диапазоне температур от -40 до +125 С.
 */

#ifndef PVD_CONTROL_H_INCLUDED
#define PVD_CONTROL_H_INCLUDED

#define PDP_DPF_VALUE_OFFSET        0x00
#define PDP_CONFIG_OFFSET           0x04
#define PDP_STATUS_OFFSET           0x08

// Перечень значений регистра настройки цифрового фильтра (DPF_VALUE)

#define PVD_DPF_VALUE_S            0
/// Маска для значений коэффициентов, определяющих длительность отфильтровываемых импульсов (на системной частоте)
#define PVD_DPF_VALUE_M            (0xFFFF << PVD_DPF_VALUE_S)

// Перечень значений регистра настроек (CONFIG)

#define PVD_CONFIG_PD_S            0
// Выключить монитор (иначе включить)
#define PVD_CONFIG_PD_M            (1 << PVD_CONFIG_PD_S)

#define PVD_CONFIG_NRESETU_S       1
/// Отключить (сбросить) детектирование нижнего порога
#define PVD_CONFIG_NRESETU_M       (1 << PVD_CONFIG_NRESETU_S)

#define PVD_CONFIG_NRESETO_S       2
/// Отключить (сбросить) детектирование верхнего порога
#define PVD_CONFIG_NRESETO_M       (1 << PVD_CONFIG_NRESETO_S)

#define PVD_CONFIG_TESTMODE_S      3
/// Переключить в тестовый режим
#define PVD_CONFIG_TESTMODE_M      (1 << PVD_CONFIG_TESTMODE_S)

#define PVD_CONFIG_UNDER_THRESH_S  4
/// Маска для значений нижнего порога срабатывания монитора
#define PVD_CONFIG_UNDER_THRESH_M  (0b1111 << PVD_CONFIG_UNDER_THRESH_S)

#define PVD_CONFIG_OVER_THRESH_S   8
/// Маска для значений верхнего порога срабатывания монитора
#define PVD_CONFIG_OVER_THRESH_M   (0b1111 << PVD_CONFIG_OVER_THRESH_S)

#define PVD_CONFIG_EN_VREFCLB_S    12
/// Использовать в качестве опорного источника напряжения калибруемый ОИН
#define PVD_CONFIG_EN_VREFCLB_M    (1 << PVD_CONFIG_EN_VREFCLB_S)

//  Перечень значений регистра статуса (STATUS)

#define PVD_STATUS_UNDER_VALUE_S   0
/// Получить текущее состояние линии монитора нижнего порога без цифровой фильтрации
#define PVD_STATUS_UNDER_VALUE_M   (1 << PVD_STATUS_UNDER_VALUE_S)

#define PVD_STATUS_OVER_VALUE_S    1
/// Получить текущее состояние линии монитора верхнего порога без цифровой фильтрации
#define PVD_STATUS_OVER_VALUE_M    (1 << PVD_STATUS_OVER_VALUE_S)

#define PVD_STATUS_UNDER_FLAG_S    2
/// Получить текущее состояние линии монитора нижнего порога с учётом цифровой фильтрации
#define PVD_STATUS_UNDER_FLAG_M    (1 << PVD_STATUS_UNDER_FLAG_S)

#define PVD_STATUS_OVER_FLAG_S     3
/// Получить текущее состояние линии монитора верхнего порога с учётом цифровой фильтрации
#define PVD_STATUS_OVER_FLAG_S     (1 << PVD_STATUS_OVER_FLAG_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h>
    /// @brief Структура регистров управления монитором напряжения VCC (PVD)
    typedef struct
    {
        volatile uint32_t       DPF_VALUE;      ///< [RW]: Регистр настройки цифрового фильтра. Биты [15:0] хранят значение коэффициента, определяющих длительность отфильтровываемых импульсов.
        volatile uint32_t       CONFIG;         ///< [RW]: Регистр настроек. Биты [12:0] кодируют параметры настроек монитора.
        volatile uint32_t       STATUS;         ///< [RO]: Регистр статуса. Биты [3:0] кодируют статус монитора напряжения.
    } PVD_CONTROL_TypeDef;
    
#endif

#endif // PVD_CONTROL_H_INCLUDED

