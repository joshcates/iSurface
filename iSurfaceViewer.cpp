/** Copyright 2012 Joshua Cates cates@sci.utah.edu
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
*/
#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkOBJExporter.h>
#include <vtkContourFilter.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkRenderWindowInteractor.h>

#include <itkImageFileReader.h>
#include "itkImageToVTKImageFilter.h"

int main (int argc, char *argv[])
{
  if (argc < 2)
    {
      std::cout << "Usage:" << argv[0] 
		<< "input_name [isosurface_value]" << std::endl;
      return 1;
    }

  double isosurface_value = 0.5;
  if (argc >2)
    {
      isosurface_value = atof(argv[2]);
    }


  typedef itk::Image<float, 3> image_type;

  try {
    itk::ImageFileReader<image_type>::Pointer reader 
      = itk::ImageFileReader<image_type>::New();
    reader->SetFileName(argv[1]);
    
    itk::ImageToVTKImageFilter<image_type>::Pointer importer 
      = itk::ImageToVTKImageFilter<image_type>::New();    
    importer->SetInput(reader->GetOutput());
    importer->Update();
    
    vtkSmartPointer<vtkImageToStructuredPoints> converter 
      = vtkSmartPointer<vtkImageToStructuredPoints>::New();
    converter->SetInput(importer->GetOutput());
    
    vtkSmartPointer<vtkContourFilter> contour_filter 
      = vtkSmartPointer<vtkContourFilter>::New();
    contour_filter->SetInputConnection(converter->GetOutputPort());
    contour_filter->SetValue(0, isosurface_value);
    contour_filter->SetNumberOfContours(1);
    
    vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
    renwin->AddRenderer(ren1);
    renwin->SetSize(500,500);
    
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renwin);
    
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(contour_filter->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    ren1->AddActor(actor);
    
    renwin->Render();
    renderWindowInteractor->Start();
  }
  catch (itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return 1;
    }
  catch( ...)
    {
      std::cerr << "Unknown error" << std::endl;
      return 2;
    }

  return 0;
}
