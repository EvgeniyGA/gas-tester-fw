cmake_minimum_required(VERSION 3.22)

project(gas-tester LANGUAGES C CXX ASM)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Targets/stm32f4/cmake/toolchain-arm-none-eabi.cmake")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # для VSCode
set(TARGET stm32f4)

set(MAIN_EXECUTABLE "gas-tester")
add_executable(${MAIN_EXECUTABLE})

file(GLOB_RECURSE APP_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/*.cpp"
)

target_sources(${MAIN_EXECUTABLE} PRIVATE ${APP_SOURCES})

target_include_directories(${MAIN_EXECUTABLE} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/App/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src"
)

set(TARGET_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Targets/${TARGET}/cmake/stm32cubemx")

add_subdirectory("App")
add_subdirectory("${TARGET_CMAKE_DIR}" "${CMAKE_BINARY_DIR}/stm32cubemx_${TARGET}")
#set(LINKER_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/Targets/${TARGET}/STM32F407XX_FLASH.ld")

# Очищаем старые флаги с -T и добавляем правильный путь
# Сначала удалим все вхождения -T из CMAKE_EXE_LINKER_FLAGS (если нужно)
#string(REGEX REPLACE "-T[ ]*\"[^\"]+\"" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

# Добавляем правильный линкер-скрипт к целевому исполняемому файлу
#target_link_options(${MAIN_EXECUTABLE} PRIVATE -T"${LINKER_SCRIPT_PATH}")

# === 3️⃣ Линкуем всё вместе ===
# После add_subdirectory у нас есть библиотеки: STM32_Drivers, USB_Device_Library, CMSIS-DSP
# Они уже прилинкованы к MAIN_EXECUTABLE внутри stm32cubemx/CMakeLists.txt
# Но если нужно добавить что-то своё — делай это здесь:
# target_link_libraries(${MAIN_EXECUTABLE} PRIVATE my_extra_lib)

# === 4️⃣ Пост-обработка: .bin, .hex, размер ===
add_custom_target(${MAIN_EXECUTABLE}_bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${MAIN_EXECUTABLE}.elf ${MAIN_EXECUTABLE}.bin
    DEPENDS ${MAIN_EXECUTABLE}.elf
    COMMENT "Generating .bin"
)

add_custom_target(${MAIN_EXECUTABLE}_hex
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${MAIN_EXECUTABLE}.elf ${MAIN_EXECUTABLE}.hex
    DEPENDS ${MAIN_EXECUTABLE}.elf
    COMMENT "Generating .hex"
)

add_custom_target(${MAIN_EXECUTABLE}_size
    COMMAND ${CMAKE_SIZE} ${MAIN_EXECUTABLE}.elf
    DEPENDS ${MAIN_EXECUTABLE}.elf
    COMMENT "Printing size"
)