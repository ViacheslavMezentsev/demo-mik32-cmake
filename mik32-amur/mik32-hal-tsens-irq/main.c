/*
 * Пример для отладочной платы lite v1.0 с MIK32_V2.
 *
 * Данный пример демонстрирует работу температурного сенсора с использованием прерываний. 
 * Датчик тактируется от внешнего осциллятора OSC32K. В примере устанавливается значение 
 * верхнего порога HI_THRESHOLD, соответствующее 35 ℃. Значение нижнего порога 
 * LOW_THRESHOLD остается по умолчанию -40 ℃.
 * Затем датчик начинает измерять температуру в непрерывном режиме. 
 * При этом включаются прерывания по окончанию преобразования и по выходу значения датчика 
 * за пороговые значения LOW_THRESHOLD и HI_THRESHOLD. По окончанию преобразования 
 * переменной temperature присваивается новое значение температуры.
 * При превышении верхнего порога температуры зажигается светодиод на выводе PORT2_7, 
 * устанавливается флаг hiFlag и не сбрасывается до тех пор, пока температура не опустится ниже 30 ℃.
 * Результат выводится по UART0.
 */
#include "mik32_hal_pcc.h"
#include "mik32_hal_tsens.h"
#include "mik32_hal_irq.h"
#include "mik32_hal_gpio.h"
#include "uart_lib.h"
#include "xprintf.h"

TSENS_HandleTypeDef htsens;

/// Флаг наличия нового значения температуры в переменной temperature.
uint32_t tsensFlag = 0;

/// Флаг превышения верхнего порогового значения.
uint32_t hiFlag = 0;

void SystemClock_Config( void );
void TSENS_Init( void );
void GPIO_Init( void );

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Значение температуры.
    uint32_t temperature = 0;

    // Инициализация HAL.
    HAL_Init();

    // Инициализация системных часов и периферии.
    SystemClock_Config();

    // Инициализация отладочного вывода.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );

    GPIO_Init();

    // Инициализация температурного сенсора.
    TSENS_Init();
    xprintf( "Initialization of the temperature sensor completed.\n" );

    // Установить верхнее пороговое значение до 35℃.
    HAL_TSENS_SetHiThreshold( &htsens, 35 );

    // Разрешить прерывание по уровню для линии TSENS.
    HAL_EPIC_MaskLevelSet( HAL_EPIC_TSENS_MASK );

    // Разрешить глобальные прерывания.
    HAL_IRQ_EnableInterrupts();

    // Запустить измерение температуры в непрерывном режиме.
    HAL_TSENS_ContinuousStart_IT( &htsens );

    while ( 1 )
    {
        // Если получено новое значение температуры.
        if ( tsensFlag ) 
        {
            temperature = TSENS_VALUE_TO_CELSIUS( __HAL_TSENS_READ_MEASUREMENT( &htsens ) );
            xprintf( "%d,%02d C\n", temperature / 100, temperature % 100 );
            tsensFlag = 0;

            // Если температура опустилась ниже 30℃ включить прерывание по превышению верхнего порогового значения.
            if ( temperature < 3000 )
            {
                HAL_GPIO_WritePin( GPIO_2, GPIO_PIN_7, GPIO_PIN_LOW );

                // Очистить флаг прерывания HI_IRQ.
                __HAL_TSENS_IRQ_CLEAR_HIIRQ( &htsens );
                __HAL_TSENS_IRQ_ENABLE_HIIRQ( &htsens );
            }
        }

        // Если было превышено верхнее пороговое значение HI_THRESHOLD.
        if ( hiFlag )
        {
            xprintf( "HI_THRESHOLD\n" );
            HAL_GPIO_WritePin( GPIO_2, GPIO_PIN_7, GPIO_PIN_HIGH );
            hiFlag = 0;
        }
    }
}


/**
 * @brief Настройка системных тактовых частот
 *
 * Настраивает тактирование системы, включая осцилляторы и делители частоты.
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
 * @brief Инициализация температурного сенсора.
 *
 * Настраивает параметры температурного сенсора, включая источник тактирования и частоту.
 */
void TSENS_Init()
{
    htsens.Instance = ANALOG_REG;

    htsens.Clock = HAL_TSENS_OSC32K;
    htsens.Frequency = 3000;

    if ( HAL_TSENS_Init( &htsens ) != HAL_OK )
    {
        xprintf( "HAL_TSENS_Init\n" );
    }
}


/**
 * @brief Инициализация GPIO.
 *
 * Настраивает вывод GPIO_2_7 как выход и устанавливает его в низкое состояние.
 */
void GPIO_Init()
{
    // Включить тактирование портов GPIO_0, GPIO_1, GPIO_2 и прерываний.
    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;

    HAL_GPIO_Init( GPIO_2, &GPIO_InitStruct );
    HAL_GPIO_WritePin( GPIO_2, GPIO_PIN_7, GPIO_PIN_LOW );
}


/**
 * @brief Обработчик прерываний.
 *
 * Проверяет наличие прерываний от температурного сенсора и обрабатывает их.
 * При наличии нового значения температуры устанавливает флаг tsensFlag.
 * При превышении верхнего порогового значения HI_THRESHOLD устанавливает флаг hiFlag.
 */
void trap_handler()
{
    // Проверить наличие прерываний от температурного сенсора.
    if ( EPIC_CHECK_TSENS() )
    {
        if ( __HAL_TSENS_IRQ_GET_EOC_MASK( &htsens ) )
        {
            tsensFlag = 1;
            /* Очистить флаг прерывания EOC_IRQ. */
            __HAL_TSENS_IRQ_CLEAR_EOC( &htsens );
        }

        if ( __HAL_TSENS_IRQ_GET_HIIRQ_MASK( &htsens ) )
        {
            /* Очистить флаг прерывания HI_IRQ. */
            __HAL_TSENS_IRQ_DISABLE_HIIRQ( &htsens );
            __HAL_TSENS_IRQ_CLEAR_HIIRQ( &htsens );
            hiFlag = 1;
        }

        /* Очистить флаг прерывания LOW_IRQ. */
        __HAL_TSENS_IRQ_CLEAR_LOWIRQ( &htsens );
    }

    /* Сброс прерываний */
    HAL_EPIC_Clear( 0xFFFFFFFF );
}
