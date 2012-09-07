project(meshconvert)
cmake_minimum_required(VERSION 2.8.3)  

## Match everything when analyzing dependencies
#INCLUDE_REGULAR_EXPRESSION("^.*$")

#FIND_PACKAGE(VTK REQUIRED)
#IF (VTK_FOUND)
 #INCLUDE( ${USE_VTK_FILE} )
#ENDIF(VTK_FOUND)


##set( MESHCONVERT_SRCS
#meshconvert.cpp
#)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#add_definitions( -Wall )
#add_executable( meshconvert   ${MESHCONVERT_SRCS}  )

#target_link_libraries( meshconvert vtkCommon
 #                       vtkGraphics vtkImaging vtkRendering vtkWidgets)


cmake_minimum_required(VERSION 2.8)
 
PROJECT(MeshConvert)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
if (APPLE)
  add_executable(MeshConvert MACOSX_BUNDLE MeshConvert.cpp)
else()
  add_executable(MeshConvert MeshConvert.cxx)
endif()
 
if(VTK_LIBRARIES)
  target_link_libraries(MeshConvert ${VTK_LIBRARIES})
else()
  target_link_libraries(MeshConvert vtkHybrid )
endif()
