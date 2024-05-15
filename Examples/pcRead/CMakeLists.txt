add_executable(pcRead main.cpp pcRead.cpp)
target_include_directories(pcRead PUBLIC "./*.h")
target_link_libraries(pcRead PeriodicTask)