#include "base/_array_blas.h"

/*!
\description y = x; // deep copy
*/
int RealArray_Copy(PRealArray y, PCRealArray x)
{
    // insufficient buffer size
    if (y->size < x->used)
    {
        return ENOBUFS;
    }

    // copy operation
    real64* dst = y->data;
    const real64* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst++ = *src++;
    }
    y->used = x->used;
    return 0;
}

/*!
\description y += x;
*/
int RealArray_Add(PRealArray y, PCRealArray x)
{
    // array size mismatch
    if (y->used != x->used)
    {
        return ERANGE;
    }

    // add array
    real64* dst = y->data;
    const real64* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst += *src;
        dst++; src++;
    }
    return 0;
}

/*!
\description y -= x;
*/
int RealArray_Subtract(PRealArray y, PCRealArray x)
{
    // array size mismatch
    if (y->used != x->used)
    {
        return ERANGE;
    }

    // y -= x, array operation
    real64* dst = y->data;
    const real64* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst -= *src;
        dst++; src++;
    }
    return 0;
}


/*!
\description y += m * x;
*/
int RealArray_MultiplyAdd(PRealArray y, PCRealArray x, real64 m)
{
    // array size mismatch
    if (y->used != x->used)
    {
        return ERANGE;
    }

    // y = m * x, array operation
    real64* dst = y->data;
    const real64* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst += m * *src;
        dst++; src++;
    }
    return 0;
}


/*!
\description return sum(x->data[i]), i = 0..used - 1;
*/
real64 RealArray_Sum(PCRealArray x)
{
    real64 sumx = 0.0;
    if (!x) return sumx;
    const real64* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        sumx += *src++;
    }
    return sumx;
}

/*!
\description innerProduct = x * y (inner product)
*/
int RealArray_InnerProduct(real64* innerProduct, PCRealArray x, PCRealArray y)
{
    // array size mismatch
    if (x->used != y->used)
    {
        return ERANGE;
    }

    // array operation
    *innerProduct = 0.0;
    const real64 *src0 = x->data, *src1 = y->data;
    for (size_t i = 0; i < x->used; i++) 
    {
        *innerProduct += *src0 * *src1;
        src0++; src1++;
    }
    return 0;
}

/*!
\description y *= x;
*/
void RealArray_Multiply(PRealArray y, real64 x)
{
    real64* dst = y->data;
    for (size_t i = 0; i < y->used; i++)
    {
        *dst *= x;
        dst++;
    }
}

/*!
\description y = x; // deep copy
*/
int ComplexArray_Copy(PComplexArray y, PCComplexArray x)
{
    // insufficient buffer size
    if (y->size < x->used)
    {
        return ENOBUFS;
    }

    // copy operation
    complex128* dst = y->data;
    const complex128* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst++ = *src++;
    }
    y->used = x->used;
    return 0;
}

/*!
\description y += x;
*/
int ComplexArray_Add(PComplexArray y, PCComplexArray x)
{
    // array size mismatch
    if (y->used != x->used)
    {
        return ERANGE;
    }

    // add array
    complex128* dst = y->data;
    const complex128* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst += *src;
        dst++; src++;
    }
    return 0;
}


/*!
\description y += m * x;
*/
int ComplexArray_MultiplyAdd(PComplexArray y, PCComplexArray x, complex128 m)
{
    // array size mismatch
    if (y->used != x->used)
    {
        return ERANGE;
    }

    // y = m * x, array operation
    complex128* dst = y->data;
    const complex128* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        *dst += m * *src;
        dst++; src++;
    }
    return 0;
}

/*!
\description sumx = sim(x->data[i]), i = 0..used - 1;
*/
complex128 ComplexArray_Sum(PCComplexArray x)
{
    complex128 sumx = 0.0;
    if (!x) return sumx;
    const complex128* src = x->data;
    for (size_t i = 0; i < x->used; i++)
    {
        sumx += *src++;
    }
    return sumx;
}

/*!
\description innerProduct = x * y (inner product)
*/
int ComplexArray_InnerProduct(complex128* innerProduct, PCComplexArray x, PCComplexArray y)
{
    // array size mismatch
    if (x->used != y->used)
    {
        return ERANGE;
    }

    // array operation
    *innerProduct = 0.0;
    const complex128 *src0 = x->data, *src1 = y->data;
    for (size_t i = 0; i < x->used; i++) 
    {
        *innerProduct += *src0 * *src1;
        src0++; src1++;
    }
    return 0;
}

/*!
\description y *= x;
*/
void ComplexArray_Multiply(PComplexArray y, complex128 x)
{
    complex128* dst = y->data;
    for (size_t i = 0; i < y->used; i++)
    {
        *dst *= x;
        dst++;
    }
}
