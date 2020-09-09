cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/platform/rendering/DirectXAdapterReader.h
    engine/platform/rendering/DirectXAdapterReader.cpp

    engine/platform/rendering/DirectXContext.h
    engine/platform/rendering/DirectXContext.cpp

    engine/platform/rendering/DirectXFrameBuffer.h
    engine/platform/rendering/DirectXFrameBuffer.cpp

    engine/platform/rendering/DirectXIndexBuffer.h
    engine/platform/rendering/DirectXIndexBuffer.cpp

    engine/platform/rendering/DirectXRenderingAPI.h
    engine/platform/rendering/DirectXRenderingAPI.cpp

    engine/platform/rendering/DirectXTexture.h
    engine/platform/rendering/DirectXTexture.cpp

    engine/platform/rendering/DirectXVertexArray.h
    engine/platform/rendering/DirectXVertexArray.cpp

    engine/platform/rendering/DirectXVertexBuffer.h
    engine/platform/rendering/DirectXVertexBuffer.cpp

    engine/platform/rendering/shaders/DirectXShader.h
    engine/platform/rendering/shaders/DirectXShader.cpp

    engine/platform/rendering/shaders/DirectXShaderBuilder.h
    engine/platform/rendering/shaders/DirectXShaderBuilder.cpp

    engine/platform/rendering/shaders/DirectXUniformBuffer.h
    engine/platform/rendering/shaders/DirectXUniformBuffer.cpp

    PARENT_SCOPE
)
