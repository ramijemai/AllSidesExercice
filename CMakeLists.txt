cmake_minimum_required(VERSION 3.12)
project(AllSidesExercice)

add_executable(AllSidesExercice
    main.cpp
    Implementations/FakeCamera.cpp
    Implementations/HdrCombiner.cpp
)

target_include_directories(AllSidesExercice PRIVATE Headers)

set_property(TARGET AllSidesExercice PROPERTY CXX_STANDARD 20)

# Python bindings
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(hdr
    bindings.cpp
    Implementations/FakeCamera.cpp
    Implementations/HdrCombiner.cpp
)
target_include_directories(hdr PRIVATE Headers)
set_property(TARGET hdr PROPERTY CXX_STANDARD 20)