/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "fncAutoGeneratedModule.h"

#include "fncPort.h"

#include <boost/xpressive/xpressive.hpp>

//#include <boost/config/warning_disable.hpp>


/*
===============================================================================
// FUNCTOR MODULE VERSION 0.0.3
// MODULE ${name of the module}
// INPUT ${name}; ${type}; ${default}; ${description}
// INPUT ${name}; ${type}; ${default}; ${description}
// ...
// OUTPUT ${name}; ${type}; ${description}
// OUTPUT ${name}; ${type}; ${description}
// ...

void ${name}_functor(...)
{
}
===============================================================================
*/

namespace
{
  fncPort::Types fncStringToType(const std::string& str, int& num_comps)
    {
    using namespace boost::xpressive;
    sregex ptarray = sregex::compile("point_array(\\d*)");
    sregex cellarray = sregex::compile("cell_array(\\d*)");
    sregex anyarray = sregex::compile("any_array(\\d*)");
    sregex float_ = sregex::compile("float(\\d*)");
    smatch what;
    if (regex_match(str, what, ptarray))
      {
      num_comps = what.size()==2? atoi(what.str(1).c_str()) : 1;
      return fncPort::point_array;
      }
    else if (regex_match(str, what, cellarray))
      {
      num_comps = what.size()==2? atoi(what.str(1).c_str()) : 1;
      return fncPort::cell_array;
      }
    else if (regex_match(str, what, anyarray))
      {
      num_comps = what.size()==2? atoi(what.str(1).c_str()) : 1;
      return fncPort::any_array;
      }
    else if (regex_match(str, what, float_))
      {
      num_comps = what.size()==2? atoi(what.str(1).c_str()) : 1;
      return fncPort::float_;
      }
    num_comps = -1;
    return fncPort::invalid;
    }
}

//-----------------------------------------------------------------------------
fncAutoGeneratedModule::fncAutoGeneratedModule()
{
}

//-----------------------------------------------------------------------------
fncAutoGeneratedModule::~fncAutoGeneratedModule()
{
}

//-----------------------------------------------------------------------------
const std::string& fncAutoGeneratedModule::GetModuleName()
{
  return this->ModuleName;
}

//-----------------------------------------------------------------------------
const std::string& fncAutoGeneratedModule::GetFunctorCode()
{
  return this->FunctorCode;
}

//-----------------------------------------------------------------------------
bool fncAutoGeneratedModule::Initialize(const std::string& definition)
{
  using namespace boost::xpressive;

  sregex module_version = sregex::compile(
    "//\\s+FUNCTOR\\s+MODULE\\s+VERSION\\s+(\\d+).(\\d+).(\\d+)");

  smatch what;
  if (regex_search(definition, what, module_version) == false)
    {
    cerr << "Could not file version string" << endl;
    return false;
    }
  if (atoi(what.str(1).c_str()) != 0 ||
    atoi(what.str(2).c_str()) != 0 ||
    atoi(what.str(3).c_str()) != 1)
    {
    cerr <<
      "Incorrect version: " << what[1] << "." << what[2] << "." << what[3] << endl;
    cerr << "Supported version: 0.0.1" << endl;
    return false;
    }

  sregex module_name = sregex::compile("//\\s+MODULE\\s+(\\w+)");
  std::string suffix = what.suffix().str();
  if (!regex_search(suffix, what, module_name))
    {
    cerr << "Failed to locate module name" << endl;
    return false;
    }

  this->ModuleName = what.str(1);

  // INPUT ${name}; ${type}; ${default};
  sregex inputs = sregex::compile(
    "//\\s+INPUT\\s+(\\S+)\\s*;\\s*(\\S+)\\s*;\\s*(\\S+)\\s*;");

  this->SetNumberOfInputs(0);

  suffix = what.suffix().str();
  sregex_iterator input_iter(suffix.begin(), suffix.end(), inputs);
  sregex_iterator end;
  for (size_t cc=0; input_iter != end; ++input_iter, ++cc)
    {
    what = *input_iter;
    cout << "Processed input: " << what[1] << endl;
    int num_comps = 0;
    fncPortPtr port(new fncPort());
    port->SetName(what.str(1));
    port->SetType(fncStringToType(what.str(2), num_comps));
    port->SetNumberOfComponents(num_comps);
    this->SetInputPort(cc, port);
    }

  // OUTPUT ${name}; ${type};
  sregex outputs = sregex::compile(
    "//\\s+OUTPUT\\s+(\\S+)\\s*;\\s*(\\S+)\\s*;");

  this->SetNumberOfOutputs(0);

  suffix = what.suffix().str();
  sregex_iterator output_iter(suffix.begin(), suffix.end(), outputs);
  for (size_t cc=0; output_iter != end; ++output_iter, ++cc)
    {
    what = *output_iter;
    cout << "Processed output: " << what[1] << endl;
    int num_comps = 0;
    fncPortPtr port(new fncPort());
    port->SetName(what.str(1));
    port->SetType(fncStringToType(what.str(2), num_comps));
    port->SetNumberOfComponents(num_comps);
    this->SetOutputPort(cc, port);
    }

  this->FunctorCode = definition;
  return true;
}
