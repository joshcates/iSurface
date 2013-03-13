project(isurface)
cmake_minimum_required(VERSION 2.8.3)  

## Match everything when analyzing dependencies
#INCLUDE_REGULAR_EXPRESSION("^.*$")

##set( MESHCONVERT_SRCS
#meshconvert.cpp
#)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#add_definitions( -Wall )

cmake_minimum_required(VERSION 2.8)
 
PROJECT(iSurface)

find_package(ITK REQUIRED)
if (ITK_FOUND)
 include(${USE_ITK_FILE})
endif(ITK_FOUND)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

if (APPLE)
  add_executable(iSurface MACOSX_BUNDLE iSurface.cpp)
  add_executable(iSurfaceToObj MACOSX_BUNDLE iSurfaceToObj.cpp)
  add_executable(iSurfaceToVtk MACOSX_BUNDLE iSurfaceToVtk.cpp)
  add_executable(Vtk2Obj MACOSX_BUNDLE Vtk2Obj.cpp)
  add_executable(iSurfaceViewer MACOSX_BUNDLE iSurfaceViewer.cpp)
#  add_executable(iSurfaceGlyphViewer MACOSX_BUNDLE  iSurfaceGlyphViewer.cpp glyph_pipeline.cxx)
  add_executable(Vtk2Stl MACOSX_BUNDLE Vtk2Stl.cpp)
else()
  add_executable(iSurface iSurface.cpp)
  add_executable(iSurfaceToObj iSurfaceToObj.cpp)
  add_executable(iSurfaceToVtk iSurfaceToVtk.cpp)
  add_executable(iSurfaceViewer iSurfaceViewer.cpp)
#  add_executable(iSurfaceGlyphViewer iSurfaceGlyphViewer.cpp glyph_pipeline.cxx)
  add_executable(Vtk2Obj Vtk2Obj.cpp)
  add_executable(Vtk2Stl Vtk2Stl.cpp)
endif()
 
target_link_libraries(iSurface ITKBasicFilters ITKCommon ITKIO vtkHybrid)
target_link_libraries(iSurfaceToObj ITKBasicFilters ITKCommon ITKIO vtkHybrid)
target_link_libraries(iSurfaceToVtk ITKBasicFilters ITKCommon ITKIO vtkHybrid)
target_link_libraries(iSurfaceViewer ITKBasicFilters ITKCommon ITKIO vtkHybrid)
#target_link_libraries(iSurfaceGlyphViewer ITKBasicFilters ITKCommon ITKIO vtkHybrid)
target_link_libraries(Vtk2Obj vtkHybrid )
target_link_libraries(Vtk2Stl vtkHybrid )
