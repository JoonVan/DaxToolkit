/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

===========================================================================*/

#include <dax/exec/math/Exp.h>

#include <dax/Types.h>
#include <dax/exec/VectorOperations.h>

#include <dax/internal/Testing.h>

#include <boost/lambda/lambda.hpp>

//-----------------------------------------------------------------------------
namespace {

const dax::Id NUM_NUMBERS = 5;
const dax::Scalar NumberList[NUM_NUMBERS] = { 0.25, 0.5, 1.0, 2.0, 3.75 };

//-----------------------------------------------------------------------------
void PowTest()
{
  std::cout << "Runing power tests." << std::endl;
  for (dax::Id index = 0; index < NUM_NUMBERS; index++)
    {
    dax::Scalar x = NumberList[index];
    dax::Scalar powx = dax::exec::math::Pow(x, 2.0);
    dax::Scalar sqrx = x*x;
    DAX_TEST_ASSERT(test_equal(powx, sqrx), "Power gave wrong result.");
    }
}

//-----------------------------------------------------------------------------
struct RaiseTo
{
  dax::Scalar Exponent;
  RaiseTo(dax::Scalar exponent) : Exponent(exponent) { }
  dax::Scalar operator()(dax::Scalar base) const {
    return dax::exec::math::Pow(base, this->Exponent);
  }
};

template<class VectorType, class FunctionType>
void RaiseToTest(FunctionType function, dax::Scalar exponent)
{
  for (dax::Id index = 0; index < NUM_NUMBERS; index++)
    {
    VectorType original;
    dax::exec::VectorFill(original, NumberList[index]);

    VectorType mathresult = function(original);

    VectorType raiseresult = dax::exec::VectorMap(original, RaiseTo(exponent));

    DAX_TEST_ASSERT(test_equal(mathresult, raiseresult),
                    "Exponent functions do not agree.");
    }
}

template<class VectorType> struct SqrtFunctor {
  VectorType operator()(VectorType x) const { return dax::exec::math::Sqrt(x); }
};
template<class VectorType>
void SqrtTest()
{
  std::cout << "Testing Sqrt "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseToTest<VectorType>(SqrtFunctor<VectorType>(), 0.5);
}

template<class VectorType> struct RSqrtFunctor {
  VectorType operator()(VectorType x) const {return dax::exec::math::RSqrt(x);}
};
template<class VectorType>
void RSqrtTest()
{
  std::cout << "Testing RSqrt "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseToTest<VectorType>(RSqrtFunctor<VectorType>(), -0.5);
}

template<class VectorType> struct CbrtFunctor {
  VectorType operator()(VectorType x) const { return dax::exec::math::Cbrt(x); }
};
template<class VectorType>
void CbrtTest()
{
  std::cout << "Testing Cbrt "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseToTest<VectorType>(CbrtFunctor<VectorType>(), 1.0/3.0);
}

template<class VectorType> struct RCbrtFunctor {
  VectorType operator()(VectorType x) const {return dax::exec::math::RCbrt(x);}
};
template<class VectorType>
void RCbrtTest()
{
  std::cout << "Testing RCbrt "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseToTest<VectorType>(RCbrtFunctor<VectorType>(), -1.0/3.0);
}

//-----------------------------------------------------------------------------
struct RaiseBy
{
  dax::Scalar Base;
  dax::Scalar ExponentBias;
  dax::Scalar ResultBias;
  RaiseBy(dax::Scalar base, dax::Scalar exponentbias, dax::Scalar resultbias)
    : Base(base), ExponentBias(exponentbias), ResultBias(resultbias) { }
  dax::Scalar operator()(dax::Scalar exponent) const {
    return dax::exec::math::Pow(this->Base, exponent + this->ExponentBias)
        + this->ResultBias;
  }
};

template<class VectorType, class FunctionType>
void RaiseByTest(FunctionType function,
                 dax::Scalar base,
                 dax::Scalar exponentbias = 0.0,
                 dax::Scalar resultbias = 0.0)
{
  for (dax::Id index = 0; index < NUM_NUMBERS; index++)
    {
    VectorType original;
    dax::exec::VectorFill(original, NumberList[index]);

    VectorType mathresult = function(original);

    VectorType raiseresult
        = dax::exec::VectorMap(original, RaiseBy(base,
                                                 exponentbias,
                                                 resultbias));

    DAX_TEST_ASSERT(test_equal(mathresult, raiseresult),
                    "Exponent functions do not agree.");
    }
}

template<class VectorType> struct ExpFunctor {
  VectorType operator()(VectorType x) const {return dax::exec::math::Exp(x);}
};
template<class VectorType>
void ExpTest()
{
  std::cout << "Testing Exp "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseByTest<VectorType>(ExpFunctor<VectorType>(), 2.71828183);
}

template<class VectorType> struct Exp2Functor {
  VectorType operator()(VectorType x) const {return dax::exec::math::Exp2(x);}
};
template<class VectorType>
void Exp2Test()
{
  std::cout << "Testing Exp2 "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseByTest<VectorType>(Exp2Functor<VectorType>(), 2.0);
}

template<class VectorType> struct ExpM1Functor {
  VectorType operator()(VectorType x) const {return dax::exec::math::ExpM1(x);}
};
template<class VectorType>
void ExpM1Test()
{
  std::cout << "Testing ExpM1 "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseByTest<VectorType>(ExpM1Functor<VectorType>(), 2.71828183, 0.0, -1.0);
}

template<class VectorType> struct Exp10Functor {
  VectorType operator()(VectorType x) const {return dax::exec::math::Exp10(x);}
};
template<class VectorType>
void Exp10Test()
{
  std::cout << "Testing Exp10 "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  RaiseByTest<VectorType>(Exp10Functor<VectorType>(), 10.0);
}

//-----------------------------------------------------------------------------
void Log2Test()
{
  std::cout << "Testing Log2" << std::endl;
  DAX_TEST_ASSERT(test_equal(dax::exec::math::Log2(dax::Scalar(0.25)),
                             dax::Scalar(-2.0)),
                  "Bad value from Log2");
  DAX_TEST_ASSERT(
        test_equal(dax::exec::math::Log2(dax::make_Vector4(0.5, 1.0, 2.0, 4.0)),
                   dax::make_Vector4(-1.0, 0.0, 1.0, 2.0)),
        "Bad value from Log2");
}

template<class VectorType, class FunctionType>
void LogBaseTest(FunctionType function, dax::Scalar base, dax::Scalar bias=0.0)
{
  for (dax::Id index = 0; index < NUM_NUMBERS; index++)
    {
    VectorType original;
    dax::exec::VectorFill(original, NumberList[index]);

    VectorType mathresult = function(original);

    VectorType basevector;
    dax::exec::VectorFill(basevector, base);
    VectorType biased;
    dax::exec::VectorFill(biased, NumberList[index] + bias);

    VectorType logresult
        = dax::exec::math::Log2(biased)/dax::exec::math::Log2(basevector);

    DAX_TEST_ASSERT(test_equal(mathresult, logresult),
                    "Log functions do not agree.");
    }
}

template<class VectorType> struct LogFunctor {
  VectorType operator()(VectorType x) const {return dax::exec::math::Log(x);}
};
template<class VectorType>
void LogTest()
{
  std::cout << "Testing Log "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  LogBaseTest<VectorType>(LogFunctor<VectorType>(), 2.71828183);
}

template<class VectorType> struct Log10Functor {
  VectorType operator()(VectorType x) const {return dax::exec::math::Log10(x);}
};
template<class VectorType>
void Log10Test()
{
  std::cout << "Testing Log10 "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  LogBaseTest<VectorType>(Log10Functor<VectorType>(), 10.0);
}

template<class VectorType> struct Log1PFunctor {
  VectorType operator()(VectorType x) const {return dax::exec::math::Log1P(x);}
};
template<class VectorType>
void Log1PTest()
{
  std::cout << "Testing Log1P "
            << dax::VectorTraits<VectorType>::NUM_COMPONENTS << " components"
            << std::endl;
  LogBaseTest<VectorType>(Log1PFunctor<VectorType>(), 2.71828183, 1.0);
}

//-----------------------------------------------------------------------------
#define RUN_VECTOR_TEMPLATE(func) \
  func<dax::Scalar>(); \
  func<dax::Vector3>(); \
  func<dax::Vector4>()

void RunExpTests()
{
  PowTest();
  RUN_VECTOR_TEMPLATE(SqrtTest);
  RUN_VECTOR_TEMPLATE(RSqrtTest);
  RUN_VECTOR_TEMPLATE(CbrtTest);
  RUN_VECTOR_TEMPLATE(RCbrtTest);
  RUN_VECTOR_TEMPLATE(ExpTest);
  RUN_VECTOR_TEMPLATE(Exp2Test);
  RUN_VECTOR_TEMPLATE(ExpM1Test);
  RUN_VECTOR_TEMPLATE(Exp10Test);
  Log2Test();
  RUN_VECTOR_TEMPLATE(LogTest);
  RUN_VECTOR_TEMPLATE(Log10Test);
  RUN_VECTOR_TEMPLATE(Log1PTest);
}

} // Anonymous namespace

//-----------------------------------------------------------------------------
int UnitTestMathExp(int, char *[])
{
  return dax::internal::Testing::Run(RunExpTests);
}
