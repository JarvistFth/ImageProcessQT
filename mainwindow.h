#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <mypixmapitem.h>
#include <QDir>
#include <QFileInfoList>
#include <imageprocessor.h>
#include <imagefilter.h>
#include <setdialog.h>
#include <matdialog.h>
#include <QVector>
#include <gaussblurdialog.h>
#include <bilateraldialog.h>
#include <onlysizedialog.h>
#include <fft.h>
#include <matrix.h>
#include <inputmatrix.h>
#include <QMatrix>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showImage(QImage img);
    bool readOneRow(QString row,QVector<double>& kernal,int& nCol);
    QVector<double> parseMatrix(QString string_mat,int& nCol);


private:
    Ui::MainWindow *ui;
    QFileDialog *fileDialog;
    QGraphicsScene *scene;
//    QGraphicsPixmapItem *pixmapItem;
    MyPixmapItem *pixmapItem;
    QDir dir;
    QFileInfoList list;
    QFileInfo info;
    SetDialog *dia;
    MatDialog *mat;
    gaussBlurDialog *gauss_dia;
    gaussBlurDialog *gauss_dia_forGrey;
    BilateralDialog *bilateral_dia;
    onlySizeDialog *size_dia;
    onlySizeDialog *size_dia_forGrey;
    onlySizeDialog *size_dia_expanding;
    onlySizeDialog *size_dia_corroding;
    InputMatrix *transformDia;
    InputMatrix *scaleDia;
    InputMatrix *rotateDia;
    InputMatrix *shearDia;
    QMatrix matrx;
    int times;
public slots:
    void on_OfBt_clicked();
    void on_RestBt_clicked();
    void on_LBt_clicked();
    void on_RBt_clicked();
    bool getDisplayedImage(QImage &) const;
    void on_actionhuiduzhuanhuan_triggered();
    void rec_SetDialog_trans_c(double);

    void Conv(QString string_mat);
    void GaussBlur();
    void GaussBlurForGrey();
    void medianBlur();
    void medianBlurforGrey();
    void BilateralBlur();
    void expanding();
    void corroding();

    void tsrc(QMatrix matrx);

private slots:
    void on_actionyansefanzhuan_triggered();
    void on_actionduishubianhuan_triggered();

    void on_actionhistagram_triggered();
    void on_actionhuidutujunheng_triggered();
    void on_actionHSVhist_triggered();
    void on_actionjuanji_triggered();

    void on_actiongaosimohu_triggered();
    void on_actionzhongzhilvbo_triggered();
    void on_actiongaosilvbohuidu_triggered();
    void on_actionshaungbianmohu_triggered();
    void on_actionsave_triggered();
    void on_actionzhongzhilvbohuidu_triggered();
    void on_actionpengzhang_triggered();
    void on_actionfushi_triggered();
    void on_actionpinyutu_triggered();
    void on_actionpinyutur_triggered();
    void on_actionshurujuzhen_triggered();
    void on_actionxuanzhuan_triggered();
    void on_actionsuofang_triggered();
    void on_actionxieqie_triggered();
};

#endif // MAINWINDOW_H
