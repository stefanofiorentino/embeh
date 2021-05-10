cmake_minimum_required(VERSION 3.0.0)
project(embeh VERSION 0.1.0)

set(DEVEL_DIRECTORY $ENV{HOME}/devel)
set(GIT_REPO_DIRECTORY ${DEVEL_DIRECTORY})
set(DEPS_DIRECTORY ${PROJECT_BINARY_DIR}/_deps)

include(CTest)
enable_testing()
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
