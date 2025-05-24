/*
 * Данный пример демонстрирует работу с SPI в режиме ведомого с использованием прерываний.
 * Ведомый передает и принимает от ведущего 20 байт.
 *
 * Результат передачи выводится по UART0.
 * Данный пример можно использовать совместно с ведущим из примера HAL_SPI_Master_IRQ.
 */
#include "mik32_hal_spi.h"
#include "mik32_hal_irq.h"
#include "uart_lib.h"
#include "xprintf.h"

SPI_HandleTypeDef hspi0;

void SystemClock_Config( void );
static void SPI0_Init( void );

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация HAL.
    HAL_Init();

    // Инициализация системных часов и периферии.
    SystemClock_Config();
    
    // Инициализация EPIC и разрешение прерываний SPI0.
    HAL_EPIC_MaskLevelSet( HAL_EPIC_SPI_0_MASK );
    HAL_IRQ_EnableInterrupts();

    // Инициализация UART0 для вывода отладочной информации.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );

    SPI0_Init();

    uint8_t slave_output[] = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9,
                               0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9 };
    uint8_t slave_input[sizeof( slave_output )];

    for ( uint32_t i = 0; i < sizeof( slave_input ); i++ )
    {
        slave_input[i] = 0;
    }

    while ( 1 )
    {
        // Ожидание готовности SPI к передаче данных.
        if ( hspi0.State == HAL_SPI_STATE_READY )
        {
            /* Обнуление массива slave_input */
            for ( uint32_t i = 0; i < sizeof( slave_input ); i++ )
            {
                slave_input[i] = 0;
            }

            /* Передача и прием данных */
            HAL_StatusTypeDef SPI_Status =
                HAL_SPI_Exchange_IT( &hspi0, slave_output, slave_input, sizeof( slave_output ) );

            if ( SPI_Status != HAL_OK )
            {
                HAL_SPI_ClearError( &hspi0 );
                hspi0.State = HAL_SPI_STATE_READY;
            }
        }

        if ( hspi0.State == HAL_SPI_STATE_END )
        {
            /* Вывод принятый данных и обнуление массива slave_input */
            for ( uint32_t i = 0; i < sizeof( slave_input ); i++ )
            {
                xprintf( "slave_input[%d] = 0x%02x\n", i, slave_input[i] );
            }

            xprintf( "\n" );

            hspi0.State = HAL_SPI_STATE_READY;
        }

        if ( hspi0.State == HAL_SPI_STATE_ERROR )
        {
            xprintf( "SPI_Error: OVR %d, MODF %d\n", hspi0.ErrorCode & HAL_SPI_ERROR_OVR, hspi0.ErrorCode & HAL_SPI_ERROR_MODF );
            HAL_SPI_Disable( &hspi0 );
            hspi0.State = HAL_SPI_STATE_READY;
        }
    }
}


/**
 * @brief   Настройка системного тактирования.
 * 
 * Настраивает тактирование микроконтроллера, включая генерацию частоты 32MHz
 * для системы и 32KHz для RTC.
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


/**
 * @brief   Инициализация SPI0 в режиме ведомого.
 * 
 * Настраивает SPI0 для работы в режиме ведомого с заданными параметрами.
 */
static void SPI0_Init( void )
{
    hspi0.Instance = SPI_0;

    /* Режим SPI */
    hspi0.Init.SPI_Mode = HAL_SPI_MODE_SLAVE;

    /* Настройки */
    hspi0.Init.CLKPhase = SPI_PHASE_ON;
    hspi0.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi0.Init.ThresholdTX = 4;

    if ( HAL_SPI_Init( &hspi0 ) != HAL_OK )
    {
        xprintf( "SPI_Init_Error\n" );
    }
}


/**
 * @brief   Обработчик прерываний.
 * 
 * Вызывается при возникновении прерывания SPI0. Обрабатывает прерывания
 * и сбрасывает их после обработки.
 */
void trap_handler()
{
    if ( EPIC_CHECK_SPI_0() )
    {
        HAL_SPI_IRQHandler( &hspi0 );
    }

    /* Сброс прерываний */
    HAL_EPIC_Clear( 0xFFFFFFFF );
}
