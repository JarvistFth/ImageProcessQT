#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    times(0),
    fileDialog(new QFileDialog(this)),
    dia(new SetDialog(parent)),
    gauss_dia(new gaussBlurDialog(parent)),
    gauss_dia_forGrey(new gaussBlurDialog(parent)),
    bilateral_dia(new BilateralDialog(parent)),
    size_dia(new onlySizeDialog(parent)),
    size_dia_forGrey(new onlySizeDialog(parent)),
    size_dia_expanding(new onlySizeDialog(parent)),
    size_dia_corroding(new onlySizeDialog(parent)),
    mat(new MatDialog(parent)),
    scene(NULL),
    pixmapItem(NULL)
{
    fileDialog->setNameFilter("Image(*.bmp *.jpg *.png)");
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    ui->setupUi(this);

    connect(dia,SIGNAL(trans_c(double)),this,SLOT(rec_SetDialog_trans_c(double)));
    connect(mat,SIGNAL(trans_kernel(QString)),this,SLOT(Conv(QString)));
    connect(gauss_dia,SIGNAL(ensure_blur()),this,SLOT(GaussBlur()));
    connect(gauss_dia_forGrey,SIGNAL(ensure_blur()),this,SLOT(GaussBlur()));
    connect(bilateral_dia,SIGNAL(ensure_bilateral()),this,SLOT(BilateralBlur()));
    connect(size_dia,SIGNAL(ensure_size()),this,SLOT(medianBlur()));
    connect(size_dia_forGrey,SIGNAL(ensure_size()),this,SLOT(medianBlurforGrey()));
    connect(size_dia_expanding,SIGNAL(ensure_size()),this,SLOT(expanding()));
    connect(size_dia_corroding,SIGNAL(ensure_size()),this,SLOT(corroding()));



}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileDialog;
    delete pixmapItem;
    delete scene;
    delete mat;
    delete dia;
    delete gauss_dia;
    delete gauss_dia_forGrey;
    delete bilateral_dia;
    delete size_dia;
    delete transformDia;
    delete scaleDia;
    delete rotateDia;
    delete shearDia;
}

void MainWindow::showImage(QImage img)
{
    pixmapItem->setPixmap(QPixmap::fromImage(img));
    scene = new QGraphicsScene(this);
    scene->addItem(pixmapItem);
    QRectF rect = pixmapItem->boundingRect();
    scene->setSceneRect(0,0,rect.width(),rect.height());
    ui->Iv->setScene(scene);
}


void MainWindow::on_OfBt_clicked()
{

    if(fileDialog->exec())
    {
        QStringList files = fileDialog->selectedFiles();
        dir = fileDialog->directory();
        QStringList filters;
        filters << "*.bmp" <<"*.jpg" <<"*.png";
        dir.setNameFilters(filters);
        list = dir.entryInfoList();
//        qDebug()<<list;
//        info = list->at(times);
        qDebug()<<files[0];
        if(pixmapItem)
        {
            delete pixmapItem;
            pixmapItem = NULL;
        }
        if(scene)
        {
            delete scene;
            scene = NULL;
        }

        pixmapItem = new MyPixmapItem(new QGraphicsPixmapItem);
        QImage image(files[0]);
        showImage(image);
    }
}
void MainWindow::on_RestBt_clicked(){
    pixmapItem->reset();
    QRectF rect = pixmapItem->boundingRect();
    scene->setSceneRect(0,0,rect.width(),rect.height());
    ui->Iv->fitInView(rect,Qt::KeepAspectRatio);
}

void MainWindow::on_LBt_clicked()
{
    if(times >= 0)
    {
        times--;
        if(times == -1)
                times=list.size()-1;
        info = list.at(times);
        QImage img (info.filePath());
        showImage(img);

    }

}

void MainWindow::on_RBt_clicked()
{
    if(times <= list.size()-1)
    {
        times++;
        if (times == list.size())
                times = 0;
        info = list.at(times);
        QImage img (info.filePath());
        showImage(img);
    }

}


bool MainWindow::getDisplayedImage(QImage &img) const
{
    QList<QGraphicsItem *>items = ui->Iv->scene()->items();
    if(items.size() == 0)
        return false;
    else{
        QGraphicsPixmapItem *item = (QGraphicsPixmapItem*)items.at(0);
        img = item->pixmap().toImage();
        return true;
    }

}

void MainWindow::on_actionhuiduzhuanhuan_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageProcessor::rgb2gray(img);
        showImage(res);
    }
}

void MainWindow::rec_SetDialog_trans_c(double c)
{
    qDebug()<<"received";
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageProcessor::logTransform(img,c);
        showImage(res);
    }
}

bool MainWindow::readOneRow(QString row, QVector<double> &kernal, int &nCol)
{
    nCol = 0;
    QStringList cols = row.trimmed().split(QRegularExpression("\\s*,|\\s+"));
    for(int j=0; j<cols.size();j++){
        bool ok;
        QString col = cols[j];
        if(col.trimmed().isEmpty())
            continue;
        double n = col.trimmed().toDouble(&ok);
//        qDebug()<<"n:"<<n;
        if(ok){
            nCol++;
            kernal.push_back(n);
//            qDebug()<<n;
        }
        else{
            return false;
        }
    }
    return true;
}


QVector<double> MainWindow::parseMatrix(QString string_mat,int& nCol)
{
    QStringList rows = string_mat.replace("\n","").trimmed().split(QRegularExpression("\\s*;"));
    qDebug()<<"rows:"<<rows;
    nCol = -1;
    QVector<double> mat;
    for(int i=0; i<rows.size(); i++){
        bool ok;
        QString row = rows[i];
//        qDebug()<<"each_row:"<<row;
        if(row.trimmed().isEmpty())
            continue;
        if(nCol = -1){
            int eachrow_num;
            ok = readOneRow(row,mat,eachrow_num);//...
            if(ok)
                nCol = eachrow_num;
            else
                return QVector<double>();
        }
        else{
            int eachrow_num;
            ok = readOneRow(row,mat,eachrow_num);
            if(ok && eachrow_num == nCol){
                continue;
            }
            else{
                nCol = -1;
                return QVector<double>();
            }
        }
    }
    return mat;
}

void MainWindow::Conv(QString string_mat)
{

    qDebug()<<"stringmat:"<<string_mat;
    int nCol,nRow;
    QVector<double> mat = parseMatrix(string_mat,nCol);
    nRow = mat.size()/nCol;
    QImage Img;
    if(getDisplayedImage(Img)){
        int nCol = 3;
//        QImage paddedimg = ImageFilter::paddingzero(Img,nCol,nRow);
        QImage ret = ImageFilter::LinearFilter(Img,mat,nCol,nRow);
//        QImage ret = ImageFilter::crop(convimg,nCol,nRow);
        showImage(ret);
    }

//    QVector<double>::iterator iter;
//    for(iter=mat.begin(); iter!= mat.end();iter++)
//        qDebug()<<*iter<<" ";
    //    qDebug()<<"qvector mat:"<<mat;
}

void MainWindow::GaussBlur()
{
       double sigma = gauss_dia->getSigma();
       int size = gauss_dia->getSize();
       qDebug()<<"sigma:"<<sigma<<" size:"<<size;
       QImage img;
       if(getDisplayedImage(img)){
           QImage res = ImageFilter::GaussBlur(img,sigma,size);
           showImage(res);
       }
}

void MainWindow::GaussBlurForGrey()
{
    double sigma = gauss_dia_forGrey->getSigma();
    int size = gauss_dia_forGrey->getSize();
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageFilter::GaussBlur_forGrey(img,sigma,size);
        showImage(res);
    }
}

void MainWindow::medianBlur()
{
    int kernalsize = size_dia->getSize();
    QImage img;
    if(getDisplayedImage(img)){
            QImage res = ImageFilter::MedianBlur(img,kernalsize);
            showImage(res);
    }
}

void MainWindow::medianBlurforGrey()
{
    int kernalsize = size_dia->getSize();
    QImage img;
    if(getDisplayedImage(img)){
            QImage res = ImageFilter::MedianBlurforGrey(img,kernalsize);
            showImage(res);
    }
}

void MainWindow::BilateralBlur()
{
    double sigma_d,sigma_r;
    int kernal_size;
    sigma_d = bilateral_dia->get_sigma_d();
    sigma_r = bilateral_dia->get_sigma_r();
    kernal_size = bilateral_dia->get_size();
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageFilter::BilateralFilter(img,kernal_size,sigma_r,sigma_d);
        showImage(res);
    }
}

void MainWindow::expanding()
{
    int kernalsize;
    kernalsize = size_dia_expanding->getSize();
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageFilter::expanding(img,kernalsize);
        showImage(res);
    }
}

void MainWindow::corroding()
{
    int kernalsize;
    kernalsize = size_dia_corroding->getSize();
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageFilter::corroding(img,kernalsize);
        showImage(res);
    }
}



void MainWindow::on_actionyansefanzhuan_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageProcessor::colorConvert(img);
        showImage(res);
    }
}



void MainWindow::on_actionduishubianhuan_triggered()
{
    dia->show();
}

void MainWindow::on_actionhistagram_triggered()
{
    QImage img;
    if(getDisplayedImage((img))){
        QImage res = ImageProcessor::col_hist(img);
        showImage(res);
    }
}

void MainWindow::on_actionhuidutujunheng_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageProcessor::gray_hist(img);
        showImage(res);
    }
}

void MainWindow::on_actionHSVhist_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        QImage res = ImageProcessor::hsl_hist(img);
        showImage(res);
    }
}

void MainWindow::on_actionjuanji_triggered()
{
    mat->show();

}

void MainWindow::on_actiongaosimohu_triggered()
{
    gauss_dia->show();
}

void MainWindow::on_actionzhongzhilvbo_triggered()
{
    size_dia->show();
}

void MainWindow::on_actiongaosilvbohuidu_triggered()
{
    gauss_dia->show();
}

void MainWindow::on_actionshaungbianmohu_triggered()
{
    bilateral_dia->show();
}

void MainWindow::on_actionsave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        QImage img;
        if(getDisplayedImage(img)){
            img.save(filename);
        }
    }
}

void MainWindow::on_actionzhongzhilvbohuidu_triggered()
{
    size_dia_forGrey->show();
}

void MainWindow::on_actionpengzhang_triggered()
{
    size_dia_expanding->show();
}

void MainWindow::on_actionfushi_triggered()
{
    size_dia_corroding->show();
}


void MainWindow::on_actionpinyutu_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        Matrix<int> m = Matrix<int>::fromQImage(img,'G');
        Matrix<complex<double>> fftmat = fft2d(m);
        Matrix<double> absmat = Matrix<double>::complexabs(fftmat);
//        cout<<absmat(0,0)<<endl;
        Matrix<double> logmat = Matrix<double>::complexlog(absmat);
        Matrix<double> fftshiftmat = Matrix<double>::fftshift(logmat);
//        cout<<fftshiftmat(0,0)<<endl;
        Matrix<int> normat = Matrix<int>::normalization(fftshiftmat);
        QImage img = Matrix<int>::toQImage(normat);
        showImage(img);
    }
}

void MainWindow::on_actionpinyutur_triggered()
{
    QImage img;
    if(getDisplayedImage(img)){
        Matrix<int> m1 = Matrix<int>::fromQImage(img,'r');
        Matrix<int> m2 = Matrix<int>::fromQImage(img,'g');
        Matrix<int> m3 = Matrix<int>::fromQImage(img,'b');
        Matrix<complex<double>> fftmat1 = fft2d(m1);
        Matrix<complex<double>> fftmat2 = fft2d(m2);
        Matrix<complex<double>> fftmat3 = fft2d(m3);
        Matrix<double> absmat1 = Matrix<double>::complexabs(fftmat1);
        Matrix<double> absmat2 = Matrix<double>::complexabs(fftmat2);
        Matrix<double> absmat3 = Matrix<double>::complexabs(fftmat3);
//        cout<<absmat(0,0)<<endl;
        Matrix<double> logmat1 = Matrix<double>::complexlog(absmat1);
        Matrix<double> logmat2 = Matrix<double>::complexlog(absmat2);
        Matrix<double> logmat3 = Matrix<double>::complexlog(absmat3);
        Matrix<double> fftshiftmat1 = Matrix<double>::fftshift(logmat1);
        Matrix<double> fftshiftmat2 = Matrix<double>::fftshift(logmat2);
        Matrix<double> fftshiftmat3 = Matrix<double>::fftshift(logmat3);
//        cout<<fftshiftmat(0,0)<<endl;
        Matrix<int> normat1 = Matrix<int>::normalization(fftshiftmat1);
        Matrix<int> normat2 = Matrix<int>::normalization(fftshiftmat2);
        Matrix<int> normat3 = Matrix<int>::normalization(fftshiftmat3);
        QImage img = Matrix<int>::toQImage(normat1,normat2,normat3);
        showImage(img);
    }
}

void MainWindow::on_actionshurujuzhen_triggered()
{

    transformDia = new InputMatrix(this,'t');
    connect(transformDia,SIGNAL(sendmatrix(QMatrix)),this,SLOT(tsrc(QMatrix)));
    transformDia->show();


}

void MainWindow::on_actionxuanzhuan_triggered()
{
    rotateDia = new InputMatrix(this,'r');
    connect(rotateDia,SIGNAL(sendmatrix(QMatrix)),this,SLOT(tsrc(QMatrix)));
    rotateDia->show();


}

void MainWindow::on_actionsuofang_triggered()
{
    scaleDia = new InputMatrix(this,'s');   
    connect(scaleDia,SIGNAL(sendmatrix(QMatrix)),this,SLOT(tsrc(QMatrix)));
    scaleDia->show();



}

void MainWindow::on_actionxieqie_triggered()
{
    shearDia = new InputMatrix(this,'c');
    connect(shearDia,SIGNAL(sendmatrix(QMatrix)),this,SLOT(tsrc(QMatrix)));
    shearDia->show();
}

void MainWindow::tsrc(QMatrix matrx)
{
    QImage img,ret;
    if(getDisplayedImage(img)){
            ret = ImageProcessor::transform(img,matrx);
//            cout<<matrx.dx()<<endl<<matrx.dy()<<endl<<matrx.m11()<<endl<<matrx.m12()<<endl;
//            cout<<matrx.m21()<<endl<<matrx.m22()<<endl;
            showImage(ret);

    }

}
