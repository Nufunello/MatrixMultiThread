#include "generatematrixwindow.h"

#include <QResizeEvent>

constexpr int C_BUTTONS = 3;

GenerateMatrixWindow::GenerateMatrixWindow(QWidget *parent)
    : QMainWindow(parent)
    , _validator{QRegExp("\\d+")}
{
    _txtCRows.setParent(this);
    _txtCColumns.setParent(this);
    _btnGenerate.setParent(this);

    _txtCRows.setText("Rows count");
    _txtCRows.setValidator(&_validator);
    _txtCRows.connect(&_txtCRows, &QLineEdit::selectionChanged, &_txtCRows, &QLineEdit::clear);

    _txtCColumns.setText("Columns count");
    _txtCColumns.setValidator(&_validator);
    _txtCColumns.connect(&_txtCColumns, &QLineEdit::selectionChanged, &_txtCColumns, &QLineEdit::clear);

    _btnGenerate.setText("Generate");
    _btnGenerate.connect(&_btnGenerate, &QPushButton::clicked, [this]{
        emit this->Generated(_txtCRows.text().toInt(), _txtCColumns.text().toInt(), -50, -50);
        this->close();
    });
}

void GenerateMatrixWindow::resizeEvent(QResizeEvent *event)
{
    const auto& size = event->size();

    const int width = size.width();
    const int avgHeight = size.height() / C_BUTTONS;

    _txtCRows.resize(width, avgHeight);
    _txtCColumns.resize(width, avgHeight);
    _btnGenerate.resize(width, avgHeight);
}

void GenerateMatrixWindow::paintEvent(QPaintEvent *)
{
    _txtCRows.move(0, 0);
    _txtCColumns.move(_txtCRows.x(), _txtCRows.y() + _txtCRows.height());
    _btnGenerate.move(_txtCColumns.x(), _txtCColumns.y() + _txtCColumns.height());
}
