#include "matdialog.h"
#include "ui_matdialog.h"

MatDialog::MatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatDialog)
{
    textEdit = new QTextEdit;
    ok_btn = new QPushButton;
    cancel_btn = new QPushButton;
    ui->setupUi(this);
    textEdit = ui->textEdit;
    ok_btn = ui->ok_Btn;
    cancel_btn = ui->cancel_Btn;
}

MatDialog::~MatDialog()
{
    delete ui;
    delete textEdit;
    delete ok_btn;
    delete cancel_btn;
}


void MatDialog::on_ok_Btn_clicked()
{
    mat_string = textEdit->toPlainText();
    emit trans_kernel(mat_string);
    this->close();
}

void MatDialog::on_cancel_Btn_clicked()
{
    this->close();
}
