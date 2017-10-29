#define _UT_runner_c
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "base/_random.h"
#include "base/_array_blas.h"
#include "base/_statistics.h"
#include "base/common_defs.h"
#include "UT/UT_runner.h"
#include "UT/UT_array.h"
#include "UT/UT_array_blas.h"
#pragma region global variables
UT_buffer gUT_buffer = {
    UT_BUFFER_LENGTH, 0
};

FILE* gUT_fp = NULL; // file pointer for test result CSV file.

const real64 UT_zero_real64 = 0.0;
const real64 UT_tol_real64 = 1.0e-8;
const complex128 UT_zero_complex128 = 0.0 + I * 0.0;
const int32_t UT_zero_int32 = 0;
const uint64_t UT_zero_size = 0;
#pragma endregion global variables

#pragma region value evaluations
bool UT_complex_NotEqual(PCAssertData data)
{
    complex128 diff =
        *(UT_vp2pT(data->refData,complex128)) - *(UT_vp2pT(data->targetData,complex128));
    return cabs(diff) > UT_vp2double(data->tolerance);
}

bool UT_complex_Equal(PCAssertData data)
{
    complex128 diff =
        *(UT_vp2pT(data->refData,complex128)) - *(UT_vp2pT(data->targetData,complex128));
    return cabs(diff) <= UT_vp2double(data->tolerance);
}

bool UT_double_NotEqual(PCAssertData data)
{
    return fabs(UT_vp2double(data->refData) - UT_vp2double(data->targetData))
        > UT_vp2double(data->tolerance);
}

bool UT_double_Equal(PCAssertData data)
{
    return fabs(UT_vp2double(data->refData) - UT_vp2double(data->targetData))
        <= UT_vp2double(data->tolerance);
}

bool UT_fload_NotEqual(PCAssertData data)
{
    return fabs(UT_vp2float(data->refData) - UT_vp2float(data->targetData))
    > UT_vp2float(data->tolerance);
}

bool UT_float_Equal(PCAssertData data)
{
    return fabs(UT_vp2float(data->refData) - UT_vp2float(data->targetData))
        <= UT_vp2float(data->tolerance);
}

bool UT_int_Equal(PCAssertData data)
{
    return (UT_vp2int(data->refData) == UT_vp2int(data->targetData));
}

bool UT_int_NotEqual(PCAssertData data)
{
    return (UT_vp2int(data->refData) != UT_vp2int(data->targetData));
}

bool UT_size_t_Equal(PCAssertData data)
{
    return (UT_vp2size_t(data->refData) == UT_vp2size_t(data->targetData));
}

bool UT_size_t_NotEqual(PCAssertData data)
{
    return (UT_vp2size_t(data->refData) != UT_vp2size_t(data->targetData));
}

bool UT_long_Equal(PCAssertData data)
{
    return (UT_vp2long(data->refData) == UT_vp2long(data->targetData));
}

bool UT_long_NotEqual(PCAssertData data)
{
    return (UT_vp2long(data->refData) != UT_vp2long(data->targetData));
}

bool UT_string_Equal(PCAssertData data)
{
    return strcmp(UT_vp2pT(data->refData, char), UT_vp2pT(data->targetData, char)) == 0;
}

bool UT_string_NotEqual(PCAssertData data)
{
    return strcmp(UT_vp2pT(data->refData, char), UT_vp2pT(data->targetData, char)) != 0;
}
#pragma endregion value evaluations

/*!
\brief format a simple scalar data type
\param data [in] pointer to data of any simple types
\param type [in] data type indentifier, refer to "common_defs.h"
\param isLast [in] true if the data is located at the end of a csv row.
*/
void UT_formatCsvSimpleType(const void* data, VMMathType type, bool isLast)
{
    static const char* formatStrings[2][16] =
    {
        {
            "\"%s\"\r\n", "\"%ls\"\r\n",
            "%d\r\n", "%d\r\n", "%d\r\n", "%ld\r\n", // intXX_t
            "%d\r\n", "%d\r\n", "%d\r\n", "%ld\r\n", // uintXX_t
            "%s\r\n", // bool
            "%e\r\n", "%le\r\n", // real numbers
            "\"(%e,%e)\"\r\n", "\"(%le,%le)\"\r\n", // complex numbers
            "\"(%d,%d)\"\r\n"// row, column pair
        },
        {
            "\"%s\",", "\"%ls\",",
            "%d,", "%d,", "%d,", "%ld,", // intXX_t
            "%d,", "%d,", "%d,", "%ld,", // uintXX_t
            "%s,", // bool
            "%e,", "%le,", // real numbers
            "\"(%e,%e)\",", "\"(%le,%le)\",", // complex numbers
            "\"(%d,%d)\","// row, column pair
        }
    };
    size_t typeOrder = (size_t)type;
    const char* formatString = isLast ? "%s\r\n" : "%s,";
    const char* *formatStrings2 = isLast ? formatStrings[0] : formatStrings[1];
    size_t typeOrderLimit = sizeof(formatStrings[0]) / sizeof(formatStrings[0][0]);
    size_t availableLength = gUT_buffer.ccLength - gUT_buffer.ccFilled - 1;
    UT_errexit(availableLength < 32 ? ENOBUFS : 0,
        __FILE__, __LINE__, "CSV buffer overflow");
    size_t writtenLength = 0;
    UT_errexit(typeOrder >= typeOrderLimit ? EINVAL : 0,
        __FILE__, __LINE__, "Invalid data type");
    data = (data == NULL) ? (const void*)"(null)" : data;
    formatString = formatStrings2[typeOrder];
    switch (type)
    {
    case VMMathType_string8:
        writtenLength = 
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString, 
                UT_vp2pcch(data));
        break;
    case VMMathType_string16:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2pcwch(data));
        break;
    case VMMathType_int8_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2int8_t(data));
        break;
    case VMMathType_int16_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2int16_t(data));
        break;
    case VMMathType_int32_t: 
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2int32_t(data));
        break;
    case VMMathType_int64_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2int64_t(data));
        break;
    case VMMathType_uint8_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2uint8_t(data));
        break;
    case VMMathType_uint16_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2uint16_t(data));
        break;
    case VMMathType_uint32_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2uint32_t(data));
        break;
    case VMMathType_uint64_t:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2uint64_t(data));
        break;
    case VMMathType_bool:
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString,
                UT_vp2boolstring(data));
        break;
    case VMMathType_real32:
    {
        float f = UT_vp2float(data);
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString, f);
        break;
    }
    case VMMathType_real64:
    {
        double d = UT_vp2double(data);
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString, d);
        break;
    }
    case VMMathType_complex64:
    {
        const float* pf = UT_vp2pT(data,float);
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString, *pf, *(pf + 1));
        break;
    }
    case VMMathType_complex128:
    {
        const double* pd = UT_vp2pT(data,double);
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString, *pd, *(pd + 1));
        break;
    }
    case VMMathType_RowColumn:
    {
        const size_t *ps = UT_vp2pT(data,size_t);
        writtenLength =
            snprintf(gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, formatString, *ps, *(ps + 1));
        break;
    }       
    }
    gUT_buffer.ccFilled += writtenLength;
}

void UT_formatCsvData(PCAssertData data)
{
    UT_formatCsvSimpleType(data->targetData, data->type, false);
    UT_formatCsvSimpleType(data->refData, data->type, false);
    UT_formatCsvSimpleType(data->tolerance, data->type, true);
}

void UT_formatCsvInfo(PCAssertInfo info)
{
    size_t available = gUT_buffer.ccLength - gUT_buffer.ccFilled - 1;
    size_t writtenLength = snprintf(
        gUT_buffer.buffer + gUT_buffer.ccFilled, available,
        "%s,\"%s\",\"%s\",0x%08x,%d,",
        info->filename, info->functionName, info->description,
        info->functionReturnCode, info->functionReturnCode
    );
    gUT_buffer.ccFilled += writtenLength;
}

void UT_buffer_Clear() 
{
    gUT_buffer.ccFilled = 0;
    gUT_buffer.buffer[0] = '\0';
}

void UT_formatPassFail(PCAssertRecord rec)
{
    const char* testResult = rec->eval(&(rec->data)) ? "pass" : "fail";
    UT_info("UT_formatPassFail(), testResult = %s\n", testResult);
    size_t availableLength = gUT_buffer.ccLength - gUT_buffer.ccFilled - 1;
    gUT_buffer.ccFilled += snprintf(
        gUT_buffer.buffer + gUT_buffer.ccFilled, availableLength, "%s,", testResult
    );
}

void UT_formatCsv(PCAssertRecord rec)
{
    UT_buffer_Clear();
    UT_info("UT_formatCsv(), UT_bufferClear() done\n");
    UT_formatCsvInfo(&(rec->info));
    UT_info("UT_formatCsv(), UT_formatCsvInfo() done, %zu, %s\n",
        gUT_buffer.ccFilled, gUT_buffer.buffer);
    
    UT_formatPassFail(rec);
    UT_info("UT_formatCsv(), UT_formatPassFail() done\n");
    if ((size_t)(rec->data.type) < (size_t)VMMathType_RealArray)
    {
        UT_formatCsvData(&(rec->data));
    }
    else
    {
        rec->dataFormatter(&(rec->data));
    }
}

void UT_open(const char* filePath)
{
    gUT_fp = fopen(filePath, "a");
    UT_info("UT_open() = 0x%16lx, errno = %d\n", (long unsigned int)gUT_fp, errno);
}
void UT_close()
{
    if (gUT_fp != NULL) fclose(gUT_fp);
}

void UT_write()
{
    int result = fputs(gUT_buffer.buffer, gUT_fp);
}

void UT_writeTestResult()
{
    UT_open(UT_RESULT_FILE);
    UT_write();
    UT_close();
}
void UT_random()
{
    RNG rng = {0};
    RNG_Init(&rng, 10.0, 10.0, 6);
    UT_info("mean = %le, sigma = %le, a = %le, b = %le\n", 
        rng.mean, rng.stddev, rng.a, rng.b);

    PRealArray pData = RealArray_New(1024*1024);
    RNG_GetReal64(&rng, pData);

    real64 mean = RealArray_Mean(pData);
    real64 stddev = RealArray_Stddev(pData, mean);
    UT_info("mean = %le, stddev = %le\n", mean, stddev);
    RealArray_Delete(&pData);
}

int main()
{
    UT_buffer_Clear();
    UT_info("UT_runner started.\n");

    //UT_random();
    UT_RealArray_NewAndDelete();
    UT_ComplexArray_NewAndDelete();

    UT_RealArrayCopy();
    UT_ComplexArrayCopy();

    UT_RealArrayAdd();
    UT_RealArray_Subtract();
    UT_RealArray_MultiplyAdd();
    
    UT_info("UT_runner exits.\n");
    return 0;
}
