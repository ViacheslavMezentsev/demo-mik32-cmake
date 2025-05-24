/*
* Пример находится в разработке и может работать некорректно.
*/
#include "mik32_memory_map.h"
#include "power_manager.h"
#include "spifi.h"
#include "array.h"
#include "uart_lib.h"
#include "xprintf.h"

#define SREG1_BUSY 1

#define READ_SREG 1
#define READ_LEN  256
#define TIMEOUT   100000

#define CHIP_ERASE_COMMAND 0xC7

#define WRITE_ENABLE_COMMAND  0x06
#define WRITE_DISABLE_COMMAND 0x04

#define MEM_CONFIG_COMMAND 0x61
#define MEM_CONFIG_VALUE   0x7F

#define READ_DATA_COMMAND 0x03
#define READ_SREG_COMMAND 0x05

#define PAGE_PROGRAM_COMMAND 0x02

#define READ_SREG2_COMMAND               0x35
#define WRITE_SREG_COMMAND               0x01
#define FAST_READ_QUAD_OUTPUT_COMMAND    0x6B
#define FAST_READ_QUAD_OUTPUT_IO_COMMAND 0xEB

#define SREG2_QUAD_ENABLE   9
#define SREG2_QUAD_ENABLE_S ( SREG2_QUAD_ENABLE - 8 )
#define SREG2_QUAD_ENABLE_M 1 << SREG2_QUAD_ENABLE_S


/**
 * @brief Функция инициализации SPIFI.
 * 
 * Эта функция настраивает SPIFI для работы с внешней памятью, включая настройку GPIO и конфигурацию регистров SPIFI.
 * 
 * @note Перед вызовом этой функции необходимо убедиться, что SPIFI и GPIO 2 включены в тактирование.
 */

void spifi_init()
{
    // Включаем тактирование SPIFI и GPIO 2.
    __HAL_PCC_SPIFI_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };    

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_SERIAL;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;

    HAL_GPIO_Init( GPIO_2, &GPIO_InitStruct );

    /*
    *
    * STAT - регистр статуса
    * INTRQ - Запись «1» в бит сбрасывает запрос на прерывание от контроллера SPIFI
    * RESET - Бит предназначен для того, чтобы прервать текущую команду периферийного режима или режима памяти
    * 
    * ADDR - Исполнительный адрес команды
    * 
    * IDATA - регистр промежуточных данных
    * 
    * CLIMIT - Верхний предел кэшируемой памяти
    * 
    * CTRL - регистр управления
    * INTEN - Бит разрешения прерывания при завершении выполнения команды (если этот бит равен «1», то прерывание разрешено)
    * 
    */
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M | SPIFI_CONFIG_STAT_RESET_M;
    SPIFI_CONFIG->ADDR = 0x00;
    SPIFI_CONFIG->IDATA = 0x00;
    SPIFI_CONFIG->CLIMIT = 0x00000000;
    //SPIFI_CONFIG -> CTRL |= SPIFI_CONFIG_CTRL_SCK_DIV(3); // должно быть 2Мгц

    // SPIFI_CONFIG->CTRL |= SPIFI_CONFIG_CTRL_INTEN_M;

    // for (volatile int i = 0; i < 1000; i++);
    xprintf( "Finish init SPIFI\n" );
}


/**
 * @brief Функция для ожидания завершения операции SPIFI.
 * 
 * Эта функция проверяет статус регистра STAT на наличие бита INTRQ, который указывает на завершение операции.
 * Если операция не завершается в течение заданного таймаута, функция возвращает 0.
 * 
 * @param spifi Указатель на структуру конфигурации SPIFI.
 * @param timeout Таймаут в миллисекундах для ожидания завершения операции.
 * @return 1 если операция завершена, 0 если таймаут истек.
 */
int SPIFI_WaitIntrqTimeout( SPIFI_CONFIG_TypeDef* spifi, uint32_t timeout )
{
    while ( timeout-- > 0 )
    {
        if ( ( spifi->STAT & SPIFI_CONFIG_STAT_INTRQ_M ) != 0 )
        {
            return 1;
        }
    }

    return 0;
}


/**
 * @brief Функция для включения записи в память SPIFI.
 * 
 * Эта функция отправляет команду на разрешение записи в память SPIFI.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 */
void write_enable()
{
    /*
    *
    * CMD  код операции
    * OPCODE - код операции
    * FRAMEFORM - Бит управления полями кода операции и адреса команды:
    * «0» – резерв;
    * «1» – выдается только код операции, адреса нет; (SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR)
    * «2» – код операции и младший байт адреса;
    * «3» – код операции и два младших байта адреса;
    * «4» – код операции и три младших байта адреса;
    * «5» – код операции и 4 байта адреса;
    * «6» – нет кода операции, три младших байта адреса;
    * «7» – нет кода операции, 4 байта адреса
    * 
    * FIELDFORM - Формат вывода полей команды:
    * «0» – все поля выводятся в последовательном режиме; (SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL)
    * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
    * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
    * «3» – все поля в четырех или двух битовом режиме
    * 
    */
    xprintf( "Start write en\n" );

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( WRITE_ENABLE_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S );

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing write enable command" );
        return;
    }
}


/**
 * @brief Функция для чтения регистра SREG1.
 * 
 * Эта функция отправляет команду на чтение регистра SREG1 и возвращает его значение.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 * 
 * @return Значение регистра SREG1.
 */
uint8_t read_sreg_1()
{
    uint8_t read_sreg = 0;
    /*
    *
    * CMD  код операции
    * OPCODE - код операции
    * FRAMEFORM - Бит управления полями кода операции и адреса команды:
    * «0» – резерв;
    * «1» – выдается только код операции, адреса нет; (SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR)
    * «2» – код операции и младший байт адреса;
    * «3» – код операции и два младших байта адреса;
    * «4» – код операции и три младших байта адреса;
    * «5» – код операции и 4 байта адреса;
    * «6» – нет кода операции, три младших байта адреса;
    * «7» – нет кода операции, 4 байта адреса
    * 
    * FIELDFORM - Формат вывода полей команды:
    * «0» – все поля выводятся в последовательном режиме; (SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL)
    * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
    * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
    * «3» – все поля в четырех или двух битовом режиме
    * 
    */
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( READ_SREG_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( READ_SREG << SPIFI_CONFIG_CMD_DATALEN_S );

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing read sreg1 command" );
        return 0;
    }

    read_sreg = SPIFI_CONFIG->DATA8;
    return read_sreg;
}


/**
 * @brief Функция для ожидания завершения операции SPIFI.
 * 
 * Эта функция проверяет статус регистра SREG1 на наличие бита BUSY, который указывает на то, что устройство занято.
 * Если устройство не занято, функция продолжает выполнение.
 */
void wait_busy()
{
    xprintf( "Wait\n" );

    // Чтение регистра SREG1, чтобы убедиться, что устройство не занято.
    while ( read_sreg_1() & SREG1_BUSY );
}


/**
 * @brief Функция для стирания всего чипа SPIFI.
 * 
 * Эта функция отправляет команду на стирание всего чипа SPIFI.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 */
void chip_erase()
{
    xprintf( "Start erase\n" );

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( CHIP_ERASE_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S );

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing chip erase command" );
        return;
    }
}


/**
 * @brief Функция для чтения данных из памяти SPIFI.
 * 
 * Эта функция отправляет команду на чтение данных из памяти SPIFI по заданному адресу и количеству байт.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 * 
 * @param address Адрес, с которого нужно прочитать данные.
 * @param byte_count Количество байт для чтения.
 */
void read_data( unsigned int address, int byte_count )
{
    xprintf( "read data\n" );

    char read_data[byte_count];
    SPIFI_CONFIG->ADDR = address;

    /*
    *
    * CMD  код операции
    * OPCODE - код операции
    * FRAMEFORM - Бит управления полями кода операции и адреса команды:
    * «0» – резерв;
    * «1» – выдается только код операции, адреса нет; (SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR)
    * «2» – код операции и младший байт адреса;
    * «3» – код операции и два младших байта адреса;
    * «4» – код операции и три младших байта адреса;
    * «5» – код операции и 4 байта адреса;
    * «6» – нет кода операции, три младших байта адре-са;
    * «7» – нет кода операции, 4 байта адреса
    * 
    * FIELDFORM - Формат вывода полей команды:
    * «0» – все поля выводятся в последовательном режиме; (SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL)
    * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
    * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
    * «3» – все поля в четырех или двух битовом режиме
    * 
    */
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( READ_DATA_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( byte_count << SPIFI_CONFIG_CMD_DATALEN_S );

    // if(SPIFI_WaitIntrqTimeout(SPIFI_CONFIG, TIMEOUT) == 0)
    // {
    //     xprintf("Timeout executing read data command");
    //     return;
    // }

    for ( int i = 0; i < byte_count; i++ )
    {
        read_data[i] = SPIFI_CONFIG->DATA8;
        // xprintf("DATA[%d] = 0x%02x\n", address +i, read_data[i]);
    }

    for ( int i = 0; i < byte_count; i++ )
    {
        if ( read_data[i] != bin_data[address + i] )
        {
            xprintf( "DATA[%d] = 0x%02x - ошибка\n", address + i, read_data[i] );
        }
    }
}


/**
 * @brief Функция для программирования страницы памяти SPIFI.
 * 
 * Эта функция отправляет команду на программирование страницы памяти SPIFI по заданному адресу и массиву данных.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 * 
 * @param ByteAddress Адрес, по которому нужно записать данные.
 * @param data Массив данных для записи.
 * @param byte_count Количество байт для записи (максимум 256).
 */
void page_program( unsigned int ByteAddress, uint8_t * data, int byte_count )
{
    if ( byte_count > 256 )
    {
        xprintf( "Количество байт больше 256\n" );
    }

    xprintf( "Start page program\n" );

    //STAT:INTRQ
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;

    //ADDRESS
    SPIFI_CONFIG->ADDR = ByteAddress;

    //IDATA
    SPIFI_CONFIG->IDATA = 0x00;

    //CLIMIT
    SPIFI_CONFIG->CLIMIT = 0x00000000;

    //CMD
    SPIFI_CONFIG->CMD = ( PAGE_PROGRAM_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( 0 << SPIFI_CONFIG_CMD_INTLEN_S ) | ( 1 << SPIFI_CONFIG_CMD_DOUT_S ) |
                        ( 0 << SPIFI_CONFIG_CMD_POLL_S ) | ( byte_count << SPIFI_CONFIG_CMD_DATALEN_S );

    for ( unsigned i = ByteAddress; i < ( ByteAddress + byte_count ); i++ )
    {
        SPIFI_CONFIG->DATA8 = data[i];
    }

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
}


/**
 * @brief Функция для стирания всего чипа SPIFI.
 * 
 * Эта функция отправляет команду на стирание всего чипа SPIFI.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 */
void erase()
{
    write_enable(); // Разрешаем запись перед стиранием
    chip_erase();   // Стираем чип
    wait_busy();    // Ждем завершения операции стирания
}


/**
 * @brief Функция для записи данных в память SPIFI.
 * 
 * Эта функция отправляет команду на запись данных в память SPIFI по заданному адресу и массиву данных.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 * 
 * @param address Адрес, по которому нужно записать данные.
 * @param data Массив данных для записи.
 * @param data_len Количество байт для записи (максимум 256).
 */
void write( int address, uint8_t data[], int data_len )
{
    if ( data_len > 256 )
    {
        xprintf( "Количество байт больше 256\n" );
        return;
    }

    write_enable();
    page_program( address, ( uint8_t * ) data, data_len );
    wait_busy();

    xprintf( "written\n" );
}


/**
 * @brief Функция для включения режима Quad SPI.
 * 
 * Эта функция отправляет команды на чтение и запись регистров SREG1 и SREG2 для включения режима Quad SPI.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 */
void quad_enable()
{
    /*
    *
    * CMD  регистр команд
    * OPCODE - код операции
    * FRAMEFORM - Бит управления полями кода операции и адреса команды:
    * «0» – резерв;
    * «1» – выдается только код операции, адреса нет; (SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR)
    * «2» – код операции и младший байт адреса;
    * «3» – код операции и два младших байта адреса;
    * «4» – код операции и три младших байта адреса;
    * «5» – код операции и 4 байта адреса;
    * «6» – нет кода операции, три младших байта адреса;
    * «7» – нет кода операции, 4 байта адреса
    * 
    * FIELDFORM - Формат вывода полей команды:
    * «0» – все поля выводятся в последовательном режиме; (SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL)
    * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
    * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
    * «3» – все поля в четырех или двух битовом режиме
    * 
    */
    xprintf( "Read SREG1\n" );

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( READ_SREG_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( 1 << SPIFI_CONFIG_CMD_DATALEN_S );

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing write enable command" );
        return;
    }

    uint8_t sreg1 = SPIFI_CONFIG->DATA8;

    xprintf( "Read SREG2\n" );

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( READ_SREG2_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( 1 << SPIFI_CONFIG_CMD_DATALEN_S );

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing write enable command" );
        return;
    }

    uint8_t sreg2 = SPIFI_CONFIG->DATA8;

    xprintf( "Enable Quad\n" );

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( WRITE_SREG_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( 2 << SPIFI_CONFIG_CMD_DATALEN_S ) | SPIFI_CONFIG_CMD_DOUT_M;
    SPIFI_CONFIG->DATA8 = sreg1;
    SPIFI_CONFIG->DATA8 = sreg2 | SREG2_QUAD_ENABLE_M;

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing write enable command" );
        return;
    }

    sreg2 = 0;
    xprintf( "Read SREG2\n" );

    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( READ_SREG2_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( 1 << SPIFI_CONFIG_CMD_DATALEN_S );

    if ( SPIFI_WaitIntrqTimeout( SPIFI_CONFIG, TIMEOUT ) == 0 )
    {
        xprintf( "Timeout executing write enable command" );
        return;
    }

    sreg2 = SPIFI_CONFIG->DATA8;

    xprintf( "SREG2_QUAD_ENABLE setting %s\n", ( sreg2 & SREG2_QUAD_ENABLE_M ) ? "correct" : "failure" );
}


/**
 * @brief Функция для чтения данных из памяти SPIFI в режиме Quad SPI.
 * 
 * Эта функция отправляет команду на чтение данных из памяти SPIFI по заданному адресу и количеству байт в режиме Quad SPI.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 * 
 * @param address Адрес, с которого нужно прочитать данные.
 * @param byte_count Количество байт для чтения.
 */
void read_data_quad( unsigned int address, int byte_count )
{
    // xprintf("read data\n");
    uint8_t read_data[byte_count];
    SPIFI_CONFIG->ADDR = address;

    /*
    *
    * CMD  код операции
    * OPCODE - код операции
    * FRAMEFORM - Бит управления полями кода операции и адреса команды:
    * «0» – резерв;
    * «1» – выдается только код операции, адреса нет; (SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR)
    * «2» – код операции и младший байт адреса;
    * «3» – код операции и два младших байта адреса;
    * «4» – код операции и три младших байта адреса;
    * «5» – код операции и 4 байта адреса;
    * «6» – нет кода операции, три младших байта адреса;
    * «7» – нет кода операции, 4 байта адреса
    * 
    * FIELDFORM - Формат вывода полей команды:
    * «0» – все поля выводятся в последовательном режиме; (SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL)
    * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
    * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
    * «3» – все поля в четырех или двух битовом режиме
    * 
    */
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( FAST_READ_QUAD_OUTPUT_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_DATA_PARALLEL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( byte_count << SPIFI_CONFIG_CMD_DATALEN_S ) | ( 1 << SPIFI_CONFIG_CMD_INTLEN_S );

    // if(SPIFI_WaitIntrqTimeout(SPIFI_CONFIG, TIMEOUT) == 0)
    // {
    //     xprintf("Timeout executing read data command");
    //     return;
    // }

    for ( int i = 0; i < byte_count; i++ )
    {
        read_data[i] = SPIFI_CONFIG->DATA8;
        // xprintf("DATA[%d] = 0x%02x\n", address +i, read_data[i]);
    }

    uint8_t flag = 0;

    for ( int i = 0; i < byte_count; i++ )
    {
        if ( read_data[i] != bin_data[address + i] )
        {
            // xprintf("[%d]=0x%02x !=0x%02x\n", address + i, read_data[i], bin_data[address + i]);
            flag = 1;
        }
    }

    if ( flag )
    {
        xprintf( "err\n" );
    }
}


/**
 * @brief Функция для чтения данных из памяти SPIFI в режиме Quad I/O.
 * 
 * Эта функция отправляет команду на чтение данных из памяти SPIFI по заданному адресу и количеству байт в режиме Quad I/O.
 * Перед выполнением команды необходимо убедиться, что устройство не занято.
 * 
 * @param address Адрес, с которого нужно прочитать данные.
 * @param byte_count Количество байт для чтения.
 */
void read_data_quad_io( unsigned int address, int byte_count )
{
    // xprintf("read data\n");
    uint8_t read_data[byte_count];
    SPIFI_CONFIG->ADDR = address;

    /*
    *
    * CMD  код операции
    * OPCODE - код операции
    * FRAMEFORM - Бит управления полями кода операции и адреса команды:
    * «0» – резерв;
    * «1» – выдается только код операции, адреса нет; (SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR)
    * «2» – код операции и младший байт адреса;
    * «3» – код операции и два младших байта адреса;
    * «4» – код операции и три младших байта адреса;
    * «5» – код операции и 4 байта адреса;
    * «6» – нет кода операции, три младших байта адреса;
    * «7» – нет кода операции, 4 байта адреса
    * 
    * FIELDFORM - Формат вывода полей команды:
    * «0» – все поля выводятся в последовательном режиме; (SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL)
    * «1» – данные выводятся в четырех или двух битовом режиме, а остальные поля в последовательном режиме;
    * «2» – код операции выводится в последовательном режиме, а остальные в четырех или двух битовом;
    * «3» – все поля в четырех или двух битовом режиме
    * 
    */
    SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
    SPIFI_CONFIG->CMD = ( FAST_READ_QUAD_OUTPUT_IO_COMMAND << SPIFI_CONFIG_CMD_OPCODE_S ) |
                        ( SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR << SPIFI_CONFIG_CMD_FRAMEFORM_S ) |
                        ( SPIFI_CONFIG_CMD_FIELDFORM_OPCODE_SERIAL << SPIFI_CONFIG_CMD_FIELDFORM_S ) |
                        ( byte_count << SPIFI_CONFIG_CMD_DATALEN_S ) | ( 2 << SPIFI_CONFIG_CMD_INTLEN_S );

    // if(SPIFI_WaitIntrqTimeout(SPIFI_CONFIG, TIMEOUT) == 0)
    // {
    //     xprintf("Timeout executing read data command");
    //     return;
    // }

    for ( int i = 0; i < byte_count; i++ )
    {
        read_data[i] = SPIFI_CONFIG->DATA8;
        // xprintf("DATA[%d] = 0x%02x\n", address +i, read_data[i]);
    }

    uint8_t flag = 0;

    for ( int i = 0; i < byte_count; i++ )
    {
        if ( read_data[i] != bin_data[address + i] )
        {
            // xprintf("[%d]=0x%02x !=0x%02x\n", address + i, read_data[i], bin_data[address + i]);
            flag = 1;
        }
    }

    if ( flag )
    {
        xprintf( "err\n" );
    }
}


/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация вывода отладочной информации.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );

    // Инициализация SPIFI и стирание памяти.
    spifi_init();
    erase();

    int bin_data_len = sizeof( bin_data );

    xprintf( "bin_data_len = %d\n", bin_data_len );

    int address = 0;

    for ( address = 0; address < bin_data_len; address += 256 )
    {
        if ( address + 256 > bin_data_len )
        {
            break;
        }

        xprintf( "address = %d\n", address );

        write( address, bin_data, 256 );
        read_data( address, 256 );
    }

    if ( ( sizeof( bin_data ) % 256 ) != 0 )
    {
        xprintf( "address = %d, +%d[%d]\n", address, bin_data_len - address - 1, address + bin_data_len - address - 1 );

        write( address, bin_data, bin_data_len - address );

        read_data( address, bin_data_len - address );
    }

    xprintf( "end\n" );
    // quad_enable();
    // if (SPIFI_CONFIG->CTRL & SPIFI_CONFIG_CTRL_MODE3_M)
    // {
    //     xprintf("MODE3 ACTIVE\n");
    // }
    // SPIFI_CONFIG->CTRL |= SPIFI_CONFIG_CTRL_MODE3_M;
    // SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M |
    //                       SPIFI_CONFIG_STAT_RESET_M;
    // SPIFI_CONFIG->ADDR = 0x00;
    // SPIFI_CONFIG->IDATA = 0x00;
    // SPIFI_CONFIG->CLIMIT = 0x00000000;
    for ( volatile int i = 0; i < 1000; i++ );

    xprintf( "Finish init SPIFI\n" );

    while ( 1 )
    {
        // xprintf("\n");
        // read_data_quad(0, 64);
        // read_data_quad(0, 64);
        // for (volatile int i = 0; i < 1; i++);
    }
}
