#if !defined(_random_h)
#define _random_h

#include "base/_array.h"

typedef union _RandomInstance {
    long n;
    unsigned long u;
    unsigned short xsubi[3];
} RandomInstance, *PRandomInstance;

void RandomInstance_Init(PRandomInstance p);

typedef struct _RNG {
    RandomInstance ri;
    real64 mean, stddev;
    int nAcc; // nAcc times of nrand are summed to generate one output of RNG.

    // coefficients of linear mapping
    // return a * x + b; // x: raw random number by nrand48
    real64 a, b;
} RNG, *PRNG;

void RNG_Init(PRNG p, real64 _mean, real64 _stddev, int _nAcc);
void RNG_GetReal64(PRNG p, PRealArray pData);
void RNG_GetComplex128(PRNG p, PComplexArray pData);

#endif
