/**
 * @file
 * Контроллер EEPROM
 * 
 * Данный контроллер предназначен для осуществления доступа к массиву блока EEPROM 
 * на запись и чтение со стороны интерфейсов AHB-Lite (только чтение) и APB (чтение и запись в несколько этапов). 
 * Данный контроллер является совместимым с Hard IP EEPROM объемом 8 Кбайт с организацией (32+6)x2k, разработанным 
 * для серийной технологии CMOSF8_4M_5V ПАО Микрон (F8 EEPROM High Density 8 Kbytes (32+6)x2k Macro (core)) 
 * и требуемым для него блоком формирования напряжений и токов (Common Supply Block for F8 EEPROM High-Density macros(supply)). 
 * Иерархически блоки Hard IP core и supply помещаются внутрь данного Soft IP. 
 * Имя блока верхнего уровня, содержащего ee_ctrl и подключенный к нему Hard IP, - EEPROM_8K_BLK.
 */

#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

#define EEPROM_EEDAT_OFFSET            0x00
#define EEPROM_EEA_OFFSET              0x04
#define EEPROM_EECON_OFFSET            0x08
#define EEPROM_EESTA_OFFSET            0x0C
#define EEPROM_EERB_OFFSET             0x10
#define EEPROM_EEADJ_OFFSET            0x14
#define EEPROM_NCYCRL_OFFSET           0x18
#define EEPROM_NCYCEP1_OFFSET          0x1C
#define EEPROM_NCYCEP2_OFFSET          0x20


/**
 * Регистр чтения записи данных (EEDAT)
 * 
 * В процессе чтения/записи данного регистра производится автоматическое 
 * инкриминирование адреса в регистре адреса страницы (EEA).
 */

/**
 * Регистр адреса страницы (EEA)
 * 
 *  Адрес слова для выполнения стирания/программирования/чтения. 
 * Если необходимо выполнить стирание или программирование, 
 * данный адрес должен использоваться в процедуре заполнения буфера записи
*/

#define EEPROM_EEA_ADDR_S               2
#define EEPROM_EEA_ADDR_M               (0b7FF << EEPROM_EEA_ADDR_S)
/// Установить или получить данные по адресу v
#define EEPROM_EEA_ADDR(v)              (((v) << EEPROM_EEA_ADDR_S) & EEPROM_EEA_ADDR_M)

// Перечень значений регистра управления (EECON)

#define EEPROM_EECON_EX_S               0
/// Запуск процедуры. Запись 1 в данный бит инициирует выполнение процедуры, процедура определяется битами OP
#define EEPROM_EECON_EX_M               (1 << EEPROM_EECON_EX_S)

#define EEPROM_EECON_OP_S               1
#define EEPROM_EECON_OP_M               (0b11 << EEPROM_EECON_OP_S)
/** Выбор процедуры, где v: 
 * 0b00 - чтение страницы;
 * 0b01 - стирание;
 * 0b10 - программирование;
 * 0b11 - значение не используется.
 */
#define EEPROM_EECON_OP(v)              (((v) << EEPROM_EECON_OP_S) & EEPROM_EECON_OP_M)

#define EEPROM_EECON_WRBEH_S            3
#define EEPROM_EECON_WRBEH_M            (0b11 << EEPROM_EECON_WRBEH_S)
/**
 * Проведение операции стирания/программирования, где v: 
 * 0b00 - обычное стирание/программирования одной страницы; 
 * 0b01 - стирание/программирования всех четных страниц; 
 * 0b10 - стирание/программирования всех нечетных страниц; 
 * 0b11 - стирание/программирования всех страниц. 
 * Данные биты должны быть установлены перед операцией заполнения буфера записи перед операциями стирания и программирования.
 */
#define EEPROM_EECON_WRBEH(v)           (((v) << EEPROM_EECON_WRBEH_S) & EEPROM_EECON_WRBEH_M)

#define EEPROM_EECON_APBNWS_S           5
/** 
 * Отключение вставки тактов ожидания в процессе обмена по APB 
 * при заполнении буфера записи (во время записи в EEDAT) 
 * и при записи адреса для процедуры чтения данных (EEA). 
 * Если такты ожидания отключены (APBNWS=1), 
 * то требуется производить опрос флага EESTA.BSY после этих операций 
 * до тех пор, пока EESTA.BSY не станет равным 0
*/
#define EEPROM_EECON_APBNWS_M           (1 << EEPROM_EECON_APBNWS_S)

#define EEPROM_EECON_DISECC_S           6
/**
 * Отключение схемы коррекции ошибок во время выполнения запрошенной процедуры. 
 * Отключение производится записью «1» в этот бит. Если схема коррекции отключена, 
 * то при записи значение бит коррекции определяется шестью младшими битами слова. 
 * Если схема коррекции включена, то при записи значение бит коррекции вычисляется 
 * схемой SEC32 - ENC. Если схема коррекции отключена, то при чтении не производится 
 * коррекция возможных ошибок в слове. Если схема коррекции включена, 
 * то при чтении данные корректируются схемой DEC32_ENC
 */
#define EEPROM_EECON_DISECC_M           (1 << EEPROM_EECON_DISECC_S)

#define EEPROM_EECON_BWE_S              7
/**
 * Разрешение записи в буфер. Данный бит следует устанавливать в «1» перед операцией 
 * заполнения буфера записи перед операциями стирания и программирования. После операций 
 * стирания и программирования данный бит автоматически очищается (то есть для последующей 
 * записи в буфер следует повторно выполнить запись «1» в этот бит)
 */
#define EEPROM_EECON_BWE_M              (1 << EEPROM_EECON_BWE_S)

#define EEPROM_EECON_IESERR_S           8
/**
 * Разрешение запроса прерывания при поднятии флага SERR:
 * 0 - запрос не поступает;
 * 1 - запрос поступает
 */
#define EEPROM_EECON_IESERR_M           (1 << EEPROM_EECON_IESERR_S)

// Перечень значений регистр статуса (EESTA)

#define EEPROM_EESTA_BSY_S              0
/// Блок занят (выполняется запрошенная операция)
#define EEPROM_EESTA_BSY_M              (1 << EEPROM_EESTA_BSY_S)

#define EEPROM_EESTA_SERR_S             1
/**
 * Индикатор одиночной (исправимой) ошибки. Аппаратно может быть только поднят, 
 * т.е. если после слова с ошибкой считано слово без ошибки, флаг не очистится. 
 * Может быть очищен программно путем записи 0
 */
#define EEPROM_EESTA_SERR_M             (1 << EEPROM_EESTA_SERR_S)

// Перечень значений регистра бит коррекции прочитанного слова (EERB)

#define EEPROM_EERB_CORRECT_S           0
/// Маска для значений для 6 бит коррекции последнего считанного слова
#define EEPROM_EERB_CORRECT_M           (0b111111 << EEPROM_EERB_CORRECT_S)

/** Перечень значений регистра настроек блоков Hard IP (EEADJ) 
 * Все поля, кроме StopEe, предназначены для тонкой настройки блока и выполнения процедуры тестирования. 
 * Не гарантируется работа блока при использовании не рекомендованных производителем значений.
 */

//BUG: все смещения для масок равны нулю!!!

#define EEPROM_EEADJ_OSCX2_S            0
/// Увеличение частоты тактового сигнала, поступающего на устройство с зарядовой накачкой, в два раза
#define EEPROM_EEADJ_OSCX2_M            (1 << EEPROM_EEADJ_OSCX2_S)

//BUG: #define EEPROM_EEADJ_HIVCTL_S           0
#define EEPROM_EEADJ_HIVCTL_S           1
/// Подстройка уровня внутреннего сигнала HIV
#define EEPROM_EEADJ_HIVCTL_M           (0b111 << EEPROM_EEADJ_HIVCTL_S)

//BUG: #define EEPROM_EEADJ_PLUSIMIX_S         0
#define EEPROM_EEADJ_PLUSIMIX_S         4
/// Сигнал управления блоком мониторинга аналоговых сигналов
#define EEPROM_EEADJ_PLUSIMIX_M         (1 << EEPROM_EEADJ_PLUSIMIX_S)

//BUG: #define EEPROM_EEADJ_VBOOSTCTL_S        0
#define EEPROM_EEADJ_VBOOSTCTL_S        5
/// Старший разряд - подстройка внутреннего сигнала Vboost, младший разряд - сигнал управления блоком мониторинга аналоговых сигнал
#define EEPROM_EEADJ_VBOOSTCTL_M        (0b11 << EEPROM_EEADJ_VBOOSTCTL_S)

//BUG: #define EEPROM_EEADJ_MINUSIMIX_S        0
#define EEPROM_EEADJ_MINUSIMIX_S        7
/// Сигнал управления блоком мониторинга аналоговых сигналов
#define EEPROM_EEADJ_MINUSIMIX_M        (1 << EEPROM_EEADJ_MINUSIMIX_S)

//BUG: #define EEPROM_EEADJ_DUMMYCTL_S         0
#define EEPROM_EEADJ_DUMMYCTL_S         8
/// Подстройка момента захвата выдаваемых данных
#define EEPROM_EEADJ_DUMMYCTL_M         (0b11 << EEPROM_EEADJ_DUMMYCTL_S)

//BUG: #define EEPROM_EEADJ_VBGCTL_S           0
#define EEPROM_EEADJ_VBGCTL_S           10
/// Подстройка опорного напряжения bandgap 
#define EEPROM_EEADJ_VBGCTL_M           (0b111 << EEPROM_EEADJ_VBGCTL_S)

//BUG: #define EEPROM_EEADJ_REFCTL_S           0
#define EEPROM_EEADJ_REFCTL_S           13
/// Подстройка тока, используемого при чтении
#define EEPROM_EEADJ_REFCTL_M           (0b111 << EEPROM_EEADJ_REFCTL_S)

//BUG: #define EEPROM_EEADJ_VCGCTL_S           0
#define EEPROM_EEADJ_VCGCTL_S           16
/// Подстройка напряжения, подаваемого на управляющие затворы при чтении
#define EEPROM_EEADJ_VCGCTL_M           (0b111 << EEPROM_EEADJ_VCGCTL_S)

//BUG: #define EEPROM_EEADJ_CGSTRICT_S         0
#define EEPROM_EEADJ_CGSTRICT_S         19
/// Включение режима точной подстройки напряжения, используемого при чтении
#define EEPROM_EEADJ_CGSTRICT_M         (1 << EEPROM_EEADJ_CGSTRICT_S)

//BUG: #define EEPROM_EEADJ_VPPCTL_S           0
#define EEPROM_EEADJ_VPPCTL_S           20
/// Подстройка времени нарастания высокого напряжени
#define EEPROM_EEADJ_VPPCTL_M           (0b11 << EEPROM_EEADJ_VPPCTL_S)

//BUG: пропущено значение STRICTCTL (добавил ниже)
#define EEPROM_EEADJ_STRICTCTL_S        22
// Точная подстройка тока, используемого при чтении
#define EEPROM_EEADJ_STRICTCTL_M        (0b111 << EEPROM_EEADJ_STRICTCTL_S)

//BUG: #define EEPROM_EEADJ_STOPEE_S           0
#define EEPROM_EEADJ_STOPEE_S           25
/// Перевод Hard IP в режим пониженного энергопотребления
#define EEPROM_EEADJ_STOPEE_M           (1 << EEPROM_EEADJ_STOPEE_S)

//BUG: #define EEPROM_EEADJ_IDDQEN_S           0
#define EEPROM_EEADJ_IDDQEN_S           26
/// Перевод Hard IP в состояние бездействия (quiescent) для Iddq тестирования
#define EEPROM_EEADJ_IDDQEN_M           (1 << EEPROM_EEADJ_IDDQEN_S)

// Регистр подстройки длительности процедур чтения и заполнения буфера записи NCYCRLNCYCRL

#define EEPROM_NCYCRL_N_LD_S            0
/**
 * Количество тактов между передними и задними фронтами сигналов Hard IP Phi1Ee 
 * и Phi1ShiftEe и задержка от начала обращения по шине до первого фронта Phi1Ee. 
 * Рекомендуемое значение при любых частотах clk 1 такт. 
 * @warning Запрещено устанавливать равным 0
 */
#define EEPROM_NCYCRL_N_LD_M            (0xFF << EEPROM_NCYCRL_N_LD_S)

#define EEPROM_NCYCRL_N_R_1_S           8
/**
 * Количество тактов, в течение которого Phi1ShiftEe имеет высокий уровень в процедуре чтения. 
 * Рекомендуемое значение N RA = ceil(51/Pclk), где Pclk – период тактового сигнала в нс, 
 * ceil – функция округления до ближайшего большего целого числа. 
 * @warning Запрещено устанавливать равным 0
 */
#define EEPROM_NCYCRL_N_R_1_M           (0xFF << EEPROM_NCYCRL_N_R_1_S)

#define EEPROM_NCYCRL_N_R_2_S           16
/**
 * Количество тактов, требуемое для завершения процедуры чтения 
 * (перевода OkSelEe на высокий уровень после процедуры чтения). 
 * Рекомендуемое значение при любых частотах clk 1 такт. 
 * @warning Запрещено устанавливать равным 0
 */
#define EEPROM_NCYCRL_N_R_2_M           (0xFF << EEPROM_NCYCRL_N_R_2_S)

// Регистр 1 подстройки длительности процедур стирания и программирования NCYCEP1

#define EEPROM_NCYCEP1_N_EP_1_S         0
/**
 * Длительность высокого уровня сигнала BUSY (вход Hard IP) в тактах. 
 * Рекомендуемое значение N RA = 2000000 нс / Pclk, где Pclk – период 
 * тактового сигнала в нс. Пример – 66667 тактов для частоты Fclk = 33,3 МГц. 
 * @warning Запрещено устанавливать равным 0
 */
#define EEPROM_NCYCEP1_N_EP_1_M         (0x3FFFF << EEPROM_NCYCEP1_N_EP_1_S)

// Регистр 2 подстройки длительности процедур стирания и программирования NCYCEP2

#define EEPROM_NCYCEP2_N_EP_2_S         0
/**
 * Задержка между задними фронтами BUSY и HvonValid (входы Hard IP) в тактах. 
 * Рекомендуемое значение N RA = 15000 нс / Pclk, где Pclk – период тактового сигнала в нс.
 * @note Пример – 500 тактов для частоты Fclk = 33,3 МГц. 
 * @warning Запрещено устанавливать равным 0
 */
#define EEPROM_NCYCEP2_N_EP_2_M         (0x7FF << EEPROM_NCYCEP2_N_EP_2_S)

//--------------------------
// EEPROM codes
//--------------------------

#define EEPROM_EECON_OP_RD      0
#define EEPROM_EECON_OP_ER      1
#define EEPROM_EECON_OP_PR      2
#define EEPROM_WRBEH_SINGLE     0
#define EEPROM_WRBEH_EVEN       1
#define EEPROM_WRBEH_ODD        2
#define EEPROM_WRBEH_GLOB       3


#ifndef __ASSEMBLER__
    #include <inttypes.h>
    /// @brief Структура регистров контроллера EEPROM
    typedef struct
    {
        volatile uint32_t EEDAT;            ///< [RW]: Регистр чтения/записи данных. Биты [31:0].
        volatile uint32_t EEA;              ///< [WO]: Регистр адреса страницы. Биты [12:2].
        volatile uint32_t EECON;            ///< [WO]: Регистр управления. Биты [8:0].
        volatile uint32_t EESTA;            ///< [WO]: Регистр статуса. Биты [1:0].
        volatile uint32_t EERB;             ///< [RO]: Регистр бит коррекции прочитанного слова. Биты [5:0].
        volatile uint32_t EEADJ;            ///< [RW]: Регистр настроек. Биты [26:0].
        volatile uint32_t NCYCRL;           ///< [RW]: Регистр подстройки длительности процедур чтения и заполнения буфера записи. Биты [23:0].
        volatile uint32_t NCYCEP1;          ///< [RW]: Регистр 1 подстройки длительности процедур стирания и программирования. Биты [17:0].
        volatile uint32_t NCYCEP2;          ///< [RW]: Регистр 2 подстройки длительности процедур стирания и программирования. Биты [10:0].
    } EEPROM_REGS_TypeDef;
#endif


#endif // EEPROM_H_INCLUDED

