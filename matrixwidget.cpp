#include "matrixwidget.h"

#include <QFileDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QResizeEvent>
#include <QTextStream>

constexpr int C_BUTTONS = 2;
constexpr int BUTTON_HEIGHT = 30;

constexpr char CELL_SEPARATOR = ',';

MatrixWidget::MatrixWidget(QWidget *parent)
    : QWidget(parent)
{
    _btnReadFromFile.setParent(this);
    _btnGenerateMatrix.setParent(this);
    _tblMatrix.setParent(this);

    _btnReadFromFile.setText("Read from file");
    _btnGenerateMatrix.setText("Generate random values...");

    _btnReadFromFile.connect(&_btnReadFromFile, &QPushButton::clicked, [this]{
        QString fileName = QFileDialog::getOpenFileName(
                    this,
                    "Select one file to open",
                    "~/Desktop",
                    "Text file (*.txt)");
        this->fillMatrixFromFile(std::move(fileName));
    });

    _btnGenerateMatrix.connect(&_btnGenerateMatrix, &QPushButton::clicked, [this]{
        _generateWindow.resize(100, 90);
        _generateWindow.show();
    });

    _generateWindow.connect(&_generateWindow, &GenerateMatrixWindow::Generated, this, &MatrixWidget::FillMatrixRandomly);
}

void MatrixWidget::resizeEvent(QResizeEvent *event)
{
    const auto& size = event->size();

    const int width = size.width();
    const int height = size.height();

    const int avgButtonWidth = width / C_BUTTONS;

    _btnReadFromFile.resize(avgButtonWidth, BUTTON_HEIGHT);
    _btnGenerateMatrix.resize(avgButtonWidth, BUTTON_HEIGHT);

    _tblMatrix.resize(width, height - BUTTON_HEIGHT);
}

void MatrixWidget::paintEvent(QPaintEvent *)
{
    _btnReadFromFile.move(0, 0);
    _btnGenerateMatrix.move(_btnReadFromFile.x() + _btnReadFromFile.width(), _btnReadFromFile.y());

    _tblMatrix.move(_btnReadFromFile.x(), _btnReadFromFile.y() + _btnReadFromFile.height());
}

void MatrixWidget::fillMatrixFromFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE> > matrix;

    QTextStream in(&file);
    int line = 0;
    while (!in.atEnd()) {
        QStringList cellsValues = in.readLine().split(CELL_SEPARATOR);
        matrix.emplace_back(this->addLineToMatrix(line++, cellsValues));
    }

    _matrix = Matrix(std::move(matrix));
}

std::vector<DEFAULT_MATRIX_VALUE_TYPE> MatrixWidget::addLineToMatrix(int itLine, QStringList cellsValues)
{
    int cColumns = cellsValues.size();

    const int firstLineIndex = 0;
    if (itLine == firstLineIndex)
    {
        _tblMatrix.setColumnCount(cColumns);
        for (int itColumns = 0; itColumns < cColumns; ++itColumns)
        {
            _tblMatrix.setColumnWidth(itColumns, 1);
        }
    }
    else
    {
        if (cColumns != _tblMatrix.columnCount())
        {
            throw std::runtime_error("Invalid column count on row" + std::to_string(itLine));
        }
    }

    std::vector<DEFAULT_MATRIX_VALUE_TYPE> row;
    row.reserve(cColumns);

    _tblMatrix.insertRow(itLine);
    for (int itCells = 0; itCells < cColumns; ++itCells)
    {
        const QString& value = cellsValues[itCells];

        QTableWidgetItem* pCellWidget = new QTableWidgetItem();
        pCellWidget->setData(Qt::DisplayRole, value);
        _tblMatrix.setItem(itLine, itCells, pCellWidget);

        row.emplace_back(value.toInt());
    }

    return row;
}

void MatrixWidget::FillMatrixRandomly(size_t cRows, size_t cColumns, int minValue, int maxValue)
{
    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE> > matrix;
    matrix.reserve(cRows);

    for (size_t iRow = 0; iRow < cRows; ++iRow)
    {
        std::vector<DEFAULT_MATRIX_VALUE_TYPE> row;
        row.reserve(cColumns);

        for (size_t iColumn = 0; iColumn < cColumns; ++iColumn)
        {
            row.emplace_back(minValue + rand() % maxValue);
        }

        this->addLineToMatrix(iRow, row);
        matrix.emplace_back(std::move(row));
    }

    _matrix = Matrix(std::move(matrix));
}

Matrix MatrixWidget::Multiply(MatrixWidget *rhs)
{
    return this->_matrix * rhs->_matrix;
}

void MatrixWidget::addLineToMatrix(int itLine, const std::vector<DEFAULT_MATRIX_VALUE_TYPE>& cellsValues)
{
    int cColumns = cellsValues.size();

    const int firstLineIndex = 0;
    if (itLine == firstLineIndex)
    {
        _tblMatrix.setColumnCount(cColumns);
        for (int itColumns = 0; itColumns < cColumns; ++itColumns)
        {
            _tblMatrix.setColumnWidth(itColumns, 1);
        }
    }

    _tblMatrix.insertRow(itLine);
    for (int itCells = 0; itCells < cColumns; ++itCells)
    {
        QTableWidgetItem* pCellWidget = new QTableWidgetItem();
        pCellWidget->setData(Qt::DisplayRole, cellsValues[itCells]);
        _tblMatrix.setItem(itLine, itCells, pCellWidget);
    }
}
