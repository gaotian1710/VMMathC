#if !defined(_array_blas_h)
#define _array_blas_h
#include <errno.h>
#include "base/_array.h"

/*!
\description y = x; // deep copy
*/
int RealArray_Copy(PRealArray y, PCRealArray x);

/*!
\description y += x;
*/
int RealArray_Add(PRealArray y, PCRealArray x);

/*!
\description y -= x;
*/
int RealArray_Subtract(PRealArray y, PCRealArray x);

/*!
\description y += m * x;
*/
int RealArray_MultiplyAdd(PRealArray y, PCRealArray x, real64 m);

/*!
\description return sum(x->data[i]), i = 0..used - 1;
*/
real64 RealArray_Sum(PCRealArray x);

/*!
\description innerProduct = x * y (inner product)
*/
int RealArray_InnerProduct(real64* innerProduct, PCRealArray x, PCRealArray y);

/*!
\description y *= x;
*/
void RealArray_Multiply(PRealArray y, real64 x);

/*!
\description y = x; // deep copy
*/
int ComplexArray_Copy(PComplexArray y, PCComplexArray x);

/*!
\description y += x;
*/
int ComplexArray_Add(PComplexArray y, PCComplexArray x);

/*!
\description y -= x;
*/
int ComplexArray_Subtract(PComplexArray y, PCComplexArray x);

/*!
\description y += m * x;
*/
int ComplexArray_MultiplyAdd(PComplexArray y, PCComplexArray x, complex128 m);

/*!
\description return sum(x->data[i]), i = 0..used - 1;
*/
complex128 ComplexArray_Sum(PCComplexArray x);

/*!
\description innerProduct = x * y (inner product)
*/
int ComplexArray_InnerProduct(complex128* innerProduct, PCComplexArray x, PCComplexArray y);

/*!
\description y *= x;
*/
void ComplexArray_Multiply(PComplexArray y, complex128 x);

#endif
