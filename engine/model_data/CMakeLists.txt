cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/model_data/ModelData.h
    engine/model_data/ModelData.cpp

    engine/model_data/StaticModel.h
    engine/model_data/AnimatedModel.h
    engine/model_data/AnimatedModel.cpp

    engine/model_data/AnimationData.h
    engine/model_data/AnimationData.cpp

    engine/model_data/Animation.h
    engine/model_data/Animation.cpp

    engine/model_data/shapes/Shapes.h
    engine/model_data/shapes/Shapes.cpp

    PARENT_SCOPE
)
