/**
 * @file
 * Модуль порта ввода/вывода общего назначения (GPIO)
 * 
 * Каждый модуль ввода/вывода (GPIO) имеет интерфейс APB и управляет двумя портами по 16 и одним по 8 внешними выводами. 
 * Каждый из внешних выводов управляется индивидуально.
 */

#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

//TODO: Добавить суффикс _OFFSET

#define GPIO_STATE				0x00 
#define GPIO_SET				0x00 
#define GPIO_CLEAR				0x04 
#define GPIO_DIRECTION_OUT		0x08 
#define GPIO_DIRECTION_IN		0x0C 
#define GPIO_OUTPUT             0x10
#define GPIO_CONTROL			0x14 

#define GPIO_TEST_MODE_S     0
/// Перевести модуль GPIO в тестовый режим
#define GPIO_TEST_MODE_M    (1 << PM_SLEEP_MODE_S)

// Перечень значений для регистров модуля GPIO

#define GPIO_PIN_0_S        0
/// Вывод 0
#define GPIO_PIN_0_M        (1 << GPIO_PIN_0_S)

#define GPIO_PIN_1_S        1
/// Вывод 1
#define GPIO_PIN_1_M        (1 << GPIO_PIN_1_S)

#define GPIO_PIN_2_S        2
/// Вывод 2
#define GPIO_PIN_2_M        (1 << GPIO_PIN_2_S)

#define GPIO_PIN_3_S        3
/// Вывод 3
#define GPIO_PIN_3_M        (1 << GPIO_PIN_3_S)

#define GPIO_PIN_4_S        4
/// Вывод 4
#define GPIO_PIN_4_M        (1 << GPIO_PIN_4_S)

#define GPIO_PIN_5_S        5
/// Вывод 5
#define GPIO_PIN_5_M        (1 << GPIO_PIN_5_S)

#define GPIO_PIN_6_S        6
/// Вывод 6
#define GPIO_PIN_6_M        (1 << GPIO_PIN_6_S)

#define GPIO_PIN_7_S        7
/// Вывод 7
#define GPIO_PIN_7_M        (1 << GPIO_PIN_7_S)

#define GPIO_PIN_8_S        8
/// Вывод 8 (только для GPIO0 и GPIO1)
#define GPIO_PIN_8_M        (1 << GPIO_PIN_8_S)

#define GPIO_PIN_9_S        9
/// Вывод 9 (только для GPIO0 и GPIO1)
#define GPIO_PIN_9_M        (1 << GPIO_PIN_9_S)

#define GPIO_PIN_10_S       10
/// Вывод 10 (только для GPIO0 и GPIO1)
#define GPIO_PIN_10_M       (1 << GPIO_PIN_10_S)

#define GPIO_PIN_11_S       11
/// Вывод 11 (только для GPIO0 и GPIO1)
#define GPIO_PIN_11_M       (1 << GPIO_PIN_11_S)

#define GPIO_PIN_12_S       12
/// Вывод 12 (только для GPIO0 и GPIO1)
#define GPIO_PIN_12_M       (1 << GPIO_PIN_12_S)

#define GPIO_PIN_13_S       13
/// Вывод 13 (только для GPIO0 и GPIO1)
#define GPIO_PIN_13_M       (1 << GPIO_PIN_13_S)

#define GPIO_PIN_14_S       14
/// Вывод 14 (только для GPIO0 и GPIO1)
#define GPIO_PIN_14_M       (1 << GPIO_PIN_14_S)

#define GPIO_PIN_15_S       15
/// Вывод 15 (только для GPIO0 и GPIO1)
#define GPIO_PIN_15_M       (1 << GPIO_PIN_15_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h>
    /// @brief Структура регистров модуля GPIO
    typedef struct
    {
        union
        {
            volatile uint32_t SET;              ///< [RW]: Регистр установки текущего состояния выводов. Биты [15:0] (для GPIO2 [7:0]) соответствуют номерам выводов (0 - низкий уровень, 1 - высокий уровень). Запись "1" переводит вывод в состояние логической "1"
            volatile uint32_t STATE;            ///< [RW]: Регистр текущего состояния выводов (синоним SET). Биты [15:0] (для GPIO2 [7:0]) соответствуют номерам выводов (0 - низкий уровень, 1 - высокий уровень).
        };                
        volatile uint32_t CLEAR;                ///< [RW1C]: Регистр сброса состояния вывода в логический "0". Биты [15:0] (для GPIO2 [7:0]) соответствуют номерам выводов. Запись "1" переводит вывод в состояние логического "0".
        volatile uint32_t DIRECTION_OUT;        ///< [RW]: Регистр установки направления выводов как "выход". Биты [15:0] (для GPIO2 [7:0]) соответствуют номерам выводов (0 - выход, 1 - вход). Запись "1" переводит вывод в состояние "выход".
        volatile uint32_t DIRECTION_IN;         ///< [RW]: Регистр установки направления выводов как "вход". Биты [15:0] (для GPIO2 [7:0]) соответствуют номерам выводов (0 - выход, 1 - вход). Запись "1" переводит вывод в состояние "вход".
        volatile uint32_t OUTPUT;               ///< [RW]: Регистр текущего состояния выводов. Биты [15:0] (для GPIO2 [7:0]) соответствуют номерам выводов (0 - низкий уровень, 1 - высокий уровень). Запись "1" переводит вывод в состояние логической "1", "0" - в состояние логического "0".
        volatile uint32_t CONTROL;              ///< [RW]: Регистр включения тестового режима. Бит [0] хранит текущее состояние режима (0 - нормальный, 1 - тестовый). Запись "1" включает тестовый режим, "0" - включает нормальный режим.
        
    } GPIO_TypeDef;

#endif

#endif    