#include "mainwindow.h"
#include "matrix.h"

#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QTableWidgetItem>

constexpr int C_INPUT_MATRIX = 2;
constexpr int BUTTON_HEIGHT = 30;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _firstMatrix(this)
    , _secondMatrix(this)
    , _btnMultiply(this)
    , _tblResult(this)
    , _lblCThread(this)
    , _txtCThread(this)
    , _lblMultiplyTime(this)
    , _lblMultiplyInfo(this)
{
    _lblMultiplyInfo.setText("Elapsed time(sec):");

    _lblCThread.setText("Thread count");
    _txtCThread.setText("1");

    _btnMultiply.setText("Multiply");
    _btnMultiply.connect(&_btnMultiply, &QPushButton::clicked, [&]{
        try {
            const int cThreads = _txtCThread.text().toInt();

            auto startMultiplyTime = std::chrono::high_resolution_clock::now();
            _firstMatrix.MultiplyAndWriteToOutput(&_secondMatrix, &_tblResult, cThreads);
            auto endMultiplyTime = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endMultiplyTime - startMultiplyTime).count();
            _lblMultiplyTime.setText(QString::number(duration));

        } catch (const std::exception& ex) {
            QMessageBox msgBox;
            msgBox.setText(ex.what());
            msgBox.exec();
        }
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    const QSize& size = event->size();

    const int widthPerInputMatrix  = size.width() / C_INPUT_MATRIX;
    const int heightPerInputMatrix = size.height() / 2 - BUTTON_HEIGHT / 2;

    _firstMatrix.resize(widthPerInputMatrix, heightPerInputMatrix);
    _secondMatrix.resize(widthPerInputMatrix, heightPerInputMatrix);

    _btnMultiply.resize(100, 30);
    _tblResult.resize(widthPerInputMatrix, heightPerInputMatrix);

    const int threadsInfoWidth = _btnMultiply.width();

    _lblCThread.resize(threadsInfoWidth, BUTTON_HEIGHT);
    _txtCThread.resize(threadsInfoWidth, BUTTON_HEIGHT);

    _lblMultiplyTime.resize(threadsInfoWidth, BUTTON_HEIGHT);
    _lblMultiplyInfo.resize(threadsInfoWidth, BUTTON_HEIGHT);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    _firstMatrix.move(0, 0);
    _secondMatrix.move(_firstMatrix.x() + _firstMatrix.width(), _firstMatrix.y());

    _btnMultiply.move(this->width() / 2 - _btnMultiply.width() / 2, this->height() / 2 - BUTTON_HEIGHT / 2);
    _tblResult.move(this->width() / 2 - _tblResult.width() / 2, _btnMultiply.y() + BUTTON_HEIGHT);

    _lblMultiplyInfo.move(_lblMultiplyTime.width() / 2, _tblResult.y() + _tblResult.height() / 3);
    _lblMultiplyTime.move(_lblMultiplyInfo.x(), _lblMultiplyInfo.y() + _lblMultiplyInfo.height());

    _lblCThread.move((_tblResult.x() + _tblResult.width()) + _lblCThread.width() / 2, _tblResult.y() + _tblResult.height() / 3);
    _txtCThread.move(_lblCThread.x(), _lblCThread.y() + _lblCThread.height());
}

