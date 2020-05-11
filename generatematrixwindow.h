#ifndef GENERATEMATRIXWINDOW_H
#define GENERATEMATRIXWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QRegExpValidator>

class GenerateMatrixWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GenerateMatrixWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*) override;

signals:
    void Generated(size_t cRows, size_t cColumns, int, int);

private:
    QLineEdit _cRows;
    QLineEdit _cColumns;

    QPushButton _btnGenerate;

    QRegExpValidator _validator;
};

#endif // GENERATEMATRIXWINDOW_H
