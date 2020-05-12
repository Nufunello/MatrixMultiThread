#ifndef MATRIXDISPLAYER_H
#define MATRIXDISPLAYER_H

#include "generatematrixwindow.h"
#include "matrix.h"

#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

class MatrixWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MatrixWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public slots:
    void FillMatrixRandomly(size_t cRows, size_t cColumns, int minValue, int maxValue);
    Matrix Multiply(MatrixWidget* rhs);

private:
    void fillMatrixFromFile(const QString& fileName);
    void clearMatrix();

    std::vector<DEFAULT_MATRIX_VALUE_TYPE> addLineToMatrix(int itLine, QStringList cellsValues);
    void                                   addLineToMatrix(int itLine, const std::vector<DEFAULT_MATRIX_VALUE_TYPE>& cellsValues);


signals:

private:
    QTableWidget _tblMatrix;
    QPushButton  _btnReadFromFile;
    QPushButton  _btnGenerateMatrix;

    GenerateMatrixWindow _generateWindow;

    Matrix _matrix;

};

#endif // MATRIXDISPLAYER_H
