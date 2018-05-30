#ifndef ONLYSIZEDIALOG_H
#define ONLYSIZEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QSpinBox>
#include <QMessageBox>
namespace Ui {
class onlySizeDialog;
}

class onlySizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit onlySizeDialog(QWidget *parent = 0);
    int getSize();
    ~onlySizeDialog();

signals:
    void ensure_size();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::onlySizeDialog *ui;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QSpinBox *sp1;
};

#endif // ONLYSIZEDIALOG_H
