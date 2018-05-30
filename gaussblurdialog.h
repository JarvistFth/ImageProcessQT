#ifndef GAUSSBLURDIALOG_H
#define GAUSSBLURDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>

namespace Ui {
class gaussBlurDialog;
}

class gaussBlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit gaussBlurDialog(QWidget *parent = 0);
    int getSize();
    double getSigma();
    ~gaussBlurDialog();

signals:
    void ensure_blur();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::gaussBlurDialog *ui;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QSpinBox *sp1;
    QDoubleSpinBox *sp2;
};

#endif // GAUSSBLURDIALOG_H
