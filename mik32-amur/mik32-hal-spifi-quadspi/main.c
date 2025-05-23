/**
 * @file main.c
 *
 * @brief Пример демонстрирует чтение и запись значений во внешнюю флеш память Winbond W25 по Standard/Single SPI.
 */
#include "mik32_hal_pcc.h"
#include "mik32_hal_spifi.h"
#include "mik32_hal_spifi_w25.h"
#include "uart_lib.h"
#include "xprintf.h"

void SystemClock_Config( void );
void read_flash( SPIFI_HandleTypeDef* spifi, uint32_t address, uint8_t dataLength, uint8_t* dataBytes );

/// Структура, используемая для работы с внешней флеш памятью Winbond W25.
SPIFI_HandleTypeDef spifi = { .Instance = SPIFI_CONFIG };

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация системных часов и периферии.
    SystemClock_Config();

    // Инициализация отладочного вывода.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );

    // Инициализация SPIFI интерфейса.
    HAL_SPIFI_MspInit( &spifi );
    HAL_SPIFI_Reset( &spifi );

    uint8_t write_values[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    uint8_t read_values[sizeof( write_values )] = { 0 };

    W25_ManufacturerDeviceIDTypeDef id = HAL_SPIFI_W25_ReadManufacturerDeviceID( &spifi );

    xprintf( "\n\nManufacturer ID: 0x%02x    DeviceID: 0x%02x\n\n", id.Manufacturer, id.Device );

    xprintf( "ENABLE QUAD\n" );
    HAL_SPIFI_W25_QuadEnable( &spifi );

    xprintf( "QUAD ENABLE = %d\n\n", ( HAL_SPIFI_W25_ReadSREG( &spifi, W25_SREG2 ) & SPIFI_W25_SREG2_QUAD_ENABLE_M ) >>
        SPIFI_W25_SREG2_QUAD_ENABLE_S );

    xprintf( "ERASE SECTOR\n" );
    HAL_SPIFI_W25_SectorErase4K( &spifi, 0 );

    read_flash( &spifi, 0, sizeof( read_values ), read_values );

    xprintf( "Write values = " );

    for ( unsigned i = 0; i < sizeof( write_values ); i++ )
    {
        xprintf( "0x%02X ", write_values[i] );
    }

    xprintf( "\n" );

    HAL_SPIFI_W25_PageProgram_Quad( &spifi, 0, sizeof( write_values ), write_values );
    read_flash( &spifi, 0, sizeof( read_values ), read_values );

    while ( 1 );
}


/**
 * @brief Чтение данных из внешней флеш памяти Winbond W25
 *
 * @param spifi Указатель на структуру SPIFI_HandleTypeDef, представляющую SPIFI интерфейс
 * @param address Адрес чтения данных
 * @param dataLength Количество байт для чтения
 * @param dataBytes Указатель на массив, куда будут записаны прочитанные данные
 */
void read_flash( SPIFI_HandleTypeDef* spifi, uint32_t address, uint8_t dataLength, uint8_t* dataBytes )
{
    HAL_SPIFI_W25_ReadData_Quad_IO( spifi, address, dataLength, dataBytes );

    xprintf( "Read values =  " );

    for ( uint8_t i = 0; i < dataLength; i++ )
    {
        xprintf( "0x%02X ", dataBytes[i] );
    }

    xprintf( "\n\n" );
}


/**
 * @brief Настройка системного тактирования
 *
 * Настраивает системное тактирование микроконтроллера, включая осцилляторы и делители частоты.
 */
void SystemClock_Config( void )
{
    PCC_InitTypeDef PCC_OscInit = { 0 };

    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
    PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
    PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
    PCC_OscInit.AHBDivider = 0;
    PCC_OscInit.APBMDivider = 0;
    PCC_OscInit.APBPDivider = 0;
    PCC_OscInit.HSI32MCalibrationValue = 128;
    PCC_OscInit.LSI32KCalibrationValue = 8;
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;

    HAL_PCC_Config( &PCC_OscInit );
}
