#if !defined(__real_array_h)
#define __real_array_h
#include <stdlib.h>
#include <math.h>
#include <complex.h>

typedef double real64;
typedef _Complex complex128;

typedef struct {
    size_t size, used;
    real64 *data;
} RealArray, *PRealArray;

typedef const RealArray *PCRealArray;

typedef struct {
    size_t size, used;
    complex128 *data;
} ComplexArray, *PComplexArray;

typedef const ComplexArray *PCComplexArray;

// proto types for memory management
PRealArray RealArray_New(size_t newSize);
void RealArray_Delete(PRealArray *toDelete);
PComplexArray ComplexArray_New(size_t newSize);
void ComplexArray_Delete(PComplexArray *toDelete);

#endif
