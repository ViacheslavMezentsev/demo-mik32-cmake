/*
 * В данном примере демонстрируется работа таймера32.
 * Нулевой канал таймера используется в режиме ШИМ. На Port0_0 генерирует периодичный сигнал (ШИМ)
 * с заполнением 50 %, частота которого задается верхним пределом таймера. Таймер считает от частоты
 * системной шины, поэтому для установки частоты нужно передать по UART0 значение: 
 * top = (частота основного генератора) / (задаваемая частота).
 * 
 * Например, для частоты 100кГц следует отправить "t320\n".
 */
#include "mik32_hal_timer32.h"
#include "xprintf.h"
#include "uart_lib.h"
#include "stdlib.h"

TIMER32_HandleTypeDef htimer32_1;
TIMER32_CHANNEL_HandleTypeDef htimer32_channel0;

void SystemClock_Config( void );
static void Timer32_1_Init( void );
void GPIO_Init();

/**
 * @brief   Функция считывания строки из UART.
 * 
 * @param   uart    Указатель на UART, в котором нужно ждать символ.
 * @param   buf     Буфер для хранения полученного символа.
 * @param   max_len Максимальная длина буфера.
 * 
 * @return  Возвращает true, если символ получен, иначе false.
 */
void UART_ReadLine( UART_TypeDef* uart, char* buf, uint32_t max_len )
{
    uint32_t i = 0;
    char ch;

    while ( i < ( max_len - 1 ) )
    {
        UART_WaitReceiving( uart );
        ch = UART_ReadByte( uart );

        // Если получен символ конца строки, то завершаем чтение.
        if ( ch == '\n' || ch == '\r' )
        {
            break;
        }

        buf[i++] = ch;
    }

    buf[i] = '\0';
}


/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация системных часов и периферии.
    SystemClock_Config();

    // Инициализация GPIO.
    GPIO_Init();

    // Инициализация UART для вывода отладочной информации.
    UART_Init( UART_0, OSC_SYSTEM_VALUE / 115200U, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M | UART_CONTROL1_RE_M, 0, 0 );

    Timer32_1_Init();

    xprintf( "UART init\n" );

    HAL_Timer32_Channel_Enable( &htimer32_channel0 );
    HAL_Timer32_Value_Clear( &htimer32_1 );
    HAL_Timer32_Start( &htimer32_1 );

    while ( 1 )
    {
        // Ждем, когда в UART появится символ 't'.
        // После этого считываем строку, которая должна содержать число - верхний предел таймера.
        UART_WaitReceiving( UART_0 );

        if ( UART_ReadByte( UART_0 ) != 't' )
        {
            continue;
        }

        char buf[8];

        // Считываем строку из UART.
        UART_ReadLine( UART_0, buf, sizeof( buf ) );

        // Преобразуем строку в число.
        int top = atoi( buf );

        if ( top > 0 )
        {
            HAL_Timer32_Top_Set( &htimer32_1, top );
            HAL_Timer32_Channel_OCR_Set( &htimer32_channel0, top >> 1 );
            HAL_Timer32_Value_Clear( &htimer32_1 );
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
 * @brief Инициализация таймера 32.
 *
 * Настраивает таймер 32 для работы в режиме ШИМ на нулевом канале.
 * Таймер будет считать от частоты системной шины, а верхний предел задается
 * пользователем через UART.
 */
static void Timer32_1_Init( void )
{
    htimer32_1.Instance = TIMER32_1;
    htimer32_1.Top = 7543;
    htimer32_1.State = TIMER32_STATE_DISABLE;
    htimer32_1.Clock.Source = TIMER32_SOURCE_PRESCALER;
    htimer32_1.Clock.Prescaler = 0;
    htimer32_1.InterruptMask = 0;
    htimer32_1.CountMode = TIMER32_COUNTMODE_FORWARD;
    
    HAL_Timer32_Init( &htimer32_1 );

    htimer32_channel0.TimerInstance = htimer32_1.Instance;
    htimer32_channel0.ChannelIndex = TIMER32_CHANNEL_0;
    htimer32_channel0.PWM_Invert = TIMER32_CHANNEL_NON_INVERTED_PWM;
    htimer32_channel0.Mode = TIMER32_CHANNEL_MODE_PWM;
    htimer32_channel0.CaptureEdge = TIMER32_CHANNEL_CAPTUREEDGE_RISING;
    htimer32_channel0.OCR = 7544 >> 1;
    htimer32_channel0.Noise = TIMER32_CHANNEL_FILTER_OFF;
    
    HAL_Timer32_Channel_Init( &htimer32_channel0 );
}


/**
 * @brief Инициализация GPIO.
 *
 * Включает тактирование для портов GPIO 0, 1 и 2.
 */
void GPIO_Init()
{
    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
}
