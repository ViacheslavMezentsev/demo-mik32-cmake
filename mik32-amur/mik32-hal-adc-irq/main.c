/*
* В данном примере демонстрируется работа с прерыванием АЦП.
* 
* Канал АЦП переключается не сразу после записи в регистр, а в конце преобразования.
* Для одиночных измерений с разных каналов рекомендуется записывать следующий канал сразу 
* после старта преобразования на текущем канале.
*
* При многократных измерениях на одном канале, после переключения канала рекомендуется сделать одно дополнительное измерение
* для переключения на выбранный канал.
*
* Так как АЦП не имеет флага прерывания, который можно сбросить, то в EPIC прерывание АЦП должно быть разрешено по фронту.
*/
#include "mik32_hal_adc.h"
#include "mik32_hal_irq.h"
#include "uart_lib.h"
#include "xprintf.h"

ADC_HandleTypeDef hadc;
uint16_t adc_value = 0;

void SystemClock_Config( void );
static void ADC_Init( void );

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    HAL_Init();

    // Настраиваем подсистему тактирования и монитор частоты МК.
    SystemClock_Config();

    // Так как АЦП не имеет флага прерывания, который можно сбросить,
    // то в EPIC прерывание АЦП должно быть разрешено по фронту.
    HAL_EPIC_MaskEdgeSet( HAL_EPIC_ADC_MASK );
    HAL_IRQ_EnableInterrupts();

    // Настраиваем UART.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );

    // Настраиваем АЦП.
    ADC_Init();

    while ( 1 )
    {
        // Запуск однократного преобразования.
        HAL_ADC_Single( &hadc );

        for ( volatile uint32_t i = 0; i < 100000U; i++ );

        xprintf( "ADC: %u (V = %u,%03u)\n", adc_value,
            ( ( adc_value * 1200U ) / 4095U ) / 1000U,
            ( ( adc_value * 1200U ) / 4095U ) % 1000U );
    }
}


/**
 * \brief   Настраивает подсистему тактирования и монитор частоты МК.
 */
void SystemClock_Config( void )
{
    PCC_InitTypeDef PCC_OscInit = { 0 };

    PCC_OscInit.OscillatorEnable         = PCC_OSCILLATORTYPE_ALL;
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
    PCC_OscInit.FreqMon.ForceOscSys      = PCC_FORCE_OSC_SYS_UNFIXED;
    PCC_OscInit.FreqMon.Force32KClk      = PCC_FREQ_MONITOR_SOURCE_OSC32K;
    PCC_OscInit.AHBDivider               = 0;
    PCC_OscInit.APBMDivider              = 0;
    PCC_OscInit.APBPDivider              = 0;
    PCC_OscInit.HSI32MCalibrationValue   = 128;
    PCC_OscInit.LSI32KCalibrationValue   = 8;
    PCC_OscInit.RTCClockSelection        = PCC_RTC_CLOCK_SOURCE_AUTO;
    PCC_OscInit.RTCClockCPUSelection     = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;

    HAL_PCC_Config( &PCC_OscInit );
}


/**
 * \brief   Инициализирует АЦП.
 */
static void ADC_Init( void )
{
    hadc.Instance = ANALOG_REG;

    hadc.Init.Sel = ADC_CHANNEL0;

    // Выбор источника опорного напряжения:
    // «1» - внешний;
    // «0» - встроенный
    hadc.Init.EXTRef = ADC_EXTREF_OFF;

    // Выбор источника внешнего опорного напряжения:
    // «1» - внешний вывод;
    // «0» - настраиваемый ОИН
    hadc.Init.EXTClb = ADC_EXTCLB_ADCREF;

    HAL_ADC_Init( &hadc );
}


/**
 * \brief   Обработчик прерываний.
 */
void trap_handler()
{
    if ( EPIC_CHECK_ADC() )
    {
        adc_value = HAL_ADC_GetValue( &hadc );
    }

    // Сброс прерываний.
    HAL_EPIC_Clear( 0xFFFFFFFF );
}
