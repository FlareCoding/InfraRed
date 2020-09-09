cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/scene/Grass.h
    engine/scene/Grass.cpp

    engine/scene/Light.h

    engine/scene/Scene.h
    engine/scene/Scene.cpp

    engine/scene/Skybox.h
    engine/scene/Skybox.cpp

    engine/scene/Terrain.h
    engine/scene/Terrain.cpp

    engine/scene/Water.h
    engine/scene/Water.cpp

    PARENT_SCOPE
)
