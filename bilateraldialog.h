#ifndef BILATERALDIALOG_H
#define BILATERALDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QMessageBox>

namespace Ui {
class BilateralDialog;
}

class BilateralDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BilateralDialog(QWidget *parent = 0);
    double get_sigma_d();
    double get_sigma_r();
    int get_size();
    ~BilateralDialog();

signals:
    void ensure_bilateral();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::BilateralDialog *ui;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QDoubleSpinBox *sigma_d;
    QDoubleSpinBox *sigma_r;
    QSpinBox *kernal_size;
};

#endif // BILATERALDIALOG_H
