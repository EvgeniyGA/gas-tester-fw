cmake_minimum_required(VERSION 3.22)

target_sources(${PROJECT_NAME} PRIVATE
    Src/app.c
    Src/process_adc.c
    Src/wave_gen.c
    Src/wave_measure.c
    Src/wave_starter.c
    Src/syscalls.c
)