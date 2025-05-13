cmake_minimum_required(VERSION 3.19)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc-riscv-none-elf.cmake)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# В качестве имени проекта используем имя папки (см. tasks.json и launch.json).
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)

add_executable(${PROJECT_NAME} main.c)

add_subdirectory(hal)
add_subdirectory(shared)
add_subdirectory(stubs)

target_link_libraries(${PROJECT_NAME} MIK32::Nano)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Опции сборки.
target_compile_definitions(${PROJECT_NAME} PRIVATE MIK32V2 BOARD_BLUEPILL_MIK32)

target_compile_options(${PROJECT_NAME} PRIVATE
    -march=rv32imc_zicsr_zifencei
    -mabi=ilp32
    -mcmodel=medlow
    # warning
    -W -Wall -Wextra
    -Wno-unused-parameter
    # optimization
    $<$<CONFIG:DEBUG>:-O2 -g3>
    $<$<CONFIG:RELEASE>:-Os>
    -ffunction-sections -fdata-sections
    # debug
    $<$<CONFIG:DEBUG>:-g3>
    #-Wa,-adhlns=${PROJECT_NAME}.lst
    # other
    -pipe
    # c/cxx flags
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -fno-exceptions -fno-rtti -funsigned-bitfields -fshort-enums -Winline>
)

target_link_options(${PROJECT_NAME} PRIVATE
    -nostartfiles
    -march=rv32imc_zicsr_zifencei
    -mabi=ilp32
    -mcmodel=medlow
    -lc -lgcc
    -T${CMAKE_CURRENT_SOURCE_DIR}/shared/ldscripts/ram.ld
    -Wl,-Map=${PROJECT_NAME}.map,--no-warn-rwx-segments,--cref,--gc-sections,--print-memory-usage
)

# Артефакты сборки.
mik32_generate_binary_file(${PROJECT_NAME})
mik32_generate_hex_file(${PROJECT_NAME})
mik32_generate_lss_file(${PROJECT_NAME})
mik32_print_size_of_target(${PROJECT_NAME})
