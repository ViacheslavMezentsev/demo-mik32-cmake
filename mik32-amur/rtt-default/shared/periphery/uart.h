#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#define UART_CONTROL1_OFFSET                        0x00
#define UART_CONTROL2_OFFSET                        0x04
#define UART_CONTROL3_OFFSET                        0x08
#define UART_DIVIDER_OFFSET                         0x0c
#define UART_FLAGS_OFFSET                           0x1c
#define UART_RXDATA_OFFSET                          0x24
#define UART_TXDATA_OFFSET                          0x28
#define UART_MODEM_OFFSET                           0x2c

// Регистр управления 1 CONTROL1

#define UART_CONTROL1_M1_S         28
/**
 * Совместно с битом 12 (M0) управляет длиной кадра:
 * M[1:0] = 00: 8 бит данных;
 * M[1:0] = 01: 9 бит данных;
 * M[1:0] = 10: 7 бит данных.
 * @warning Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL1_M1_M         (1 << UART_CONTROL1_M1_S)

#define UART_CONTROL1_M0_S         12
/**
 * Совместно с битом 28 (M1) управляет длиной кадра:
 * M[1:0] = 00: 8 бит данных;
 * M[1:0] = 01: 9 бит данных;
 * M[1:0] = 10: 7 бит данных.
 * @warning Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL1_M0_M         (1 << UART_CONTROL1_M0_S)

#define UART_CONTROL1_M_8BIT_M     (0)
#define UART_CONTROL1_M_9BIT_M     (UART_CONTROL1_M0_M)
#define UART_CONTROL1_M_7BIT_M     (UART_CONTROL1_M1_M)
#define UART_CONTROL1_M_M          (UART_CONTROL1_M1_M | UART_CONTROL1_M0_M)
#define UART_CONTROL1_M(V)         (((((v) / 10) << UART_CONTROL1_M1_S) | (((v) % 10) << UART_CONTROL1_M0_S)) & UART_CONTROL1_M_M)

#define UART_CONTROL1_PCE_S        10
/**
 * Контроль четности:
 * 0 – контроль четности выключен;
 * 1 – контроль четности включен.
 * @warning Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL1_PCE_M        (1 << UART_CONTROL1_PCE_S)

#define UART_CONTROL1_PS_S         9
/**
 * Выбор способа формирования бита четности:
 * 0 – бит четности;
 * 1 – бит нечетности.
 * @warning Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL1_PS_M         (1 << UART_CONTROL1_PS_S)

#define UART_CONTROL1_PEIE_S       8
/**
 * Управление прерыванием при ошибке в принятом бите четности:
 * 0 – прерывание по ошибке в бите четности выключено;
 * 1 – прерывание по ошибке в бите четности включено
 */
#define UART_CONTROL1_PEIE_M       (1 << UART_CONTROL1_PEIE_S)

#define UART_CONTROL1_TXEIE_S      7
/**
 * Управление прерыванием при передаче данных в момент записи данных в сдвиговый регистр:
 * 0 – прерывание выключено;
 * 1 – прерывание включено
 */
#define UART_CONTROL1_TXEIE_M      (1 << UART_CONTROL1_TXEIE_S)

#define UART_CONTROL1_TCIE_S       6
/**
 * Управление прерыванием при успешной передаче данных:
 * 0 – прерывание при передаче данных выключено;
 * 1 – прерывание при передаче данных включено
 */
#define UART_CONTROL1_TCIE_M       (1 << UART_CONTROL1_TCIE_S)

#define UART_CONTROL1_RXNEIE_S     5
/**
 * Управление прерыванием при успешном приеме данных или перезаписи полученных данных:
 * 0 – прерывание выключено;
 * 1 – прерывание включено
 */
#define UART_CONTROL1_RXNEIE_M     (1 << UART_CONTROL1_RXNEIE_S)

#define UART_CONTROL1_IDLEIE_S     4
/**
 * Управление прерыванием при отсутствии входных транзакций:
 * 0 – прерывание выключено;
 * 1 – прерывание включено
 */
#define UART_CONTROL1_IDLEIE_M     (1 << UART_CONTROL1_IDLEIE_S)

#define UART_CONTROL1_TE_S         3
/**
 * Управление передатчиком:
 * 0 – передатчик выключен;
 * 1 – передатчик включен
 */
#define UART_CONTROL1_TE_M         (1 << UART_CONTROL1_TE_S)

#define UART_CONTROL1_RE_S         2
/**
 * Управление приемником:
 * 0 – приемник выключен;
 * 1 – приемник включен
 */
#define UART_CONTROL1_RE_M         (1 << UART_CONTROL1_RE_S)

#define UART_CONTROL1_UE_S         0
/**
 * Ресет USART. Отменяет все текущие операции. Не очищает регистры конфигурации, но сбрасывает флаги
 * 0 – USART выключен
 * 1 – USART включен
 */
#define UART_CONTROL1_UE_M         (1 << UART_CONTROL1_UE_S)

//  Регистр управления 2 CONTROL2

#define UART_CONTROL2_MSBFIRST_S   19
/**
 * Управление очередностью приема и передачи данных:
 * 0 - начинать с 0 бита;
 * 1 - начинать с 9, 8 или 7 в зависимости от настроек длины посылки.
 * 
 * Этот бит не влияет на передачу и прием бита четности.
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_MSBFIRST_M   (1 << UART_CONTROL2_MSBFIRST_S)

#define UART_CONTROL2_DATAINV_S    18
/**
 * Управление полярностью принимаемых и передаваемых данных:
 * 0 – обычная полярность;
 * 1 – обратная полярность (1=L, 0=H).
 * 
 * Изменение полярности также влияет и на бит четности. 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_DATAINV_M    (1 << UART_CONTROL2_DATAINV_S)

#define UART_CONTROL2_TXINV_S      17
/**
 * Управление полярностью TX выхода. Регистр влияет не только на данные, но и на стоп биты, старт биты и тд.:
 * 0 – обычная полярность;
 * 1 – обратная полярность (1=L, 0=H).
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_TXINV_M      (1 << UART_CONTROL2_TXINV_S)

#define UART_CONTROL2_RXINV_S      16
/**
 * Управление полярностью RX входа. Регистр влияет не только на данные, но и на стоп биты, старт биты и тд.:
 * 0 – обычная полярность;
 * 1 – обратная полярность (1=L, 0=H).
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_RXINV_M      (1 << UART_CONTROL2_RXINV_S)

#define UART_CONTROL2_SWAP_S       15
/**
 * Регистр отвечает за обмен функциями TX выхода и RX входа:
 * 0 – обычное функционирование;
 * 1 – обмен функциями (TX = RX и RX = TX).
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_SWAP_M       (1 << UART_CONTROL2_SWAP_S)

#define UART_CONTROL2_LBM_S        14
/**
 * Регистр режима обратной внутренней петли:
 * 0 – обычное функционирование;
 * 1 – сигналы TX и RTS подаются внутрь блока в обход входов RX и CTS.
 * 
 * При этом TX = 1 и RTS = 1, входы RX и CTS не активны.
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_LBM_M        (1 << UART_CONTROL2_LBM_S)

#define UART_CONTROL2_STOP_1_S     13
/**
 * Количество стоп битов для приемника и передатчика:
 * 0 – 1 стоп бит;
 * 1 – 2 стоп бита.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_STOP_1_M     (1 << UART_CONTROL2_STOP_1_S)

#define UART_CONTROL2_CLKEN_S      11
/**
 * Управление синхронным режимом:
 * 0 – синхронный режим выключен;
 * 1 – синхронный режим включен.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_CLKEN_M      (1 << UART_CONTROL2_CLKEN_S)

#define UART_CONTROL2_CPOL_S       10
/**
 * Управление полярностью выходного тактового сигнала CK:
 * 0 – логический «0» вне транзакции;
 * 1 – логическая «1» вне транзакции.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_CPOL_M       (1 << UART_CONTROL2_CPOL_S)

#define UART_CONTROL2_CPHA_S       9
/**
 * Управление фазой выходного тактового сигнала CK:
 * 0 – первое изменение тактового сигнала – фронт;
 * 1 – первое изменение тактового сигнала – срез (обратный фронт).
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_CPHA_M       (1 << UART_CONTROL2_CPHA_S)

#define UART_CONTROL2_LBCL_S       8
/**
 * Управление последним тактовым импульсом:
 * 0 – последний тактовый импульс отсутствует;
 * 1 – последний тактовый импульс присутствует.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL2_LBCL_M       (1 << UART_CONTROL2_LBCL_S)

#define UART_CONTROL2_LBDIE_S      6    
/**
 * Управление прерыванием при обнаружении break состояния на RX линии:
 * 0 – прерывание выключено;
 * 1 – прерывание включено
 */
#define UART_CONTROL2_LBDIE_M      (1 << UART_CONTROL2_LBDIE_S)

// Регистр управления 3 CONTROL3

#define UART_CONTROL3_OVRDIS_S     12
/**
 * Выбор реакции на переполнение:
 * 0 – при переполнении взводится флаг ошибки (ORE), новые данные не записываются;
 * 1 – флаг переполнения не взводится, данные перезаписываются.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL3_OVRDIS_M     (1 << UART_CONTROL3_OVRDIS_S)

#define UART_CONTROL3_CTSIE_S      10
/**
 * Управление прерыванием при обнаружении изменения CTS:
 * 0 – прерывание выключено;
 * 1 – прерывание включено
 */
#define UART_CONTROL3_CTSIE_M      (1 << UART_CONTROL3_CTSIE_S)

#define UART_CONTROL3_CTSE_S       9
/**
 * Выбор реакции на входной сигнал CTS:
 * 0 – сигнал игнорируется;
 * 1 – сигнал управляет передачей данных трансмиттером. Передача разрешена при (CTS = 0).
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL3_CTSE_M       (1 << UART_CONTROL3_CTSE_S)

#define UART_CONTROL3_RTSE_S       8
/**
 * Управление выходным сигналом RTS:
 * 0 – сигнал всегда в разрешающем состоянии (RTS = 0);
 * 1 – сигнал находится в разрешающем состоянии (RTS = 0), только когда приемник готов принять данные.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL3_RTSE_M       (1 << UART_CONTROL3_RTSE_S)

#define UART_CONTROL3_DMAT_S       7
/**
 * Управление работой сигнала dma_trq:
 * 0 – сигнал выключен;
 * 1 – сигнал включен 
 */
#define UART_CONTROL3_DMAT_M       (1 << UART_CONTROL3_DMAT_S)

#define UART_CONTROL3_DMAR_S       6
/**
 * Управление работой сигнала dma_rrq:
 * 0 – сигнал выключен;
 * 1 – сигнал включен
 */
#define UART_CONTROL3_DMAR_M       (1 << UART_CONTROL3_DMAR_S)

#define UART_CONTROL3_HDSEL_S      3
/**
 * Выбор между полудуплексным и дуплексным режимами работы. В полудуплексном режиме RX 
 * не используется, все данные передаются и принимаются через TX. При наличии данных 
 * на передачу происходит отправка данных в остальное время прием:
 * 0 – дуплексный режим работы;
 * 1 – полудуплексный режим работы.
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_CONTROL3_HDSEL_M      (1 << UART_CONTROL3_HDSEL_S)

#define UART_CONTROL3_BKRQ_S       2
/**
 * Переход передатчика в break состояние:
 * 0 – обычный режим работы;
 * 1 – состояние break на линии TX
 */
#define UART_CONTROL3_BKRQ_M       (1 << UART_CONTROL3_BKRQ_S)

// TODO: В документации указано, что бит [1] - резерв
// #define UART_CONTROL3_IREN_S       1
// #define UART_CONTROL3_IREN_M       (1 << UART_CONTROL3_IREN_S)

#define UART_CONTROL3_EIE_S        0
/**
 * Управление прерыванием при обнаружении ошибок приема данных (FE, ORE, NF):
 * 0 – прерывание выключено;
 * 1 – прерывание включено
 */
#define UART_CONTROL3_EIE_M        (1 << UART_CONTROL3_EIE_S)

// Регистр делителя DIVIDER

#define UART_DIVIDER_BRR_S        0
/**
 * Значение делителя входного тактового сигнала (clk_in). При BRR < 16 USART не активен
 * Бодрейт рассчитывается по следующей формуле:
 * BR = F/BRR, где BR - бодрейт; F - значение частоты входного тактового сигнала
 * 
 * Этот бит может быть изменен только при остановке работы (UE=0)
 */
#define UART_DIVIDER_BRR_M        (0xFFFF << UART_DIVIDER_BRR_S)

// Регистр флагов и прерываний FLAGS

#define UART_FLAGS_REACK_S         22
/**
 * Флаг готовности приемника к работе после снятия сброса (UE и RE):
 * 0 – ресет активен;
 * 1 – ресет снят
 */
#define UART_FLAGS_REACK_M         (1 << UART_FLAGS_REACK_S)

#define UART_FLAGS_TEACK_S         21
/**
 * Флаг готовности передатчика к работе после снятия сброса (UE и TE):
 * 0 – ресет активен;
 * 1 – ресет снят
 */
#define UART_FLAGS_TEACK_M         (1 << UART_FLAGS_TEACK_S)

#define UART_FLAGS_BUSY_S          16
/**
 * Флаг активности на линии RX:
 * 0 – нет активности на линии RX;
 * 1 – идет передача данных или break состояние
 */
#define UART_FLAGS_BUSY_M          (1 << UART_FLAGS_BUSY_S)

#define UART_FLAGS_CTS_S           10
/**
 * Флаг отображает текущее значение сигнала CTS:
 * 0 – CTS = 0;
 * 1 – CTS = 1
 */
#define UART_FLAGS_CTS_M           (1 << UART_FLAGS_CTS_S)

#define UART_FLAGS_CTSIF_S         9
/**
 * Флаг взводится при изменении значения сигнала CTS. Флаг сбрасывается записью 1:
 * 0 – состояние CTS не менялось с момента сброса флага;
 * 1 – состояние CTS менялось с момента сброса флага
 */
#define UART_FLAGS_CTSIF_M         (1 << UART_FLAGS_CTSIF_S)

#define UART_FLAGS_LBDF_S          8
/**
 * Флаг обнаружения break состояния на линии RX. Флаг сбрасывается записью 1:
 * 0 – состояние break не обнаружено с момента сброса флага;
 * 1 – состояние break обнаружено с момента сброса флага
 */
#define UART_FLAGS_LBDF_M          (1 << UART_FLAGS_LBDF_S)

#define UART_FLAGS_TXE_S           7
/**
 * Флаг взводится при передаче данных в момент записи данных в сдвиговый регистр. 
 * Флаг сбрасывается записью данных в регистр TDR или записью 1:
 * 0 – данные не записывались в сдвиговый регистр с момента сброса флага;
 * 1 – данные записывались в сдвиговый регистр
 */
#define UART_FLAGS_TXE_M           (1 << UART_FLAGS_TXE_S)

#define UART_FLAGS_TC_S            6
/**
 * Флаг взводится при передаче данных в момент отправки последнего стоп бита. Флаг сбрасывается записью 1:
 * 0 – данные не отправлялись с момента сброса флага;
 * 1 – данные отправлялись с момента сброса флага
 */
#define UART_FLAGS_TC_M            (1 << UART_FLAGS_TC_S)

#define UART_FLAGS_RXNE_S          5
/**
 * Флаг взводится при записи принятых данных в регистр RDR из сдвигового регистра. 
 * Флаг сбрасывается записью 1 или чтением регистра RDR:
 * 0 – данные не принимались после сброса флага;
 * 1 – данные принимались после сброса флаг
 */
#define UART_FLAGS_RXNE_M          (1 << UART_FLAGS_RXNE_S)

#define UART_FLAGS_IDLE_S          4
/**
 * Флаг взводится при отсутствии активности на линии RX в течении 8 битовых тактов при взведенном флаге RXNE. 
 * Флаг сбрасывается записью 1:
 * 0 – состояние idle не обнаружено после сброса флага;
 * 1 – состояние idle обнаружено после сброса флага
 */
#define UART_FLAGS_IDLE_M          (1 << UART_FLAGS_IDLE_S)

#define UART_FLAGS_ORE_S           3
/**
 * Флаг взводится при попытке перезаписи RDR. Флаг сбрасывается записью 1:
 * 0 – попыток перезаписи не обнаружено с момента сброса флага;
 * 1 – попытка перезаписи обнаружена с момента сброса флага
 */
#define UART_FLAGS_ORE_M           (1 << UART_FLAGS_ORE_S)

#define UART_FLAGS_NF_S            2
/**
 * Флаг обнаружения ложных переключений на линии RX. Флаг сбрасывается записью 1:
 * 0 – помех не обнаружено с момента сброса флага;
 * 1 – помехи обнаружены с момента сброса флага
 */
#define UART_FLAGS_NF_M            (1 << UART_FLAGS_NF_S)

#define UART_FLAGS_FE_S            1
/**
 * Флаг взводится при обнаружении ошибок в стоп бите или битах. Флаг сбрасывается записью 1:
 * 0 – ошибок приема не обнаружено с момента сброса флага;
 * 1 – ошибка приема обнаружена с момента сброса флага
 */
#define UART_FLAGS_FE_M            (1 << UART_FLAGS_FE_S)

#define UART_FLAGS_PE_S            0
/**
 * Флаг обнаружения ошибочного бита четности. Флаг сбрасывается записью 1:
 * 0 – ошибок в битах четности не обнаружено с момента сброса флага;
 * 1 – ошибка в бите четности обнаружена с момента сброса флага
 */
#define UART_FLAGS_PE_M            (1 << UART_FLAGS_PE_S)

/**
 * Регистр принятых данных RXDATA
 * [R]: RDR [8:0] - принятые данные
 */

/**
 * Регистр передаваемых данных TXDATA
 * [RW]: TDR [8:0] - передаваемые данные
 */

// Регистр модемных сигналов MODEM

#define UART_MODEM_DTR_S           8
/**
 * Регистр управления сигналом DTR:
 * 0 – отсутствие сигнала готовности;
 * 1 – готовность к приему данных
 */
#define UART_MODEM_DTR_M           (1 << UART_MODEM_DTR_S)

#define UART_MODEM_DCD_S           7
/**
 * Регистр содержит текущее значение сигнала DCD:
 * 0 – отсутствие несущей;
 * 1 – обнаружение несущей
 */
#define UART_MODEM_DCD_M           (1 << UART_MODEM_DCD_S)

#define UART_MODEM_RI_S            6
/**
 * Регистр содержит текущее значение сигнала RI:
 * 0 – отсутствие звонка;
 * 1 – звонок (вызов) на телефонной линии
 */
#define UART_MODEM_RI_M            (1 << UART_MODEM_RI_S)

#define UART_MODEM_DSR_S           5
/**
 * Регистр содержит текущее значение сигнала DSR:
 * 0 – источник данных не готов;
 * 1 – источник данных готов
 */
#define UART_MODEM_DSR_M           (1 << UART_MODEM_DSR_S)

#define UART_MODEM_DCDIF_S         3
/**
 * Флаг взводится при изменении входного сигнала DCD. Флаг сбрасывается записью 1:
 * 0 – изменений сигнала не обнаружено с момента сброса флага;
 * 1 – сигнал изменен с момента сброса флага
 */
#define UART_MODEM_DCDIF_M         (1 << UART_MODEM_DCDIF_S)

#define UART_MODEM_RIIF_S          2
/**
 * Флаг взводится при изменении входного сигнала RI с 0 на 1. Флаг сбрасывается записью 1:
 * 0 – изменений сигнала с 0 на 1 не обнаружено с момента сброса флага;
 * 1 – сигнал изменен с момента сброса флага
 */
#define UART_MODEM_RIIF_M          (1 << UART_MODEM_RIIF_S)

#define UART_MODEM_DSRIF_S         1
/**
 * Флаг взводится при изменении входного сигнала DSR. Флаг сбрасывается записью 1:
 * 0 – изменений сигнала не обнаружено с момента сброса флага;
 * 1 – сигнал изменен с момента сброса флага
 */
#define UART_MODEM_DSRIF_M         (1 << UART_MODEM_DSRIF_S)


#ifndef __ASSEMBLER__
    #include <inttypes.h>

    /// @brief Структура регистров приёмо-передатчика USART
    typedef struct
    {  
        volatile uint32_t CONTROL1;             ///< Регистр управления 1
        volatile uint32_t CONTROL2;             ///< Регистр управления 2
        volatile uint32_t CONTROL3;             ///< Регистр управления 3
        volatile uint32_t DIVIDER;              ///< Регистр настройки делителя
        volatile uint32_t _reserved0;           ///< Резерв
        volatile uint32_t _reserved1;           ///< Резерв
        volatile uint32_t _reserved2;           ///< Резерв
        volatile uint32_t FLAGS;                ///< Регистр прерываний
        volatile uint32_t _reserved3;           ///< Резерв
        volatile uint32_t RXDATA;               ///< Регистр принятых данных
        volatile uint32_t TXDATA;               ///< Регистр передаваемых данных
        volatile uint32_t MODEM;                ///< Регистр управления модемом
        
    } UART_TypeDef;
#endif

#endif // UART_H_INCLUDED


