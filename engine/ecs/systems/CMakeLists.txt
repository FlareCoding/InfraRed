cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/ecs/systems/PhysicsSystem.h
    engine/ecs/systems/PhysicsSystem.cpp

    engine/ecs/systems/RenderComponentSystem.h
    engine/ecs/systems/RenderComponentSystem.cpp

    engine/ecs/systems/ScriptingSystem.h
    engine/ecs/systems/ScriptingSystem.cpp

    PARENT_SCOPE
)
