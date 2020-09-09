cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/rendering/shader_graph/ShaderGraph.h
    engine/rendering/shader_graph/ShaderGraph.cpp

    engine/rendering/shader_graph/ShaderGraphNode.h
    engine/rendering/shader_graph/ShaderGraphNode.cpp

    engine/rendering/shader_graph/ShaderGraphNodesFunctions.h
    engine/rendering/shader_graph/ShaderGraphNodesFunctions.cpp

    engine/rendering/shader_graph/ShaderGraphNodesPrimitives.h
    engine/rendering/shader_graph/ShaderGraphNodesPrimitives.cpp

    PARENT_SCOPE
)
