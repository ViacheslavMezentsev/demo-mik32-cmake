/**
 * @file
 * Блок управления аналоговой подсистемой
 * 
 */

#ifndef ANALOG_REG_H_INCLUDED
#define ANALOG_REG_H_INCLUDED

#define ANALOG_REG_DAC0_CFG_OFFSET                  0x00
#define ANALOG_REG_DAC0_VALUE_OFFSET                0x04
#define ANALOG_REG_DAC1_CFG_OFFSET                  0x08
#define ANALOG_REG_DAC1_VALUE_OFFSET                0x0c
#define ANALOG_REG_PVD_DPF_VALUE_OFFSET             0x10
#define ANALOG_REG_PVD_CONFIG_OFFSET                0x14
//BUG: В документации указано смещение 0x1c
#define ANALOG_REG_PVD_STATUS_OFFSET                0x18
#define ANALOG_REG_TSENS_CFG_OFFSET                 0x1c
#define ANALOG_REG_TSENS_THRESHOLD_OFFSET           0x20
#define ANALOG_REG_TSENS_IRQ_OFFSET                 0x24
#define ANALOG_REG_TSENS_CLEAR_IRQ_OFFSET           0x28
#define ANALOG_REG_TSENS_VALUE_OFFSET               0x2c
#define ANALOG_REG_TSENS_SINGLE_OFFSET              0x30
#define ANALOG_REG_TSENS_CONTINUOUS_OFFSET          0x34
#define ANALOG_REG_REFV_CONFIG_OFFSET               0x38
#define ANALOG_REG_ADC_CONFIG_OFFSET                0x3c
#define ANALOG_REG_ADC_CONTINUOUS_OFFSET            0x40
#define ANALOG_REG_ADC_SINGLE_OFFSET                0x44
#define ANALOG_REG_ADC_VALID_OFFSET                 0x48
#define ANALOG_REG_ADC_VALUE_OFFSET                 0x4c
#define ANALOG_REG_TEST_MUX_OFFSET                  0x50


/**
 * Регистры DAC0_CFG , DAC1_CFG 
 */

/**
 * [RW]: VALUE [11:0]  - Входные данные ЦАП для преобразования
 */


#define DAC_CFG_EN18_S          0
/**
 * Управление питанием ЦАП:
 * «0» – выключен;
 * «1» – включен
 */
#define DAC_CFG_EN18_M          (1 << DAC_CFG_EN18_S)

#define DAC_CFG_RESN_S          1
/// Управление сбросом ЦАП, активный уровень «0»
#define DAC_CFG_RESN_M          (1 << DAC_CFG_RESN_S)

#define DAC_CFG_DIV_S           2
#define DAC_CFG_DIV_M           (0xFF << DAC_CFG_DIV_S)
/// Значение делителя тактового сигнала. Частота определяется как F_ЦАП=F_IN/(DIV+1)
#define DAC_CFG_DIV(v)          (((v) << DAC_CFG_DIV_S) & DAC_CFG_DIV_M)

#define DAC_CFG_EXTEN_S         10
/**
 * Выбор источника опорного напряжения:
 * «1» - внешний;
 * «0» - встроенный
 */
#define DAC_CFG_EXTEN_M         (1 << DAC_CFG_EXTEN_S)

#define DAC_EXTPAD_EN_S         11
/**
 * Выбор источника внешнего опорного напряжения:
 * «1» - внешний вывод;
 * «0» - настраиваемый ОИН.
 */
#define DAC_EXTPAD_EN_M         (1 << DAC_EXTPAD_EN_S)

#define DAC_EMPTY_READ_S        13 //Need New Name
/**
 * «1» - значение, хранящееся в DAC_VALUE было сдвинуто в ЦАП, возможна запись следующего значения;
 * «0» - в регистре DAC_Value находится необработанное значение; 
 * @warning Автоматически сбрасывается при записи в DAC_Value
 */
#define DAC_EMPTY_READ_M        (1 << DAC_EMPTY_READ_S)

/**
 * Температурный сенсор преобразует электрические величины, прямо пропорционально зависящие от температуры кристалла, 
 * в двоичное представление этой температуры. Технические характеристики:
 * - точность измерения температуры ±0,1 ℃;
 * - погрешность измерения температуры в диапазоне [–40 °C; 125 °C] не превышает 1 %;
 * - наличие спящего режима с пониженным потреблением тока;
 * - функция подстройки для уменьшения влияния разброса техпроцесса.
 * - Тактовая частота 32КГц-100КГц
 */

///  Регистр TSENS_CFG температурного сенсора

#define TSENS_CFG_NPD_S             0
/**
 * Управление питанием сенсора:
 * «0» – сенсор выключен;
 * «1» – сенсор включен.
 */
#define TSENS_CFG_NPD_M             (1 << TSENS_CFG_NPD_S)

#define TSENS_CFG_NPD_CLK_S         1
/**
 * Управление тактированием сенсора:
 * «0» – тактирование сенсора выключено;
 * «1» – тактирование сенсора включено.
 */
#define TSENS_CFG_NPD_CLK_M         (1 << TSENS_CFG_NPD_CLK_S)

#define TSENS_CFG_NRST_S            2
/// Управление сбросом сенсора, активный уровень «0»
#define TSENS_CFG_NRST_M            (1 << TSENS_CFG_NRST_S)

#define TSENS_CFG_CLK_MUX_S         3
/**
 * Выбор источника тактирования сенсора (FIN):
 * 0x0 – системная частота (sys_clk);
 * 0x1 – частота шины AHB (hclk);
 * 0x2 – частота внешнего осцилятора 32 МГц;
 * 0x3 – частота HSI32M;
 * 0x4 – частота внешнего осцилятора 32 кГц;
 * 0x5 – частота LSI32K.
 */
#define TSENS_CFG_CLK_MUX_M         (0b111 << TSENS_CFG_CLK_MUX_S)

#define TSENS_CFG_DIV_S             6
/// Значение делителя тактового сигнала. Частота сенсора определяется как TSENS = FIN/(2*(DIV + 1)
#define TSENS_CFG_DIV_M             (0x3FF << TSENS_CFG_DIV_S)

// Регистр TSENS_TRESHOLD температурного сенсора

#define TSENS_TRESHOLD_HI_S    0
/**
 * При значении выхода сенсора больше HI_THRESHOLD будет формироваться прерывание. 
 * Значение по умолчанию соответствует температуре 125 С
 */
#define TSENS_TRESHOLD_HI_M    (0x3FF << TSENS_TRESHOLD_HI_S)

#define TSENS_TRESHOLD_LOW_S   10
/**
 * При значении выхода сенсора менее LOW_THRESHOLD будет формироваться прерывание. 
 * Значение по умолчанию соответсвует температуре -40 С.
 */
#define TSENS_TRESHOLD_LOW_M   (0x3FF << TSENS_TRESHOLD_LOW_S)

// Регистр TSENS_IRQ температурного сенсора

#define TSENS_IRQ_EOC_MASK_S    0
/// Маска прерывания по окончанию преобразования
#define TSENS_IRQ_EOC_MASK_M    (1 << TSENS_IRQ_EOC_MASK_S)

#define TSENS_IRQ_HI_MASK_S     1
/// Маска прерывания при событии «выход сенсора больше порогового значения HI_ THRESHOLD»
#define TSENS_IRQ_HI_MASK_M     (1 << TSENS_IRQ_HI_MASK_S)

#define TSENS_IRQ_LOW_MASK_S    2
/// Маска прерывания при событии «выход сенсораменьше порогового значения LOW_ THRESHOLD»
#define TSENS_IRQ_LOW_MASK_M    (1 << TSENS_IRQ_LOW_MASK_S)

#define TSENS_IRQ_EOC_IRQ_S     3
/// Статус прерывания по окончанию преобразования
#define TSENS_IRQ_EOC_IRQ_M     (1 << TSENS_IRQ_EOC_IRQ_S)

#define TSENS_IRQ_HI_IRQ_S      4
/// Статус прерывания событии «выход сенсора больше порогового значения HI_ THRESHOLD»
#define TSENS_IRQ_HI_IRQ_M      (1 << TSENS_IRQ_HI_IRQ_S)

#define TSENS_IRQ_LOW_IRQ_S     5
/// Статус прерывания события «выход сенсора меньше порогового значения LOW_ THRESHOLD»
#define TSENS_IRQ_LOW_IRQ_M     (1 << TSENS_IRQ_LOW_IRQ_S)

// Регистр TSENS_CLEAR_IRQ температурного сенсора

#define TSENS_CLEAR_IRQ_EOC_CLEAR_S     0
/// Сброс прерывания по окончанию преобразования
#define TSENS_CLEAR_IRQ_EOC_CLEAR_M     (1 << TSENS_CLEAR_IRQ_EOC_CLEAR_S)

#define TSENS_CLEAR_IRQ_HI_CLEAR_S      1
/// Сброс прерывания событии «выход сенсора больше порогового значения HI_ THRESHOLD»
#define TSENS_CLEAR_IRQ_HI_CLEAR_M      (1 << TSENS_CLEAR_IRQ_HI_CLEAR_S)

#define TSENS_CLEAR_IRQ_LOW_CLEAR_S     2
/// Сброс прерывания события «выход сенсораменьше порогового значения LOW_ THRESHOLD»
#define TSENS_CLEAR_IRQ_LOW_CLEAR_M     (1 << TSENS_CLEAR_IRQ_LOW_CLEAR_S)

// Регистр TSENS_VALUE температурного сенсора

#define TSENS_VALUE_EOC_S               10
/// Текущее значение выхода окончания преобразования
#define TSENS_VALUE_EOC_M               (1 << TSENS_VALUE_EOC_S)

#define TSENS_VALUE_VALUE_S             0
/// Последнее измеренное значение сенсором
#define TSENS_VALUE_VALUE_M             (0x3FF << TSENS_VALUE_VALUE_S)

/// Преобразовать градусы (по Цельсию) в значение температурного сенсора
#define TSENS_CELSIUS_TO_VALUE(i)       (40960*100/(((6406600-93*((int)(i)*100+27315))*100)/((int)(i)*100+27315))) // (uint32_t)(4096/((10.3*622)/(i+273.15)-10.3 +1))
/// Преобразовать значение температурного сенсора в градусы (по Цельсию)
#define TSENS_VALUE_TO_CELSIUS(v)       ((640660*(uint32_t)(v))/(40960+93*(uint32_t)(v))*10-27315) // Значение температуры в 100 раз больше

/**
 * Регистр TSENS_SINGLE температурного сенсора
 * SINGLE [0] - Запуск одного измерения [W1]
 */

/**
 *  Регистр TSENS_CONTINUOUS температурного сенсора
 *  CONTINIUS [0] - Запуск / остановка непрерывного измерения температуры [RW]
 */


/**
 * Аналого–цифровой преобразователь (АЦП) предназначен для формирования 12-битного цифрового кода, 
 * пропорционального аналоговому входному напряжению
 */

#define ADC_CONFIG_EN_S                0
/**
 * Управление питанием АЦП:
 * «0» - выключен;
 * «1» - включен.
 */
#define ADC_CONFIG_EN_M                (1 << ADC_CONFIG_EN_S)

#define ADC_CONFIG_RESETN_S            1
/// Управление сбросом АЦП, активный уровень «0»
#define ADC_CONFIG_RESETN_M            (1 << ADC_CONFIG_RESETN_S)

#define ADC_CONFIG_EXTREF_S            2
/**
 * Выбор источника опорного напряжения:
 * «1» - внешний;
 * «0» - встроенный
 */
#define ADC_CONFIG_EXTREF_M            (1 << ADC_CONFIG_EXTREF_S)

#define ADC_CONFIG_EXTPAD_EN_S         3
/**
 * Выбор источника внешнего опорного напряжения:
 * «1» - внешний вывод;
 * «0» - настраиваемый ОИН
 */
#define ADC_CONFIG_EXTPAD_EN_M         (1 << ADC_CONFIG_EXTPAD_EN_S)

#define ADC_CONFIG_SEL_S               4
/// Выбор канала АЦП
#define ADC_CONFIG_SEL_M               (0b111 << ADC_CONFIG_SEL_S)

#define ADC_CONFIG_SAH_TIME_S          8
#define ADC_CONFIG_SAH_TIME_READ_S     9
#define ADC_CONFIG_SAH_TIME_WRITE_S    8
/// Время выборки очередного отсчета в тактах АЦП
#define ADC_CONFIG_SAH_TIME_M          (0x3F << ADC_CONFIG_SAH_TIME_S)
#define ADC_CONFIG_SAH_TIME_READ_M     (0x3F << ADC_CONFIG_SAH_TIME_READ_S)
#define ADC_CONFIG_SAH_TIME_WRITE_M    (0x3F << ADC_CONFIG_SAH_TIME_WRITE_S)

/**
 * Регистр ADC_CONTINUOUS АЦП
 * [RW]: CONTINUOUS [0] - Запуск / остановка непрерывного измерения
 */

/**
 * Регистр ADC_SINGLE АЦП 
 * [RW]: SINGLE [0] - Запуск однократного измерения
 */

/**
 * Регистр ADC_VALID АЦП 
 * [RO]: VALID [0] - Признак наличия актуальных данных
 */

/**
 *  Регистр ADC_VAlUE АЦП
 * [RO]: VALUE [11:0] - Результат преобразования
 */


// Регистр управления калибруемыми источниками напряжения и тока REF_CLB

#define REF_CLB_VCOEF_S         0
/**
 * Разрешение работы калибруемых источников:
 * «1» - включить;
 * «0» - выключить.
 */
#define REF_CLB_VCOEF_M         (0xF << REF_CLB_VCOEF_S)

#define REF_CLB_ICOEF_S         4
/// Коэффициент настройки опорного источника тока
#define REF_CLB_ICOEF_M         (0xF << REF_CLB_ICOEF_S)

#define REF_CLB_EN_S            8
/// Коэффициент настройки опорного источника напряжения
#define REF_CLB_EN_M            (1 << REF_CLB_EN_S)

#define TEST_DIG_MUX_COV_LDO_M  (0 << 9)
#define TEST_DIG_MUX_BATON_M    (1 << 9)
#define TEST_DIG_MUX_BOR_M      (2 << 9)
#define TEST_DIG_MUX_PGL_BU_M   (3 << 9)
#define TEST_DIG_MUX_POR_M      (4 << 9)
#define TEST_DIG_MUX_BGO_M      (5 << 9)
#define TEST_DIG_MUX_PGL_M      (6 << 9)

#define TEST_DIG_MUX_OSC_CLK32M_M     (12 << 9)
#define TEST_DIG_MUX_OSC_CLK32K_M     (13 << 9)
#define TEST_DIG_MUX_RC_CLK32M_M      (14 << 9)
#define TEST_DIG_MUX_RC_CLK32K_M      (15 << 9)

/**
 * Цифро-аналоговый преобразователь (ЦАП) предназначен для формирования 
 * напряжения пропорционального 12-битному управляющему коду
 */

#ifndef __ASSEMBLER__
    #include <inttypes.h>
    /// @brief Структура регистров цифро-аналогового преобразователя
    typedef struct
    {
        volatile uint32_t CFG;                          ///< [RW]: Регистр настройки ЦАП
        volatile uint32_t VALUE;                        ///< [RW]: Входные данные для ЦАП
    } DAC_TypeDef;
    
    /// @brief Структура регистров блока управления аналоговой подсистемой
    typedef struct
    {
        volatile DAC_TypeDef    DAC0;                   ///< [RW]: Регистры ЦАП 0
        volatile DAC_TypeDef    DAC1;                   ///< [RW]: Регистры ЦАП 1
        volatile uint32_t       PVD_DPF_VALUE;          ///< [RW]: Регистр настройки цифрового фильтра монитора напряжения AVCC (см. PVD Control)
        volatile uint32_t       PVD_CONFIG;             ///< [RW]: Регистр настроек монитора напряжения AVCC (см. PVD Control)
        volatile uint32_t       PVD_STATUS;             ///< [RO]: Регистр статуса монитора напряжения AVCC (см. PVD Control)
        volatile uint32_t       TSENS_CFG;              ///< [RW]: Регистр управления температурным сенсором
        volatile uint32_t       TSENS_THRESHOLD;        ///< [RW]: Регистр пороговых значений температурного сенсора
        volatile uint32_t       TSENS_IRQ;              ///< [RW]: Регистр прерываний температурного сенсора
        volatile uint32_t       TSENS_CLEAR_IRQ;        ///< [W1C]: Регистр сброса прерываний температурного сенсора
        volatile uint32_t       TSENS_VALUE;            ///< [RO]: Регистр данных температурного сенсора
        volatile uint32_t       TSENS_SINGLE;           ///< [WO]: Регистр запуска однократного измерения температурным сенсором. Запись "1" запускает однократное измерение
        //TODO: уточнить уровень доступа к регистру (в списке WO, в описании - RW).
        volatile uint32_t       TSENS_CONTINUOUS;       ///< [WO]: Регистр запуска непрерывного измерения температурным сенсором. Запись "1" Запускает непрерывные измерения температуры, "0" - останавливает.
        volatile uint32_t       REFV_CONFIG;            ///< [RW]: Регистр управления калибруемыми источниками (ЦАП).
        volatile uint32_t       ADC_CONFIG;             ///< [RW]: Регистр настроек АЦП.
        volatile uint32_t       ADC_CONTINUOUS;         ///< [RW]: Регистр управления непрерывным измерением АЦП. Запись "1" запускает непрерывное изменение, "0" - останавливает.
        volatile uint32_t       ADC_SINGLE;             ///< [RW]: Регистр управления однократным измерением АЦП. Запись "1" запускает однократное измерение.
        volatile uint32_t       ADC_VALID;              ///< [RW]: Регистр статуса АЦП. Бит [0] содержит признак наличия актуальных данных.
        volatile uint32_t       ADC_VALUE;              ///< [RW]: Регистр данных АЦП. Биты [11:0] хранят результат преобразования.
        volatile uint32_t       TEST_MUX;
    } ANALOG_REG_TypeDef;
    
#endif

#endif // ANALOG_REG_H_INCLUDED