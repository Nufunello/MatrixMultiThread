#include "generatematrixwindow.h"

#include <QResizeEvent>

constexpr int C_BUTTONS = 3;

GenerateMatrixWindow::GenerateMatrixWindow(QWidget *parent)
    : QMainWindow(parent)
    , _validator{QRegExp("\\d+")}
{
    _cRows.setParent(this);
    _cColumns.setParent(this);
    _btnGenerate.setParent(this);

    _cRows.setText("Rows count");
    _cRows.setValidator(&_validator);
    _cRows.connect(&_cRows, &QLineEdit::selectionChanged, &_cRows, &QLineEdit::clear);

    _cColumns.setText("Columns count");
    _cColumns.setValidator(&_validator);
    _cColumns.connect(&_cColumns, &QLineEdit::selectionChanged, &_cColumns, &QLineEdit::clear);

    _btnGenerate.setText("Generate");
    _btnGenerate.connect(&_btnGenerate, &QPushButton::clicked, [this]{
        emit this->Generated(_cRows.text().toInt(), _cColumns.text().toInt(), -50, -50);
        this->close();
    });
}

void GenerateMatrixWindow::resizeEvent(QResizeEvent *event)
{
    const auto& size = event->size();

    const int width = size.width();
    const int avgHeight = size.height() / C_BUTTONS;

    _cRows.resize(width, avgHeight);
    _cColumns.resize(width, avgHeight);
    _btnGenerate.resize(width, avgHeight);
}

void GenerateMatrixWindow::paintEvent(QPaintEvent *)
{
    _cRows.move(0, 0);
    _cColumns.move(_cRows.x(), _cRows.y() + _cRows.height());
    _btnGenerate.move(_cColumns.x(), _cColumns.y() + _cColumns.height());
}
