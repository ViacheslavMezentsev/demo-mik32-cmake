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
#include "mik32_hal_pcc.h"
#include "SEGGER_RTT.h"

// SEGGER RTT: IP: localhost, PORT: 19021.
#define print(s)                        SEGGER_RTT_WriteString( 0, s ); HAL_DelayMs( &system_timer, 1 )
#define println(s)                      print( s "\n" )
#define printf( format, ... )           SEGGER_RTT_printf( 0, ( const char * ) ( format ), ##__VA_ARGS__ ); HAL_DelayMs( &system_timer, 1 )

void systemClockConfig( void );
static void initScr1Timer( void );

SCR1_TIMER_HandleTypeDef system_timer;

/**
 * Настраивает подсистему тактирования и монитор частоты МК.
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
 * Инициализирует встроенный в ядро МК системный таймер.
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
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализируем подсистему тактирования и монитор частоты МК.
    systemClockConfig();

    // Инициализируем системный таймер.
    initScr1Timer();

    // Настраиваем терминал 0 для работы в неблокирующем режиме.
    SEGGER_RTT_ConfigUpBuffer( 0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM );

    uint8_t n = 0;

    // Запускаем "бесконечный" цикл выполнения программы.
    while ( 1 )
    {
        // Ожидание 1 сек с использованием таймера (1 секунда = 1000 миллисекунд).
        HAL_DelayMs( &system_timer, 1000 );

        printf( "%u\n", n++ );
    }
}
