cmake_minimum_required(VERSION 3.10)

add_subdirectory(components)
add_subdirectory(systems)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/ecs/IComponent.h
    engine/ecs/Components.h

    engine/ecs/ComponentManager.h
    engine/ecs/ComponentManager.cpp

    engine/ecs/Entity.h
    engine/ecs/Entity.cpp

    PARENT_SCOPE
)
