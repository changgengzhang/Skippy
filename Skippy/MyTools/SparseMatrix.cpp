#include "SparseMatrix.h"

template<typename T>
SparseMatrix<T>::SparseMatrix(int n)
{
	this->construct(n, n);
}

template<typename T>
SparseMatrix<T>::SparseMatrix(int rows, int columns)
{
	this->construct(rows, columns);
}

template<typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix & m)
{
	this->copyData(m);
	
}

template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator = (const SparseMatrix<T> & m)
{
	if (&m != this)
	{
		this->destruct();
		this->copyData(m);
	}
	return *this;
}

template<typename T>
SparseMatrix<T>::~SparseMatrix()
{
	this->destruct();
}

// ================== priavte values get ===============
template<typename T>
const int SparseMatrix<T>::getNumRow() const
{
	return m_numRows;
}


template<typename T>
const int SparseMatrix<T>::getNumCol() const
{
	return m_numCols;
}


template<typename T>
const int SparseMatrix<T>::getNonzeroCount() const
{
	return m_numNonzero;
}


template<typename T>
const T SparseMatrix<T>::get(int row, int col) const
{
	this->validateCoordinates(row, col);

	int currCol = -1;
	
	for (int pos = m_rowPtr.at(row); pos < m_rowPtr.at(row + 1); pos++)
	{
		currCol = m_colIndex.at(pos);

		if (currCol == col)
		{
			return  m_value.at(pos);
		}
		else if (currCol > col)
		{
			break;
		}
	}
	return 0;
}

template<typename T>
const SparseMatrix<T>& SparseMatrix<T>::set(T value, int row, int col)
{
	this->validateCoordinates(row, col);

	int currCol = -1;
	int pos = m_rowPtr.at(row);

	for (; pos < m_rowPtr.at(row + 1); pos++)
	{
		currCol = m_colIndex.at(pos);
		if (currCol >= col)
		{
			break;
		}
	}

	if (currCol == col)
	{	
		m_value[pos] = value;    
	}
	else
	{
		m_value.insert(m_value.begin() + pos, value);
		m_colIndex.insert(m_colIndex.begin() + pos, col);

		// update row ptr
		for (int i = row + 1; i < m_numRows + 1; i++)
		{
			m_rowPtr[i] += 1;
		}
	}
	return *this;
}


template<typename T>
const SparseMatrix<T>& SparseMatrix<T>::setIfNotExist(T value, int row, int col)
{
	this->validateCoordinates(row, col);

	int currCol = -1;
	int pos = m_rowPtr.at(row);

	for (; pos < m_rowPtr.at(row + 1); pos++)
	{
		currCol = m_colIndex.at(pos);
		if (currCol >= col)
		{
			break;
		}
	}

	if (currCol != col)
	{
		m_value.insert(m_value.begin() + pos, value);
		m_colIndex.insert(m_colIndex.begin() + pos, col);

		// update row ptr
		for (int i = row + 1; i < m_numRows + 1; i++)
		{
			m_rowPtr[i] += 1;
		}
	}
	return *this;
}


template<typename T>
const bool SparseMatrix<T>::isExist(int row, int col) const
{
	this->validateCoordinates(row, col);

	int currCol = -1;
	
	for (int pos = m_rowPtr.at(row); pos < m_rowPtr.at(row + 1); pos++)
	{
		currCol = m_colIndex.at(pos);

		if (currCol == col)
		{
			return  true;
		}
		else if (currCol > col)
		{
			break;
		}
	}
	return false;
}


template<typename T>
const QVector<T> SparseMatrix<T>::getOneRowValue(int row) const
{
	this->validateCoordinates(row, m_numCols - 1);

	QVector<T> rowValue;
	T value;
	
	for (int pos = m_rowPtr.at(row); pos < m_rowPtr.at(row + 1); pos++)
	{
		value = m_value.at(pos);
		rowValue.append(value);
	}
	return rowValue;
}


template<typename T>
const uint SparseMatrix<T>::getOneRowValueCount(int row) const
{
	// return this->getOneRowValue(row).count();
	return m_rowPtr.at(row + 1) - m_rowPtr.at(row);
}


template<typename T>
const QVector<T> SparseMatrix<T>::getOneRowColIndex(int row) const
{
	this->validateCoordinates(row, m_numCols - 1);
	QVector<T> rowColIndex;
	T colIndex;
	
	for (int pos = m_rowPtr.at(row); pos < m_rowPtr.at(row + 1); pos++)
	{
		colIndex = m_colIndex.at(pos);
		rowColIndex.append(colIndex);
	}
	return rowColIndex;
}


// ===================  assistant function ==============

template<typename T>
void SparseMatrix<T>::construct(int rows, int columns)
{
	if (rows < 0 || columns < 0)
		throw InvalidDimensionsException("Matrix dimensions cannot be zero or negative.");

	m_numRows = rows;
	m_numCols = columns;
	m_numNonzero = 0;

	for (int i = 0; i < m_numRows + 1; i++)
	{
		m_rowPtr.append(0);
	}
}

template<typename T>
void SparseMatrix<T>::copyData(const SparseMatrix &m)
{
	m_numRows = m.m_numRows;
	m_numCols = m.m_numCols;
	m_numNonzero = m.m_numNonzero;

	m_value = m.m_value;
	m_colIndex = m.m_colIndex;
	m_rowPtr = m.m_rowPtr;
}

template<typename T>
void SparseMatrix<T>::destruct()
{
	
}

template<typename T>
void SparseMatrix<T>::validateCoordinates(int row, int col) const
{
	if (row < 0 || col < 0 || row >= m_numRows || col >= m_numCols)
	{
		throw InvalidCoordinatesException("Coordinates out of range.");
	}
}

template<typename T>
void SparseMatrix<T>::printMatrix() const
{
	QString out;
	int pos;

	for (uint i = 0; i < m_rowPtr.count() - 1; i++)
	{
		pos = m_rowPtr.at(i);
		out = out + "row " + QString::number(i);

		for (uint j = pos; j < m_rowPtr.at(i + 1); j++)
		{
			out = out + " (" + QString::number(m_colIndex.at(j)) + " ," + QString::number(m_value.at(j)) + ") ";
		}
		out.append("\n");
	}
	qInfo() << out.toStdString().c_str();
}



// ==================== explicit instantiations  ===========================
template class SparseMatrix<int>;