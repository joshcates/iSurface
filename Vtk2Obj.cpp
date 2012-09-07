#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkOBJExporter.h>

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
      std::cout << "Usage:" << argv[0] << "input_name output_prefix" << std::endl;
      return 1;
    }

  vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
  renwin->AddRenderer(ren1);
  renwin->SetSize(500,500);

  //vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  //  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  // renderWindowInteractor->SetRenderWindow(renwin);

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  ren1->AddActor(actor);
  
  renwin->Render();
 
  vtkSmartPointer<vtkOBJExporter> writer = vtkSmartPointer<vtkOBJExporter>::New();
  writer->SetInput(renwin);
  writer->SetFilePrefix(argv[2]);
  writer->Update();

  

  return 0;
}
