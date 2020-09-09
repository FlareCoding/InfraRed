cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/imgui/imconfig.h
    engine/imgui/imgui_internal.h
    engine/imgui/imstb_rectpack.h
    engine/imgui/imstb_textedit.h
    engine/imgui/imstb_truetype.h
    engine/imgui/imgui_draw.cpp
    engine/imgui/imgui_widgets.cpp

    engine/imgui/imgui.h
    engine/imgui/imgui.cpp

    engine/imgui/imgui_impl_dx11.h
    engine/imgui/imgui_impl_dx11.cpp

    engine/imgui/imgui_impl_win32.h
    engine/imgui/imgui_impl_win32.cpp

    PARENT_SCOPE
)
