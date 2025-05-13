/**
 * @file
 * 64-битный таймер ядра SCR1
 */

#ifndef SCR1_TIMER_H_INCLUDED
#define SCR1_TIMER_H_INCLUDED

#define SCR1_TIMER_CTRL_ENABLE_S             0
/// Включение или выключение таймера (0 - выключен, 1 - включен)
#define SCR1_TIMER_CTRL_ENABLE_M             (1 << SCR1_TIMER_CTRL_ENABLE_S)

#define SCR1_TIMER_CTRL_CLKSRC_S             1
#define SCR1_TIMER_CTRL_CLKSRC_M             (1 << SCR1_TIMER_CTRL_CLKSRC_S)
/// Использовать встроенный в ядро источник тактирования
#define SCR1_TIMER_CTRL_CLKSRC_INTERNAL_M    (0 << SCR1_TIMER_CTRL_CLKSRC_S)
/// Использовать внешний источник тактирования RTC
#define SCR1_TIMER_CTRL_CLKSRC_RTC_M         (1 << SCR1_TIMER_CTRL_CLKSRC_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h>

    typedef struct
    {
        volatile uint32_t TIMER_CTRL;       ///< Регистр настройки таймера
        volatile uint32_t TIMER_DIV;        ///< Регистр настройки делителя частоты
        volatile uint32_t MTIME;            ///< Регистр текущего значения младшего слова
        volatile uint32_t MTIMEH;           ///< Регистр текущего значения старшего слова
        volatile uint32_t MTIMECMP;         ///< Регистр сравнения младшего слова
        volatile uint32_t MTIMECMPH;        ///< Регистр сравнения старшего слова
    } SCR1_TIMER_TypeDef;
#endif

#endif // SCR1_TIMER_H_INCLUDED
