#include "imageprocessor.h"
using std::min_element;
using std::max_element;
using std::vector;

ImageProcessor::ImageProcessor()
{

}


QImage ImageProcessor::rgb2gray(const QImage &img)
{
    QImage tmp(img);
    int width = tmp.width();
    int height = tmp.height();
    for(int i=0; i<width; i++)
    {
        for(int j=0 ;j<height; j++)
        {
            QRgb rgb = tmp.pixel(i,j);
            int r = qRed(rgb);
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            int gray = (r+g+b)/3;
            tmp.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return tmp;

}

QImage ImageProcessor::colorConvert(const QImage &img)
{
    QImage tmp(img);
    int width = tmp.width();
    int height = tmp.height();
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            QRgb rgb = tmp.pixel(i,j);
            int r0 = 255 - qRed(rgb);
            int g0 = 255 - qGreen(rgb);
            int b0 = 255 - qBlue(rgb);
            tmp.setPixel(i,j,qRgb(r0,g0,b0));
        }
    }
    return tmp;
}

QImage ImageProcessor::logTransform(const QImage &img,double c)
{
    QImage tmp(img);
    int width = tmp.width();
    int height = tmp.height();
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            QRgb rgb = tmp.pixel(i,j);
            double r = qGray(rgb)/255.0;
            int res = c * std::log(1+r)*255;
            if(res >255){
                res = 255;
            }
            else if(res <0){
                res = 0;
            }
            else{
                tmp.setPixel(i,j,qRgb(res,res,res));
            }

        }
    }
    return tmp;
}

void ImageProcessor::calculateHist(int origin[],int res[],int picsize,int graysize){

    /*
     * @params:
     * picsize : size of a picture
     * graysize : grayscale , usually 256
     * origin: array , the number of channels graysize
     * p: array of channels graysize 's probabilities
     * tmp: sum of series probabilities
     * res: result after mapping, number inside is the grayscale of each channel after mapping
     */

    float *tmp = new float [graysize];
    float *p = new float [graysize];
    for(int i=0;i<graysize;i++){
        p[i] = (origin[i]*1.0)/(picsize);
    }

    for(int i=0;i<graysize;i++){
        if(i==0){
            tmp[i] = p[i];
        }
        else{
            tmp[i] = tmp[i-1] + p[i];
        }
        res[i] = (int)((graysize-1)*tmp[i]+0.5);
    }
    delete []tmp;
    delete []p;
}

QImage ImageProcessor::hsl_hist(const QImage &img)
{
    /*
     * @params:
     * tmp: result of hsl_hist
     * S: size of tmp
     * rgb: rgb of each pixel
     * hsl: hsl struct including h,s,l
     * p: 2-dimension hsl array which size is S
     * max: the max num of grayscale
     */

    QImage tmp(img);
    int width = img.width();
    int height = img.height();
    int S = width * height;
    QRgb rgb;

    typedef struct HSL{
        float h;
        float s;
        float l;
    }hsl;

    hsl **p = new hsl *[width];
    for(int i=0; i<width; i++)
        p[i] = new hsl[height];

    float max = 0 ;
    for (int i=0;i<width;i++){
        for(int j=0; j<height; j++){
            rgb = tmp.pixel(i,j);
            RGB2HSL(qRed(rgb),qGreen(rgb),qBlue(rgb),&p[i][j].h,&p[i][j].s,&p[i][j].l);
            max = max > p[i][j].l?max:p[i][j].l;
        }
    }

    int n = (int)(max + 0.5);
    int *ii = new int [n+1];
    int *res_ii = new int[n+1];
    memset(ii,0,n*sizeof(int));

    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            ii[(int)(p[i][j].l+0.5)]++;
        }
    }

    calculateHist(ii,res_ii,S,n+1);

    for (int i=0;i<width;i++){
        for(int j=0; j<height; j++){
            p[i][j].l = res_ii[(int) (p[i][j].l + 0.5)];
            float r,g,b;
            HSL2RGB(p[i][j].h,p[i][j].s,p[i][j].l,&r,&g,&b);
            r = r>255?255:(int)(r+0.5);
            g = g>255?255:(int)(g+0.5);
            b = b>255?255:(int)(b+0.5);
            tmp.setPixel(i,j,qRgb(r,g,b));
        }

    }

    // 释放
    for(int i=0; i<width;i++)
        delete []p[i];
    p = NULL;
    delete []ii;
    delete []res_ii;

    return tmp;


}

float ImageProcessor::Hue_2_RGB(float v1, float v2, float vH)
{

       if ( vH < 0 )
           vH += 1.0;
       if( vH > 1 )
           vH -= 1.0;
       if ( ( 6.0 * vH ) < 1 )
           return ( v1 + ( v2 - v1 ) * 6 * vH );
       if ( ( 2.0 * vH ) < 1 )
           return ( v2 );
       if ( ( 3.0 * vH ) < 2 )
           return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6 );
       return  v1 ;

}

void ImageProcessor::HSL2RGB(float H,float S,float L,float *R,float *G, float *B)
{
    L = L / 255.0;
    float var_1,var_2;
    if ( S == 0 )
    {

       *R = L * 255;
       *G = L * 255;
       *B = L * 255;
    }
    else
    {
       if ( L < 0.5 )
           var_2 = L * ( 1 + S );
       else
           var_2 = ( L + S ) - ( S * L );

       var_1 = 2 * L - var_2;

       *R = 255 * Hue_2_RGB( var_1, var_2, H + ( 1 / 3. ) );
       *G = 255 * Hue_2_RGB( var_1, var_2, H );
       *B = 255 * Hue_2_RGB( var_1, var_2, H - ( 1 / 3. ) );
    }



}

void ImageProcessor::RGB2HSL(float R,float G,float B,float *H,float *S,float *L)
{

    float del_Max,var_Max,var_Min,
    del_R,del_G,del_B,
    var_R,var_G,var_B;

    var_R = ( R / 255.0 );
    var_G = ( G / 255.0 );
    var_B = ( B / 255.0 );

    var_Min = qMin(qMin( var_R, var_G), var_B );    //Min. value of RGB
    var_Max = qMax(qMax( var_R, var_G), var_B );    //Max. value of RGB
    del_Max = var_Max - var_Min;             //Delta RGB value

    *L = ( var_Max + var_Min )/ 2.0;

    if ( del_Max == 0 )                     //This is a gray, no chroma...
    {
        *H = 0;
        *S = 0;
    }
    else                                    //Chromatic data...
    {
       if ( *L < 0.5 )
           *S = del_Max / (float)( var_Max + var_Min );
       else
           *S = del_Max / (float)( 2.0 - var_Max - var_Min );

       del_R = ( ( ( var_Max - var_R ) / 6. ) + ( del_Max / 2. ) ) / del_Max;
       del_G = ( ( ( var_Max - var_G ) / 6. ) + ( del_Max / 2. ) ) / del_Max;
       del_B = ( ( ( var_Max - var_B ) / 6. ) + ( del_Max / 2. ) ) / del_Max;

       if      ( var_R == var_Max )
           *H = del_B - del_G;
       else if ( var_G == var_Max )
           *H = ( 1 / 3. ) + del_R - del_B;
       else if ( var_B == var_Max )
           *H = ( 2 / 3. ) + del_G - del_R;

        if ( *H < 0 )
            *H += 1.0;
        if ( *H > 1.0 )
            *H -= 1.0;
    }
    *L = (int)((*L) * 255);

}

QImage ImageProcessor::col_hist(const QImage &img)
{

    /* @params
     * r[],g[],b[]  原图灰度级数量统计
     * p_r[],p_g[],p_b[] 原图灰度级各灰度概率
     * r_tmp[],g_tmp[],b_tmp[] 均衡后个灰度级概率
     * return_r,return_g,return_b 均衡化后对应像素灰度值
     *
     *
     */

    QImage tmp(img);
    int width = tmp.width();
    int height = tmp.height();
    int s = width*height;
    int r[256],g[256],b[256];
    memset(r,0,sizeof(r));
    memset(g,0,sizeof(g));
    memset(b,0,sizeof(b));
    int return_r[256],return_g[256],return_b[256];
    int r_res,b_res,g_res,r_temp,b_temp,g_temp;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            QRgb rgb = tmp.pixel(i,j);
            r[qRed(rgb)]++;
            g[qGreen(rgb)]++;
            b[qBlue(rgb)]++;
        }
    }

    calculateHist(r,return_r,s,256);
    calculateHist(g,return_g,s,256);
    calculateHist(b,return_b,s,256);

    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            QRgb rgb_res = tmp.pixel(i,j);
            r_temp = qRed(rgb_res);
            g_temp = qGreen(rgb_res);
            b_temp = qBlue(rgb_res);
            r_res = return_r[r_temp];
            g_res = return_g[g_temp];
            b_res = return_b[b_temp];
            tmp.setPixel(i,j,qRgb(r_res,g_res,b_res));
        }
    }
    return tmp;
}

QImage ImageProcessor::gray_hist(const QImage &img)
{
    QImage tmp(img);
    int width = tmp.width();
    int height = tmp.height();
    int s = width * height;
    int hist[256],map[256];
    memset(hist,0,sizeof(hist));
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
           hist[qGray(img.pixel(i,j))]++;
         }
    }
    calculateHist(hist,map,s,256);
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
          int g = map[qGray(tmp.pixel(i,j))];
          tmp.setPixel(i,j,qRgb(g,g,g));
        }
    }
    return tmp;

}


QImage ImageProcessor::transform(const QImage &img, QMatrix &matrix)
{
    QImage ret(img);
    //判断矩阵可逆
    if(!matrix.isInvertible())
    {
        return ret;
    }
    int x_min,x_max,y_min,y_max;
    getNewImageRange(ret,x_min,x_max,y_min,y_max,matrix);
    int new_w = x_max - x_min;
    int new_h = y_max - y_min;
    QImage new_img(new_w,new_h,ret.format());
    QMatrix MatrixT = matrix.inverted();
    for(int i = 0;i<new_w;i++)
    {
        for(int j = 0;j<new_h;j++)
        {
            double new_x,new_y;
            MatrixT.map(double(i+x_min),double(j+y_min),&new_x,&new_y);
            QRgb rgb = bilinearInterpolation(ret,new_x,new_y);
            new_img.setPixel(i,j,rgb);
        }
    }
    return new_img;
}

QRgb ImageProcessor::bilinearInterpolation(const QImage &img,double &x, double &y)
{
    if(x>=img.width()||x<0||y>=img.height()||y<0)
    {

        return qRgb(0,0,0);
    }
    int x0 = floor(x);
    int x1 = ceil(x);
    int y0 = floor(y);
    int y1 = ceil(y);
    if(x1>=img.width())
        x1 = img.width()-1;
    if(y1>=img.height())
        y1 = img.height()-1;
    if(x0<0)
        x0 = 0;
    if(y0<0)
        y0 = 0;
    double delta_x = x - x0;
    double delta_y = y - y0;
    int r,g,b;
    Matrix<double> matrix1(1,2,0.0);
    Matrix<double> matrix2(2,1,0.0);
    Matrix<double> matrix3(2,2,0.0);
    Matrix<double> result(1,1,0.0);
    matrix1(0,0) = 1.0 - delta_x;
    matrix1(0,1) = delta_x;
    matrix2(0,0) = 1.0 - delta_y;
    matrix2(1,0) = delta_y;
    matrix3 = RGBMatrix(img,x0,y0,x1,y1,'r');

    result = matrix1*matrix3;
    result = result * matrix2;

    r = result(0,0);
    matrix3 = RGBMatrix(img,x0,y0,x1,y1,'g');
    result = matrix1*matrix3;
    result = result * matrix2;
    g = result(0,0);
    matrix3 = RGBMatrix(img,x0,y0,x1,y1,'b');
    result = matrix1*matrix3;
    result = result * matrix2;
    b = result(0,0);

    return qRgb(r,g,b);
}

Matrix<double> ImageProcessor::RGBMatrix(const QImage &img,int x0, int y0, int x1, int y1, char c)
{
    Matrix<double> matrix(2,2,0);
    int r1,r2,r3,r4,b1,b2,b3,b4,g1,g2,g3,g4;
    switch(c)
    {
    case 'r':
        r1 = qRed(img.pixel(x0,y0));
        r2 = qRed(img.pixel(x0,y1));
        r3 = qRed(img.pixel(x1,y0));
        r4 = qRed(img.pixel(x1,y1));
        matrix(0,0) = r1;
        matrix(0,1) = r2;
        matrix(1,0) = r3;
        matrix(1,1) = r4;
        break;
    case 'g':
        g1 = qGreen(img.pixel(x0,y0));
        g2 = qGreen(img.pixel(x0,y1));
        g3 = qGreen(img.pixel(x1,y0));
        g4 = qGreen(img.pixel(x1,y1));
        matrix(0,0) = g1;
        matrix(0,1) = g2;
        matrix(1,0) = g3;
        matrix(1,1) = g4;
        break;
    case 'b':
        b1 = qBlue(img.pixel(x0,y0));
        b2 = qBlue(img.pixel(x0,y1));
        b3 = qBlue(img.pixel(x1,y0));
        b4 = qBlue(img.pixel(x1,y1));
        matrix(0,0) = b1;
        matrix(0,1) = b2;
        matrix(1,0) = b3;
        matrix(1,1) = b4;
        break;
    }
    return matrix;
}

void ImageProcessor::getNewImageRange(const QImage &img, int &x_min, int &x_max, int &y_min, int &y_max,QMatrix &matrix)
{
    int width = img.width();
    int height = img.height();
    int x0,x1,x2,x3,y0,y1,y2,y3;
    matrix.map(0,0,&x0,&y0);
    matrix.map(width-1,0,&x1,&y1);
    matrix.map(0,height-1,&x2,&y2);
    matrix.map(width-1,height-1,&x3,&y3);
    vector<int> x,y;
    x.push_back(x0); x.push_back(x1);
    x.push_back(x2); x.push_back(x3);
    y.push_back(y0); y.push_back(y1);
    y.push_back(y2); y.push_back(y3);
    x_max = *max_element(x.begin(),x.end());
    x_min = *min_element(x.begin(),x.end());
    y_max = *max_element(y.begin(),y.end());
    y_min = *min_element(y.begin(),y.end());
}


