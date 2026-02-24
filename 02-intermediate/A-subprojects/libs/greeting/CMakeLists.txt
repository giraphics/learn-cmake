add_library(greeting
    src/greeting.cpp
)

target_include_directories(greeting PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
