cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC
    core/Include
    peripherals/Include
    utilities/Include
)

target_sources(${PROJECT_NAME} PRIVATE
    core/Source/mik32_hal_scr1_timer.c

    peripherals/Source/mik32_hal.c
    peripherals/Source/mik32_hal_adc.c
    peripherals/Source/mik32_hal_crc32.c
    peripherals/Source/mik32_hal_crypto.c
    peripherals/Source/mik32_hal_dac.c
    peripherals/Source/mik32_hal_dma.c
    peripherals/Source/mik32_hal_eeprom.c
    peripherals/Source/mik32_hal_gpio.c
    peripherals/Source/mik32_hal_i2c.c
    peripherals/Source/mik32_hal_irq.c
    peripherals/Source/mik32_hal_otp.c
    peripherals/Source/mik32_hal_pcc.c
    peripherals/Source/mik32_hal_rtc.c
    peripherals/Source/mik32_hal_spi.c
    peripherals/Source/mik32_hal_spifi.c
    peripherals/Source/mik32_hal_timer16.c
    peripherals/Source/mik32_hal_timer32.c
    peripherals/Source/mik32_hal_tsens.c
    peripherals/Source/mik32_hal_wdt.c

#    utilities/Source/mik32_hal_spifi_psram.c
    utilities/Source/mik32_hal_spifi_w25.c
    utilities/Source/mik32_hal_ssd1306.c
)
