//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2012 Sandia Corporation.
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//=============================================================================

#include <dax/cont/ArrayContainerControlBasic.h>
#include <dax/cont/DeviceAdapterSerial.h>

// These header files help tease out when the default template arguments to
// ArrayHandle are inappropriately used.
#include <dax/cont/internal/ArrayContainerControlError.h>
#include <dax/cont/internal/DeviceAdapterError.h>

#include <dax/cont/worklet/Elevation.h>

#include <dax/cont/ArrayHandle.h>
#include <dax/cont/UniformGrid.h>

#include <dax/cont/internal/Testing.h>

#include <vector>

namespace {

const dax::Id DIM = 64;

//-----------------------------------------------------------------------------
static void TestElevation()
{
  dax::cont::UniformGrid<dax::cont::ArrayContainerControlTagBasic,
                         dax::cont::DeviceAdapterTagSerial> grid;
  grid.SetExtent(dax::make_Id3(0, 0, 0), dax::make_Id3(DIM-1, DIM-1, DIM-1));

  dax::cont::ArrayHandle<dax::Scalar,
                         dax::cont::ArrayContainerControlTagBasic,
                         dax::cont::DeviceAdapterTagSerial> elevationHandle;

  std::cout << "Running Elevation worklet" << std::endl;
  dax::cont::worklet::Elevation(grid.GetPointCoordinates(),
                                elevationHandle);

  std::cout << "Checking result" << std::endl;
  std::vector<dax::Scalar> elevation(grid.GetNumberOfPoints());
  elevationHandle.CopyInto(elevation.begin());
  dax::Id3 ijk;
  for (ijk[2] = 0; ijk[2] < DIM; ijk[2]++)
    {
    for (ijk[1] = 0; ijk[1] < DIM; ijk[1]++)
      {
      for (ijk[0] = 0; ijk[0] < DIM; ijk[0]++)
        {
        dax::Id pointIndex = grid.ComputePointIndex(ijk);
        dax::Scalar elevationValue = elevation[pointIndex];
        dax::Vector3 pointCoordinates =grid.ComputePointCoordinates(pointIndex);
        // Wrong, but what is currently computed.
        dax::Scalar elevationExpected
            = sqrt(dax::dot(pointCoordinates, pointCoordinates));
        DAX_TEST_ASSERT(test_equal(elevationValue, elevationExpected),
                        "Got bad elevation.");
        }
      }
    }
}

} // Anonymous namespace

//-----------------------------------------------------------------------------
int UnitTestWorkletElevation(int, char *[])
{
  return dax::cont::internal::Testing::Run(TestElevation);
}
