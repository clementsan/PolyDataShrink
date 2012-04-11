project(PolyDataShrink)

cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# include a test for endianness (used to be done by default)
INCLUDE (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
TEST_BIG_ENDIAN(CMAKE_WORDS_BIGENDIAN)

# We need Slicer3

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# This block builds a module as both a shared object and an executable

set(MyModule_SOURCE 
main.cxx 

)

set (VTK_LIBRARIES
  vtkRendering
  vtkGraphics
  vtkImaging
  vtkIO
  vtkCommon
  vtkHybrid
  vtkFiltering 
) 

add_executable(PolyDataShrink ${MyModule_SOURCE})
target_link_libraries( PolyDataShrink ${VTK_LIBRARIES})


