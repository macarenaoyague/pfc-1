cmake_minimum_required(VERSION 3.20)
project(PFC_PROJECT)

set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.78 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

if(UNIX AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

include_directories(./Algorithms ./Classes)

file(GLOB source_files 
        main.cpp
        Algorithms/*.hpp
        Classes/*.hpp)

add_executable(${PROJECT_NAME} 
        ${source_files})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Graphs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem)