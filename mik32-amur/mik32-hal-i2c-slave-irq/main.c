/*
 * В данном примере демонстрируется работа I2C в режиме ведомого с использованием прерываний.
 * Ведомый считывает 10 байт от ведущего, а затем записывает.
 * Используется растягивание сигнала SCL.
 * Результат выводится в UART0.
 *
 * Данный пример может быть использован совместно с ведущим из примера Hal_I2C_Master_IRQ.
 */
#include "mik32_hal_i2c.h"
#include "mik32_hal_irq.h"
#include "uart_lib.h"
#include "xprintf.h"

I2C_HandleTypeDef hi2c0;

void SystemClock_Config( void );
static void I2C0_Init( void );

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация HAL.
    HAL_Init();

    // Настройка тактирования.
    SystemClock_Config();

    // Инициализация отладочного вывода.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );

    // Инициализация I2C0 в режиме ведомого.
    I2C0_Init();

    // Инициализация EPIC для обработки прерываний I2C.
    HAL_EPIC_MaskLevelSet( HAL_EPIC_I2C_0_MASK );
    HAL_IRQ_EnableInterrupts();

    uint8_t data[10];

    for ( unsigned i = 0; i < sizeof( data ); i++ )
    {
        data[i] = ( uint8_t ) i;
    }

    while ( 1 )
    {
        /* Прием данных */
        xprintf( "\nSlave_Receive\n" );
        HAL_StatusTypeDef error_code = HAL_I2C_Slave_Receive_IT( &hi2c0, data, sizeof( data ) );

        if ( error_code != HAL_OK )
        {
            xprintf( "Slave_Receive error #%d, code %d, ISR %d\n", error_code, hi2c0.ErrorCode, hi2c0.Instance->ISR );
            HAL_I2C_Reset( &hi2c0 );
        }

        while ( hi2c0.State != HAL_I2C_STATE_END );

        for ( unsigned i = 0; i < sizeof( data ); i++ )
        {
            xprintf( "Data_read[%d] = %d\n", i, data[i] );
            data[i] = 0;
        }

        for ( unsigned i = 0; i < sizeof( data ); i++ )
        {
            data[i] = ( uint8_t ) i;
        }

        /* Передача данных */
        xprintf( "\nSlave_Transmit\n" );
        error_code = HAL_I2C_Slave_Transmit_IT( &hi2c0, data, sizeof( data ) );

        if ( error_code != HAL_OK )
        {
            xprintf( "nSlave_Transmit error #%d, code %d, ISR %d\n", error_code, hi2c0.ErrorCode, hi2c0.Instance->ISR );
            HAL_I2C_Reset( &hi2c0 );
        }

        while ( hi2c0.State != HAL_I2C_STATE_END );
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
 * @brief   Инициализация I2C0 в режиме ведомого.
 * 
 * Настраивает I2C0 для работы в режиме ведомого с заданными параметрами.
 * Устанавливает адреса, частоту и другие параметры I2C.
 */
static void I2C0_Init( void )
{
    /* Общие настройки */
    hi2c0.Instance = I2C_0;

    hi2c0.Init.Mode = HAL_I2C_MODE_SLAVE;

    hi2c0.Init.DigitalFilter = I2C_DIGITALFILTER_OFF;
    hi2c0.Init.AnalogFilter = I2C_ANALOGFILTER_DISABLE;
    hi2c0.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    hi2c0.Init.OwnAddress1 = 0x36;
    hi2c0.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c0.Init.OwnAddress2Mask = I2C_OWNADDRESS2_MASK_DISABLE;
    hi2c0.Init.OwnAddress2 = 0x55;
    hi2c0.Init.GeneralCall = I2C_GENERALCALL_DISABLE;
    hi2c0.Init.SBCMode = I2C_SBC_DISABLE;

    /* Настройка частоты */
    hi2c0.Clock.PRESC = 15;
    hi2c0.Clock.SCLDEL = 15;
    hi2c0.Clock.SDADEL = 15;
    hi2c0.Clock.SCLH = 15;
    hi2c0.Clock.SCLL = 15;

    if ( HAL_I2C_Init( &hi2c0 ) != HAL_OK )
    {
        xprintf( "I2C_Init error\n" );
    }
}


/**
 * @brief   Обработчик прерываний I2C.
 * 
 * Вызывается при возникновении прерываний I2C.
 * Проверяет, было ли прерывание вызвано I2C_0 и вызывает соответствующий обработчик HAL.
 */
HAL_StatusTypeDef HAL_I2C_Slave_SBC( I2C_HandleTypeDef* hi2c, uint8_t* pData, uint16_t DataSize, uint32_t ByteCount )
{
    if ( ByteCount == 5 )
    {
        HAL_I2C_Slave_NACK( hi2c );
        return HAL_ERROR;
    }

    HAL_I2C_Slave_ACK( hi2c );
    return HAL_OK;
}


/**
 * @brief   Обработчик прерываний EPIC.
 * 
 * Вызывается при возникновении прерываний EPIC.
 * Проверяет, было ли прерывание вызвано I2C_0 и вызывает соответствующий обработчик HAL.
 */
void trap_handler()
{
    if ( EPIC_CHECK_I2C_0() )
    {
        HAL_I2C_IRQHandler( &hi2c0 );
    }

    /* Сброс прерываний */
    HAL_EPIC_Clear( 0xFFFFFFFF );
}
