/**
 * @file
 * Карта памяти МК Амур
 * 
 * Описывает распределение адресного пространства между периферийными устройствами и модулями памяти
 
+=============+==========================================+
| Смещение    | Устройство                               |
+=============+==========================================+
| 0x0000_0000 | Загрузчик (16Кб)                         |
+-------------+------------------------------------------+
| 0x0004_0000 | DMA_Config                               |
+-------------+------------------------------------------+
| 0x0005_0000 | Устройства на шине APB_M                 |
+-------------+------------------------------------------+
|     +0x0000 | Блок управления питанием и тактированием |
+-------------+------------------------------------------+
|     +0x0400 | Контроллер прерываний                    |
+-------------+------------------------------------------+
|     +0x0800 | Таймер32_0                               |
+-------------+------------------------------------------+
|     +0x0C00 | Контроллер выводов                       |
+-------------+------------------------------------------+
|     +0x1000 | Сторожевой таймер шины                   |
+-------------+------------------------------------------+
|     +0x1400 | OTP                                      |
+-------------+------------------------------------------+
|     +0x1800 | Монитор напряжения                       |
+-------------+------------------------------------------+
| 0x0006_0000 | Устройства батарейного домена            |
+-------------+------------------------------------------+
|      +0x000 | WakeUp                                   |
+-------------+------------------------------------------+
|      +0x400 | RTC                                      |
+-------------+------------------------------------------+
| 0x0007_0000 | Устройства подсистемы памяти             |
+-------------+------------------------------------------+
|      +0x000 | Регистры контроллера SPIFI               |
+-------------+------------------------------------------+
|      +0x400 | Регистры контроллера EEPROM              |
+-------------+------------------------------------------+
| 0x0008_0000 | Устройства на шине AHB_P                 |
+-------------+------------------------------------------+
|      +0x000 | Ускоритель симметричной криптографии     |
+-------------+------------------------------------------+
|      +0x400 | CRC32                                    |
+-------------+------------------------------------------+
| 0x0008_1000 | Устройства на шине APB_P                 |
+-------------+------------------------------------------+
|     +0x0000 | Сторожевой таймер                        |
+-------------+------------------------------------------+
|     +0x0400 | USART_0                                  |
+-------------+------------------------------------------+
|     +0x0800 | USART_1                                  |
+-------------+------------------------------------------+
|     +0x0C00 | TIMER16_0                                |
+-------------+------------------------------------------+
|     +0x1000 | TIMER16_1                                |
+-------------+------------------------------------------+
|     +0x1400 | TIMER16_2                                |
+-------------+------------------------------------------+
|     +0x1800 | TIMER32_1                                |
+-------------+------------------------------------------+
|     +0x1С00 | TIMER32_2                                |
+-------------+------------------------------------------+
|     +0x2000 | SPI_0                                    |
+-------------+------------------------------------------+
|     +0x2400 | SPI_1                                    |
+-------------+------------------------------------------+
|     +0x2800 | I2C_0                                    |
+-------------+------------------------------------------+
|     +0x2C00 | I2C_1                                    |
+-------------+------------------------------------------+
|     +0x3000 | GPIO16_0                                 |
+-------------+------------------------------------------+
|     +0x3400 | GPIO16_1                                 |
+-------------+------------------------------------------+
|     +0x3800 | GPIO8_2                                  |
+-------------+------------------------------------------+
|     +0x3С00 | Модуль формирования прерываний GPIO      |
+-------------+------------------------------------------+
|     +0x4000 | Блок управления аналоговой подсистемой   |
+-------------+------------------------------------------+
| 0x0100_0000 | EEPROM (8Кб)                             |
+-------------+------------------------------------------+
| 0x0200_0000 | SRAM (16Кб)                              |
+-------------+------------------------------------------+
| 0x8000_0000 | Контроллер SPIFI (прямой доступ до 2 Гб) |
+-------------+------------------------------------------+

*/

#ifndef MIK32_MEMORY_MAP_H_INCLUDED
#define MIK32_MEMORY_MAP_H_INCLUDED

#define EEPROM_BASE_ADDRESS                 0x01000000
#define EEPROM_SIZE                         (8*1024)

#define RAM_BASE_ADDRESS                    0x02000000
#define RAM_SIZE                            (16*1024)

#define SPIFI_BASE_ADDRESS                  0x80000000
#define SPIFI_SIZE                          (16*1024*1024)

#define BOOT_BASE_ADDRESS                   0x00000000
#define BOOT_SIZE                           (16*1024)

#define DMA_CONFIG_BASE_ADDRESS             0x00040000

#define PM_BASE_ADDRESS                     0x00050000
#define EPIC_BASE_ADDRESS                   0x00050400
#define TIMER32_0_BASE_ADDRESS              0x00050800
#define PAD_CONFIG_BASE_ADDRESS             0x00050c00
#define WDT_BUS_BASE_ADDRESS                0x00051000
#define OTP_BASE_ADDRESS                    0x00051400
#define PVD_CONTROL_BASE_ADDRESS            0x00051800

#define WU_BASE_ADDRESS                     0x00060000
#define RTC_BASE_ADDRESS                    0x00060400
#define BOOT_MANAGER_BASE_ADDRESS           0x00060800

#define SPIFI_CONFIG_BASE_ADDRESS           0x00070000
#define EEPROM_REGS_BASE_ADDRESS            0x00070400

#define CRYPTO_BASE_ADDRESS                 0x00080000
#define CRC_BASE_ADDRESS                    0x00080400

#define WDT_BASE_ADDRESS                    0x00081000
#define UART_0_BASE_ADDRESS                 0x00081400
#define UART_1_BASE_ADDRESS                 0x00081800
#define TIMER16_0_BASE_ADDRESS              0x00081C00
#define TIMER16_1_BASE_ADDRESS              0x00082000
#define TIMER16_2_BASE_ADDRESS              0x00082400
#define TIMER32_1_BASE_ADDRESS              0x00082800
#define TIMER32_2_BASE_ADDRESS              0x00082C00
#define SPI_0_BASE_ADDRESS                  0x00083000
#define SPI_1_BASE_ADDRESS                  0x00083400
#define I2C_0_BASE_ADDRESS                  0x00083800
#define I2C_1_BASE_ADDRESS                  0x00083C00
#define GPIO_0_BASE_ADDRESS                 0x00084000
#define GPIO_1_BASE_ADDRESS                 0x00084400
#define GPIO_2_BASE_ADDRESS                 0x00084800
#define GPIO_IRQ_BASE_ADDRESS               0x00084C00
#define ANALOG_REG_BASE_ADDRESS             0x00085000

// Встроенный в ядро SCR1 64-битный таймер

#define SCR1_TIMER_BASE_ADDRESS             0x00490000

// Объявление структур, описывающих регистры встроенных блоков, модулей, контроллеров и периферийных устройств

#define DMA_CONFIG      (( DMA_CONFIG_TypeDef       *)  DMA_CONFIG_BASE_ADDRESS     )

#define PM              (( PM_TypeDef               *)  PM_BASE_ADDRESS             ) 
#define EPIC            (( EPIC_TypeDef             *)  EPIC_BASE_ADDRESS           )     
#define TIMER32_0       (( TIMER32_TypeDef          *)  TIMER32_0_BASE_ADDRESS      ) 
#define PVD_CONTROL     (( PVD_CONTROL_TypeDef      *)  PVD_CONTROL_BASE_ADDRESS    ) 
#define PAD_CONFIG      (( PAD_CONFIG_TypeDef       *)  PAD_CONFIG_BASE_ADDRESS     ) 
#define WDT_BUS         (( WDT_BUS_TypeDef          *)  WDT_BUS_BASE_ADDRESS        ) 
#define OTP             (( OTP_TypeDef              *)  OTP_BASE_ADDRESS            )

#define WU              (( WU_TypeDef               *)  WU_BASE_ADDRESS             ) 
#define RTC             (( RTC_TypeDef              *)  RTC_BASE_ADDRESS            )
#define BOOT_MANAGER    (( BOOT_MANAGER_TypeDef     *)  BOOT_MANAGER_BASE_ADDRESS   )

#define SPIFI_CONFIG    (( SPIFI_CONFIG_TypeDef     *)  SPIFI_CONFIG_BASE_ADDRESS   )
#define EEPROM_REGS     (( EEPROM_REGS_TypeDef      *)  EEPROM_REGS_BASE_ADDRESS    )
#define CRYPTO          (( CRYPTO_TypeDef           *)  CRYPTO_BASE_ADDRESS         )
#define CRC             (( CRC_TypeDef              *)  CRC_BASE_ADDRESS            )

#define WDT             (( WDT_TypeDef              *)  WDT_BASE_ADDRESS            )
#define UART_0          (( UART_TypeDef             *)  UART_0_BASE_ADDRESS         )
#define UART_1          (( UART_TypeDef             *)  UART_1_BASE_ADDRESS         )
#define TIMER16_0       (( TIMER16_TypeDef          *)  TIMER16_0_BASE_ADDRESS      )
#define TIMER16_1       (( TIMER16_TypeDef          *)  TIMER16_1_BASE_ADDRESS      )
#define TIMER16_2       (( TIMER16_TypeDef          *)  TIMER16_2_BASE_ADDRESS      )
#define TIMER32_1       (( TIMER32_TypeDef          *)  TIMER32_1_BASE_ADDRESS      )
#define TIMER32_2       (( TIMER32_TypeDef          *)  TIMER32_2_BASE_ADDRESS      )
#define SPI_0           (( SPI_TypeDef              *)  SPI_0_BASE_ADDRESS          )
#define SPI_1           (( SPI_TypeDef              *)  SPI_1_BASE_ADDRESS          )
#define I2C_0           (( I2C_TypeDef              *)  I2C_0_BASE_ADDRESS          )
#define I2C_1           (( I2C_TypeDef              *)  I2C_1_BASE_ADDRESS          )
#define GPIO_0          (( GPIO_TypeDef             *)  GPIO_0_BASE_ADDRESS         )
#define GPIO_1          (( GPIO_TypeDef             *)  GPIO_1_BASE_ADDRESS         )
#define GPIO_2          (( GPIO_TypeDef             *)  GPIO_2_BASE_ADDRESS         )
#define GPIO_IRQ        (( GPIO_IRQ_TypeDef         *)  GPIO_IRQ_BASE_ADDRESS       )
#define ANALOG_REG      (( ANALOG_REG_TypeDef       *)  ANALOG_REG_BASE_ADDRESS     )

// Объявление структуры регистров встроенного в ядро SCR1 64-битного таймера

#define SCR1_TIMER      (( SCR1_TIMER_TypeDef       *)  SCR1_TIMER_BASE_ADDRESS     )

#endif // MIK32_MEMORY_MAP_H_INCLUDED
