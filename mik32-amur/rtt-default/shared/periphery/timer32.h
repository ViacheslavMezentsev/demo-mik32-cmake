/**
 * @file 
 * Модуль 32-разрядного таймера
 * 
 * Модуль таймера выполнен на основе 32-х битного счётчика, поддерживает три режима счёта – прямой, 
 * обратный и двунаправленный и имеет четыре канала ШИМ и поддержку режимов захвата и сравнения 
 * (модуль таймера32_0 реализован без ШИМ и режимов захвата/сравнения). 
 * 
 * Особенности таймера/счетчика:
 * - 32-х битный счётчик трехрежимный;
 * - 32-х битный программируемый делитель тактовой частоты;
 * - четыре вида маскируемых прерываний (переполнение счётчика, опустошение счётчика, захват значения, совпадение по сравнению);
 * - возможность тактирования от системной тактовой частоты или внешнего вывода;
 * - четыре независимых канала, способных работать в режиме захвата, сравнения или ШИМ (только для timer32_1, timer32_2).
 */

#ifndef TIMER32_H_INCLUDED
#define TIMER32_H_INCLUDED


#define TIMER32_VALUE_OFFSET			    0x00
#define TIMER32_TOP_OFFSET				    0x04
#define TIMER32_PRESCALER_OFFSET		    0x08
#define TIMER32_CONTROL_OFFSET			    0x0C
#define TIMER32_ENABLE_OFFSET               0x10
#define TIMER32_INT_MASK_OFFSET             0x14   
#define TIMER32_INT_CLEAR_OFFSET            0x18   
#define TIMER32_INT_FLAGS_OFFSET            0x1C   

// Смещение регистров канала от базового адреса таймера

#define TIMER32_CH1_BASE			        0x80
#define TIMER32_CH2_BASE			        0x90
#define TIMER32_CH3_BASE			        0xA0
#define TIMER32_CH4_BASE			        0xB0

// Смещение регистра канала от базового адреса канала

#define TIMER32_CH_CONTROL_OFFSET	        0x00
#define TIMER32_CH_OCR_OFFSET		        0x04
#define TIMER32_CH_ICR_OFFSET		        0x08
#define TIMER32_CH_SIZE			            0x10

/**
 * Регистр VALUE модуля таймера
 * [RO] TIM_VAL [31:0] - Текущее значение счетчика
 */

/**
 *  Регистр TOP модуля таймера
 * [RW] TIM_TOP [31:0] - Максимальное значение счетчика (ограничивает счетную последовательность сверху)
 */

/** 
 * Регистр PRESCALE модуля таймера
 * Значение предварительного делителя. Предделитель вырабатывает тактовый сигнал для счета, 
 * частота которого в (TIM_PRESCALE + 1) раз меньше входной. Диапазон значений от 0 до  (232 - 1).
 */


#define TIMER32_PRESCALER_ENABLE_S	        8
#define TIMER32_PRESCALER_ENABLE_M	        (1 << TIMER32_PRESCALER_ENABLE_S)
#define TIMER32_PRESCALER_DISABLE_M	        (0 << TIMER32_PRESCALER_ENABLE_S)
//
#define TIMER32_PRESCALER_S			        0
#define TIMER32_PRESCALER_M			        (0xFF << TIMER32_PRESCALER_S)

//  Регистр CONTROL модуля таймера

#define TIMER32_CONTROL_CLOCK_S	    	    2
/**
 * Выбор источника тактового сигнала для счета:
 * - 0b00 – Частота APB (для timer32_0 – APB_M, для timer32_1, timer32_2 – APB_P);
 * - 0b01 – синхронный вход тактирования TIM1 (настраивается в регистрах TIMER_CFG блока управления питанием и тактированием);
 * - 0b10 – вход микросхемы timerX_Tx_pin_pad (X – номер таймера); 
 * - 0b11 – асинхронный вход тактирования TIM2 настраивается в регистрах TIMER_CFG блока управления питанием и тактированием;
 */
#define TIMER32_CONTROL_CLOCK_M		        (3 << TIMER32_CONTROL_CLOCK_S)

#define TIMER32_CONTROL_CLOCK_PRESCALER_M	(0 << TIMER32_CONTROL_CLOCK_S)
#define TIMER32_CONTROL_CLOCK_TIM1_M		(1 << TIMER32_CONTROL_CLOCK_S)
#define TIMER32_CONTROL_CLOCK_TX_PIN_M		(2 << TIMER32_CONTROL_CLOCK_S)
#define TIMER32_CONTROL_CLOCK_TIM2_M		(3 << TIMER32_CONTROL_CLOCK_S)

#define TIMER32_CONTROL_MODE_S	        	0
/**
 * Режим счёта таймера:
 * - 0b00 – прямой;
 * - 0b01 – обратный;
 * - 0b10 – двунаправленный
 */
#define TIMER32_CONTROL_MODE_M		        (3 << TIMER32_CONTROL_MODE_S)

#define TIMER32_CONTROL_MODE_UP_M	        (0 << TIMER32_CONTROL_MODE_S)
#define TIMER32_CONTROL_MODE_DOWN_M	        (1 << TIMER32_CONTROL_MODE_S)
#define TIMER32_CONTROL_MODE_BIDIR_M	    (2 << TIMER32_CONTROL_MODE_S)	

//  Регистр ENABLE модуля таймера

#define TIMER32_ENABLE_TIM_CLR_S		    1
/// Сброс (обнуление) текущего значения счетчика при записи «1»
#define TIMER32_ENABLE_TIM_CLR_M		    (1 << TIMER32_ENABLE_TIM_CLR_S)

#define TIMER32_ENABLE_TIM_EN_S			    0
/**
 * Запуск /останов работы счетчика:
 * - 0 - счетчик выключен;
 * - 1 - счетчик работает.
 */
#define TIMER32_ENABLE_TIM_EN_M			    (1 << TIMER32_ENABLE_TIM_EN_S)

// INT_MASK, INT_CLEAR, INT_FLAGS

#define TIMER32_INT_OVERFLOW_S		        0
/// переполнение счётчика
#define TIMER32_INT_OVERFLOW_M		        (1 << TIMER32_INT_OVERFLOW_S)

#define TIMER32_INT_UNDERFLOW_S		        1
/// опустошение счётчика
#define TIMER32_INT_UNDERFLOW_M		        (1 << TIMER32_INT_UNDERFLOW_S)

#define TIMER32_INT_IC_S                    2 
/// захват значения
#define TIMER32_INT_IC_M(channelIndex)      ((1 << channelIndex) << TIMER32_INT_IC_S)

#define TIMER32_INT_OC_S                    6 
/// совпадение по сравнению
#define TIMER32_INT_OC_M(channelIndex)      ((1 << channelIndex) << TIMER32_INT_OC_S)

//TODO: Добавлено описание регистра INT_FLAG модуля таймера [RO]

#define TIMER32_INT_FLAG_OC_INT_CH4_M       (1 << 9)        ///< Статус события: совпадение 4 канала тайме
#define TIMER32_INT_FLAG_OC_INT_CH3_M       (1 << 8)        ///< Статус события: совпадение 3 канала таймера
#define TIMER32_INT_FLAG_OC_INT_CH2_M       (1 << 7)        ///< Статус события: совпадение 2 канала тайме
#define TIMER32_INT_FLAG_OC_INT_CH1_M       (1 << 6)        ///< Статус события: совпадение 1 канала тайм
#define TIMER32_INT_FLAG_IC_INT_CH4_M       (1 << 5)        ///< Статус события: захват 4 канала таймера
#define TIMER32_INT_FLAG_IC_INT_CH3_M       (1 << 4)        ///< Статус события: захват3 канала таймера
#define TIMER32_INT_FLAG_IC_INT_CH2_M       (1 << 3)        ///< Статус события: захват2 канала таймера
#define TIMER32_INT_FLAG_IC_INT_CH1_M       (1 << 2)        ///< Статус события: захват1 канала таймера
#define TIMER32_INT_FLAG_UDF_INT_M          (1 << 1)        ///< Статус события: опустошение счетчика
#define TIMER32_INT_FLAG_OVF_INT_M          (1 << 0)        ///< Статус события: переполнение счетчик

//TODO: Добавлено описание регистра INT_MSK модуля таймера [RW]

#define TIMER32_INT_MSK_OC_INT_CH4          (1 << 9)        ///< Маска прерывания совпадения 4 канала таймер
#define TIMER32_INT_MSK_OC_INT_CH3          (1 << 8)        ///< Маска прерывания совпадения 3 канала таймер
#define TIMER32_INT_MSK_OC_INT_CH2          (1 << 7)        ///< Маска прерывания совпадения 2 канала таймера
#define TIMER32_INT_MSK_OC_INT_CH1          (1 << 6)        ///< Маска прерывания совпадения 1 канала тайм
#define TIMER32_INT_MSK_IC_INT_CH4          (1 << 5)        ///< Маска прерывания захвата 4 канала таймера
#define TIMER32_INT_MSK_IC_INT_CH3          (1 << 4)        ///< Маска прерывания захвата 3 канала таймера
#define TIMER32_INT_MSK_IC_INT_CH2          (1 << 3)        ///< Маска прерывания захвата 2 канала таймера
#define TIMER32_INT_MSK_IC_INT_CH1          (1 << 2)        ///< Маска прерывания захвата 1 канала таймера
#define TIMER32_INT_MSK_UDF_INT_M           (1 << 1)        ///< Маска прерывания опустошения счетчика
#define TIMER32_INT_MSK_OVF_INT_M           (1 << 0)        ///< Маска прерывания по переполнению счетчика

//CH_CNTRL
#define TIMER32_CH_CNTRL_NOISE_S	        0
/**
 * Управление фильтрацией входных помех на входах микросхеме 
 * Timer32_1_ch-Timer32_1_ch4, Timer32_2_ch1-Timer32_2_ch4 в режиме захвата. 
 * 0 – фильтрация выключена;
 * 1 – фильтрация включена (фильтруются помехи продолжительностью менее 3 тактов частоты APB_P)
 */
#define TIMER32_CH_CNTRL_NOISE_M	        (1 << TIMER32_CH_CNTRL_NOISE_S)

#define TIMER32_CH_CNTRL_CAPTURE_EDGE_S	    4
/**
 * Режим сигнала захвата:
 * 0 – фронт;
 * 1 – срез.
 */
#define TIMER32_CH_CNTRL_CAPTURE_EDGE_M	    (1 << TIMER32_CH_CNTRL_CAPTURE_EDGE_S)
#define TIMER32_CH_CNTRL_CAPTURE_POS_M	    (0 << TIMER32_CH_CNTRL_CAPTURE_EDGE_S)
#define TIMER32_CH_CNTRL_CAPTURE_NEG_M	    (1 << TIMER32_CH_CNTRL_CAPTURE_EDGE_S)

#define TIMER32_CH_CNTRL_MODE_S			    5
/**
 * Режим работы канала:
 * 0b00 – зарезервировано;
 * 0b01 – режим сравнения;
 * 0b10 – режим захвата;
 * 0b11 – ШИМ
 */
#define TIMER32_CH_CNTRL_MODE_M			    (3 << TIMER32_CH_CNTRL_MODE_S)
#define TIMER32_CH_CNTRL_MODE_COMPARE_M	    (1 << TIMER32_CH_CNTRL_MODE_S)
#define TIMER32_CH_CNTRL_MODE_CAPTURE_M	    (2 << TIMER32_CH_CNTRL_MODE_S)
#define TIMER32_CH_CNTRL_MODE_PWM_M		    (3 << TIMER32_CH_CNTRL_MODE_S)

#define TIMER32_CH_CNTRL_ENABLE_S		    7
/// Включение/выключение канала
#define TIMER32_CH_CNTRL_ENABLE_M		    (1 << TIMER32_CH_CNTRL_ENABLE_S)
#define TIMER32_CH_CNTRL_DISABLE_M		    (0 << TIMER32_CH_CNTRL_ENABLE_S)

#define TIMER32_CH_CNTRL_INVERTED_PWM_S	    8
/**
 * Тип вывода в режиме ШИМ инвертирование:
 * 0 – прямой (не инвертированный выход);
 * 1 – инвертированный выход
 */
#define TIMER32_CH_CNTRL_INVERTED_PWM_M	    (1 << TIMER32_CH_CNTRL_INVERTED_PWM_S)

#define TIMER32_CH_CNTRL_DIR_S	            9
/// Направление передачи данных. Устанавливается автоматически в зависимости от режима работы (1 — выход, 0 – вход)
#define TIMER32_CH_CNTRL_DIR_M	            (1 << TIMER32_CH_CNTRL_DIR_S)

/**
 *  Регистр CHy_OCR модуля таймера
 * [RW]: OCR [31:0] - Значение таймера в режиме сравнения
 */

/**
 *  Регистр CHy_ICR модуля таймера
 * [RW] ICR [31:0] - Значение таймера в режиме захвата
 */


#ifndef __ASSEMBLER__
    #include <inttypes.h>

    /// @brief Структура регистров канала 32-битного таймера
    typedef struct
    {
        volatile uint32_t CNTRL;                    ///< [RW]: Конфигурационный регистр канала
        volatile uint32_t OCR;                      ///< [RW]: Значение сравнения канала
        volatile uint32_t ICR;                      ///< [RW]: Значение захвата канала
        volatile uint32_t reserved0;                ///< Резерв
    } TIMER32_CHANNEL_TypeDef;
    
    /// @brief Структура регистров 32-битного таймера
    typedef struct
    {
        volatile uint32_t VALUE;                    ///< [RO]: Текущее значение основного таймера
        volatile uint32_t TOP;                      ///< [RW]: Максимальное значение счётной последовательности
        volatile uint32_t PRESCALER;                ///< [RW]: Значение делителя
        volatile uint32_t CONTROL;                  ///< [RW]: Конфигурационный регистр основного таймера
        volatile uint32_t ENABLE;                   ///< [RW]: Регистр включения таймера
        volatile uint32_t INT_MASK;                 ///< [RW]: Регистр маски прерываний
        volatile uint32_t INT_CLEAR;                ///< [W1C]: Регистр сброса флагов прерываний
        volatile uint32_t INT_FLAGS;                ///< [RO]: Регистр флагов прерываний
        
        volatile uint8_t  reserved0[0x60];          ///< Резерв
        TIMER32_CHANNEL_TypeDef CHANNELS[4];        ///< Регистры 4-х каналов 32-битного таймера
        
    } TIMER32_TypeDef;
#endif

#endif // TIMER32_H_INCLUDED
