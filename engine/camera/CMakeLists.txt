cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/camera/ICamera.h
    engine/camera/ICamera.cpp

    engine/camera/FPSCamera.h
    engine/camera/FPSCamera.cpp

    engine/camera/OrthographicCamera.h
    engine/camera/OrthographicCamera.cpp

    engine/camera/ThirdPersonCamera.h
    engine/camera/ThirdPersonCamera.cpp

    engine/camera/FreeRoamCamera.h
    engine/camera/FreeRoamCamera.cpp

    PARENT_SCOPE
)
