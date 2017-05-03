file (GLOB sources "../src/util/*.cpp")
add_library(cpp-util ${sources})
target_include_directories(cpp-util PUBLIC ../include ../include/util)
