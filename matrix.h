#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <thread>

typedef float DEFAULT_MATRIX_VALUE_TYPE;

class Matrix
{
public:
    Matrix() = default;
    Matrix(std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> values);

private:

public:
    size_t                     getRowCount()                const;

    DEFAULT_MATRIX_VALUE_TYPE  get(size_t i, size_t j)      const;

    Matrix                     operator*(const Matrix& rhs) const;

private:
    static  std::vector<DEFAULT_MATRIX_VALUE_TYPE> multiplyRow(const Matrix& rhs, const Matrix& lhs, size_t cColumns, size_t iRow);

    static  std::vector<std::thread>               multiplyMatrixes(Matrix& resultMatrix, const Matrix& lhs, const Matrix& rhs, const size_t& cRows, const size_t& cColumns);

            Matrix(size_t cRows, size_t cColumns);

private:
    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> _values;

};

#endif // MATRIX_H
