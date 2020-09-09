cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/rendering/shaders/Shader.h
    engine/rendering/shaders/Shader.cpp

    engine/rendering/shaders/ShaderBuilder.h
    engine/rendering/shaders/ShaderBuilder.cpp

    engine/rendering/shaders/Shaders.h
    engine/rendering/shaders/Shaders.cpp

    engine/rendering/shaders/UniformBuffer.h
    engine/rendering/shaders/UniformBuffer.cpp

    engine/rendering/shaders/UniformBufferSlotMappings.h

    PARENT_SCOPE
)
