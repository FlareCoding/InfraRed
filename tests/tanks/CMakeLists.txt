cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME Tanks)

add_executable(
    ${TARGET_NAME}
    scripts/BulletScript.h
    scripts/BulletScript.cpp
    scripts/PlayerControlScript.h
    scripts/PlayerControlScript.cpp

    levels/MainMenuLevel.h
    levels/MainMenuLevel.cpp
    levels/ArenaLevel.h
    levels/ArenaLevel.cpp

    Application.h 
    Application.cpp
    main.cpp 
)
target_link_libraries(${TARGET_NAME} PUBLIC InfraRedEngine)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory              
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${TARGET_NAME}>/shaders
    COMMENT "Copying HLSL Shaders" VERBATIM
)
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory              
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
    COMMENT "Copying Assets" VERBATIM
)
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy              
        ${CMAKE_SOURCE_DIR}/deps/libs/assimp-vc142-mtd.dll $<TARGET_FILE_DIR:${TARGET_NAME}>/
    COMMENT "Copying Assimp DLL" VERBATIM
)
