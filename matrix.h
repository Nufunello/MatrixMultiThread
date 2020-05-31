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
    size_t                                        getRowCount()                const;

    DEFAULT_MATRIX_VALUE_TYPE                     get(size_t i, size_t j)      const;

    Matrix                                        multiply(const Matrix& rhs, const size_t threadCount) const;

    const std::vector<DEFAULT_MATRIX_VALUE_TYPE>& getRow(size_t row)           const;

public:
    DEFAULT_MATRIX_VALUE_TYPE& get(size_t i, size_t j);

    void clear();

public:
    static  std::vector<DEFAULT_MATRIX_VALUE_TYPE> multiplyRow(const Matrix& rhs, const Matrix& lhs, size_t cColumns, size_t iRow);

    static  Matrix                                 multiplyMatrixes(const Matrix& lhs, const Matrix& rhs, const size_t& cRows, const size_t& cColumns, const size_t threadCount);

            Matrix(size_t cRows, size_t cColumns);

private:
    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> _values;

};

#endif // MATRIX_H
