cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME CheckersEngineTests)
project(${PROJECT_NAME})

IF(APPLE)
    SET(GCC_CHAR_IS_UNSIGNED_CHAR "-funsigned-char")
    SET(STD_C11 "-std=c++11 -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
ENDIF (APPLE)


enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME}
	AIPlayerTests.cpp
    CheckersBoardTests.cpp
    PosTests.cpp
 )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CHAR_IS_UNSIGNED_CHAR} ${STD_C11}")

include_directories(${CheckersEngine_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} CheckersEngine)
target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gtest_main)
