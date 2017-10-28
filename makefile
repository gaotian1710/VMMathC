CC = gcc-5
BASE_OBJ = _array.o _array_blas.o _random.o _statistics.o _matrix.o
UT_OBJ = UT_runner.o UT_array.o UT_array_blas.o
BASE_INC = base/_array.h base/_array_blas.h base/_random.h base/_matrix.h
BASE_C = base/_array.c base/_array_blas.c base/_random.c base/_matrix.c


UT_runner	:	$(UT_OBJ) $(BASE_OBJ)
	$(CC) -o $@ $^ -lm

UT_runner.o	:	UT/UT_runner.c UT/UT_runner.h $(BASE_INC)
	$(CC) -c -I . $<

UT_array.o	:	UT/UT_array.c UT/UT_runner.h UT/UT_array.h $(BASE_INC)
	$(CC) -c -I . $<

UT_array_blas.o	:	UT/UT_array_blas.c UT/UT_runner.h UT/UT_array_blas.h $(BASE_INC)
	$(CC) -c -I . $<

_array.o	:	base/_array.c base/_array.h
	$(CC) -c -I . $<

_array_blas.o	:	base/_array_blas.c base/_array_blas.h
	$(CC) -c -I . $<

_random.o	:	base/_random.c base/_random.h
	$(CC) -c -I . $<

_statistics.o	:	base/_statistics.c base/_statistics.h
	$(CC) -c -I . $<

_matrix.o	:	base/_matrix.c base/_matrix.h
	$(CC) -c -I . $<

clean	:
	rm *.o
	rm UT_runner

