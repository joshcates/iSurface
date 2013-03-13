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
#include "vtkPolyDataWriter.h"
#include <vtkRenderer.h>
#include <vtkContourFilter.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkRenderWindowInteractor.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkAntiAliasBinaryImageFilter.h>
#include <itkReinitializeLevelSetImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h>
#include "itkImageToVTKImageFilter.h"

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
      std::cout << "Usage:" << argv[0] 
		<< "input output [isosurface_value = 0.5] [smoothing_sigma = 0.0] [display_result=1]"
                << std::endl;
      return 1;
    }

  double sigma = 0.0f;
  double isosurface_value = 0.5f;
  int    display_result = 1;

  if (argc >3)
    { isosurface_value = atof(argv[3]); }
  if (argc >4)
    { sigma = atof(argv[4]);  }
  if (argc >5)
    { display_result = atoi(argv[5]);  }

  // std::cout << "isosurf val = " << isosurface_value << std::endl;
  // std::cout << "sigma = " << sigma << std::endl;
  // std::cout << "display_result = " << display_result << std::endl;

  typedef itk::Image<float, 3> image_type;

  try {
    itk::ImageFileReader<image_type>::Pointer reader 
      = itk::ImageFileReader<image_type>::New();
    reader->SetFileName(argv[1]);
    
    itk::AntiAliasBinaryImageFilter<image_type, image_type>::Pointer anti
      = itk::AntiAliasBinaryImageFilter<image_type, image_type>::New();
    anti->SetInput(reader->GetOutput());
    anti->SetNumberOfIterations(150);
    anti->SetMaximumRMSError(0.0);
    anti->SetIsoSurfaceValue(isosurface_value);
     
    itk::ReinitializeLevelSetImageFilter<image_type>::Pointer filt
      = itk::ReinitializeLevelSetImageFilter<image_type>::New();
    filt->SetInput(anti->GetOutput());
    filt->NarrowBandingOff();
    filt->SetLevelSetValue(0.0);
    filt->Update();

    itk::ImageToVTKImageFilter<image_type>::Pointer importer 
      = itk::ImageToVTKImageFilter<image_type>::New();    
    
    if (sigma > 0.0f)
      {        
        itk::DiscreteGaussianImageFilter<image_type, image_type>::Pointer blur
          = itk::DiscreteGaussianImageFilter<image_type, image_type>::New();
        blur->SetInput(filt->GetOutput());
        blur->SetVariance(sigma * sigma);
        blur->SetUseImageSpacingOn();
        blur->Update();

        importer->SetInput(blur->GetOutput());
      }
    else 
      {
        importer->SetInput(filt->GetOutput());
      }
    importer->Update();
    
    vtkSmartPointer<vtkImageToStructuredPoints> converter 
      = vtkSmartPointer<vtkImageToStructuredPoints>::New();
    converter->SetInput(importer->GetOutput());
    
    vtkSmartPointer<vtkContourFilter> contour_filter 
      = vtkSmartPointer<vtkContourFilter>::New();
    contour_filter->SetInputConnection(converter->GetOutputPort());
    contour_filter->SetValue(0, 0.0);
    contour_filter->SetNumberOfContours(1);
    
    vtkSmartPointer<vtkPolyDataWriter> writer
      = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetInputConnection(contour_filter->GetOutputPort());
    writer->SetFileName(argv[2]);
    writer->Write();
    
    if (display_result != 0)
      {
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
