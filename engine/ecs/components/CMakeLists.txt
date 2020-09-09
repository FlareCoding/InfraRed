cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/ecs/components/TransformComponent.h
    engine/ecs/components/TransformComponent.cpp

    engine/ecs/components/StaticRenderComponent.h
    engine/ecs/components/StaticRenderComponent.cpp

    engine/ecs/components/AnimatedRenderComponent.h
    engine/ecs/components/AnimatedRenderComponent.cpp

    engine/ecs/components/ColliderComponent.h
    engine/ecs/components/ColliderComponent.cpp

    engine/ecs/components/InformationComponent.h
    engine/ecs/components/InformationComponent.cpp

    engine/ecs/components/ScriptComponent.h
    engine/ecs/components/ScriptComponent.cpp

    PARENT_SCOPE
)
