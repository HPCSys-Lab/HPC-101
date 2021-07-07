GFORTRAN    = gfortran-mp-7 -Wall
CC          = gcc-mp-7 -Wall
MPI_FORTRAN = mpif90 -Wall
MPI_CC      = mpicc -Wall
LD = -lm

DIR_SEQ = objSeqDir
DIR_PAR = objParDir
SRC_SEQ = explicitSeq.c explUtilSeq.c
OBJ_SEQ = $(addprefix $(DIR_SEQ)/,$(SRC_SEQ:.c=.o))
SRC_PAR = explicitPar.c explUtilPar.c updateBound.c readParam.c
OBJ_PAR = $(addprefix $(DIR_PAR)/,$(SRC_PAR:.c=.o))

.PHONY: all
	
all: explicitSeq explicitPar

explicitSeq: $(OBJ_SEQ)
	$(CC) $(LD) -o $@ $^

$(DIR_SEQ)/%.o: %.c
	$(CC) -c $< -o $@

explicitPar: $(OBJ_PAR)
	$(MPI_CC) $(LD) -o $@ $^

$(DIR_PAR)/%.o: %.c
	$(MPI_CC) -c $< -o $@

$(OBJ_SEQ): | $(DIR_SEQ)
$(OBJ_PAR): | $(DIR_PAR)

$(DIR_SEQ):
	mkdir $(DIR_SEQ)

$(DIR_PAR):
	mkdir $(DIR_PAR)

clean:
	rm -f explicitSeq explicitPar
	rm -rf $(DIR_SEQ)
	rm -rf $(DIR_PAR)
