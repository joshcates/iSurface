/** Copyright 2013 Joshua Cates cates@sci.utah.edu
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
#ifndef __glyph_pipeline_h
#define __glyph_pipeline_h

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkDiskSource.h"
#include "vtkGlyph3D.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkPolyData.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkFloatArray.h"
#include "vtkTransform.h"
#include "vtkPointData.h"
#include "vtkUnsignedLongArray.h"
#include "vtkLookupTable.h"

#include <cstdlib>
#include <vector>
#include <string>
  
class glyph_pipeline
{
public:
  glyph_pipeline();
  ~glyph_pipeline() {}

  vtkActor *output()
  { return m_actor; }

  void load_points(const char *fn);
  // void write_points(const char *fn);
  
  void scale(double s)
  {
    m_glyphs->SetScaleFactor(s);
    m_glyphs->Update();
  }

  void transform(vtkAbstractTransform *t)
  { m_transformer->SetTransform(t); }
  
  void color(double r, double g, double b)
  { m_actor->GetProperty()->SetDiffuseColor(r,g,b); }

  void opacity(double o)
  { m_actor->GetProperty()->SetOpacity(o); }

  vtkPoints *points()
  { return m_transformer->GetOutput()->GetPoints(); }

  vtkTransformPolyDataFilter *transformer()
  { return m_transformer; }

  void lut(vtkLookupTable* l)
  {
    m_lut = l;
    m_mapper->SetLookupTable(m_lut);
  }
  
  vtkLookupTable *lut()
  { return m_lut;}

  /** Generate a random list of colors.  New glyphs will take on the next color
      in the list. */
  struct rgb
  {
    float r;
    float g;
    float b;
  };
  void generate_color_list(int);
  const std::vector<rgb> &get_color_list() const
  { return m_color_list; }
  void set_color_list(const std::vector<rgb> &c)
  { m_color_list = c; }

  void recolor();

protected:
  glyph_pipeline(const glyph_pipeline &o);  
  const glyph_pipeline& operator=(const glyph_pipeline &o);

  std::vector<rgb> m_color_list;
  unsigned int     m_number_of_points;
  
  vtkSmartPointer<vtkGlyph3D>      m_glyphs;
  vtkSmartPointer<vtkActor>        m_actor;
  vtkSmartPointer<vtkPolyData>     m_pointset;
  vtkSmartPointer<vtkSphereSource> m_sphere;
  vtkSmartPointer<vtkDiskSource>   m_disk;
  vtkSmartPointer<vtkPoints>       m_points;
  vtkSmartPointer<vtkLookupTable>  m_lut;
  vtkSmartPointer<vtkPolyDataMapper>    m_mapper;
  vtkSmartPointer<vtkUnsignedLongArray> m_scalars;
  vtkSmartPointer<vtkTransformPolyDataFilter> m_transformer;
};


#endif
