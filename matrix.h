#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

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

    static  Matrix                                 multiplyMatrixes(const Matrix& rhs, const Matrix& lhs, const size_t& cRows, const size_t& cColumns);

    Matrix(size_t cRows);

private:
    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> _values;

};

#endif // MATRIX_H
