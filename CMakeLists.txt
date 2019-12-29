cmake_minimum_required(VERSION 3.10)
project(logtractor)

add_definitions("-std=c++2a")

include(build/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(libs/di/include)

add_executable(logtractor logtractor.cpp src/config.cpp src/config.hpp src/services/logtractor_service.cpp src/services/logtractor_service.hpp src/logger.hpp)
target_link_libraries(logtractor ${CONAN_LIBS})
