#include "mainwindow.h"
#include "matrix.h"

#include <QDebug>

std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> generateMatrix(size_t cRows, size_t cColumns)
{
    int first_value = -50, last_value = 50;

    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE>> matrix;
    matrix.reserve(cRows);

    for (size_t iRow = 0; iRow < cRows; ++iRow)
    {
        std::vector<DEFAULT_MATRIX_VALUE_TYPE> row;
        row.reserve(cColumns);

        for (size_t iColumn = 0; iColumn < cColumns; ++iColumn)
        {
            row.emplace_back(first_value + rand() % last_value);
        }

        matrix.emplace_back(std::move(row));
    }

    return matrix;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto matr = generateMatrix(100, 100);

    Matrix A(matr);
    Matrix B(matr);

    auto t1 = std::chrono::high_resolution_clock::now();
    Matrix C = A*B;
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    qDebug() << duration;
}

MainWindow::~MainWindow()
{
}

