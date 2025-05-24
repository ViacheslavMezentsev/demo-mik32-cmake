/*
 * Пример для платы DIP-MIK32.
 *
 * Данный пример демонстрирует работу с прерыванием системного таймера по сравнению.
 * В примере настраивается вывод PORT1_3, который подключен к светодиоду, в режим GPIO.
 * По достижению счетчика таймера значения сравнения срабатывает прерывание, по которому
 * переменная flag устанавливается в 1. В зависимости от значения flag инвертируется сигнал
 * на выводе PORT1_3.
 **/
#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_scr1_timer.h"

// Значение счетчика таймера (1 секунда при частоте ядра 32МГц).
#define DELAY_VALUE 32000000

void SystemClock_Config( void );
void GPIO_Init( void );

// Флаг, по которому инвертируется выходной сигнал на выводе PORT1_3.
// Устанавливается в обработчике прерывания trap_handler.
volatile int flag = 0;

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Настройки тактирования.
    SystemClock_Config();

    // Инициализация GPIO.
    GPIO_Init();

    // Инициализация системного таймера.
    HAL_SCR1_Timer_Init( HAL_SCR1_TIMER_CLKSRC_INTERNAL, 0 );

    // Установка режима работы системного таймера - прерывание по сравнению.
    __HAL_SCR1_TIMER_SET_CMP( DELAY_VALUE );
    
    // Разрешить прерывание системного таймера.
    __HAL_SCR1_TIMER_IRQ_ENABLE();

    while ( 1 )
    {
        // Ожидание прерывания от системного таймера.
        if ( flag )
        {
            HAL_GPIO_TogglePin( GPIO_1, GPIO_PIN_3 );
            flag = 0;
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
 * @brief   Инициализация GPIO.
 * 
 * Настраивает вывод PORT1_3 в режим GPIO, устанавливает его в низкий уровень.
 */
void GPIO_Init()
{
    // Включение тактирования порта GPIO_1.
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;

    HAL_GPIO_Init( GPIO_1, &GPIO_InitStruct );
    HAL_GPIO_WritePin( GPIO_1, GPIO_InitStruct.Pin, __LOW );
}


/**
 * @brief   Обработчик прерывания системного таймера.
 * 
 * Устанавливает флаг flag в 1, сбрасывает счетчик системного таймера и
 * перезаписывает значение сравнения для сброса запроса прерывания по сравнению.
 */
void trap_handler()
{
    flag = 1;

    // Сброс счетчика системного таймера.
    __HAL_SCR1_TIMER_SET_TIME( 0 );

    // Перезапись значения сравнения используется для сброса запроса прерывания по сравнению.
    __HAL_SCR1_TIMER_SET_CMP( DELAY_VALUE );
}
