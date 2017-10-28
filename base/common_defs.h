/*!
\description common definitions
*/

#if !defined(_common_defs_h)
#define _common_defs_h

// common error numbers specific to out library
#define E_INDEX_OUT_OF_RANGE            0x8000
#define E_NO_CONVERGENCE                0x8001
#define E_NOT_IMPLEMENTED               0x8002
#define E_SINGULAR_MATRIX               0x8003
#define E_TYPE_MISMATCH                 0x8004
#define E_MATRIX_SIZE_MISMATCH          0x8005
#define E_NOT_SQUARE_MATRIX             0x8006

// data type enumeration
typedef enum _VMMathType {
    // basic scalar types
    VMMathType_string8 = 0, // ASCII
    VMMathType_string16, // UTF-16LE
    VMMathType_int8_t,
    VMMathType_int16_t,
    VMMathType_int32_t,
    VMMathType_int64_t,
    VMMathType_uint8_t,
    VMMathType_uint16_t,
    VMMathType_uint32_t,
    VMMathType_uint64_t,
    VMMathType_bool,
    VMMathType_real32,
    VMMathType_real64,
    VMMathType_complex64,
    VMMathType_complex128,
    VMMathType_RowColumn,

    // non-basic types
    VMMathType_RealArray,
    VMMathType_ComplexArray,
    VMMathType_RealMatrix,
    VMMathType_ComplexMatrix,
    VMMathType_RealPolynomial,
    VMMathType_ComplexPolynomial
} VMMathType, *PVMMathType;
typedef const VMMathType *PCVMMathType;
#endif
