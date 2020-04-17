#include "matrix.h"

#include <thread>

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

Matrix Matrix::multiplyMatrixes(const Matrix& lhs, const Matrix& rhs, const size_t& cRows, const size_t& cColumns)
{
    Matrix resultMatrix(cRows);
    auto& resultTwoDimVector = resultMatrix._values;

    for (size_t iRow = 0; iRow < cRows; ++iRow)
    {
        std::thread thread([&]{
            std::vector<DEFAULT_MATRIX_VALUE_TYPE> multipliedRow = Matrix::multiplyRow(lhs, rhs, iRow, cColumns);
            resultTwoDimVector[iRow] = std::move(multipliedRow);
        });
        thread.join();
    }

    return resultMatrix;
}

std::vector<DEFAULT_MATRIX_VALUE_TYPE> Matrix::multiplyRow(const Matrix& lhs, const Matrix& rhs, size_t iRow, size_t cColumns)
{
    std::vector<DEFAULT_MATRIX_VALUE_TYPE> row(cColumns);

    for (size_t iColumn = 0; iColumn < cColumns; ++iColumn)
    {
        //std::thread ([&]{
            row[iColumn] = multiplyRowOnColumn(lhs, rhs, iRow, iColumn);
        //}).join();
    }

    return row;
}

Matrix Matrix::operator*(const Matrix &rhs) const
{
    const Matrix& lhs = *this;

    const auto& cRows    = this->_values.size(),
                cColumns = this->_values.front().size();

    if (cColumns != rhs.getRowCount())
    {
        throw std::exception("First matrix column count should be equal second matrix row count");
    }

    return multiplyMatrixes(lhs, rhs, cRows, cColumns);
}

Matrix::Matrix(size_t cRows)
    : _values(cRows)
{
}
