cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/audio/AudioSource.h
    engine/audio/AudioSource.cpp
    engine/audio/AudioEngine.h
    engine/audio/AudioEngine.cpp

    PARENT_SCOPE
)
