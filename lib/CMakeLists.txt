set(SOURCES
    engine.cpp
    forest.cpp
    likelihood.cpp
    merge.cpp
    network.cpp
    special_functions.cpp
    split.cpp
    statistics.cpp
    )

set(HEADERS
    engine.h
    forest.h
    likelihood.h
    merge.h
    model.h
    network.h
    special_functions.h
    split.h
    statistics.h
    )

add_library(bhcd ${HEADERS} ${SOURCES})
