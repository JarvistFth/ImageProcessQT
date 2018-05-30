#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QDoubleSpinBox>
#include <QSlider>

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = 0);
    ~SetDialog();

private slots:
    void doublespinbox2slider();
    void slider2doublespinbox();
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void trans_c(double c);


private:
    Ui::SetDialog *ui;
    QSlider *sli;
    QDoubleSpinBox *spbox;

};

#endif // SETDIALOG_H
