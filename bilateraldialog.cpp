#include "bilateraldialog.h"
#include "ui_bilateraldialog.h"

BilateralDialog::BilateralDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BilateralDialog)
{
    sigma_d = new QDoubleSpinBox;
    sigma_r = new QDoubleSpinBox;
    kernal_size = new QSpinBox;

    ui->setupUi(this);

    sigma_d = ui->sigmad_sp1;
    sigma_r = ui->sigmar_sp2;
    kernal_size = ui->spinBox;

}

double BilateralDialog::get_sigma_d()
{
    return sigma_d->value();
}

double BilateralDialog::get_sigma_r()
{
    return sigma_r->value();
}

int BilateralDialog::get_size()
{
     return kernal_size->value();
}

BilateralDialog::~BilateralDialog()
{
    delete sigma_d;
    delete sigma_r;
    delete kernal_size;
    delete ui;

}

void BilateralDialog::on_pushButton_clicked()
{
    if(kernal_size->value() % 2 != 0){
        emit ensure_bilateral();
        this->close();
    }
    else{
        QMessageBox::information(this,
            tr("Error"),
            tr("please input odd number for kernalsize!"));
        return;
    }
}

void BilateralDialog::on_pushButton_2_clicked()
{
    this->close();
}
