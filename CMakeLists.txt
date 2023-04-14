cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
project(conanTools)
set(CMAKE_SYSTEM_VERSION 1)
set(KANZI_PATH $ENV{KANZI_PATH} CACHE STRING "")
set(CLUSTER_HMI_LIBRARY_PATH $ENV{CLUSTER_HMI_LIBRARY_PATH} CACHE STRING "")
set(CMAKE_INSTALL_PREFIX ${CLUSTER_HMI_LIBRARY_PATH})


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


include(${PROJECT_SOURCE_DIR}/version.cmake)
generate_build_info()

add_subdirectory(src)
