#include "gaussblurdialog.h"
#include "ui_gaussblurdialog.h"

gaussBlurDialog::gaussBlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gaussBlurDialog)
{
    ui->setupUi(this);
    ok_btn = new QPushButton;
    cancel_btn = new QPushButton;
    sp1 = new QSpinBox;//size
    sp2 = new QDoubleSpinBox;//sigma
    ok_btn = ui->pushButton;
    cancel_btn = ui->pushButton_2;
    sp1 = ui->spinBox;
    sp2 = ui->doubleSpinBox;
}

int gaussBlurDialog::getSize()
{
    return sp1->value();
}

double gaussBlurDialog::getSigma()
{
    return sp2->value();
}

gaussBlurDialog::~gaussBlurDialog()
{
    delete ui;
}

void gaussBlurDialog::on_pushButton_2_clicked()
{
    this->close();
}

void gaussBlurDialog::on_pushButton_clicked()
{
    if(sp1->value() % 2 != 0){
        emit ensure_blur();
        this->close();
    }
    else{
        QMessageBox::information(this,
            tr("Error"),
            tr("please input odd number for kernalsize!"));
        return;
    }

}
