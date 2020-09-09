cmake_minimum_required(VERSION 3.10)

add_subdirectory(shader_graph)
add_subdirectory(shaders)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/rendering/BufferLayout.h
    engine/rendering/BufferLayout.cpp

    engine/rendering/FrameBuffer.h
    engine/rendering/FrameBuffer.cpp

    engine/rendering/ImGuiRenderer.h
    engine/rendering/ImGuiRenderer.cpp

    engine/rendering/IndexBuffer.h
    engine/rendering/IndexBuffer.cpp

    engine/rendering/Material.h
    engine/rendering/Material.cpp

    engine/rendering/Renderer.h
    engine/rendering/Renderer.cpp

    engine/rendering/Renderer2D.h
    engine/rendering/Renderer2D.cpp

    engine/rendering/Renderer3D.h
    engine/rendering/Renderer3D.cpp

    engine/rendering/RenderingAPI.h
    engine/rendering/RenderingAPI.cpp

    engine/rendering/Texture.h
    engine/rendering/Texture.cpp

    engine/rendering/VertexArray.h
    engine/rendering/VertexArray.cpp

    engine/rendering/VertexBuffer.h
    engine/rendering/VertexBuffer.cpp

    engine/rendering/RenderingContext.h
    engine/rendering/VertexData.h

    PARENT_SCOPE
)
