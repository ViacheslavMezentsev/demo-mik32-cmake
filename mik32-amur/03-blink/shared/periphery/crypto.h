/**
 * @file
 * Крипто-блок
 * 
 * Крипто-блок реализует три специализированных преобразования:
 * - AES128, FIPS 197 128-битный блочный шифр, со 128-битным ключом;
 * - «Магма», ГОСТ 34.12–2018 64-битный блочный шифр;
 * - «Кузнечик», ГОСТ 34.12–2018 128-битный блочный шифр.
 * Крипто-блок состоит из трёх вычислительных ядер с общим управляющим конечным автоматом и общим буфером ввода вывода. 
 * В процессе вычислений крипто-блок настраивается на выполнение одного из трёх преобразований, при этом работает только 
 * одно из трёх ядер. В процессе работы крипто-блок формирует флаги разрешения записи и чтения для ПДП контроллера, 
 * сообщающие о состоянии готовности входных и выходных буферов к передаче данных. 
 */

#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

#define CRYPTO_BLOCK_OFFSET                 0x00
#define CRYPTO_KEY_OFFSET                   0x04
#define CRYPTO_INIT_OFFSET                  0x08
#define CRYPTO_CONFIG_OFFSET                0x0c

// Регистр конфигурации крипто-блока

/**
 * Задаёт режим хода вычислительного ядра:
 * «0» – прямой ход (шифрование);
 * «1» – обратный ход (расшифровка) 
 */

#define CRYPTO_CONFIG_DECODE_S              0
#define CRYPTO_CONFIG_ENCODE_M              (0 << CRYPTO_CONFIG_DECODE_S)
#define CRYPTO_CONFIG_DECODE_M              (1 << CRYPTO_CONFIG_DECODE_S)

/**
 * Регистр выбора алгоритма шифрования:
 * «0» – «Кузнечик»;
 * «1» – «Магма»;
 * «2» – «AES»
 */

#define CRYPTO_CONFIG_CORE_SEL_S            1
#define CRYPTO_CONFIG_CORE_SEL_M            (0X3 << CRYPTO_CONFIG_CORE_SEL_S)
#define CRYPTO_CONFIG_CORE_SEL_KUZNECHIK_M  (0X0 << CRYPTO_CONFIG_CORE_SEL_S)
#define CRYPTO_CONFIG_CORE_SEL_MAGMA_M      (0X1 << CRYPTO_CONFIG_CORE_SEL_S)
#define CRYPTO_CONFIG_CORE_SEL_AES_M        (0X2 << CRYPTO_CONFIG_CORE_SEL_S)

/**
 * Регистр выбора режима шифрования:
 * «0» – ECB;
 * «1» – CBC;
 * «2» – CTR
 */

#define CRYPTO_CONFIG_MODE_SEL_S            3
#define CRYPTO_CONFIG_MODE_SEL_M            (0X3 << CRYPTO_CONFIG_MODE_SEL_S)
#define CRYPTO_CONFIG_MODE_SEL_ECB_M        (0X0 << CRYPTO_CONFIG_MODE_SEL_S)
#define CRYPTO_CONFIG_MODE_SEL_CBC_M        (0X1 << CRYPTO_CONFIG_MODE_SEL_S)
#define CRYPTO_CONFIG_MODE_SEL_CTR_M        (0X2 << CRYPTO_CONFIG_MODE_SEL_S)

/**
 * Регистр выбора режима перестановки слова:
 * «0» – нет перестановки;
 * «1» – перестановка по полуслову;
 * «2» – перестановки по байтам;
 * «3» – перестановка по битам
 */

#define CRYPTO_CONFIG_SWAP_MODE_S           5
#define CRYPTO_CONFIG_SWAP_MODE_M           (0X3 << CRYPTO_CONFIG_SWAP_MODE_S)
#define CRYPTO_CONFIG_SWAP_MODE_NONE_M      (0X0 << CRYPTO_CONFIG_SWAP_MODE_S)
#define CRYPTO_CONFIG_SWAP_MODE_HALF_WORD_M (0X1 << CRYPTO_CONFIG_SWAP_MODE_S)
#define CRYPTO_CONFIG_SWAP_MODE_BYTE_M      (0X2 << CRYPTO_CONFIG_SWAP_MODE_S)
#define CRYPTO_CONFIG_SWAP_MODE_BIT_M       (0X3 << CRYPTO_CONFIG_SWAP_MODE_S)

/**
 * Выбор порядка загрузки/выгрузки:
 * «0» – от младшего слова к старшему;
 * «1» – от старшего слова к младшему
 */

#define CRYPTO_CONFIG_ORDER_MODE_S          7
#define CRYPTO_CONFIG_ORDER_MODE_M          (1 << CRYPTO_CONFIG_ORDER_MODE_S)
#define CRYPTO_CONFIG_ORDER_MODE_LSW_M      (0 << CRYPTO_CONFIG_ORDER_MODE_S)
#define CRYPTO_CONFIG_ORDER_MODE_MSW_M      (1 << CRYPTO_CONFIG_ORDER_MODE_S)

#define CRYPTO_CONFIG_C_RESET_S             8
/// Сброс счётчиков загружаемых/выгружаемых данных 
#define CRYPTO_CONFIG_C_RESET_M             (1 << CRYPTO_CONFIG_C_RESET_S)

#define CRYPTO_CONFIG_READY_S               9
/**
 * Состояние доступности модуля:
 * «0» – занято;
 * «1» – доступно
 */
#define CRYPTO_CONFIG_READY_M               (1 << CRYPTO_CONFIG_READY_S)

#define CRYPTO_CONFIG_WRITE_STATUS_S        10
/**
 * Буфер данных доступен для записи:\
 * «0» – недоступно;
 * «1» – доступно
 */
#define CRYPTO_CONFIG_WRITE_STATUS_M        (1 << CRYPTO_CONFIG_WRITE_STATUS_S)

#define CRYPTO_CONFIG_READ_STATUS_S         11
/**
 * Буфер данных доступен для чтения:
 * «0» – недоступно;
 * «1» – доступно
 */
#define CRYPTO_CONFIG_READ_STATUS_M         (1 << CRYPTO_CONFIG_READ_STATUS_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h>
    /// @brief Структура регистров крипто-блока
    typedef struct
    {
        volatile uint32_t BLOCK;                ///< [RW]: Буфер данных для шифрования
        volatile uint32_t KEY;                  ///< [WO]: Буфер данных ключа
        volatile uint32_t INIT;                 ///< [WO]: Буфер данных вектора инициализации
        volatile uint32_t CONFIG;               ///< [RW]: Регистр конфигурации
    } CRYPTO_TypeDef;
#endif

#endif // CRYPTO_H_INCLUDED