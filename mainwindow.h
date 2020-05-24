#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "matrixwidget.h"

#include <QMainWindow>

#include <QTableWidget>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*) override;

private:
    MatrixWidget _firstMatrix;
    MatrixWidget _secondMatrix;

    QPushButton  _btnMultiply;
    QTableWidget _tblResult;

    QLabel    _lblCThread;
    QLineEdit _txtCThread;

    QLabel _lblMultiplyTime;
    QLabel _lblMultiplyInfo;


};
#endif // MAINWINDOW_H
