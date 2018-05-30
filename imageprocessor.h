#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <QImage>
#include "matrix.h"

class ImageProcessor
{
public:
    ImageProcessor();
    static QImage rgb2gray(const QImage &img);
    static QImage colorConvert(const QImage &img);
    static QImage logTransform(const QImage &img,double c);
    static QImage col_hist(const QImage &img);
    static QImage gray_hist(const QImage &img);
    static void calculateHist(int origin[],int res[],int picsize,int graysize);
    static QImage hsl_hist(const QImage &img);
    static void RGB2HSL(float R,float G,float B,float *H,float *S,float *L);
    static void HSL2RGB(float H,float S,float L,float *R,float *G, float *B);
    static float Hue_2_RGB(float v1,float v2,float H);
    static QImage transform(const QImage &img, QMatrix &matrix);
    static QRgb bilinearInterpolation(const QImage &img,double &x, double &y);
    static Matrix<double> RGBMatrix(const QImage &img,int x0,int y0,int x1,int y1,char c);
    static void getNewImageRange(const QImage &img,int &x_min,int &x_max,int &y_min,int &y_max,QMatrix &matrix);
};


#endif // IMAGEPROCESSOR_H
