cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/physics/BoundingVolumes.h

    engine/physics/BoundingBox.h
    engine/physics/BoundingBox.cpp

    engine/physics/OBB.h
    engine/physics/OBB.cpp

    engine/physics/ConvexHull.h
    engine/physics/ConvexHull.cpp

    engine/physics/CollisionDetection.h
    engine/physics/CollisionDetection.cpp

    PARENT_SCOPE
)
