#if !defined(_statistics_h)
#define _statistics_h

#include "base/_array.h"

real64 RealArray_Mean(PCRealArray p);
real64 RealArray_Stddev(PCRealArray p, real64 mean);

complex128 ComplexArray_Mean(PCComplexArray p);
real64 ComplexArray_Stddev(PCComplexArray p, complex128 mean);

#endif
