add_executable(AllSidesExercice
    main.cpp
    "Implementations/FakeCamera.cpp"
    "Headers/FakeCamera.h"
    "Implementations/HdrCombiner.cpp"
    "Headers/HdrCombiner.h"
    "Headers/Constants.h"
)

set_property(TARGET AllSidesExercice PROPERTY CXX_STANDARD 20)

# Add include directories for cleaner includes
target_include_directories(AllSidesExercice PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Headers)