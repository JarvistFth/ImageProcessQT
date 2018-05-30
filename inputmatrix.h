#ifndef INPUTMATRIX_H
#define INPUTMATRIX_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMatrix>

namespace Ui {
class InputMatrix;
}

class InputMatrix : public QDialog
{
    Q_OBJECT

public:
    explicit InputMatrix(QWidget *parent = 0 , char s = 't');
    ~InputMatrix();
    QMatrix getMatrix();

signals:
    void sendmatrix(QMatrix matrx);

private slots:
    void on_ok_btn_clicked();
    void on_cancel_Btn_clicked();

private:
    Ui::InputMatrix *ui;
    char matrixtype;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QLineEdit *m1;
    QLineEdit *m2;
    QLineEdit *m3;
    QLineEdit *m4;
    QLineEdit *m5;
    QLineEdit *m6;
    QLineEdit *m7;
    QLineEdit *m8;
    QLineEdit *m9;
    QLabel *matrixName;
    QMatrix matrix2main;




};

#endif // INPUTMATRIX_H
