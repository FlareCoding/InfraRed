cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/asset_management/AssetManager.h
    engine/asset_management/AssetManager.cpp

    engine/asset_management/AssetLoadingQueue.h
    engine/asset_management/AssetLoadingQueue.cpp

    engine/asset_management/loaders/scene/IFRSceneLoader.h
    engine/asset_management/loaders/scene/IFRSceneLoader.cpp

    engine/asset_management/loaders/assimp/AssimpLoader.h
    engine/asset_management/loaders/assimp/AssimpLoader.cpp

    PARENT_SCOPE
)
