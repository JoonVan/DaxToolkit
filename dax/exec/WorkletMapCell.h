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
#ifndef __dax_exec_WorkletMapCell_h
#define __dax_exec_WorkletMapCell_h

#include <dax/exec/internal/WorkletBase.h>
#include <dax/cont/arg/Field.h>
#include <dax/cont/arg/Topology.h>
#include <dax/cont/sig/Tag.h>

namespace dax {
namespace exec {

///----------------------------------------------------------------------------
/// Superclass for worklets that map points to cell. Use this when the worklet
/// needs "CellArray" information i.e. information about what points form a
/// cell.
///
class WorkletMapCell : public dax::exec::internal::WorkletBase
{
public:
  typedef dax::cont::sig::Cell DomainType;

  DAX_EXEC_EXPORT WorkletMapCell() { }
protected:
  typedef dax::cont::arg::Field Field;
  typedef dax::cont::arg::Topology Topology;
  typedef dax::cont::arg::Topology::Vertices Vertices;
  typedef dax::cont::sig::Point Point;
  typedef dax::cont::sig::Cell Cell;
};

}
}

#endif //__dax_exec_WorkletMapCell_h
