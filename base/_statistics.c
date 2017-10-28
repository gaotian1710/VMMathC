#include "base/_statistics.h"
#include "base/_array_blas.h"

real64 RealArray_Mean(PCRealArray p)
{
    return RealArray_Sum(p) / (real64)(p->used);
}

real64 RealArray_Stddev(PCRealArray p, real64 mean)
{
    real64 sumDev = 0.0;
    const real64* pcSample = p->data;
    for (size_t i = 0; i < p->used; i++)
    {
        real64 dev = *pcSample++ - mean;
        sumDev += (dev * dev);
    }
    return sqrt(sumDev / (real64)(p->used - 1));
}

complex128 ComplexArray_Mean(PCComplexArray p)
{
    return ComplexArray_Sum(p) / CMPLX((real64)(p->used), 0.0);
}

real64 ComplexArray_Stddev(PCComplexArray p, complex128 mean)
{
    complex128 sumDev = CMPLX(0.0, 0.0);
    const complex128* pcSample = p->data;
    for (size_t i= 0; i < p->used; i++)
    {
        complex128 dev = (*pcSample++ - mean);
        sumDev += (dev * conj(dev));
    }
    return sqrt(creal(sumDev) / (real64)(p->used - 1));
}