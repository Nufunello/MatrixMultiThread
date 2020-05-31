#include "matrix.h"

#include <QThread>
#include <QDebug>

Matrix::Matrix(std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> values) :
    _values(std::move(values))
{
}

size_t Matrix::getRowCount() const
{
    return _values.size();
}

DEFAULT_MATRIX_VALUE_TYPE Matrix::get(size_t i, size_t j) const
{
    return _values[i][j];
}

DEFAULT_MATRIX_VALUE_TYPE multiplyRowOnColumn(const Matrix& lhs, const Matrix& rhs, const size_t iLhs, size_t jRhs)
{
    DEFAULT_MATRIX_VALUE_TYPE sum = 0;
    const size_t m = rhs.getRowCount();

    for (size_t r = 0; r < m; ++r)
        sum += lhs.get(iLhs,r) * rhs.get(r,jRhs);

    return sum;
}

std::vector<DEFAULT_MATRIX_VALUE_TYPE> Matrix::multiplyRow(const Matrix& biggerByRows, const Matrix& another, size_t iRow, size_t cColumns)
{
    std::vector<DEFAULT_MATRIX_VALUE_TYPE> row;
    row.reserve(cColumns);

    for (size_t iColumn = 0; iColumn < cColumns; ++iColumn)
    {
        row.emplace_back(multiplyRowOnColumn(biggerByRows, another, iRow, iColumn));
    }

    return row;
}

Matrix Matrix::multiplyMatrixes(const Matrix& lhs, const Matrix& rhs, const size_t& cRows, const size_t& cColumns, const size_t cThreads)
{
    Matrix resultMatrix(cRows, cColumns);
    auto& resultTwoDimVector = resultMatrix._values;

    const Matrix& maxRowMatrix  = (lhs.getRowCount() > rhs.getRowCount()) ? lhs : rhs;
    const Matrix& anotherMatrix = (&maxRowMatrix == &lhs) ? rhs : lhs;

    auto rowFunc = [&](const size_t iRow){
        resultTwoDimVector[iRow] = Matrix::multiplyRow(maxRowMatrix, anotherMatrix, iRow, cColumns);
    };

    std::vector<std::thread> threads;
    threads.reserve(cThreads);

    const size_t rowsPerThread = cRows / cThreads;

    for (size_t iThread = 0; iThread < cThreads; ++iThread)
    {
        std::thread thread([&rowFunc, iThread, rowsPerThread]() {
            const size_t lastRowForThread = (iThread + 1) * rowsPerThread;
            for (size_t iRow = iThread * rowsPerThread; iRow < lastRowForThread; ++iRow)
            {
                rowFunc(iRow);
            }
        });

        threads.emplace_back(std::move(thread));
    }


    size_t rowsCalculated = cThreads * rowsPerThread;
    if (rowsCalculated < cRows)
    {
        const size_t rowsLeft = cRows - rowsCalculated;
        for (size_t iThread = 0; iThread < rowsLeft; ++iThread)
        {
            auto& thread = threads[iThread];
            if (thread.joinable())
            {
                thread.join();
            }

            thread = std::thread(rowFunc, rowsCalculated++);
        }
    }

    for (size_t iThread = 0; iThread < cThreads; ++iThread)
    {
        auto& thread = threads[iThread];
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return resultMatrix;
}

Matrix Matrix::multiply(const Matrix &rhs, const size_t cThreads) const
{
    const Matrix& lhs = *this;

    const auto& cRows    = this->getRowCount(),
                cColumns = this->_values.front().size();

    if (cColumns != rhs.getRowCount())
    {
        throw std::exception("First matrix column count should be equal second matrix row count");
    }

    return  multiplyMatrixes(lhs, rhs, cRows, cColumns, cThreads);
}

DEFAULT_MATRIX_VALUE_TYPE &Matrix::get(size_t i, size_t j)
{
    return _values[i][j];
}

void Matrix::clear()
{
    _values.clear();
}

const std::vector<DEFAULT_MATRIX_VALUE_TYPE> &Matrix::getRow(size_t row) const
{
    return _values[row];
}

Matrix::Matrix(size_t cRows, size_t cColumns)
{
    _values.reserve(cRows);

    for (size_t i = 0; i < cRows; ++i)
    {
        std::vector<DEFAULT_MATRIX_VALUE_TYPE> temp;
        temp.reserve(cColumns);
        _values.emplace_back(std::move(temp));
    }
}
