#include "matrixwidget.h"

#include <QFileDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QResizeEvent>
#include <QTextStream>

constexpr int C_BUTTONS = 2;
constexpr int BUTTON_HEIGHT = 30;

constexpr char CELL_SEPARATOR = ',';

std::vector<DEFAULT_MATRIX_VALUE_TYPE> addCells(QTableWidget* out, const int itLine, const int cColumns, const QStringList& inputContainer)
{
    std::vector<DEFAULT_MATRIX_VALUE_TYPE> row;
    row.reserve(cColumns);

    for (int itCells = 0; itCells < cColumns; ++itCells)
    {
        const QString& value = inputContainer[itCells];

        QTableWidgetItem* pCellWidget = new QTableWidgetItem();
        pCellWidget->setData(Qt::DisplayRole, value);
        out->setItem(itLine, itCells, pCellWidget);

        row.emplace_back(value.toInt());
    }

    return row;
}

std::vector<DEFAULT_MATRIX_VALUE_TYPE> addCells(QTableWidget* out, const int itLine, const int cColumns, std::vector<DEFAULT_MATRIX_VALUE_TYPE> inputContainer)
{
    for (int itCells = 0; itCells < cColumns; ++itCells)
    {
        QTableWidgetItem* pCellWidget = new QTableWidgetItem();
        pCellWidget->setData(Qt::DisplayRole, inputContainer[itCells]);
        out->setItem(itLine, itCells, pCellWidget);
    }

    return inputContainer;
}

template<typename InputContainer>
std::vector<DEFAULT_MATRIX_VALUE_TYPE> addLine(QTableWidget* out, const int itLine, const InputContainer& inputContainer)
{
    const int cColumns = inputContainer.size();
    const int firstLineIndex = 0;
    if (itLine == firstLineIndex)
    {
        out->setColumnCount(cColumns);
        for (int itColumns = 0; itColumns < cColumns; ++itColumns)
        {
            out->setColumnWidth(itColumns, 1);
        }
    }
    else
    {
        if (cColumns != out->columnCount())
        {
            throw std::runtime_error("Invalid column count on row" + std::to_string(itLine));
        }
    }

    out->insertRow(itLine);
    return addCells(out, itLine, cColumns, inputContainer);
}

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

    _tblMatrix.connect(&_tblMatrix, &QTableWidget::cellChanged, [this](int itRow, int itColumn){
        int cRows = _matrix.getRowCount();
        if (cRows > itRow)
        {
            _matrix.get(itRow,itColumn) = _tblMatrix.itemAt(itRow, itColumn)->data(Qt::DisplayRole).toFloat();
        }
    });
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
    clearMatrix();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    std::vector<std::vector<DEFAULT_MATRIX_VALUE_TYPE> > matrix;

    QTextStream in(&file);
    int line = 0;
    while (!in.atEnd())
    {
        QStringList cellsValues = in.readLine().split(CELL_SEPARATOR);
        try {
            matrix.emplace_back(addLine(&_tblMatrix, line++, cellsValues));
        } catch (const std::exception& exception) {
            QMessageBox msgBox;
            msgBox.setText(QString("Error while reading from file") + exception.what());
            msgBox.exec();

            _tblMatrix.setRowCount(0);
            _tblMatrix.setColumnCount(0);
            return;
        }
    }

    _matrix = Matrix(std::move(matrix));
}

void MatrixWidget::clearMatrix()
{
    _tblMatrix.setRowCount(0);
    _tblMatrix.setColumnCount(0);

    _matrix.clear();
}

void MatrixWidget::FillMatrixRandomly(size_t cRows, size_t cColumns, int minValue, int maxValue)
{
    clearMatrix();
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

        addLine(&_tblMatrix, iRow, row);
        matrix.emplace_back(std::move(row));
    }

    _matrix = Matrix(std::move(matrix));
}

void MatrixWidget::MultiplyAndWriteToOutput(MatrixWidget *rhs, QTableWidget *out, const size_t cThreads)
{
    out->clear();
    out->setRowCount(0);
    out->setColumnCount(0);

    Matrix multiplyResult = this->_matrix.multiply(rhs->_matrix, cThreads);
    for (size_t itRow = 0; itRow < multiplyResult.getRowCount(); ++itRow)
    {
        addLine(out, itRow, multiplyResult.getRow(itRow));
    }
}
