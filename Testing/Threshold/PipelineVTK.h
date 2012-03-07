/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

===========================================================================*/

#ifndef __PipelineVTK_h
#define __PipelineVTK_h

#include <dax/cont/UniformGrid.h>
class vtkImageData;

void RunPipelineVTK(int pipeline, const dax::cont::UniformGrid &dgrid, vtkImageData* grid);

#endif //__PipelineVTK_h
