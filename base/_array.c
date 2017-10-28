#include "common_defs.h"
#include "base/_array.h"

// implementations for memory management
PRealArray RealArray_New(size_t newSize)
{
    PRealArray obj = (PRealArray)malloc(sizeof(RealArray) + sizeof(real64) * newSize);
    obj->size = obj->used = newSize;
    obj->data = (real64*)(obj + 1);
    for (size_t i = 0; i < obj->size; i++)
    {
        obj->data[i] = 0.0;
    }
    return obj;
}

void RealArray_Delete(PRealArray *toDelete)
{
    if (*toDelete)
    {
        free((void*)(*toDelete));
        *toDelete = NULL;
    }
}

PComplexArray ComplexArray_New(size_t newSize)
{
    PComplexArray obj = (PComplexArray)malloc(sizeof(ComplexArray) + sizeof(complex128) * newSize);
    obj->size = obj->used = newSize;
    obj->data = (complex128*)(obj + 1);
    for (size_t i = 0; i < obj->size; i++)
    {
        obj->data[i] = 0.0 + 0.0I;
    }
    return obj;
}

void ComplexArray_Delete(PComplexArray *toDelete)
{
    if (*toDelete)
    {
        free((void*)(*toDelete));
        *toDelete = NULL;
    }
}