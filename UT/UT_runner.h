#pragma once
#define UT_INFO_LOG_FILE        "UT_info.log"
#define UT_RESULT_FILE          "UT_result.txt"

#define UT_BUFFER_LENGTH        1024
typedef struct _UT_buffer {
    size_t ccLength, ccFilled;
    char buffer[UT_BUFFER_LENGTH];
} UT_buffer, *PUT_buffer;


#define UT_info(...) { \
    FILE* fp = fopen(UT_INFO_LOG_FILE, "a"); \
    fprintf(fp, "%s,",__TIME__); fprintf(fp, __VA_ARGS__); \
    fclose(fp); \
    printf("%s,",__TIME__); printf(__VA_ARGS__); \
}

#define UT_errexit(errcode,file,line,message) if (errcode) { \
    FILE* fp = fopen(UT_INFO_LOG_FILE, "a"); \
    fprintf(fp, "%s,",__TIME__); \
    fprintf(fp, "0x%08x(%d),%s,%d,%s\n", errcode, errcode, file, line, message); \
    fclose(fp); \
    printf("%s,",__TIME__); \
    printf("0x%08x(%d),%s,%d,%s\n", errcode, errcode, file, line, message); \
    exit(errcode); \
}

#define UT_vp2pT(vp,T) ((const T*)vp)
#define UT_vp2double(vp) (*UT_vp2pT(vp,double))
#define UT_vp2int(vp) (*UT_vp2pT(vp,int))
#define UT_vp2size_t(vp) (*UT_vp2pT(vp,size_t))
#define UT_vp2float(vp) (*((const float*)vp))
#define UT_vp2long(vp) (*((const long*)vp))
#define UT_vp2pcch(vp)  ((const char*)vp)
#define UT_vp2pcwch(vp) ((const wchar_t*)vp)
#define UT_vp2int8_t(vp) (*((const int8_t*)vp))
#define UT_vp2int16_t(vp) (*((const int16_t*)vp))
#define UT_vp2int32_t(vp) (*((const int32_t*)vp))
#define UT_vp2int64_t(vp) (*((const int64_t*)vp))
#define UT_vp2uint8_t(vp) (*((const uint8_t*)vp))
#define UT_vp2uint16_t(vp) (*((const uint16_t*)vp))
#define UT_vp2uint32_t(vp) (*((const uint32_t*)vp))
#define UT_vp2uint64_t(vp) (*((const uint64_t*)vp))
#define UT_vp2boolstring(vp) (*((const bool*)vp) ? "true" : "false")


typedef struct _AssertData {
    const void* refData;
    const void* targetData;
    const void* tolerance;
    VMMathType type;
} AssertData, *PAssertData;

typedef const AssertData *PCAssertData;

typedef bool (*UT_evaluate)(PCAssertData data);
typedef void (*UT_dataFormatter)(PCAssertData data);

typedef struct _AssertInfo {
    const char* functionName;
    const char* filename;
    const char* description;
    int functionReturnCode;
} AssertInfo, *PAssertInfo;

typedef const AssertInfo *PCAssertInfo;

typedef void (*UT_infoFormatter)(PCAssertInfo info);

typedef struct _AssertRecord {
    AssertInfo  info;
    AssertData  data;
    UT_infoFormatter infoFormatter;
    UT_evaluate   eval; // function returning true if the test fails.
    UT_dataFormatter dataFormatter; // function pointer to format refData and targetData, optional
} AssertRecord, *PAssertRecord;

typedef const AssertRecord *PCAssertRecord;

// Pass/fail discrimination methods
bool UT_double_NotEqual(PCAssertData data);
bool UT_double_Equal(PCAssertData data);
bool UT_fload_NotEqual(PCAssertData data);
bool UT_float_Equal(PCAssertData data);
bool UT_int_Equal(PCAssertData data);
bool UT_int_NotEqual(PCAssertData data);
bool UT_size_t_Equal(PCAssertData data);
bool UT_size_t_NotEqual(PCAssertData data);
bool UT_long_Equal(PCAssertData data);
bool UT_long_NotEqual(PCAssertData data);
bool UT_string_Equal(PCAssertData data);
bool UT_string_NotEqual(PCAssertData data);
bool UT_complex_Equal(PCAssertData data);
bool UT_complex_NotEqual(PCAssertData data);

// Test data formatting methods, accessing gUT_buffer
void UT_buffer_Clear(); // clear buffer to help the formatting methods.
void UT_formatCsvData(PCAssertData data); // only for c standard types like int32_t, double,...
void UT_formatCsvInfo(PCAssertInfo info);
void UT_formatCsv(PCAssertRecord rec);

void UT_open(const char* filePath);
void UT_close();
void UT_write();
void UT_writeTestResult();