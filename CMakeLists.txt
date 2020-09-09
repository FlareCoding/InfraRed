cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE -D_UNICODE)
project(InfraRed)

include_directories(engine)
include_directories(InfraRedEngine deps/includes)
link_directories(deps/libs)

set(IFR_SOURCES)
add_subdirectory(engine)

add_library(
    InfraRedEngine
    ${IFR_SOURCES}
)
target_link_libraries(InfraRedEngine PRIVATE assimp-vc142-mtd.lib)
target_compile_definitions(InfraRedEngine PRIVATE IFR_BUILD)

add_subdirectory(tests)
