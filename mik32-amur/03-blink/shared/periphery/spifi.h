/**
 * @file
 * Контроллер SPIFI
 * 
 * Контроллер интерфейса флэш-памяти SPIFI – это цифровой автомат, предназначенный 
 * для информационного сопряжения процессора системы на кристалле (CPU SOC) 
 * с микросхемами флэш-памяти (МФП) ныне существующих и перспективных типов. 
 * Данный контроллер избавляет процессор от выполнения рутинных операций 
 * при работе с флэш-памятью (при выполнении операций чтения, записи и стирания памяти).
 * В начале работы процессор настраивает контроллер SPIFI, а в процессе работы процессор 
 * обменивается информацией с флэш-памятью по параллельной скоростной синхронной шине AHB. 
 * Контроллер может работать в двух режимах: 
 * - режим работы с периферией; 
 * - режим работы с памятью.
 * Контроллер SPIFI также имеет режим загрузки «boot mode» (разновидность режима работы с памятью), 
 * в котором начальная загрузка SOC может быть осуществлена из SPI флэш-памяти. 
 * В этом случае отпадает необходимость грузиться с параллельной загрузочной памяти 
 * с большим количеством выводов (что плохо для габаритов и надежности). 
 * Контроллер SPIFI может работать с флэш-памятью как в обычном последовательном режиме, 
 * когда данные передаются/принимаются по двум однонаправленным линиям (MISO/MOSI), 
 * так и в параллельных режимах (двух и четырехбитном), когда данные передаются/принимаются 
 * по двум или четырем двунаправленным линиям.
 */

#ifndef SPIFI_H_INCLUDED
#define SPIFI_H_INCLUDED

#define SPIFI_CONFIG_CTRL_OFFSET            0x00
#define SPIFI_CONFIG_CMD_OFFSET             0x04
#define SPIFI_CONFIG_ADDR_OFFSET            0x08
#define SPIFI_CONFIG_IDATA_OFFSET           0x0C
#define SPIFI_CONFIG_CLIMIT_OFFSET          0x10
#define SPIFI_CONFIG_DATA_OFFSET            0x14
#define SPIFI_CONFIG_MCMD_OFFSET            0x18
#define SPIFI_CONFIG_STAT_OFFSET            0x1C


/**
 * Регистр управления CTRL
 * Данный регистр управляет выполнением всех операций контроллера и должен быть настроен перед инициализацией любой команды. 
 */

#define SPIFI_CONFIG_CTRL_TIMEOUT_S				  0
#define SPIFI_CONFIG_CTRL_TIMEOUT_M         (0xFFFF << SPIFI_CONFIG_CTRL_TIMEOUT_S)
/** 
 * Количество периодов сигнала SPIFI_SCK без чтения данных в режиме работы с памятью, 
 * которое вызывает завершение выполнения команды установкой сигнала SPIFI_CS 
 * в состояние «1» и сбросом бита CMD
*/
#define SPIFI_CONFIG_CTRL_TIMEOUT(v)			  (((v) << SPIFI_CONFIG_CTRL_TIMEOUT_S) & SPIFI_CONFIG_CTRL_TIMEOUT_M)

#define SPIFI_CONFIG_CTRL_CSHIGH_S				  16
#define SPIFI_CONFIG_CTRL_CSHIGH_M          (0xF << SPIFI_CONFIG_CTRL_CSHIGH_S)
/**
 * Количество периодов сигнала SPIFI_SCK минус один, в течение которых 
 * сигнал SPIFI_CS остается в неактивном состоянии перед началом выполнения команды
 */
#define SPIFI_CONFIG_CTRL_CSHIGH(v)				  (((v) << SPIFI_CONFIG_CTRL_CSHIGH_S) & SPIFI_CONFIG_CTRL_CSHIGH_M)

#define SPIFI_CONFIG_CTRL_CACHE_EN_S        20
/// Бит разрешения кэширования
#define SPIFI_CONFIG_CTRL_CACHE_EN_M        (0x1 << SPIFI_CONFIG_CTRL_CACHE_EN_S)

#define SPIFI_CONFIG_CTRL_D_CACHE_DIS_S		  21
/**
 * Бит запрещения кеширования данных (транзакций AHB, для которых сигнал HPROT[0] = 1):
 * - «0» – при разрешенном кэшировании кэширование данных выполняется;
 * - «1» – данные не кэшируются.
 */
#define SPIFI_CONFIG_CTRL_D_CACHE_DIS_M		  (0x1 << SPIFI_CONFIG_CTRL_D_CACHE_DIS_S)

#define SPIFI_CONFIG_CTRL_INTEN_S				    22
/// Бит разрешения прерывания при завершении выполнения команды (если этот бит равен «1», то прерывание разрешено)
#define SPIFI_CONFIG_CTRL_INTEN_M				    (0x1 << SPIFI_CONFIG_CTRL_INTEN_S)

#define SPIFI_CONFIG_CTRL_MODE3_S				    23
/// Бит режима 3
#define SPIFI_CONFIG_CTRL_MODE3_M				    (0x1 << SPIFI_CONFIG_CTRL_MODE3_S)

#define SPIFI_CONFIG_CTRL_SCK_DIV_S				  24
#define SPIFI_CONFIG_CTRL_SCK_DIV_M         (0x7 << SPIFI_CONFIG_CTRL_SCK_DIV_S)
/**
 * Делитель для получения тактового сигнала SPIFI_SCK из системного тактового сигнала HCLK. 
 * Частота рассчитывается по формуле: F_SPIFI_SCK = F_HCLK / 2^(SCK_DIV+1)
 */
#define SPIFI_CONFIG_CTRL_SCK_DIV(v)			  (((v) << SPIFI_CONFIG_CTRL_SCK_DIV_S) & SPIFI_CONFIG_CTRL_SCK_DIV_M)

#define SPIFI_CONFIG_CTRL_PREFETCH_DIS_S		27
/// Бит разрешения упреждающих выборок кэш памяти «0» – выборки разрешены
#define SPIFI_CONFIG_CTRL_PREFETCH_DIS_M		(0x1 << SPIFI_CONFIG_CTRL_PREFETCH_DIS_S)

#define SPIFI_CONFIG_CTRL_DUAL_S				    28
/** 
 * Бит выбора протокола: 
 * - сигналы IO[3:0] – (0–4) битовый протокол; 
 * - сигналы IO[1:0] – (1–2) битовый протокол.
*/
#define SPIFI_CONFIG_CTRL_DUAL_M				    (0x1 << SPIFI_CONFIG_CTRL_DUAL_S)

#define SPIFI_CONFIG_CTRL_RFCLK_S				    29
/**
 * Бит выбора активного перепада сигнала для стробирования входных данных:
 * «0» – положительный;
 * «1» – отрицательный.
 */
#define SPIFI_CONFIG_CTRL_RFCLK_M				    (0x1 << SPIFI_CONFIG_CTRL_RFCLK_S)

#define SPIFI_CONFIG_CTRL_FBCLK_S				    30
/**
 * Бит выбора сигнала стробирования входных данных с выходного буфера тактового сигнала:
 * «0» – внутренний сигнал;
 * «1» – внешний сигнал.
 */
#define SPIFI_CONFIG_CTRL_FBCLK_M				    (0x1 << SPIFI_CONFIG_CTRL_FBCLK_S)

#define SPIFI_CONFIG_CTRL_DMAEN_S				    31
/**
 * Бит разрешения запросов DMA от SPIFI контроллера:
 * «1» – запросы разрешены;
 * «0» - запросы запрещены.
 *  * @warning Устанавливать только в периферийном режиме
 */
#define SPIFI_CONFIG_CTRL_DMAEN_M				    (0x1 << SPIFI_CONFIG_CTRL_DMAEN_S)

/**
 * Регистр команд CMD
 * Регистр команд CMD определяет последовательность действий контроллера 
 * при выполнении команд периферийного режима. Этот регистр может быть записан 
 * только тогда, когда биты CMD и MCINIT равны нулю. 
 */

#define SPIFI_CONFIG_CMD_DATALEN_S			        0
#define SPIFI_CONFIG_CMD_DATALEN_M              (0x3FFF << SPIFI_CONFIG_CMD_DATALEN_S)
/**
 * В тех случаях, когда бит POLL равен «0», это поле определяет количество байт данных 
 * при выполнении команды. Если это поле равно «0», то выполняемая команда не содержит данных
 */
#define SPIFI_CONFIG_CMD_DATALEN(v)			        (((v) << SPIFI_CONFIG_CMD_DATALEN_S) & SPIFI_CONFIG_CMD_DATALEN_M)

//TODO проверить откуда эти значения. Недокументированны. Пересекаются с областью DATALEN [13:0]
#define SPIFI_CONFIG_CMD_POLL_INDEX_S		        0 
#define SPIFI_CONFIG_CMD_POLL_INDEX_M		        (0b111 << SPIFI_CONFIG_CMD_POLL_INDEX_S) 
#define SPIFI_CONFIG_CMD_POLL_INDEX(v)          (((v) << SPIFI_CONFIG_CMD_POLL_INDEX_S) & SPIFI_CONFIG_CMD_POLL_INDEX_M)

#define SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE_S  3 
#define SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE_M	(1 << SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE_S) 
#define SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE(v)	(((v) << SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE_S) & SPIFI_CONFIG_CMD_POLL_REQUIRED_VALUE_M)

#define SPIFI_CONFIG_CMD_POLL_S				          14
/**
 * Бит должен быть установлен при выполнении команды, которая содержит входное поле данных 
 * и циклически запрашивает состояние бита входного потока битов из регистра статуса флэш-памяти. 
 * Номер проверяемого бита в байте статуса должен быть указан в битах DATALEN[2:0], 
 * а требуемое значение бита – в бите DATALEN[3]. Как только значения этих битов станут равны, 
 * контроллер завершает выполнение текущей команды, деактивируя сигнал SPIFI_CS, 
 * и формирует прерывание, если оно разрешено. После завершения процесса поллинга 
 * необходимо считать один байт из регистра DATA, так как в нем сохраняется последний принятый статус FLASH
 */
#define SPIFI_CONFIG_CMD_POLL_M				          (0x1 << SPIFI_CONFIG_CMD_POLL_S)

#define SPIFI_CONFIG_CMD_DOUT_S				          15
/// Бит направления передачи данных «0» – чтение из флэш
#define SPIFI_CONFIG_CMD_DOUT_M				          (0x1 << SPIFI_CONFIG_CMD_DOUT_S)

#define SPIFI_CONFIG_CMD_INTLEN_S			          16
#define SPIFI_CONFIG_CMD_INTLEN_M			          (0x7 << SPIFI_CONFIG_CMD_INTLEN_S)
/**
 * Количество байт промежуточных данных, которые хранятся в регистре IDATA. 
 * Если количество байт больше, чем может хранить регистр IDATA, 
 * то остаток добивается нулевыми значениями
 */
#define SPIFI_CONFIG_CMD_INTLEN(v)			        (((v) << SPIFI_CONFIG_CMD_INTLEN_S) & SPIFI_CONFIG_CMD_INTLEN_M)

#define SPIFI_CONFIG_CMD_FIELDFORM_S		        19
#define SPIFI_CONFIG_CMD_FIELDFORM_M		        (0x3 << SPIFI_CONFIG_CMD_FIELDFORM_S)
/**
 * Формат вывода полей команды:
 * «0» – все поля выводятся в последовательном режиме;
 * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
 * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
 * «3» – все поля в четырех или двух битовом режиме
 */
#define SPIFI_CONFIG_CMD_FIELDFORM(v)			      (((v) << SPIFI_CONFIG_CMD_FIELDFORM_S) & SPIFI_CONFIG_CMD_FIELDFORM_M)

#define SPIFI_CONFIG_CMD_FRAMEFORM_S		        21
#define SPIFI_CONFIG_CMD_FRAMEFORM_M		        (0x7 << SPIFI_CONFIG_CMD_FRAMEFORM_S)
/**
 * Бит управления полями кода операции и адреса команды:
 * «0» – резерв;
 * «1» – выдается только код операции, адреса нет;
 * «2» – код операции и младший байт адреса;
 * «3» – код операции и два младших байта адреса;
 * «4» – код операции и три младших байта адреса;
 * «5» – код операции и 4 байта адреса;
 * «6» – нет кода операции, три младших байта адреса;
 * «7» – нет кода операции, 4 байта адреса
 */
#define SPIFI_CONFIG_CMD_FRAMEFORM(v)			      (((v) << SPIFI_CONFIG_CMD_FRAMEFORM_S) & SPIFI_CONFIG_CMD_FRAMEFORM_M)

#define SPIFI_CONFIG_CMD_OPCODE_S			          24
#define SPIFI_CONFIG_CMD_OPCODE_M			          (0xFF << SPIFI_CONFIG_CMD_OPCODE_S)
/// Код операции (не используется для некоторых значений поля FRAMEFORM)
#define SPIFI_CONFIG_CMD_OPCODE(v)              (((v) << SPIFI_CONFIG_CMD_OPCODE_S) & SPIFI_CONFIG_CMD_OPCODE_M)

#define SPIFI_CONFIG_CMD_FRAMEFORM_RESERVED			      0
#define SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR	    1
#define SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_1ADDR		    2
#define SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_2ADDR		    3
#define SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR		    4	
#define SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_4ADDR		    5
#define SPIFI_CONFIG_CMD_FRAMEFORM_NOOPCODE_3ADDR	    6
#define SPIFI_CONFIG_CMD_FRAMEFORM_NOOPCODE_4ADDR	    7

#define SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL		      0
#define SPIFI_CONFIG_CMD_FIELDFORM_DATA_PARALLEL	    1
#define SPIFI_CONFIG_CMD_FIELDFORM_OPCODE_SERIAL	    2
#define SPIFI_CONFIG_CMD_FIELDFORM_ALL_PARALLEL		    3

/**
 * @warning Регистр адреса (ADDR) должен быть настроен перед инициализацией любой команды. 
 * При выполнении команды первым выводится старший бит адреса.
 */

/**
 * @warning Регистр промежуточных данных (IDATA) должен быть настроен перед инициализацией команды, 
 * в которой используются промежуточные данные. При выполнении команды первым выводится младший бит слова.
 */

/**
 * Регистр верхней границы адреса кэширования (CLIMIT). Контроллер SPIFI имеет возможность кэширования 
 * полученных из флэш-памяти данных в целях увеличения производительности. В регистре CLIMIT хранится
 * значение верхней границы кэшируемой памяти. 
 * @warning Ячейки памяти с адресами большими, чем значение CLIMIT, не являются кэшируемыми.
 */

/**
 * Регистр данных (DATA). По адресу данного регистра подключены FIFO буфера данных, которые с другой 
 * стороны подключены к шине AHB. Поэтому при обращении по шине AHB к данному регистру данные 
 * читаются из или пишутся в соответствующий FIFO буфер. Если выходной буфер чтения пуст 
 * или входной буфер записи полон, то при отправке запроса по шине AHB будет вызвано исключение 
 * (код 5 “Load access fault”). Всего в регистр DATA записывается или читается из него количество байт, 
 * указанное в поле DATALEN регистра CMD. Если включен режим поллинга (бит POLL регистра CMD активен), 
 * то данные в выходной буфер не записываются, а первый байт входного буфера используется для хранения 
 * статуса FLASH. После завершения поллинга необходимо считать один байт из регистра DATA.
 */

/**
 * Регистр команд памяти (MCMD)/ Регистр определяет последовательность действий контроллера 
 * при выполнении команд режима работы с памятью. Этот регистр может быть записан только тогда,
 * когда биты CMD и MCINIT равны нулю.
 */

#define SPIFI_CONFIG_MCMD_POLL_S			    14
/// Бит должен быть установлен в «0
#define SPIFI_CONFIG_MCMD_POLL_M			    (0x1 << SPIFI_CONFIG_MCMD_POLL_S)

#define SPIFI_CONFIG_MCMD_DOUT_S			    15
/// Бит должен быть установлен в «0
#define SPIFI_CONFIG_MCMD_DOUT_M			    (0x1 << SPIFI_CONFIG_MCMD_DOUT_S)

#define SPIFI_CONFIG_MCMD_INTLEN_S			  16
/**
 * Кол-во байт промежуточных данных (они хранятся в регистре IDATA). 
 * Если кол-во байт больше, чем может хранить регистр IDATA, остаток заполняется нулевыми значениями
 */
#define SPIFI_CONFIG_MCMD_INTLEN_M			  (0x7 << SPIFI_CONFIG_MCMD_INTLEN_S)

#define SPIFI_CONFIG_MCMD_FIELDFORM_S		  19
/**
 * Формат вывода полей команды:
 * «0» – все поля выводятся в последовательном режиме;
 * «1» – данные выводятся в четырех или двух битовом режиме, остальные поля – в последовательном;
 * «2» – код операции выводится в последовательном режиме, остальные – в четырех или двух битовом;
 * «3» – все поля в четырех или двух битовом режиме.
 */
#define SPIFI_CONFIG_MCMD_FIELDFORM_M		  (0x3 << SPIFI_CONFIG_MCMD_FIELDFORM_S)

#define SPIFI_CONFIG_MCMD_FRAMEFORM_S		  21
/**
 * Бит управления полями кода операции и адреса команды:
 * «0» – резерв;
 * «1» – резерв;
 * «2» – код операции + младший байт адреса;
 * «3» – код операции + два младших байта адреса;
 * «4» – код операции + три младших байта адреса;
 * «5» – код операции + 4 байта адреса;
 * «6» – нет кода операции, три младших байта адреса;
 * «7» – нет кода операции, 4 байта адреса
 */
#define SPIFI_CONFIG_MCMD_FRAMEFORM_M		  (0x7 << SPIFI_CONFIG_MCMD_FRAMEFORM_S)

#define SPIFI_CONFIG_MCMD_OPCODE_S			  24
/// Код операции (не используется для некоторых значений поля FRAMEFORM)
#define SPIFI_CONFIG_MCMD_OPCODE_M			  (0xFF << SPIFI_CONFIG_MCMD_OPCODE_S)

/// Регистр статуса (STAT). Данный регистр содержит текущее состояние контроллера.

#define SPIFI_CONFIG_STAT_MCINIT_S		    0
/**
 * Бит устанавливается в «1», если программное обеспечение успешно записало регистр команд памяти. 
 * Сброс бита осуществляется аппаратным сбросом или установкой в «1» бита RESET
 */
#define SPIFI_CONFIG_STAT_MCINIT_M		    (0x1 << SPIFI_CONFIG_STAT_MCINIT_S)

#define SPIFI_CONFIG_STAT_CMD_S			      1 
/**
 * Бит устанавливается в «1», если программное обеспечение успешно записало регистр команд. 
 * Сброс бита осуществляется аппаратным сбросом или установкой в «1» бита RESET
 * Также сброс бита CMD происходит при завершении выполнения команды, когда деактивируется сигнал SPIFI_CS
 */
#define SPIFI_CONFIG_STAT_CMD_M			      (0x1 << SPIFI_CONFIG_STAT_CMD_S)

#define SPIFI_CONFIG_STAT_RESET_S		      4
/**
 * Бит предназначен для того, чтобы прервать текущую команду периферийного режима 
 * или режима памяти. Бит сбрасывается, когда контроллер готов к выполнению новой команды
 */
#define SPIFI_CONFIG_STAT_RESET_M		      (0x1 << SPIFI_CONFIG_STAT_RESET_S)

#define SPIFI_CONFIG_STAT_INTRQ_S		      5
/// Запись «1» в бит сбрасывает запрос на прерывание от контроллера SPIFI
#define SPIFI_CONFIG_STAT_INTRQ_M		      (0x1 << SPIFI_CONFIG_STAT_INTRQ_S)

#define SPIFI_CONFIG_STAT_VERSION_S		    24
/// Версия контроллера SPIFI (поле доступно только для чтения)
#define SPIFI_CONFIG_STAT_VERSION_M		    (0xFF << SPIFI_CONFIG_STAT_VERSION_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h> 
    /// @brief Структура регистров контроллера SPIFI
    typedef struct
    {
      volatile uint32_t CTRL;           ///< Регистр управления
      volatile uint32_t CMD;         	  ///< Регистр команд
      volatile uint32_t ADDR;         	///< Регистр адреса
      volatile uint32_t IDATA;        	///< Регистр промежуточных данных
      volatile uint32_t CLIMIT;         ///< Регистр верхней границы адреса кеширования. Старший бит — незначащий, принимается контроллером за "1"
      union
      {
            volatile uint32_t DATA;			///< Регистр данных
            volatile uint8_t  DATA8;		///< 8-битное значение
            volatile uint16_t DATA16;		///< 16-битное значение
            volatile uint32_t DATA32;		///< 32-битное значение
      };  
	  
      volatile uint32_t MCMD;           ///< Регистр команд памяти
      volatile uint32_t STAT;           ///< Регистр статуса
    }SPIFI_CONFIG_TypeDef;
#endif


#endif // SPIFI_H_INCLUDED