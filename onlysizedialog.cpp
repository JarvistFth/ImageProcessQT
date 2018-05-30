#include "onlysizedialog.h"
#include "ui_onlysizedialog.h"

onlySizeDialog::onlySizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::onlySizeDialog)
{
    ok_btn = new QPushButton;
    cancel_btn = new QPushButton;
    sp1 = new QSpinBox;

    ui->setupUi(this);

    ok_btn = ui->pushButton;
    cancel_btn = ui->pushButton_2;
    sp1 = ui->spinBox;
}

int onlySizeDialog::getSize()
{
    return sp1->value();
}

onlySizeDialog::~onlySizeDialog()
{
    delete ok_btn;
    delete cancel_btn;
    delete sp1;
    delete ui;

}

void onlySizeDialog::on_pushButton_clicked()
{
    if(sp1->value() % 2 != 0){
        emit ensure_size();
        this->close();
    }
    else{
        QMessageBox::information(this,
            tr("Error"),
            tr("please input odd number for kernalsize!"));
        return;
    }
}

void onlySizeDialog::on_pushButton_2_clicked()
{
    this->close();
}
