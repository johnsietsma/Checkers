cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(LIBRARY_NAME CheckersEngine)
project(${LIBRARY_NAME})

IF(APPLE)
    SET(GCC_CHAR_IS_UNSIGNED_CHAR "-funsigned-char")
    SET(STD_C11 "-std=c++11 -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
ENDIF (APPLE)


add_library(${LIBRARY_NAME}
  CheckersBoard.h
  CheckersBoard.cpp
  Move.h
  Piece.h
  Pos.h
)

target_include_directories (${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}\\src)

IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(CORE_VIDEO_LIBRARY CoreVideo)
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(CARBON_LIBRARY Carbon)
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
   FIND_LIBRARY(IOKIT_LIBRARY IOKit)
   MARK_AS_ADVANCED (CARBON_LIBRARY CORE_VIDEO_LIBRARY COCOA_LIBRARY OpenGL_LIBRARY IOKIT_LIBRARY)
   SET(EXTRA_LIBS ${CARBON_LIBRARY} ${CORE_VIDEO_LIBRARY} ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY})

   #target_link_libraries(${LIBRARY_NAME} glfw ${GLFW_LIBRARIES})
ELSE(APPLE)
   target_link_libraries(${LIBRARY_NAME} opengl32.lib)
   #target_link_libraries(${LIBRARY_NAME} glfw ${GLFW_LIBRARIES})
ENDIF (APPLE)

target_link_libraries(${LIBRARY_NAME} ${EXTRA_LIBS})

