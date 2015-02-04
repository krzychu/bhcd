add_executable(tests.run 
    tests.cpp)

target_link_libraries(tests.run bhcd gtest_main)
