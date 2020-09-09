cmake_minimum_required(VERSION 3.10)

set(
    IFR_SOURCES ${IFR_SOURCES}

    engine/events/Event.h
    engine/events/Events.h
    engine/events/KeyboardEvents.h
    engine/events/MouseEvents.h
    engine/events/WindowEvents.h

    PARENT_SCOPE
)
