cmake_minimum_required(VERSION 3.10)
project(logtractor)

add_definitions("-std=c++2a")

include(build/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(libs/di/include)

add_executable(logtractor logtractor.cpp src/config.cpp src/config.hpp src/services/logtractor_service.cpp src/services/logtractor_service.hpp src/logger.hpp src/source.hpp src/sources/source_udp.cpp src/sources/source_udp.hpp src/appender.hpp src/appenders/appender_file.cpp src/appenders/appender_file.hpp src/message.hpp)
target_link_libraries(logtractor ${CONAN_LIBS})

if ("${APP_STATIC}" EQUAL "true")
	target_link_options(logtractor PUBLIC "-static")
endif()