#define _matrix_c
#include <stdbool.h>
#include "base/_matrix.h"
#include "base/common_defs.h"
#include "base/_array_blas.h"

bool RowColumn_Equal(PCRowColumn x, PCRowColumn y)
{
    return (x->row == y->row) && (x->column == y->column);
}

bool RowColumn_NotEqual(PCRowColumn x, PCRowColumn y)
{
    return (x->row != y->row) || (x->column != y->column);
}

void RowColumn_SetSquare(PRowColumn y, size_t x)
{
    (y->row) = (y->column) = x;
}

size_t RowColumn_Size(PCRowColumn p)
{
    return (p->row) * (p->column);
}

PRealMatrix RealMatrix_New(PCRowColumn rc)
{
    PRealMatrix p = (PRealMatrix)malloc(
        sizeof(RealMatrix) + sizeof(real64) * rc->row * rc->column
    );
    p->rc = *rc;
    p->elements.used = p->elements.size = rc->row * rc->column;
    return p;
}
void RealMatrix_Delete(PRealMatrix *ppMatrix)
{
    if (*ppMatrix)
    {
        free((void*)(*ppMatrix));
        *ppMatrix = NULL;
    }
}

PRealMatrices RealMatrices_New(size_t _size)
{
    return (PRealMatrices)malloc(sizeof(RealMatrices) + sizeof(PRealMatrix));
}

void RealMatrices_Delete(PRealMatrices* ppMatrices)
{
    if (*ppMatrices != NULL)
    {
        free((void*)(*ppMatrices));
        *ppMatrices = NULL;
    }
}

int RealMatrix_Copy(PRealMatrix y, PCRealMatrix x)
{
    if (y->rc.row == x->rc.row && y->rc.column ==x->rc.column)
    {
        return RealArray_Copy(&(y->elements), &(x->elements));
    }
    else
    {
        return E_MATRIX_SIZE_MISMATCH;
    }
}

int RealMatrix_Add(PRealMatrix y, PCRealMatrix x)
{
    if (y->rc.row == x->rc.row && y->rc.column ==x->rc.column)
    {
        return RealArray_Add(&(y->elements), &(x->elements));
    }
    else
    {
        return E_MATRIX_SIZE_MISMATCH;
    }
}

int RealMatrix_Subtract(PRealMatrix y, PCRealMatrix x)
{
    if (y->rc.row == x->rc.row && y->rc.column ==x->rc.column)
    {
        return RealArray_Subtract(&(y->elements), &(x->elements));
    }
    else
    {
        return E_MATRIX_SIZE_MISMATCH;
    }
}

const real64* RealMatrix_ConstRowHead(PCRealMatrix p, size_t row_index)
{
    return p->elements.data + row_index * p->rc.column;
}

real64* RealMatrix_RowHead(PRealMatrix p, size_t row_index)
{
    return p->elements.data + row_index * p->rc.column;
}

const real64* RealMatrix_ConstColumnHead(PCRealMatrix p, size_t column_index)
{
    return p->elements.data + column_index;
}

real64* RealMatrix_ColumnHead(PRealMatrix p, size_t column_index)
{
    return p->elements.data + column_index;
}

int RealMatrix_Multiply(PRealMatrix z, PCRealMatrix y, PCRealMatrix x)
{
    if (z->rc.row == y->rc.row && z->rc.column == x->rc.column && y->rc.column == x->rc.row)
    {
        real64* pDst = z->elements.data;
        for (size_t row = 0; row < z->rc.row; row++)
        {
            for (size_t column = 0; column < z->rc.column; column++)
            {
                *pDst = 0.0;
                const real64* pSrcLeft = RealMatrix_ConstRowHead(y, row);
                const real64* pSrcRight = RealMatrix_ConstColumnHead(x, column);
                for (size_t i = 0; i < y->rc.column; i++)
                {
                    *pDst += *pSrcLeft * *pSrcRight;
                    pSrcLeft++;
                    pSrcRight += x->rc.column;
                }
            }
        }
        return 0;
    }
    else
    {
        return E_MATRIX_SIZE_MISMATCH;
    }
}

int RealMatrix_CopySubMatrix(PRealMatrix y, PCRowColumn y_offset,
    PCRealMatrix x, PCRowColumn x_offset, PCRowColumn x_size)
{
    size_t x_EndRow = x_offset->row + x_size->row;
    size_t x_EndColumn = x_offset->column + x_size->column;
    if (x_EndRow > x->rc.row ||
        x_EndColumn > x->rc.column ||
        y_offset->row + x_size->row > y->rc.row ||
        y_offset->column + x_size->column > y->rc.column)
    {
        return E_MATRIX_SIZE_MISMATCH;
    }
    else
    {
        for (size_t xRow = x_offset->row, yRow = y_offset->row; xRow < x_EndRow; xRow++, yRow++)
        {
            real64* pDst = RealMatrix_RowHead(y, yRow) + y_offset->column;
            const real64* pSrc = RealMatrix_ConstRowHead(x, xRow) + x_offset->column;
            for (size_t xColumn = x_offset->column, yColumn = y_offset->column; xColumn < x_EndColumn; xColumn++, yColumn++)
            {
                *pDst++ = *pSrc++;
            }
        }
        return 0;
    }
}

int RealMatrix_Unit(PRealMatrix x)
{
    real64* pDst = RealMatrix_RowHead(x, 0);
    for (size_t row = 0; row < x->rc.row; row++)
    {
        for (size_t column = 0; column < x->rc.column; column++)
        {
            *pDst = (row == column) ? 1.0 : 0.0;
        }        
    }
    return 0;
}

bool RealMatrix_IsNotSquare(PCRealMatrix x)
{
    return x->rc.row != x->rc.column;
}

int RealMatrix_SelectPivot(PCRealMatrix m, size_t _row)
{
    real64 pivotNorm = *(RealMatrix_ConstRowHead(m, _row) + _row);
    int pivotRow = _row;
    for (int rowCheck = _row + 1; rowCheck < m->rc.row; rowCheck++)
    {
        real64 pivotNormCheck = *(RealMatrix_ConstRowHead(m, rowCheck) + _row);
        if (pivotNormCheck > pivotNorm) 
        {
            pivotNorm = pivotNormCheck;
            pivotRow = rowCheck;
        }
    }
    return pivotNorm < MinimumPivotNorm ? -1 : pivotRow;
}

void RealMatrix_SwapRows(PRealMatrix m, size_t row0, size_t row1)
{
    real64 tmp;
    real64 *p0 = RealMatrix_RowHead(m, row0);
    real64 *p1 = RealMatrix_RowHead(m, row1);
    for (size_t column = 0; column < m->rc.column; column++)
    {
        tmp = *p0;
        *p0++ = *p1;
        *p1++ = tmp;
    }
}

real64* RealMatrix_Element(PRealMatrix m, size_t row, size_t column)
{
    return m->elements.data + row * m->rc.column + column;
}

const real64* RealMatrix_ConstElement(PCRealMatrix m, size_t row, size_t column)
{
    return m->elements.data + row * m->rc.column + column;
}

int RealMatrices_Invert(PRealMatrix work)
{
    real64* rowHead = RealMatrix_RowHead(work, 0);
    size_t columns = work->rc.column;
    for (size_t rowPv = 0; rowPv < work->rc.row; 
         rowPv++, rowHead += work->rc.column, columns--)
    {
        // select pivot
        size_t rowPivotCandidate = rowPv;
        real64 normPivotCandidate = abs(*RealMatrix_Element(work, rowPivotCandidate, rowPivotCandidate));

        for (size_t row = rowPivotCandidate + 1; row < work->rc.row; row++)
        {
            real64 normTemp = abs(*(RealMatrix_Element(work, row, rowPv)));
            if (normTemp > normPivotCandidate)
            {
                normPivotCandidate = normTemp;
                rowPivotCandidate = row;;
            }
        }

        if (rowPivotCandidate != rowPv)
        {
            RealMatrix_SwapRows(work,rowPivotCandidate, rowPv);
        }

        // normalize  pivot row
        real64 normalizingWith = 
            1.0/(*(RealMatrix_ConstElement(work, rowPv, rowPv)));
        RealArray ra = {columns, columns, rowHead};
        RealArray_Multiply(&ra, normalizingWith);

        // delete pivot column elements other than pivot itself
        RealArray raToDelete = { ra.size, ra.used, work->elements.data + rowPv };
        for (size_t rowToDelete = 0; rowToDelete < work->rc.row;
             rowToDelete++, raToDelete.data += work->rc.column)
        {
            if (rowToDelete != rowPv)
            {
                real64 m = -(*(RealMatrix_ConstElement(work, rowToDelete, rowPv)));
                RealArray_MultiplyAdd(&raToDelete, &ra, m);
            }
        }
    }
    return 0;
}

int RealMatrix_Invert(PRealMatrix y, PCRealMatrix x, PRealMatrix work)
{
    // check size of x and y
    if (RealMatrix_IsNotSquare(x) || RealMatrix_IsNotSquare(y)) return E_NOT_SQUARE_MATRIX;
    if (RowColumn_NotEqual(&(x->rc), &(y->rc))) return E_MATRIX_SIZE_MISMATCH;

    // check size of work
    if (work->rc.row != x->rc.row || work->rc.column != (2 * x->rc.column)) return E_MATRIX_SIZE_MISMATCH;

    // fill work
    RowColumn zero = {0,0};
    RealMatrix_CopySubMatrix(work, &zero, x, &zero, &(x->rc));
    for (size_t row = 0; row < x->rc.row; row++)
    {
        for (size_t column = 0; column < x->rc.column; column++)
        {
            *(RealMatrix_Element(work, row, column + x->rc.column)) =
                (row == column) ? 1.0 : 0.0;
        }
    }

    // matrix operation
    int err = RealMatrices_Invert(work);
    if (err) return err;

    // copy result to y
    RowColumn offset = {0, x->rc.column};
    RealMatrix_CopySubMatrix(y, &zero, work, &offset, &(x->rc));
    return 0;
}