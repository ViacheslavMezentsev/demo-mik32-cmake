cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC include libs periphery)

target_sources(${PROJECT_NAME} PRIVATE
    libs/dma_lib.c
    libs/rtc_lib.c
    libs/spi_lib.c
    libs/uart_lib.c
    libs/xprintf.c
    runtime/crt0.S
)
