#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <qvector.h>
#include <qdebug.h>
#include "Exceptions.h"


template<typename T>
class SparseMatrix
{
public:
	// ========== construct and destruct ===============
	SparseMatrix(int n);
	SparseMatrix(int rows, int columns);
	SparseMatrix(const SparseMatrix<T> & m);
	~SparseMatrix();

	// ========== operation overload ==================
	SparseMatrix<T>& operator = (const SparseMatrix<T> & m);
	
	// ========= priavte values get ==============
	const int getNumRow() const;
	const int getNumCol() const;
	const int getNonzeroCount() const;
	const QVector<T> getOneRowValue(int row) const;
	const QVector<T> getOneRowColIndex(int row) const;
	const uint getOneRowValueCount(int row) const;

	const T get(int row, int col) const;
	const SparseMatrix<T>& set(T value, int row, int col);
	const SparseMatrix<T>& setIfNotExist(T value, int row, int col);
	const bool isExist(int row, int col) const;

	// ======== print ============
	void printMatrix() const;

private:
	// ========== assistant function  ===============
	void construct(int rows, int columns);
	void copyData(const SparseMatrix<T> &m);
	void destruct();

	// ========== parameter valid check ============
	void validateCoordinates(int row, int col) const;
	
private:
	// ========== values ==========================
	int m_numRows;
	int m_numCols;
	int m_numNonzero;

	QVector<T> m_value;
	QVector<int> m_colIndex;
	QVector<int> m_rowPtr;

};


#endif // !SPARSE_MATRIX_H
