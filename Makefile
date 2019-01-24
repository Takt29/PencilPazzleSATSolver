TARGET = nonogramSolver sudokuSolver
CXX = g++
CXX_OPTION = -std=c++14
LIBS = lib/CNFGenerator.o lib/CNFLiteral.o lib/CNFStatus.o lib/MiniSatRunner.o
LIBS_H = lib/CNFGenerator.h lib/CNFLiteral.h lib/CNFStatus.h lib/MiniSatRunner.h

all: ${TARGET}

nonogramSolver: solver/NonogramSolver.o ${LIBS} ${LIBS_H}
	${CXX} ${CXX_OPTION} ${LIBS} solver/NonogramSolver.o -o $@

sudokuSolver: solver/SudokuSolver.o ${LIBS} ${LIBS_H}
	${CXX} ${CXX_OPTION} ${LIBS} solver/SudokuSolver.o -o $@

.cpp.o:
	${CXX} ${CXX_OPTION} -c $< -o $@

clear:
	rm ${LIBS}
