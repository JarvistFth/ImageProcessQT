#ifndef MATDIALOG_H
#define MATDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
namespace Ui {
class MatDialog;
}

class MatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatDialog(QWidget *parent = 0);
    ~MatDialog();

private slots:

    void on_ok_Btn_clicked();

    void on_cancel_Btn_clicked();


signals:
    void trans_kernel(QString kernel);

private:
    Ui::MatDialog *ui;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QTextEdit *textEdit;
    QString mat_string;
};

#endif // MATDIALOG_H
