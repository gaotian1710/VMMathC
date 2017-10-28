#include "base/_random.h"
#include <time.h>
#include <stdint.h>

void RandomInstance_Init(PRandomInstance p) 
{
    struct timespec t;
    int err = clock_gettime(CLOCK_REALTIME, &t);
    p->n = (long)t.tv_sec ^ (long)t.tv_nsec;
}

// private method
void RNG_InitAB(PRNG p)
{
    real64 Xmean = -0.5 * (real64)(p->nAcc);
    real64 SIGMA = (real64)INT32_MAX * sqrt((real64)(p->nAcc)) / sqrt(3.0);
    real64 X0 = Xmean - SIGMA, X1 = Xmean + SIGMA;
    real64 _x0 = p->mean - p->stddev, _x1 = p->mean + p->stddev;
    p->a = (_x0 - _x1)/(X0 - X1);
    p->b = (_x1 * X0 - _x0 * X1) / (X0 - X1);
}

void RNG_Init(PRNG p, real64 _mean, real64 _stddev, int _nAcc) 
{
    RandomInstance_Init((PRandomInstance)p);
    p->mean = _mean;
    p->stddev = _stddev;
    p->nAcc = _nAcc;
    RNG_InitAB(p);
}

void RNG_GetReal64(PRNG p, PRealArray pData) 
{
    real64* pNumber = pData->data;
    real64 r;
    pData->used = pData->size;
    for (size_t i = 0; i < pData->size; i++)
    {
        r = 0.0;
        for (int i = 0; i < p->nAcc; i++)
        {
            p->ri.n = jrand48(p->ri.xsubi);
            r += (real64)(p->ri.n);
        }
        *pNumber = p->a * r + p->b;
        pNumber++;
    }
}

void RNG_GetComplex128(PRNG p, PComplexArray pData) 
{

}
