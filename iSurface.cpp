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
#include <itkImageFileWriter.h>
#include <itkAntiAliasBinaryImageFilter.h>
#include <itkReinitializeLevelSetImageFilter.h>
#include "itkImageToVTKImageFilter.h"

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
      std::cout << "Usage:" << argv[0] 
		<< "input_name output_prefix [smoothing_parameter]" << std::endl;
      return 1;
    }

  double sigma = 0.0;
  double isosurface_value = 0.5;
  if (argc >3)
    {
      
    }
  if (argc >4)
    {

    }

  typedef itk::Image<float, 3> image_type;

  try {
    itk::ImageFileReader<image_type>::Pointer reader 
      = itk::ImageFileReader<image_type>::New();
    reader->SetFileName(argv[1]);

    itk::AntiAliasBinaryImageFilter<image_type, image_type>::Pointer anti
      = itk::AntiAliasBinaryImageFilter<image_type, image_type>::New();
    anti->SetInput(reader->GetOutput());
    anti->SetNumberOfIterations(100);
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
    importer->SetInput(filt->GetOutput());
    importer->Update();

    vtkSmartPointer<vtkImageToStructuredPoints> converter 
      = vtkSmartPointer<vtkImageToStructuredPoints>::New();
    converter->SetInput(importer->GetOutput());

    vtkSmartPointer<vtkContourFilter> contour_filter 
      = vtkSmartPointer<vtkContourFilter>::New();
    contour_filter->SetInputConnection(converter->GetOutputPort());
    contour_filter->SetValue(0, 0.0);
    contour_filter->SetNumberOfContours(1);
    
    // vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
    // reader->SetFileName(argv[1]);
    // reader->Update();
    
    // itk::ImageFileWriter<image_type>::Pointer fwriter = itk::ImageFileWriter<image_type>::New();
    // fwriter->SetInput(filt->GetOutput());
    // fwriter->SetFileName("tmp.nrrd");
    // fwriter->Update();

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
    
    vtkSmartPointer<vtkOBJExporter> writer = vtkSmartPointer<vtkOBJExporter>::New();
    writer->SetInput(renwin);
    writer->SetFilePrefix(argv[2]);
    writer->Update();
    
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
