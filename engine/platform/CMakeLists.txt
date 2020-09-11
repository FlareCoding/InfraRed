cmake_minimum_required(VERSION 3.10)

add_subdirectory(audio)
add_subdirectory(input)
add_subdirectory(rendering)
add_subdirectory(window)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/platform/DirectXResources.h
    engine/platform/DirectXResources.cpp

    PARENT_SCOPE
)
