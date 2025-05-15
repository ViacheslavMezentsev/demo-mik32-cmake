/*
 * Данный пример демонстрирует работу с GPIO и PAD_CONFIG.
 * В примере настраивается вывод, который подключенный к светодиоду, в режим GPIO.
 */
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"

void SystemClock_Config();
void GPIO_Init();

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Настраиваем подсистему тактирования и монитор частоты МК.
    SystemClock_Config();

    // Настраиваем порты ввода-вывода.
    GPIO_Init();

    while ( 1 )
    {
        HAL_GPIO_TogglePin( GPIO_0, GPIO_PIN_9 );

        HAL_DelayMs( 500 );
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
 * \brief   Инициализирует подсистему ввода/вывода общего назначения. Настраивает выводы.
 */
void GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    // Включить тактирование GPIO_0.
    __HAL_PCC_GPIO_0_CLK_ENABLE();

    // Настроить режим работы выхода, подключенного к светодиоду.
    GPIO_InitStruct.Pin  = GPIO_PIN_9;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;

    HAL_GPIO_Init( GPIO_0, &GPIO_InitStruct );
}
