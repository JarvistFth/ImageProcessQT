#include "setdialog.h"
#include "ui_setdialog.h"

SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    spbox = new QDoubleSpinBox;
    sli = new QSlider;
    sli = ui->horizontalSlider;
    spbox = ui->doubleSpinBox;
//    spbox->setDecimals(2);
    spbox->setSingleStep(0.01);
//    spbox->setMaximum(100.00);
//    spbox->setMinimum(0.00);
    spbox->setValue(0.00);
    sli->setMaximum(10000);
    sli->setMinimum(0);
    sli->setValue(0);

    connect(spbox,SIGNAL(valueChanged(double)),this,SLOT(doublespinbox2slider()));
    connect(sli,SIGNAL(valueChanged(int)),this,SLOT(slider2doublespinbox()));

}

SetDialog::~SetDialog()
{
    delete ui;
}


void SetDialog::on_buttonBox_accepted()
{
    bool ok;
    double c = spbox->value();
    emit trans_c(c);
    this->close();
}

void SetDialog::on_buttonBox_rejected()
{
    this->close();
}

void SetDialog::slider2doublespinbox(){
    spbox->setValue((double)(sli->value()/100.0));
}

void SetDialog::doublespinbox2slider(){
    sli->setValue((int)(spbox->value()*100));
}
