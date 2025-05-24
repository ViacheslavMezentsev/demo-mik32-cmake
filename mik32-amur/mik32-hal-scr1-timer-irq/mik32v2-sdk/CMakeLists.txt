cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/libs"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/periphery"

    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/core/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/utilities/Include"
)

target_sources(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/libs/dma_lib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/libs/rtc_lib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/libs/spi_lib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/libs/uart_lib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/libs/xprintf.c"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/shared/runtime/crt0.S"
    shared/runtime/crt0.S

    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/core/Source/mik32_hal_scr1_timer.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_adc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_crc32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_crypto.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_dac.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_dma.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_eeprom.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_gpio.c"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_i2c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_irq.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_otp.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_pcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_rtc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_spi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_spifi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_timer16.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_timer32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_tsens.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/peripherals/Source/mik32_hal_wdt.c"

    #"${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/utilities/Source/mik32_hal_spifi_psram.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/utilities/Source/mik32_hal_spifi_w25.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/framework-mik32v2-sdk/hal/utilities/Source/mik32_hal_ssd1306.c"
)
