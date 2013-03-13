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
//#include <vtkPolyDataMapper.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
#include <vtkSTLWriter.h>
#include <vtkTriangleFilter.h>

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
      std::cout << "Usage:" << argv[0] << "input output" << std::endl;
      return 1;
    }

  vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Stl only supports triangle meshes
  vtkSmartPointer<vtkTriangleFilter> triangles = vtkSmartPointer<vtkTriangleFilter>::New();
  triangles->SetInputConnection(reader->GetOutputPort());
  
  vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
  writer->SetInputConnection(triangles->GetOutputPort());
  writer->SetFileName(argv[2]);
  writer->Write();

  // vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();

  // vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
  // renwin->AddRenderer(ren1);
  // renwin->SetSize(500,500);

  // vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  //  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  // renderWindowInteractor->SetRenderWindow(renwin);

  //  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  // mapper->SetInputConnection(reader->GetOutputPort());
  // vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  // actor->SetMapper(mapper);
  // ren1->AddActor(actor);
  
  //renwin->Render();
 
  //  vtkSmartPointer<vtkOBJExporter> writer = vtkSmartPointer<vtkOBJExporter>::New();
  // writer->SetInput(renwin);
  // writer->SetFilePrefix(argv[2]);
  // writer->Update();

  return 0;
}
