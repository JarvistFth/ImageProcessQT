#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H
#include <QImage>
#include <imageprocessor.h>
#include <qdebug.h>
#include <QString>
#include <algorithm>
class ImageFilter
{
public:
    ImageFilter();
    static QVector<int> getPatch(const QImage &img,int i,int j,int nCol,int nRow,char s);
    static QImage paddingzero(const QImage &img,int &nCol,int &nRow);
    static QImage LinearFilter(const QImage &img,QVector<double>mat,int nCol,int nRow);
    static QImage LinearFilter_forGrey(const QImage &img,QVector<double>mat,int nCol,int nRow);
    static QImage crop(const QImage &img,int nCol,int nRow);
    static int calculate(int color);
    static QVector<double> normalization(QVector <double>mat);
    static QImage GaussBlur(const QImage &img,double sigma,int size);
    static QImage GaussBlur_forGrey(const QImage &img,double sigma,int size);
    static QVector<double> createBilateralKernal(int size,double sigma_r);
    static QVector<double> createGaussKernal(int size,double sigma);
    static QImage BilateralFilter(const QImage &img, int size,double sigma_r,double sigma_s);

    static QImage MedianBlur(const QImage &img,int size);
    static QImage MedianBlurforGrey(const QImage &img,int size);

    static QImage expanding(const QImage &img,int size);
    static QImage corroding(const QImage &img,int size);

};

#endif // IMAGEFILTER_H
