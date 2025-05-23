/*
 * Данный пример демонстрирует работу с Timer16.
 * Таймер тактируется и считает в непрерывном режиме от внутреннего источника (sys_clk - системная частота).
 * Значения счетчика периодически выводятся по UART0. На выводе PORT0_10 генерируется ШИМ сигнал.
 */
#include "mik32_hal_timer16.h"
#include "mik32_hal_usart.h"
#include "xprintf.h"

Timer16_HandleTypeDef htimer16_1;
USART_HandleTypeDef husart0;

void SystemClock_Config( void );
static void Timer16_1_Init( void );
static void GPIO_Init();
static void USART_Init();

/**
 * @brief   Точка входа в программу.
 * 
 */
int main()
{
    // Инициализация системных часов и периферии.
    SystemClock_Config();

    // Инициализация периферии GPIO и USART.
    GPIO_Init();
    USART_Init();

    Timer16_1_Init();

    // Запуск таймера в продолжительном режиме.
    // HAL_Timer16_Counter_Start(&htimer16_1, 0xFFFF);

    // Генерация волновой формы.
    HAL_Timer16_StartPWM( &htimer16_1, 0xFFFF, 0xFFFF / 2 );
    // HAL_Timer16_StartOneShot(&htimer16_1, 0xFFFF, 0xFFFF / 2);
    // HAL_Timer16_StartSetOnes(&htimer16_1, 0xFFFF, 0xFFFF / 2);

    uint16_t lastCounterValue = -1;
    uint16_t counterValue = 0;

    while ( 1 )
    {        
        counterValue = HAL_Timer16_GetCounterValue( &htimer16_1 );

        if ( counterValue != lastCounterValue )
        {
            /* Вывод значения счетчика */
            xprintf( "CNT = %d\n", HAL_Timer16_GetCounterValue( &htimer16_1 ) );
            lastCounterValue = counterValue;
        }

        /* Ожидание совпадения с CMP */
        if ( __HAL_TIMER16_GET_FLAG( &htimer16_1, TIMER16_FLAG_CMPM ) )
        {
            xprintf( "CMP\n" );

            __HAL_TIMER16_CLEAR_FLAG( &htimer16_1, TIMER16_FLAG_CMPM );
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
 * @brief   Инициализация таймера 16.
 * 
 * Настраивает таймер 16 для работы в непрерывном режиме с тактированием от внутреннего источника.
 * Таймер будет генерировать ШИМ сигнал на выводе PORT0_10.
 */
static void Timer16_1_Init( void )
{
    htimer16_1.Instance = TIMER16_1;

    /* Настройка тактирования */
    htimer16_1.Clock.Source = TIMER16_SOURCE_INTERNAL_SYSTEM;
    htimer16_1.CountMode = TIMER16_COUNTMODE_INTERNAL; /* При тактировании от Input1 не имеет значения */
    htimer16_1.Clock.Prescaler = TIMER16_PRESCALER_1;
    htimer16_1.ActiveEdge = TIMER16_ACTIVEEDGE_RISING; /* Выбирается при тактировании от Input1 */

    /* Настройка режима обновления регистра ARR и CMP */
    htimer16_1.Preload = TIMER16_PRELOAD_AFTERWRITE;

    /* Настройка триггера */
    htimer16_1.Trigger.Source = TIMER16_TRIGGER_TIM1_GPIO1_9;
    htimer16_1.Trigger.ActiveEdge =
        TIMER16_TRIGGER_ACTIVEEDGE_SOFTWARE; /* При использовании триггера значение должно быть отлично от software */
    htimer16_1.Trigger.TimeOut = TIMER16_TIMEOUT_DISABLE; /* Разрешить повторное срабатывание триггера */

    /* Настройки фильтра */
    htimer16_1.Filter.ExternalClock = TIMER16_FILTER_NONE;
    htimer16_1.Filter.Trigger = TIMER16_FILTER_NONE;

    /* Настройка режима энкодера */
    htimer16_1.EncoderMode = TIMER16_ENCODER_DISABLE;

    htimer16_1.Waveform.Enable = TIMER16_WAVEFORM_GENERATION_ENABLE;
    htimer16_1.Waveform.Polarity = TIMER16_WAVEFORM_POLARITY_NONINVERTED;

    HAL_Timer16_Init( &htimer16_1 );
}


/**
 * @brief   Инициализация GPIO.
 * 
 * Настраивает тактирование портов GPIO, необходимых для работы с таймером и USART.
 */
static void GPIO_Init()
{
    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
}


/**
 * @brief   Инициализация USART.
 * 
 * Настраивает USART0 для работы в асинхронном режиме с заданными параметрами.
 */
static void USART_Init()
{
    husart0.Instance = UART_0;
    husart0.transmitting = Enable;
    husart0.receiving = Disable;
    husart0.frame = Frame_8bit;
    husart0.parity_bit = Disable;
    husart0.parity_bit_inversion = Disable;
    husart0.bit_direction = LSB_First;
    husart0.data_inversion = Disable;
    husart0.tx_inversion = Disable;
    husart0.rx_inversion = Disable;
    husart0.swap = Disable;
    husart0.lbm = Disable;
    husart0.stop_bit = StopBit_1;
    husart0.mode = Asynchronous_Mode;
    husart0.xck_mode = XCK_Mode3;
    husart0.last_byte_clock = Disable;
    husart0.overwrite = Disable;
    husart0.rts_mode = AlwaysEnable_mode;
    husart0.dma_tx_request = Disable;
    husart0.dma_rx_request = Disable;
    husart0.channel_mode = Duplex_Mode;
    husart0.tx_break_mode = Disable;
    husart0.Interrupt.ctsie = Disable;
    husart0.Interrupt.eie = Disable;
    husart0.Interrupt.idleie = Disable;
    husart0.Interrupt.lbdie = Disable;
    husart0.Interrupt.peie = Disable;
    husart0.Interrupt.rxneie = Disable;
    husart0.Interrupt.tcie = Disable;
    husart0.Interrupt.txeie = Disable;
    husart0.Modem.rts = Disable;     // out
    husart0.Modem.cts = Disable;     // in
    husart0.Modem.dtr = Disable;     // out
    husart0.Modem.dcd = Disable;     // in
    husart0.Modem.dsr = Disable;     // in
    husart0.Modem.ri = Disable;      // in
    husart0.Modem.ddis = Disable;    // out
    husart0.baudrate = 9600;

    HAL_USART_Init( &husart0 );
}
