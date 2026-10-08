cmake_minimum_required(VERSION 3.20)

project(
  swarm_spatial_link
  VERSION 0.1.0
  DESCRIPTION "Network-adaptive middleware core for multi-robot spatial state"
  LANGUAGES CXX
)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SSL_BUILD_DEMO "Build the weak-link replay demo" ON)

add_library(swarm_spatial_link
  src/adaptive_qos.cpp
  src/clock_offset_estimator.cpp
  src/keyframe_scheduler.cpp
)
add_library(swarm_spatial::link ALIAS swarm_spatial_link)

target_include_directories(swarm_spatial_link
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(swarm_spatial_link PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
  target_compile_options(swarm_spatial_link PRIVATE /W4)
endif()

if(SSL_BUILD_DEMO)
  add_executable(weak_link_demo src/weak_link_demo.cpp)
  target_link_libraries(weak_link_demo PRIVATE swarm_spatial::link)
endif()

if(BUILD_TESTING)
  add_executable(link_tests tests/link_tests.cpp)
  target_link_libraries(link_tests PRIVATE swarm_spatial::link)
  add_test(NAME link_tests COMMAND link_tests)
endif()
