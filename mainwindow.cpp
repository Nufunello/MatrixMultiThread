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
{
    _btnMultiply.setText("Multiply");
    _btnMultiply.connect(&_btnMultiply, &QPushButton::clicked, [&]{
        try {
            Matrix multiplyResult = _firstMatrix.Multiply(&_secondMatrix);

            const size_t cRows = multiplyResult.getRowCount();
            _tblResult.setRowCount(cRows);

            for (size_t itRow = 0; itRow < cRows; ++itRow)
            {
                const auto& row = multiplyResult.getRow(itRow);

                const size_t cColumns = row.size();
                _tblResult.setColumnCount(cColumns);

                for (size_t itColumn = 0; itColumn < cColumns; ++itColumn)
                {
                    auto widgetItem = new QTableWidgetItem();
                    widgetItem->setData(Qt::DisplayRole, row[itColumn]);
                    _tblResult.setItem(itRow, itColumn, widgetItem);
                }
            }
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
}

void MainWindow::paintEvent(QPaintEvent *)
{
    _firstMatrix.move(0, 0);
    _secondMatrix.move(_firstMatrix.x() + _firstMatrix.width(), _firstMatrix.y());

    _btnMultiply.move(this->width() / 2 - _btnMultiply.width() / 2, this->height() / 2 - BUTTON_HEIGHT / 2);
    _tblResult.move(this->width() / 2 - _tblResult.width() / 2, _btnMultiply.y() + BUTTON_HEIGHT);
}

