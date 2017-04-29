file (GLOB sources *.cpp)
add_executable(unitTest ${sources})
target_link_libraries(unitTest ${Boost_LIBRARIES} cpp-util)
