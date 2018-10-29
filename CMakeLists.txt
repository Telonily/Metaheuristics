cmake_minimum_required(VERSION 3.12)
project(Metaheuristics)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT "D:/User/libs/boost_1_68_0")
FIND_PACKAGE(Boost REQUIRED)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_subdirectory(libs/googletest)
include_directories(libs/googletest/include)
add_executable(Metaheuristics main.cpp src/Loader.cpp src/Loader.h src/Node.cpp src/Node.h src/Item.cpp src/Item.h src/GA.cpp src/GA.h src/GADataContainer.h src/Individual.h src/TabuList.cpp src/TabuList.h)
target_link_libraries(Metaheuristics gtest gtest_main)
