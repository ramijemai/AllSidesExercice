cmake_minimum_required(VERSION 3.12)
project(AllSidesExercice)

add_executable(AllSidesExercice
    main.cpp
    Implementations/FakeCamera.cpp
    Implementations/HdrCombiner.cpp
)

target_include_directories(AllSidesExercice PRIVATE Headers)

set_property(TARGET AllSidesExercice PROPERTY CXX_STANDARD 20)