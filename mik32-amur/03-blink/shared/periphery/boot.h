/**
 * @file
 * Менеджер загрузки
 * 
 * Обеспечивает управление выбранным источником загрузки (SPI FLASH, ОЗУ, EEPROM);
 */

#ifndef BOOT_MANAGER_H_INCLUDED
#define BOOT_MANAGER_H_INCLUDED

#define BOOT_MANAGER_BOOT_OFFSET        0x00

// Перечень значений регистра управления источником загрузки (BOOT)

#define BOOT_EEPROM                  0          ///< Загрузка из встроенной ЭСППЗУ (EEPROM).
#define BOOT_RAM                     1          ///< Загрузка из ОЗУ (RAM).
#define BOOT_SPIFI                   2          ///< Загрузка из внешнего устройства памяти (SPIFI).

#ifndef __ASSEMBLER__
    #include <inttypes.h> 
    /// @brief Структура регистров менеджера загрузки
    typedef struct
    {   
        volatile uint32_t BOOT;     ///< [RW] Регистр управления источником загрузки. Биты [1:0] кодируют источник загрузки.
    } BOOT_MANAGER_TypeDef;
#endif

#endif 

