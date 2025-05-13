/**
 * @file
 * Модуль управления прерываниями GPIO
 * 
 * Обработка прерываний выводов реализована в отдельном модуле. 
 * Входные линии трех блоков ввода-вывода подключаются к 8 мультиплексорам формируя 8 линий, для которых формируются прерывания. 
 * Каждый мультиплексор управляется независимо, на основе соответствующих бит поля LINE_MUX. 
 *
 *  Схема коммутации линий прерываний представлена в таблице.
+============+==========+==========+===========+============+============+============+============+============+
| Перывание  | Линия_0  | Линия_1  | Линия_2   | Линия_3    | Линия_4    | Линия_5    | Линия_6    | Линия_7    |
|            | Mux[3:0] | Mux[7:4] | Mux[11:8] | Mux[15:12] | Mux[19:16] | Mux[23:20] | Mux[27:24] | Mux[31:28] |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=0  | GPIO0_0  | GPIO0_1  | GPIO0_2   | GPIO0_3    | GPIO0_4    | GPIO0_5    | GPIO0_6    | GPIO0_7    |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=1  | GPIO0_8  | GPIO0_9  | GPIO0_10  | GPIO0_11   | GPIO0_12   | GPIO0_13   | GPIO0_14   | GPIO0_15   |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=2  | GPIO1_0  | GPIO1_1  | GPIO1_2   | GPIO1_3    | GPIO1_4    | GPIO1_5    | GPIO1_6    | GPIO1_7    |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=3  | GPIO1_8  | GPIO1_9  | GPIO1_10  | GPIO1_11   | GPIO1_12   | GPIO1_13   | GPIO1_14   | GPIO1_15   |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=4  | GPIO2_0  | GPIO2_1  | GPIO2_2   | GPIO2_3    | GPIO2_4    | GPIO2_5    | GPIO2_6    | GPIO2_7    |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=5  | GPIO0_4  | GPIO0_5  | GPIO0_6   | GPIO0_7    | GPIO0_0    | GPIO0_1    | GPIO0_2    | GPIO0_3    |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=6  | GPIO0_12 | GPIO0_13 | GPIO0_14  | GPIO0_15   | GPIO0_8    | GPIO0_9    | GPIO0_10   | GPIO0_11   |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=7  | GPIO1_4  | GPIO1_5  | GPIO1_6   | GPIO1_7    | GPIO1_0    | GPIO1_1    | GPIO1_2    | GPIO1_3    |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=8  | GPIO1_12 | GPIO1_13 | GPIO1_14  | GPIO1_15   | GPIO1_8    | GPIO1_9    | GPIO1_10   | GPIO1_11   |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=9  | GPIO2_4  | GPIO2_5  | GPIO2_6   | GPIO2_7    | GPIO2_0    | GPIO2_1    | GPIO2_2    | GPIO2_3    |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
| MUX [i]=10 | 0        | 0        | 0         | 0          | 0          | 0          | 0          | 0          |
+------------+----------+----------+-----------+------------+------------+------------+------------+------------+
*/

#ifndef GPIO_IRQ_H_INCLUDED
#define GPIO_IRQ_H_INCLUDED

#define GPIO_IRQ_STATE_OFFSET               0x00
#define GPIO_IRQ_LINE_MUX_OFFSET            0x04
#define GPIO_IRQ_INTERRUPT_OFFSET           0x08
#define GPIO_IRQ_ENABLE_SET_OFFSET          0x0C
#define GPIO_IRQ_ENABLE_CLEAR_OFFSET        0x10
#define GPIO_IRQ_EDGE_OFFSET                0x14
#define GPIO_IRQ_LEVEL_OFFSET               0x18
#define GPIO_IRQ_LEVEL_SET_OFFSET           0x1C
#define GPIO_IRQ_LEVEL_CLEAR_OFFSET         0x20
#define GPIO_IRQ_ANY_EDGE_SET_OFFSET        0x24
#define GPIO_IRQ_ANY_EDGE_CLEAR_OFFSET      0x28
#define GPIO_IRQ_CLEAR_OFFSET               0x2C


// Формирует маску для значения по номеру линии (line_num)
#define GPIO_IRQ_LINE_M(line_num) (1 << (line_num))

// Формирует маску для значения регистра LINE_MUX по номеру линии (line_num)
#define GPIO_IRQ_LINE_MUX_M(line_num) (0xF << ((line_num) << 2))

/**
 * Назначает линии line_num вывод, на основе указанного в MUX номера строки
 * Например линии 0 назначается GPIO2_0, тогда mux == 4, line_num == 0
 */
#define GPIO_IRQ_LINE_MUX(mux, line_num) (((mux) << ((line_num) << 2)) & GPIO_IRQ_LINE_MUX_M(line_num))

// Перечень выводов для назначения линии_0

#define GPIO_IRQ_LINE_0_GPIO0_0     (GPIO_IRQ_LINE_MUX(0, 0))       ///< Направить прерывания GPIO0_0 на линию_0
#define GPIO_IRQ_LINE_0_GPIO0_8     (GPIO_IRQ_LINE_MUX(1, 0))       ///< Направить прерывания GPIO0_8 на линию_0
#define GPIO_IRQ_LINE_0_GPIO1_0     (GPIO_IRQ_LINE_MUX(2, 0))       ///< Направить прерывания GPIO1_0 на линию_0
#define GPIO_IRQ_LINE_0_GPIO1_8     (GPIO_IRQ_LINE_MUX(3, 0))       ///< Направить прерывания GPIO1_8 на линию_0
#define GPIO_IRQ_LINE_0_GPIO2_0     (GPIO_IRQ_LINE_MUX(4, 0))       ///< Направить прерывания GPIO2_0 на линию_0
#define GPIO_IRQ_LINE_0_GPIO0_4     (GPIO_IRQ_LINE_MUX(5, 0))       ///< Направить прерывания GPIO0_4 на линию_0
#define GPIO_IRQ_LINE_0_GPIO0_12    (GPIO_IRQ_LINE_MUX(6, 0))       ///< Направить прерывания GPIO0_12 на линию_0
#define GPIO_IRQ_LINE_0_GPIO1_4     (GPIO_IRQ_LINE_MUX(7, 0))       ///< Направить прерывания GPIO1_4 на линию_0
#define GPIO_IRQ_LINE_0_GPIO1_12    (GPIO_IRQ_LINE_MUX(8, 0))       ///< Направить прерывания GPIO1_12 на линию_0
#define GPIO_IRQ_LINE_0_GPIO2_4     (GPIO_IRQ_LINE_MUX(9, 0))       ///< Направить прерывания GPIO2_4 на линию_0

// Перечень выводов для назначения линии_1

#define GPIO_IRQ_LINE_1_GPIO0_1     (GPIO_IRQ_LINE_MUX(0, 1))       ///< Направить прерывания GPIO0_1 на линию_1
#define GPIO_IRQ_LINE_1_GPIO0_9     (GPIO_IRQ_LINE_MUX(1, 1))       ///< Направить прерывания GPIO0_9 на линию_1
#define GPIO_IRQ_LINE_1_GPIO1_1     (GPIO_IRQ_LINE_MUX(2, 1))       ///< Направить прерывания GPIO1_1 на линию_1
#define GPIO_IRQ_LINE_1_GPIO1_9     (GPIO_IRQ_LINE_MUX(3, 1))       ///< Направить прерывания GPIO1_9 на линию_1
#define GPIO_IRQ_LINE_1_GPIO2_1     (GPIO_IRQ_LINE_MUX(4, 1))       ///< Направить прерывания GPIO2_1 на линию_1
#define GPIO_IRQ_LINE_1_GPIO0_5     (GPIO_IRQ_LINE_MUX(5, 1))       ///< Направить прерывания GPIO0_5 на линию_1
#define GPIO_IRQ_LINE_1_GPIO0_13    (GPIO_IRQ_LINE_MUX(6, 1))       ///< Направить прерывания GPIO0_13 на линию_1
#define GPIO_IRQ_LINE_1_GPIO1_5     (GPIO_IRQ_LINE_MUX(7, 1))       ///< Направить прерывания GPIO1_5 на линию_1
#define GPIO_IRQ_LINE_1_GPIO1_13    (GPIO_IRQ_LINE_MUX(8, 1))       ///< Направить прерывания GPIO1_13 на линию_1
#define GPIO_IRQ_LINE_1_GPIO2_5     (GPIO_IRQ_LINE_MUX(9, 1))       ///< Направить прерывания GPIO2_5 на линию_1

// Перечень выводов для назначения линии_2

#define GPIO_IRQ_LINE_2_GPIO0_2     (GPIO_IRQ_LINE_MUX(0, 2))       ///< Направить прерывания GPIO0_2 на линию_2
#define GPIO_IRQ_LINE_2_GPIO0_10    (GPIO_IRQ_LINE_MUX(1, 2))       ///< Направить прерывания GPIO0_10 на линию_2
#define GPIO_IRQ_LINE_2_GPIO1_2     (GPIO_IRQ_LINE_MUX(2, 2))       ///< Направить прерывания GPIO1_2 на линию_2
#define GPIO_IRQ_LINE_2_GPIO1_10    (GPIO_IRQ_LINE_MUX(3, 2))       ///< Направить прерывания GPIO1_10 на линию_2
#define GPIO_IRQ_LINE_2_GPIO2_2     (GPIO_IRQ_LINE_MUX(4, 2))       ///< Направить прерывания GPIO2_2 на линию_2
#define GPIO_IRQ_LINE_2_GPIO0_6     (GPIO_IRQ_LINE_MUX(5, 2))       ///< Направить прерывания GPIO0_6 на линию_2
#define GPIO_IRQ_LINE_2_GPIO0_14    (GPIO_IRQ_LINE_MUX(6, 2))       ///< Направить прерывания GPIO0_14 на линию_2
#define GPIO_IRQ_LINE_2_GPIO1_6     (GPIO_IRQ_LINE_MUX(7, 2))       ///< Направить прерывания GPIO1_6 на линию_2
#define GPIO_IRQ_LINE_2_GPIO1_14    (GPIO_IRQ_LINE_MUX(8, 2))       ///< Направить прерывания GPIO1_14 на линию_2
#define GPIO_IRQ_LINE_2_GPIO2_6     (GPIO_IRQ_LINE_MUX(9, 2))       ///< Направить прерывания GPIO2_6 на линию_2

// Перечень выводов для назначения линии_3

#define GPIO_IRQ_LINE_3_GPIO0_3     (GPIO_IRQ_LINE_MUX(0, 3))       ///< Направить прерывания GPIO0_3 на линию_3
#define GPIO_IRQ_LINE_3_GPIO0_11    (GPIO_IRQ_LINE_MUX(1, 3))       ///< Направить прерывания GPIO0_11 на линию_3
#define GPIO_IRQ_LINE_3_GPIO1_3     (GPIO_IRQ_LINE_MUX(2, 3))       ///< Направить прерывания GPIO1_3 на линию_3
#define GPIO_IRQ_LINE_3_GPIO1_11    (GPIO_IRQ_LINE_MUX(3, 3))       ///< Направить прерывания GPIO1_11 на линию_3
#define GPIO_IRQ_LINE_3_GPIO2_3     (GPIO_IRQ_LINE_MUX(4, 3))       ///< Направить прерывания GPIO2_3 на линию_3
#define GPIO_IRQ_LINE_3_GPIO0_7     (GPIO_IRQ_LINE_MUX(5, 3))       ///< Направить прерывания GPIO0_7 на линию_3
#define GPIO_IRQ_LINE_3_GPIO0_15    (GPIO_IRQ_LINE_MUX(6, 3))       ///< Направить прерывания GPIO0_15 на линию_3
#define GPIO_IRQ_LINE_3_GPIO1_7     (GPIO_IRQ_LINE_MUX(7, 3))       ///< Направить прерывания GPIO1_7 на линию_3
#define GPIO_IRQ_LINE_3_GPIO1_15    (GPIO_IRQ_LINE_MUX(8, 3))       ///< Направить прерывания GPIO1_15 на линию_3
#define GPIO_IRQ_LINE_3_GPIO2_7     (GPIO_IRQ_LINE_MUX(9, 3))       ///< Направить прерывания GPIO2_7 на линию_3

// Перечень выводов для назначения линии_4

#define GPIO_IRQ_LINE_4_GPIO0_4     (GPIO_IRQ_LINE_MUX(0, 4))       ///< Направить прерывания GPIO0_4 на линию_4
#define GPIO_IRQ_LINE_4_GPIO0_12    (GPIO_IRQ_LINE_MUX(1, 4))       ///< Направить прерывания GPIO0_12 на линию_4
#define GPIO_IRQ_LINE_4_GPIO1_4     (GPIO_IRQ_LINE_MUX(2, 4))       ///< Направить прерывания GPIO1_4 на линию_4
#define GPIO_IRQ_LINE_4_GPIO1_12    (GPIO_IRQ_LINE_MUX(3, 4))       ///< Направить прерывания GPIO1_12 на линию_4
#define GPIO_IRQ_LINE_4_GPIO2_4     (GPIO_IRQ_LINE_MUX(4, 4))       ///< Направить прерывания GPIO2_4 на линию_4
#define GPIO_IRQ_LINE_4_GPIO0_0     (GPIO_IRQ_LINE_MUX(5, 4))       ///< Направить прерывания GPIO0_0 на линию_4
#define GPIO_IRQ_LINE_4_GPIO0_8     (GPIO_IRQ_LINE_MUX(6, 4))       ///< Направить прерывания GPIO0_8 на линию_4
#define GPIO_IRQ_LINE_4_GPIO1_0     (GPIO_IRQ_LINE_MUX(7, 4))       ///< Направить прерывания GPIO1_0 на линию_4
#define GPIO_IRQ_LINE_4_GPIO1_8     (GPIO_IRQ_LINE_MUX(8, 4))       ///< Направить прерывания GPIO1_8 на линию_4
#define GPIO_IRQ_LINE_4_GPIO2_0     (GPIO_IRQ_LINE_MUX(9, 4))       ///< Направить прерывания GPIO2_0 на линию_4

// Перечень выводов для назначения линии_5

#define GPIO_IRQ_LINE_5_GPIO0_5     (GPIO_IRQ_LINE_MUX(0, 5))       ///< Направить прерывания GPIO0_5 на линию_5
#define GPIO_IRQ_LINE_5_GPIO0_13    (GPIO_IRQ_LINE_MUX(1, 5))       ///< Направить прерывания GPIO0_13 на линию_5
#define GPIO_IRQ_LINE_5_GPIO1_5     (GPIO_IRQ_LINE_MUX(2, 5))       ///< Направить прерывания GPIO1_5 на линию_5
#define GPIO_IRQ_LINE_5_GPIO1_13    (GPIO_IRQ_LINE_MUX(3, 5))       ///< Направить прерывания GPIO1_13 на линию_5
#define GPIO_IRQ_LINE_5_GPIO2_5     (GPIO_IRQ_LINE_MUX(4, 5))       ///< Направить прерывания GPIO2_5 на линию_5
#define GPIO_IRQ_LINE_5_GPIO0_1     (GPIO_IRQ_LINE_MUX(5, 5))       ///< Направить прерывания GPIO0_1 на линию_5
#define GPIO_IRQ_LINE_5_GPIO0_9     (GPIO_IRQ_LINE_MUX(6, 5))       ///< Направить прерывания GPIO0_9 на линию_5
#define GPIO_IRQ_LINE_5_GPIO1_1     (GPIO_IRQ_LINE_MUX(7, 5))       ///< Направить прерывания GPIO1_1 на линию_5
#define GPIO_IRQ_LINE_5_GPIO1_9     (GPIO_IRQ_LINE_MUX(8, 5))       ///< Направить прерывания GPIO1_9 на линию_5
#define GPIO_IRQ_LINE_5_GPIO2_1     (GPIO_IRQ_LINE_MUX(9, 5))       ///< Направить прерывания GPIO2_1 на линию_5

// Перечень выводов для назначения линии_6

#define GPIO_IRQ_LINE_6_GPIO0_6     (GPIO_IRQ_LINE_MUX(0, 6))       ///< Направить прерывания GPIO0_6 на линию_6
#define GPIO_IRQ_LINE_6_GPIO0_14    (GPIO_IRQ_LINE_MUX(1, 6))       ///< Направить прерывания GPIO0_14 на линию_6
#define GPIO_IRQ_LINE_6_GPIO1_6     (GPIO_IRQ_LINE_MUX(2, 6))       ///< Направить прерывания GPIO1_6 на линию_6
#define GPIO_IRQ_LINE_6_GPIO1_14    (GPIO_IRQ_LINE_MUX(3, 6))       ///< Направить прерывания GPIO1_14 на линию_6
#define GPIO_IRQ_LINE_6_GPIO2_6     (GPIO_IRQ_LINE_MUX(4, 6))       ///< Направить прерывания GPIO2_6 на линию_6
#define GPIO_IRQ_LINE_6_GPIO0_2     (GPIO_IRQ_LINE_MUX(5, 6))       ///< Направить прерывания GPIO0_2 на линию_6
#define GPIO_IRQ_LINE_6_GPIO0_10    (GPIO_IRQ_LINE_MUX(6, 6))       ///< Направить прерывания GPIO0_10 на линию_6
#define GPIO_IRQ_LINE_6_GPIO1_2     (GPIO_IRQ_LINE_MUX(7, 6))       ///< Направить прерывания GPIO1_2 на линию_6
#define GPIO_IRQ_LINE_6_GPIO1_10    (GPIO_IRQ_LINE_MUX(8, 6))       ///< Направить прерывания GPIO1_10 на линию_6
#define GPIO_IRQ_LINE_6_GPIO2_2     (GPIO_IRQ_LINE_MUX(9, 6))       ///< Направить прерывания GPIO2_2 на линию_6

// Перечень выводов для назначения линии_7

#define GPIO_IRQ_LINE_7_GPIO0_7     (GPIO_IRQ_LINE_MUX(0, 7))       ///< Направить прерывания GPIO0_7 на линию_7
#define GPIO_IRQ_LINE_7_GPIO0_15    (GPIO_IRQ_LINE_MUX(1, 7))       ///< Направить прерывания GPIO0_15 на линию_7
#define GPIO_IRQ_LINE_7_GPIO1_7     (GPIO_IRQ_LINE_MUX(2, 7))       ///< Направить прерывания GPIO1_7 на линию_7
#define GPIO_IRQ_LINE_7_GPIO1_15    (GPIO_IRQ_LINE_MUX(3, 7))       ///< Направить прерывания GPIO1_15 на линию_7
#define GPIO_IRQ_LINE_7_GPIO2_7     (GPIO_IRQ_LINE_MUX(4, 7))       ///< Направить прерывания GPIO2_7 на линию_7
#define GPIO_IRQ_LINE_7_GPIO0_3     (GPIO_IRQ_LINE_MUX(5, 7))       ///< Направить прерывания GPIO0_3 на линию_7
#define GPIO_IRQ_LINE_7_GPIO0_11    (GPIO_IRQ_LINE_MUX(6, 7))       ///< Направить прерывания GPIO0_11 на линию_7
#define GPIO_IRQ_LINE_7_GPIO1_3     (GPIO_IRQ_LINE_MUX(7, 7))       ///< Направить прерывания GPIO1_3 на линию_7
#define GPIO_IRQ_LINE_7_GPIO1_11    (GPIO_IRQ_LINE_MUX(8, 7))       ///< Направить прерывания GPIO1_11 на линию_7
#define GPIO_IRQ_LINE_7_GPIO2_3     (GPIO_IRQ_LINE_MUX(9, 7))       ///< Направить прерывания GPIO2_3 на линию_7


#ifndef __ASSEMBLER__
    #include <inttypes.h>

    /// @brief Структура регистров модуля управления прерываниями GPIO
    typedef struct
    {
        volatile uint32_t STATE;                ///< [RO]: Регистр текущего состояния выводов линий после мультиплексоров. Биты [7:0] соответствуют номеру линий.
        volatile uint32_t LINE_MUX;             ///< [WO]: Регистр управления мультиплексорами. Каждый мультиплексор управляется своими 4 битами.
        volatile uint32_t INTERRUPT;            ///< [RW]: Регистр текущего состояния прерываний. Биты [7:0] соответствуют номеру линии (0 - линия не сработала, 1 - линия сработала).
        volatile uint32_t ENABLE_SET;           ///< [RW]: Регистр разрешения прерываний. Биты [7:0] хранят текущее состояние разрешения прерываний для каждой линии. Запись "1" разрешает прерывание от соответствующего вывода.
        volatile uint32_t ENABLE_CLEAR;         ///< [RW]: Регистр запрета прерываний. Биты [7:0] хранят текущее состояние разрешения прерываний для каждой линии. Запись "1" запрещает прерывание от соответствующего вывода.
        volatile uint32_t EDGE;                 ///< [RW]: Регистр типа прерываний. Биты [7:0] хранят текущее состояние типа прерывания (0 - по уровню, 1 - по событию (фронт или спад)) для каждой линии. Запись "1" устанавливает прерывание по событию, "0" - не влияет.
        volatile uint32_t LEVEL;                ///< [RW]: Регистр типа прерываний (инвертированный). Биты [7:0] хранят текущее состояние типа прерывания (0 - по событию (фронт или спад), 1 - по уровню) для каждой линии. Запись "1" устанавливает прерывание по уровню, "0" - не влияет.
        volatile uint32_t LEVEL_SET;            ///< [RW]: Регистр выбора уровня/события прерываний. Биты [7:0] хранят текущее состояние уровня / события (0 - по спаду или уровню логического "0", 1 - по фронту или уровню логической "1") для каждой линии. Запись "1" устанавливает прерывание по фронту или уровню логической "1".
        volatile uint32_t LEVEL_CLEAR;          ///< [RW]: Регистр уровня/события прерывания. Биты [7:0] хранят текущую конфигурацию срабатывания прерываний (0 - по спаду или логическому "0", 1 - по фронту или логической "1") для каждой линии. Запись "1" - устанавливает прерывание по спаду или уровню логического "0".
        volatile uint32_t ANY_EDGE_SET;         ///< [RW]: Регистр установки срабатывания прерываний по любому событию. Биты [7:0] хранят текущую конфигурацию срабатывания прерываний (0 - прерывания не формируются, 1 - прерывания формируются) для каждой линии. Запись "1" - устанавливает формирование прерываний по любому изменению вывода.
        volatile uint32_t ANY_EDGE_CLEAR;       ///< [RW]: Регистр выключения срабатывания прерываний по любому событию. Биты [7:0] хранят текущую конфигурацию срабатывания прерываний (0 - прерывания не формируются, 1 - прерывания формируются) для каждой линии. Запись "1" - выключает формирование прерываний по любому изменению вывода.
        volatile uint32_t CLEAR;                ///< [W1C]: Регистр сброса флагов прерывания соответствующего канала. Биты [7:0] отвечают за сброс сработавшего прерывания. Запись "1" очищает флаг прерывания соответствующей линии регистра INTERRUPT.
    } GPIO_IRQ_TypeDef;

#endif

#endif    