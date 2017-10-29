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

void UT_RealArrayAdd()
{
    int errRef = 0;
    int err = errRef;
    // normal case
    PRealArray ra0 = RealArray_New(10);
    PRealArray ra1 = RealArray_New(10);
    for (size_t i = 0; i < ra0->used; i++)
    {
        ra0->data[i] = ra1->data[i] = (real64)i;
    }
    err = RealArray_Add(ra0, ra1);
    AssertInfo ai = { "RealArray_Add", "_array_blas.c", "successfull error code", err };
    real64 eval = 0.0;
    { // calculate error
        for (size_t i = 0; i < ra0->used; i++)
        {
            real64 diff = ra0->data[i] - 2.0 * ra1->data[i];
            eval += diff * diff;
        }
        AssertData ad = { &UT_zero_real64, &eval, &UT_tol_real64, VMMathType_real64 };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_double_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }

    // error by size mismatch
    errRef = ERANGE;
    RealArray_Delete(&ra1);
    ra1 = RealArray_New(11);
    ai.description = "failure error code";
    ai.functionReturnCode = RealArray_Add(ra0, ra1);
    {
        AssertData ad = { (const void*)&errRef, (const void*)&ai.functionReturnCode,
            (const void*)NULL, VMMathType_int32_t };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
    // delete real arrays
    RealArray_Delete(&ra0); RealArray_Delete(&ra1);
}
void UT_RealArray_Subtract()
{
    PRealArray ra0 = RealArray_New(10), ra1 = RealArray_New(10);
    for (size_t i = 0; i < ra0->used; i++)
    {
        ra0->data[i] = ra1->data[i] = (real64)i;
    }
    AssertInfo ai = { "RealArray_Subtract", "_array_blas.c", "successful error code", 0 };
    ai.functionReturnCode = RealArray_Subtract(ra0, ra1);
    { // assert error code
        AssertData ad = {
            (const void*)&UT_zero_int32, (const void*)&ai.functionReturnCode, 
            (const void*)NULL, VMMathType_int32_t
        };
        AssertRecord ar = { ai, ad,UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
    { // assert subtraction result
        ai.description = "subtraction numeric result";
        real64 eval = 0.0;
        for (size_t i = 0; i < ra0->used; i++)
        {
            eval += (ra0->data[i]) * (ra0->data[i]);
        }
        AssertData ad = { &UT_zero_real64, &eval, &UT_tol_real64, VMMathType_real64 };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_double_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }

    RealArray_Delete(&ra1);
    int errRef = ERANGE;
    ra1 = RealArray_New(11);
    ai.description = "failure error code by size mismacht";
    ai.functionReturnCode = RealArray_Subtract(ra0, ra1);
    {// assert function return code
        AssertData ad = { &errRef, &ai.functionReturnCode, (const void*)NULL, VMMathType_int32_t };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
}

void UT_RealArray_MultiplyAdd()
{
    PRealArray ra0 = RealArray_New(10), ra1 = RealArray_New(10);
    for (size_t i = 0; i < ra0->used; i++)
    {
        ra0->data[i] = (real64)i;
        ra1->data[i] = 1.5 * (real64)i;
    }
    AssertInfo ai = { "RealArray_MultiplyAdd", "_array_blas.c", "successful return code",
        RealArray_MultiplyAdd(ra0, ra1, -1.0/1.5)
    };
    { // assert return code
        AssertData ad = { &UT_zero_int32, &ai.functionReturnCode, (const void*)NULL, VMMathType_int32_t };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
    { // assert numeric result
        real64 eval = 0.0;
        for (size_t i = 0; i < ra0->used; i++)
        {
            eval += (ra0->data[i]) * (ra0->data[i]);
        }
        AssertData ad = { &UT_zero_real64, &eval, &UT_tol_real64, VMMathType_real64 };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_double_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }

    // error by size mismatch
    RealArray_Delete(&ra1);
    ra1 = RealArray_New(ra0->used + 1);
    ai.functionReturnCode = RealArray_MultiplyAdd(ra0, ra1, 1.5);
    ai.description = "failure return code by size mismatch";
    int errRef = ERANGE;
    {
        AssertData ad = { &errRef, &ai.functionReturnCode, (const void*)NULL, VMMathType_int32_t };
        AssertRecord ar = { ai, ad, UT_formatCsvInfo, UT_int_Equal, UT_formatCsvData };
        UT_formatCsv(&ar);
        UT_writeTestResult();
    }
}

void UT_RealArray_Sum()
{
    
}
// void UT_RealArray_InnerProduct();
// void UT_RealArray_Multiply();
void UT_ComplexArrayCopy()
{

}
// void UT_ComplexArrayAdd();
// void UT_ComplexArray_Subtract();
// void UT_ComplexArray_MultiplyAdd();
// void UT_ComplexArray_Sum();
// void UT_ComplexArray_InnerProduct();
// void UT_ComplexArray_Multiply();
