cmake_minimum_required(VERSION 3.23)

project(Data)

add_executable(
    ${PROJECT_NAME}
)

set(fft_path_ ./Packages/FFTReal-2.11/ffft)

target_sources(
    ${PROJECT_NAME}
    PUBLIC
        FILE_SET    fft_ 
        TYPE        HEADERS
        BASE_DIRS   ./Packages/FFTReal-2.11
        FILES       ${fft_path_}/Array.h
                    ${fft_path_}/Array.hpp
                    ${fft_path_}/def.h
                    ${fft_path_}/DynArray.h
                    ${fft_path_}/DynArray.hpp
                    ${fft_path_}/FFTReal.h
                    ${fft_path_}/FFTReal.hpp
                    ${fft_path_}/OscSinCos.h
                    ${fft_path_}/OscSinCos.hpp
    PRIVATE
        FILE_SET    include_ 
        TYPE        HEADERS
        BASE_DIRS   ./include
        FILES       ./include/Alarm.h
                    ./include/Exceptions.h
                    ./include/Functions.h
                    ./include/Packet.h
                    ./include/Sequence.h
                    ./include/Spectrum.h
                    ./include/TimeHistory.h
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        ./src/Data.cpp
        ./src/Packet.cpp
)