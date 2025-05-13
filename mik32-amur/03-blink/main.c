/**
 * @file
 * Пример демонстрирует работу с встроенным в ядро МК таймером с помощью функций библиотеки HAL.
 * В листинге приведен порядок настройки:
 * - подсистемы тактирования и монитора частоты МК (systemClockConfig);
 * - выводов общего назначения (initGPIO);
 * - встроенного в ядро МК таймера (initScr1Timer);
 * - управления светодиодом по таймеру (blinkLED).
 * 
 * Ожидаемое поведение при выполнении приложения:
 * 1) светодиод светит в течение 1 секунды;
 * 2) светодиод выключается на 1 секунду;
 * 3) п. 1-2 повторяются пока подается питание на МК.
 * 
 * Примечание:
 * участки программ, заключенные в секции #define BOARD_ELBEAR_ACE_UNO, #define BOARD_NUKE_MIK32 и #define BOARD_BLUEPILL_MIK32,
 * настраивают и определяют поведение отладочных плат с учетом их технической реализации.
 */

#include "mik32_hal_scr1_timer.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_pcc.h"

SCR1_TIMER_HandleTypeDef system_timer;

void systemClockConfig( void );
static void initScr1Timer( void );
void initGPIO( void );
void blinkLED( void );


/**
 * \brief   Реализует логику приложения. Управляет уровнем сигнала на выводе.
 */
void blinkLED()
{
#if defined BOARD_ELBEAR_ACE_UNO
    // Переключить уровень сигнала на выходе, подключенного к светодиоду на противоположный.
    HAL_GPIO_TogglePin( GPIO_2, GPIO_PIN_7 );

#elif defined BOARD_NUKE_MIK32
    // Переключить уровень сигнала на выходе, подключенного к светодиоду на противоположный.
    HAL_GPIO_TogglePin( GPIO_0, GPIO_PIN_9 );

#elif defined BOARD_BLUEPILL_MIK32
    // Переключить уровень сигнала на выходе, подключенного к светодиоду на противоположный.
    HAL_GPIO_TogglePin( GPIO_0, GPIO_PIN_9 );

#endif
    // Ожидание 1 секунда с использованием таймера (1 секунда = 1000 миллисекунд).
    HAL_DelayMs( &system_timer, 1000 );
}


/**
 * \brief   Настраивает подсистему тактирования и монитор частоты МК.
 */
void systemClockConfig( void )
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
 * \brief   Инициализирует встроенный в ядро МК системный таймер.
 */
static void initScr1Timer( void )
{
    system_timer.Instance = SCR1_TIMER;

    // Использовать тактирование от ядра МК.
    system_timer.ClockSource = SCR1_TIMER_CLKSRC_INTERNAL;

    // Не использовать делитель частоты (10-битное число).
    system_timer.Divider = 0;

    HAL_SCR1_Timer_Init( &system_timer );
}


/**
 * \brief   Инициализирует подсистему ввода/вывода общего назначения. Настраивает выводы.
 */
void initGPIO( void )
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;

#if defined BOARD_ELBEAR_ACE_UNO
    // Включить тактирование GPIO_2
    __HAL_PCC_GPIO_2_CLK_ENABLE();

    // Настроить режим работы выхода, подключенного к светодиоду
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init( GPIO_2, &GPIO_InitStruct );

#elif defined BOARD_NUKE_MIK32
    // Включить тактирование GPIO_0
    __HAL_PCC_GPIO_0_CLK_ENABLE();

    // Настроить режим работы выхода, подключенного к светодиоду № 1 (зелёный)
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init( GPIO_0, &GPIO_InitStruct );

#elif defined BOARD_BLUEPILL_MIK32
    // Включить тактирование GPIO_0
    __HAL_PCC_GPIO_0_CLK_ENABLE();

    // Настроить режим работы выхода, подключенного к светодиоду.
    GPIO_InitStruct.Pin = GPIO_PIN_9;

    HAL_GPIO_Init( GPIO_0, &GPIO_InitStruct );

#endif
}


/**
 * @brief   Точка входа в программу.
 * 
 */
int main( void )
{
    // 1. Инициализировать подсистему тактирования и монитор частоты МК.
    systemClockConfig();

    // 2. Инициализировать подсистему ввода/вывода общего назначения, настроить выводы для кнопки и светодиода(ов).
    initGPIO();

    // 3. Инициализировать системный таймер.
    initScr1Timer();

    // 4. Запустить "бесконечный" цикл выполнения программы.
    while ( 1 )
    {
        // 4. Управлять уровнем сигнала вывода по таймеру.
        blinkLED();
    }
}
