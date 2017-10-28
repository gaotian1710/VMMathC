#if !defined(_matrix_h)
#define _matrix_h
#include "base/_array.h"

#if defined(_matrix_c)
real64 MinimumPivotNorm;
#else
extern real64 MinimumPivotNorm;
#endif

typedef struct _RowColumn {
    size_t row, column;
} RowColumn, *PRowColumn;

typedef const RowColumn *PCRowColumn;

bool RowColumn_Equal(PCRowColumn x, PCRowColumn y);
bool RowColumn_NotEqual(PCRowColumn x, PCRowColumn y);
void RowColumn_SetSquare(PRowColumn y, size_t x);
size_t RowColumn_Size(PCRowColumn p);

typedef struct _RealMatrix {
    RowColumn rc;
    RealArray elements;
} RealMatrix, *PRealMatrix;

typedef const RealMatrix *PCRealMatrix;

typedef struct _RealMatrices {
    size_t size;
    PRealMatrix matrices[0];
} RealMatrices, *PRealMatrices;

typedef const RealMatrices *PCRealMatrices;

typedef struct _ComplexMatrix {
    RowColumn rc;
    ComplexArray elements;
} ComplexMatrix, *PComplexMatrix;

typedef const ComplexMatrix *PCComplexMatrix;

typedef struct _ComplexMatrices {
    size_t size;
    PComplexMatrix matrices[0];
} ComplexMatrices, *PComplexMatrices;

typedef const ComplexMatrices *PCComplexMatrices;

PRealMatrix RealMatrix_New(PCRowColumn rc);
void RealMatrix_Delete(PRealMatrix *ppMatrix);
PRealMatrices RealMatrices_New(size_t _size);
void RealMatrices_Delete(PRealMatrices* ppMatrices);

/*!
\brief y = x;
*/
int RealMatrix_Copy(PRealMatrix y, PCRealMatrix x);

/*!
\brief y += x;
*/
int RealMatrix_Add(PRealMatrix y, PCRealMatrix x);

/*!
\brief y -= x;
*/
int RealMatrix_Subtract(PRealMatrix y, PCRealMatrix x);

/*!
\brief z = y * x;
*/
int RealMatrix_Multiply(PRealMatrix z, PCRealMatrix y, PCRealMatrix x);

/*!
\brief copy submatrix of x to submatrix of y.
*/
int RealMatrix_CopySubMatrix(PRealMatrix y, PCRowColumn y_offset,
    PCRealMatrix x, PCRowColumn x_offset, PCRowColumn x_size);

/*!
\brief create a unit matrix
*/
int RealMatrix_Unit(PRealMatrix x);

/*!
\brief y = invert(x);
*/
int RealMatrix_Invert(PRealMatrix y, PCRealMatrix x, PRealMatrix work);

PComplexMatrix ComplexMatrix_New(PCRowColumn rc);
void ComplexMatrix_Delete(PComplexMatrix *ppMatrix);
PRealMatrices RealMatrices_New(size_t _size);
void RealMatrices_Delete(PRealMatrices* ppMatrices);

/*!
\brief y = x;
*/
int ComplexMatrix_Copy(PComplexMatrix y, PCComplexMatrix x);

/*!
\brief y += x;
*/
int ComplexMatrix_Add(PComplexMatrix y, PCComplexMatrix x);

/*!
\brief y -= x;
*/
int ComplexMatrix_Subtract(PComplexMatrix y, PCComplexMatrix x);

/*!
\brief z = y * x;
*/
int ComplexMatrix_Multiply(PComplexMatrix z, PCComplexMatrix y, PCComplexMatrix x);

/*!
\brief copy submatrix of x to submatrix of y.
*/
int ComplexMatrix_CopySubMatrix(PComplexMatrix y, PCRowColumn y_offset,
    PCComplexMatrix x, PCRowColumn x_offset, PCRowColumn x_size);

/*!
\brief y = invert(x);
*/
int ComplexMatrix_Invert(PComplexMatrix y, PCComplexMatrix x, PComplexMatrices work);

#endif
