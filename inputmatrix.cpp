#include "inputmatrix.h"
#include "ui_inputmatrix.h"

InputMatrix::InputMatrix(QWidget *parent ,char s) :
    QDialog(parent),
    ui(new Ui::InputMatrix)
{
    ui->setupUi(this);
    matrixName = ui->MatrixName;
    ok_btn = ui->ok_btn;
    cancel_btn = ui->cancel_Btn;
    m1 = ui->m1;
    m2 = ui->m2;
    m3 = ui->m3;
    m4 = ui->m4;
    m5 = ui->m5;
    m6 = ui->m6;
    m7 = ui->m7;
    m8 = ui->m8;
    m9 = ui->m9;
    switch(s){
    case 't':
        this->matrixtype = 't';
        m1->setText("1");
        m5->setText("1");
        m9->setText("1");
        m2->setText("0");
        m3->setText("0");
        m4->setText("0");
        m6->setText("0");
        matrixName->setText("transform matrix");
        m1->setEnabled(false);
        m5->setEnabled(false);
        m9->setEnabled(false);
        m2->setEnabled(false);
        m3->setEnabled(false);
        m4->setEnabled(false);
        m6->setEnabled(false);
        break;
    case 's':
        this->matrixtype = 's';
        m2->setText("0");
        m3->setText("0");
        m4->setText("0");
        m6->setText("0");
        m7->setText("0");
        m8->setText("0");
        m9->setText("1");
        matrixName->setText("scale matrix");
        m2->setEnabled(false);
        m3->setEnabled(false);
        m4->setEnabled(false);
        m6->setEnabled(false);
        m7->setEnabled(false);
        m8->setEnabled(false);
        m9->setEnabled(false);
        break;
    case 'r':
        this->matrixtype = 'r';
        m3->setText("0");
        m6->setText("0");
        m7->setText("0");
        m8->setText("0");
        m9->setText("1");
        matrixName->setText("rotate matrix");
        m3->setEnabled(false);
        m6->setEnabled(false);
        m7->setEnabled(false);
        m8->setEnabled(false);
        m9->setEnabled(false);
        break;
    case 'c':
        this->matrixtype = 'c';
        m1->setText("1");
        m5->setText("1");
        m9->setText("1");
        m7->setText("0");
        m3->setText("0");
        m8->setText("0");
        m6->setText("0");
        matrixName->setText("shear matrix");
        m1->setEnabled(false);
        m5->setEnabled(false);
        m9->setEnabled(false);
        m7->setEnabled(false);
        m3->setEnabled(false);
        m8->setEnabled(false);
        m6->setEnabled(false);
        break;
    default:
        break;
    }
}

InputMatrix::~InputMatrix()
{
    delete ui;
}

QMatrix InputMatrix::getMatrix()
{
        QMatrix ret = QMatrix(m1->text().toDouble(),m2->text().toDouble(),
                                  m4->text().toDouble(),m5->text().toDouble(),
                                  m7->text().toDouble(),m8->text().toDouble());
        return ret;

}

void InputMatrix::on_ok_btn_clicked()
{
    matrix2main = getMatrix();
    emit sendmatrix(matrix2main);
    this->close();
}

void InputMatrix::on_cancel_Btn_clicked()
{
    this->close();
}
