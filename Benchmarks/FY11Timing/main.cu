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

#define BOOST_SP_DISABLE_THREADS

//included after defining the device adapter
#ifndef DAX_DEVICE_ADAPTER
  #define DAX_DEVICE_ADAPTER DAX_DEVICE_ADAPTER_CUDA
#endif

#include "ArgumentsParser.h"
#include "Pipeline.h"

void RunPipeline(int pipeline, const dax::cont::UniformGrid<> &grid)
{
  switch (pipeline)
    {
    case 1:
      RunPipeline1(grid);
      break;
    case 2:
      RunPipeline2(grid);
      break;
    case 3:
      RunPipeline3(grid);
      break;
    case 4:
      RunPipeline4(grid);
      break;
    default:
      std::cout << "Invalid pipeline selected." << std::endl;
      exit(1);
      break;
    }
}

dax::cont::UniformGrid<> CreateInputStructure(dax::Id dim)
{
  dax::cont::UniformGrid<> grid;
  grid.SetOrigin(dax::make_Vector3(0.0, 0.0, 0.0));
  grid.SetSpacing(dax::make_Vector3(1.0, 1.0, 1.0));
  grid.SetExtent(dax::make_Id3(0, 0, 0), dax::make_Id3(dim-1, dim-1, dim-1));
  return grid;
}

int main(int argc, char* argv[])
  {
  dax::testing::ArgumentsParser parser;
  if (!parser.parseArguments(argc, argv))
    {
    return 1;
    }

  //init grid vars from parser
  const dax::Id MAX_SIZE = parser.problemSize();

  dax::cont::UniformGrid<> grid = CreateInputStructure(MAX_SIZE);

  int pipeline = parser.pipeline();
  std::cout << "Pipeline #" << pipeline << std::endl;

  RunPipeline(pipeline, grid);

  return 0;
}
