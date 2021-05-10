function(ADD_EMBEH_TEST TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_include_directories(${TEST_NAME} 
                                    PRIVATE
                               ${PROJECT_SOURCE_DIR})
    target_compile_features(${TEST_NAME} PUBLIC cxx_std_17)
    add_test(NAME embeh_${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
endfunction()

ADD_EMBEH_TEST(main main.cpp)