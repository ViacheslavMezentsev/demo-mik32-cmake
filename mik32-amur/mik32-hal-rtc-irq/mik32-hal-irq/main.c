/*
 * В данном примере демонстрируется работа RTC.
 * В RTC настраивается текущее время и время срабатывания будильника, которое отличается от текущего на 5 секунд.
 * Будильник проводит сравнение времени и даты по всем полям.
 *
 * Текущее время RTC выводится по UART0. На 5й секунде должен сработать будильник и отправить "Alarm!" по UART0.
 */
#include "mik32_hal_rtc.h"
#include "uart_lib.h"
#include "xprintf.h"

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef LastTime = { 0 };
RTC_TimeTypeDef CurrentTime = { 0 };
RTC_DateTypeDef CurrentDate = { 0 };

void SystemClock_Config( void );
static void RTC_Init( void );

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация системных часов и периферии.
    SystemClock_Config();

    // Инициализация UART0.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0 );
    xprintf( "Start\n" );

    // Инициализация RTC.
    RTC_Init();

    LastTime = HAL_RTC_GetTime( &hrtc );

    while ( 1 )
    {
        CurrentTime = HAL_RTC_GetTime( &hrtc );

        if ( CurrentTime.Seconds != LastTime.Seconds )
        {
            LastTime = CurrentTime;
            CurrentDate = HAL_RTC_GetDate( &hrtc );

            xprintf( "\n%d век\n", CurrentDate.Century );
            xprintf( "%d.%d.%d\n", CurrentDate.Day, CurrentDate.Month, CurrentDate.Year );

            CurrentTime = HAL_RTC_GetTime( &hrtc );

            const char* days[] =
            {
                "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"
            };

            if ( CurrentTime.Dow >= 1 && CurrentTime.Dow <= 7 )
            {
                xprintf( "%s\n", days[ CurrentTime.Dow - 1 ] );
            }

            xprintf( "%d:%d:%d.%d\n", CurrentTime.Hours, CurrentTime.Minutes, CurrentTime.Seconds, hrtc.Instance->TOS );
        }

        // Проверка флага будильника.
        if ( HAL_RTC_GetAlrmFlag( &hrtc ) )
        {
            // Если флаг установлен, то выводим сообщение и очищаем флаг.
            xprintf( "\nAlarm!\n" );
            
            // Также отключаем будильник, чтобы он не срабатывал повторно.
            // В данном примере будильник срабатывает один раз.
            // Если требуется, чтобы будильник срабатывал периодически, то можно его заново включить.
            // Для этого нужно заново настроить время и дату будильника.
            HAL_RTC_AlarmDisable( &hrtc );
            HAL_RTC_ClearAlrmFlag( &hrtc );
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
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_OSC32K;
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;

    HAL_PCC_Config( &PCC_OscInit );
}


/**
 * @brief   Инициализация RTC.
 * 
 * Настраивает RTC, устанавливает текущее время и дату, а также настраивает будильник,
 * который сработает через 5 секунд после запуска программы.
 */
static void RTC_Init( void )
{
    __HAL_PCC_RTC_CLK_ENABLE();

    RTC_TimeTypeDef sTime = { 0 };
    RTC_DateTypeDef sDate = { 0 };
    RTC_AlarmTypeDef sAlarm = { 0 };

    hrtc.Instance = RTC;

    /* Выключение RTC для записи даты и времени */
    HAL_RTC_Disable( &hrtc );

    /* Установка даты и времени RTC */
    sTime.Dow = RTC_WEEKDAY_TUESDAY;
    sTime.Hours = 20;
    sTime.Minutes = 30;
    sTime.Seconds = 0;

    HAL_RTC_SetTime( &hrtc, &sTime );

    sDate.Century = 21;
    sDate.Day = 19;
    sDate.Month = RTC_MONTH_JULY;
    sDate.Year = 22;

    HAL_RTC_SetDate( &hrtc, &sDate );

    /* Включение будильника. Настройка даты и времени будильника */
    sAlarm.AlarmTime.Dow = RTC_WEEKDAY_TUESDAY;
    sAlarm.AlarmTime.Hours = 20;
    sAlarm.AlarmTime.Minutes = 30;
    sAlarm.AlarmTime.Seconds = 5;

    sAlarm.AlarmDate.Century = 21;
    sAlarm.AlarmDate.Day = 19;
    sAlarm.AlarmDate.Month = RTC_MONTH_JULY;
    sAlarm.AlarmDate.Year = 22;

    sAlarm.MaskAlarmTime = RTC_TALRM_CDOW_M | RTC_TALRM_CH_M | RTC_TALRM_CM_M | RTC_TALRM_CS_M;
    sAlarm.MaskAlarmDate = RTC_DALRM_CC_M | RTC_DALRM_CD_M | RTC_DALRM_CM_M | RTC_DALRM_CY_M;

    HAL_RTC_SetAlarm( &hrtc, &sAlarm );

    HAL_RTC_Enable( &hrtc );
}
