/**
 * @file
 * Часы реального времени
 * 
 * Модуль реализует функцию часов реального времени. Основные возможности: 
 * - 24-часовой режим работы;
 * - функция календаря с корректировкой для високосного года;
 * - программируемый будильник с возможностью генерации прерываний;
 * - двоично-десятичное кодирование данных.
 */

#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#define RTC_TIME_OFFSET                 0x00
#define RTC_DATE_OFFSET                 0x04
#define RTC_TALRM_OFFSET                0x08
#define RTC_DALRM_OFFSET                0x0C
#define RTC_CTRL_OFFSET                 0x10
#define RTC_BACKUP_REG_OFFSET           0x20

/**
 * Регистр управления RRTC_CTRL – регистр управления модулем.
 */

#define RTC_CTRL_EN_S                   31
/// Модуль включён и производит отсчёт времени, когда установлен
#define RTC_CTRL_EN_M                   (1 << RTC_CTRL_EN_S)

#define RTC_CTRL_ALRM_S                 30
/// Установленный бит свидетельствует о совпадении одного разрешенного или всех разрешённых полей будильника. Бит необходимо сбрасывать при помощи управляющего ПО
#define RTC_CTRL_ALRM_M                 (1 << RTC_CTRL_ALRM_S)

#define RTC_CTRL_INTE_S                 29
/// Бит разрешения прерывания на выходах irq и irq_async. Прерывания появляются при наличии установленного бита ALRM
#define RTC_CTRL_INTE_M                 (1 << RTC_CTRL_INTE_S)

#define RTC_CTRL_FLAG_S                 28
/// Сигнал проведения синхронизации между тактовыми доменами. После записи в любой регистр и пока данный бит читается равным «1», запрещено выполнять любую новую запись
#define RTC_CTRL_FLAG_M                 (1 << RTC_CTRL_FLAG_S)

#define RTC_CTRL_RESET_STROBE_S         27
/// Запись “1” – сброс признака установки активного уровня ALARM_PAD
#define RTC_CTRL_RESET_STROBE_M         (1 << RTC_CTRL_RESET_STROBE_S)

/**
 * Регистр времени будильника RRTC_TALRM. Регистр хранит время, при совпадении которого 
 * со значением регистра RRTC_TIME, будет сгенерировано соответствующее прерывание. 
 * Структура регистра полностью идентична регистру RRTC_TIME за исключением полей управления.
 */

#define RTC_TALRM_CDOW_S                31
/// Разрешает сравнения дней недели, когда установле
#define RTC_TALRM_CDOW_M                (1 << RTC_TALRM_CDOW_S)

#define RTC_TALRM_CH_S                  30
/// Разрешает сравнения часов, когда установлен
#define RTC_TALRM_CH_M                  (1 << RTC_TALRM_CH_S)

#define RTC_TALRM_CM_S                  29
/// Разрешает сравнения минут, когда установле
#define RTC_TALRM_CM_M                  (1 << RTC_TALRM_CM_S)

#define RTC_TALRM_CS_S                  28
/// Разрешает сравнения секунд, когда установле
#define RTC_TALRM_CS_M                  (1 << RTC_TALRM_CS_S)

#define RTC_TALRM_CTOS_S                27
/// Резерв
#define RTC_TALRM_CTOS_M                (1 << RTC_TALRM_CTOS_S)

/**
 * Регистр даты будильника RRTC_DALRM. Регистр хранит дату, при совпадении которой 
 * со значением регистра RRTC_DATE, будет сгенерировано соответствующее прерывание. 
 * Структура регистра полностью идентична регистру RRTC_DATE за исключением полей управления.
 */

#define RTC_DALRM_CC_S                  30
/// Разрешает сравнения веков, когда установлен
#define RTC_DALRM_CC_M                  (1 << RTC_DALRM_CC_S)

#define RTC_DALRM_CY_S                  29
/// Разрешает сравнения годов, когда установлен
#define RTC_DALRM_CY_M                  (1 << RTC_DALRM_CY_S)

#define RTC_DALRM_CM_S                  28
/// Разрешает сравнения месяцев, когда установлен
#define RTC_DALRM_CM_M                  (1 << RTC_DALRM_CM_S)

#define RTC_DALRM_CD_S                  27
/// Разрешает сравнения дней, когда установлен
#define RTC_DALRM_CD_M                  (1 << RTC_DALRM_CD_S)

//  Поля регистра RRTC_TIME

#define RTC_TIME_DOW_S                  24
/**
 * День недели. Допустимые значения в двоичном виде:
 * «001» – понедельник;
 * «010» – вторник;
 * «011» – среда;
 * «100» – четверг;
 * «101» – пятница;
 * «110» – суббота;
 * «111» – воскресенье
 */
#define RTC_TIME_DOW_M                  (0x7 << RTC_TIME_DOW_S)

#define RTC_TIME_TH_S                   22
/// Поле десятков часов. Доступные значения от 0 до 2
#define RTC_TIME_TH_M                   (0x3 << RTC_TIME_TH_S)

#define RTC_TIME_H_S                    18
/**
 * Поле единиц часов. Допустимые значения:
 * - TH = 0 – От 0 до 9;
 * - TH = 2 – От 0 до 3
 */
#define RTC_TIME_H_M                    (0xF << RTC_TIME_H_S)

#define RTC_TIME_TM_S                   15
/// Поле десятков минут. Допустимые значения от 0 до 5
#define RTC_TIME_TM_M                   (0x7 << RTC_TIME_TM_S)

#define RTC_TIME_M_S                    11
/// Поле единиц минут. Допустимые значения от 0 до 9
#define RTC_TIME_M_M                    (0xF << RTC_TIME_M_S)

#define RTC_TIME_TS_S                   8
/// Поле десятков секунд. Допустимые значения от 0 до 5 
#define RTC_TIME_TS_M                   (0x7 << RTC_TIME_TS_S)

#define RTC_TIME_S_S                    4
/// Поле единиц секунд. Допустимые значения от 0 до 9
#define RTC_TIME_S_M                    (0xF << RTC_TIME_S_S)

#define RTC_TIME_TOS_S                  0
/// Резерв
#define RTC_TIME_TOS_M                  (0xF << RTC_TIME_TOS_S)

//  Поля регистра RRTC_DATE

#define RTC_DATE_TC_S                   23
/// Поле десятков веков. Допустимые значения от 0 до 9 
#define RTC_DATE_TC_M                   (0xF << RTC_DATE_TC_S)

#define RTC_DATE_C_S                    19
/// Поле единиц веков. Допустимые значения от 0 до 9
#define RTC_DATE_C_M                    (0xF << RTC_DATE_C_S)

#define RTC_DATE_TY_S                   15
/// Поле десятков годов. Допустимые значения от 0 до 9
#define RTC_DATE_TY_M                   (0xF << RTC_DATE_TY_S)

#define RTC_DATE_Y_S                    11
/// Поле единиц годов. Допустимые значения от 0 до 9
#define RTC_DATE_Y_M                    (0xF << RTC_DATE_Y_S)

#define RTC_DATE_TM_S                   10
/// Поле десятков месяцев. Допустимые значения от 0 до 1
#define RTC_DATE_TM_M                   (0x1 << RTC_DATE_TM_S)

#define RTC_DATE_M_S                    6
/**
 * Поле единиц месяцев. Допустимые значения:
 * - TM = 0 – от 0 до 9;
 * - TM = 1 – от 0 до 2
 */
#define RTC_DATE_M_M                    (0xF << RTC_DATE_M_S)

#define RTC_DATE_TD_S                   4
/**
 * Поле десятков дней. Допустимые значения:
 * - {TM,M} != 03 – от 0 до 3;
 * - {TM,M} == 03 – от 0 до 2
 */
#define RTC_DATE_TD_M                   (0x3 << RTC_DATE_TD_S)

#define RTC_DATE_D_S                    0
/**
 * Поле единиц дней. Допустимые значения: 
 * - TD = 2 – от 0 до 3;
 * - TD = 3 – от 0 до 1
 */
#define RTC_DATE_D_M                    (0xF << RTC_DATE_D_S)

/**
 * Регистры общего назначения RRTC_REG0 – RRTC_REG15.
 * В адресном пространстве модуля доступно 16 32-разрядных регистра. Они могут использоваться в произвольных целях. 
 * Начальное значения регистров при включении питания 0х00000000. Регистры располагаются в батарейном домене 
 * и сохраняют свое значение при отключении системного питания и при внешнем сбросе.
 */

#ifndef __ASSEMBLER__
    #include <inttypes.h> 
    /// @brief Структура регистров модуля часов реального времени
    typedef struct
    {   
        union
        {
            //FIXME: Структура не выровнена до 32-бит! Убрать объединение регистра с битовой структурой
            struct {
                volatile uint32_t TOS:4;            ///< Резерв
                volatile uint32_t S:4;              ///< Поле единиц секунд. 
                volatile uint32_t TS:3;             ///< Поле десятков секунд.
                volatile uint32_t M:4;              ///< Поле единиц минут. 
                volatile uint32_t TM:3;             ///< Поле десятков минут. 
                volatile uint32_t H:4;              ///< Поле единиц часов. 
                volatile uint32_t TH:2;             ///< Поле десятков часов. 
                volatile uint32_t DOW:3;            ///< День недели. 
            };
            uint32_t TIME;                          ///< Регистр времени
        };
        
        volatile uint32_t DATE;                     ///< Регистр даты
        volatile uint32_t TALRM;                    ///< Регистр времени будильника
        volatile uint32_t DALRM;                    ///< Регистр даты будильника
        volatile uint32_t CTRL;                     ///< Регистр управления
        volatile uint32_t _space[3];                ///< Резерв 
        volatile uint32_t REG[16];                  ///< 16-ть регистров общего назначения
    } RTC_TypeDef;
#endif

#endif 

