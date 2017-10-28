#include "UT/UT_common.h"
#include "base/_array_blas.h"
#include "UT/UT_runner.h"

void UT_RealArrayCopy()
{
    // successful copy
    PRealArray a0 = RealArray_New(10);
    PRealArray a1 = RealArray_New(10);
    int err = 0, errRef = 0, zero = 0;
    a0->used = 0;
    for (size_t i = 0; i < a0->size; i++)
    {
        a0->used++;
        a0->data[i] = (real64)i;
    }

    AssertInfo ai = { "RealArray_Copy", "_array_blas.c", "successful error code", err };
    err = RealArray_Copy(a1, a0);
    {
        AssertData ad = { (const void*)&errRef, (const void*)&err, (const void*)&zero, VMMathType_int32_t };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }

    ai.description = "compare copy source and destination";
    real64 sqSumDiff = 0.0; // square sum of differences of each elements
    const real64 sqSumDiffRef = 0.0, sqSumDiffTol = 1.0e-8;
    for (size_t i = 0; i < a0->used; i++)
    {
        real64 diff = a0->data[i] - a1->data[i];
        sqSumDiff += diff * diff;
    }
    {
        AssertData ad = { (const void*)&sqSumDiffRef, (const void*)&sqSumDiff, (const void*)&sqSumDiffTol, VMMathType_real64 };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_double_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
    // insufficient size failure
    RealArray_Delete(&a1);
    a1 = RealArray_New(9);
    err = RealArray_Copy(a1, a0);
    {
        ai.description = "failure error code by insufficient buffer";
        errRef = ENOBUFS;
        AssertData ad = {
            (const void*)&errRef, (const void*)&err, (const void*)&zero,
            VMMathType_int32_t
        };
        AssertRecord ar = {
            ai, ad, UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData
        };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
    RealArray_Delete(&a1);
}
// void UT_RealArrayAdd();
// void UT_RealArray_Subtract();
// void UT_RealArray_MultiplyAdd();
// void UT_RealArray_Sum();
// void UT_RealArray_InnerProduct();
// void UT_RealArray_Multiply();
// void UT_ComplexArrayCopy();
// void UT_ComplexArrayAdd();
// void UT_ComplexArray_Subtract();
// void UT_ComplexArray_MultiplyAdd();
// void UT_ComplexArray_Sum();
// void UT_ComplexArray_InnerProduct();
// void UT_ComplexArray_Multiply();
