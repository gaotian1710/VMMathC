#include "UT/UT_common.h"
#include "base/_array.h"
#include "UT_runner.h"
#include "UT_array.h"

void UT_RealArray_NewAndDelete()
{
    const int indexMax = 0xffffff;
    PRealArray p0 = RealArray_New(2 * indexMax + 1);
    PRealArray p1 = RealArray_New(2 * indexMax + 1);
    size_t index = 0;
    for (int i = -indexMax; i <= indexMax; i++)
    {
        p0->data[index] = (real64)i;
        p1->data[index] = (real64)(-i);
        index++;
    }

    index = 0;
    real64 sum = 0.0;
    for (int i = -indexMax; i <= indexMax; i++)
    {
        sum += (p0->data[index] + p1->data[index]);
        index++;
    }
    UT_info("sum = %le\n", sum);
    real64 refZero = 0.0;
    real64 tol = 1.0e-8;

    AssertInfo ai = { "RealArray_New()", "_array.c", "real array memory allocation", 0 };
    AssertData ad = {
        (const void*)&refZero, (const void*)&sum, (const void*)&tol, VMMathType_real64
    };
    AssertRecord ar = {
        ai, ad, NULL, UT_double_Equal, NULL
    };
    UT_formatCsv(&ar);
    UT_writeTestResult();
}

void UT_ComplexArray_NewAndDelete()
{
    const int indexMax = 0xffffff;
    PComplexArray p0 = ComplexArray_New(2 * indexMax + 1);
    PComplexArray p1 = ComplexArray_New(2 * indexMax + 1);
    size_t index = 0;
    for (int i = -indexMax; i <= indexMax; i++)
    {
        p0->data[index] = CMPLX((real64)i ,(real64)i);
        p1->data[index] = CMPLX((real64)(-i), (real64)i);
        index++;
    }

    index = 0;
    complex128 sum = CMPLX(0.0, 0.0);
    for (int i = -indexMax; i <= indexMax; i++)
    {
        sum += (p0->data[index] + p1->data[index]);
        index++;
    }
    UT_info("sum = %le, %le\n", creal(sum), cimag(sum));
    complex128 refZero = CMPLX(0.0, 0.0);
    complex128 tol = CMPLX(1.0e-8, 0.0);

    AssertInfo ai = { "ComplexArray_New()", "_array.c", "complex array memory allocation", 0 };
    AssertData ad = {
        (const void*)&refZero, (const void*)&sum, (const void*)&tol, VMMathType_complex128
    };
    AssertRecord ar = {
        ai, ad, NULL, UT_complex_Equal, NULL
    };
    UT_formatCsv(&ar);
    UT_writeTestResult();
}
