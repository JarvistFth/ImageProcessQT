#include "imagefilter.h"
using namespace std;
ImageFilter::ImageFilter()
{

}



QImage ImageFilter::paddingzero(const QImage &img,int &nCol,int &nRow)
{
    QImage tmp(img.width()+nCol-1,img.height()+nCol-1,img.format());
    int width = tmp.width();
    int height = tmp.height();
    qDebug()<<width<<" h:"<<height;
    for(int i = 0;i < width ; i++){
        for(int j = 0; j < height ; j++){
            if(j < nRow/2 || j >= height - nRow/2 ||i < nCol/2 || i >= width - nCol/2)
              tmp.setPixel(i,j,qRgb(0,0,0));
            else
              tmp.setPixel(i,j,img.pixel(i-nCol/2,j-nCol/2));
        }  
    }
    return tmp;
}

QVector<int> ImageFilter::getPatch(const QImage &img, int i, int j, int nCol, int nRow, char s)
{
    /*
     * params: i for col j for row
     *
     *
     */
    QVector<int> ret;
    QRgb rgb;
    for(int y = -nRow/2; y<=nRow/2;y++){
        for(int x = -nCol/2; x<=nCol/2; x++){
            rgb = img.pixel(i+x,j+y);
            if(s == 'r'){
                ret.push_back(qRed(rgb));
            }
            else if(s == 'g'){
            ret.push_back(qGreen(rgb));
            }
            else if(s == 'b'){
                ret.push_back(qBlue(rgb));
            }
        }
    }
    return ret;
}

QImage ImageFilter::LinearFilter(const QImage &img, QVector<double>mat,int nCol,int nRow)
{
    QImage tmp = paddingzero(img,nCol,nRow);
    int width = tmp.width();
    int height = tmp.height();
    QVector<double>filter_mat;
    QVector<int>Rpatch,Bpatch,Gpatch;
    filter_mat = normalization(mat);
    double sumR,sumG,sumB;
    int r,g,b;
    for(int i = nCol/2; i< width - nCol/2; i++){
        for(int j = nRow/2; j<height - nRow/2; j++){
            sumR = 0; sumG = 0; sumB = 0;
            r = 0; g = 0; b = 0;
            Rpatch = getPatch(tmp,i,j,nCol,nRow,'r');
            Gpatch = getPatch(tmp,i,j,nCol,nRow,'g');
            Bpatch = getPatch(tmp,i,j,nCol,nRow,'b');
//            qDebug()<<"Rpatch:"<<Rpatch<<" Gpatch:"<<Gpatch<<" Bpatch:"<<Bpatch;
            for(int k = 0; k < filter_mat.size(); k++){
                sumR += filter_mat.at(k) * Rpatch.at(k);
                sumG += filter_mat.at(k) * Gpatch.at(k);
                sumB += filter_mat.at(k) * Bpatch.at(k);
            }
            r = calculate((int)sumR);
            g = calculate((int)sumG);
            b = calculate((int)sumB);
            tmp.setPixel(i,j,qRgb(r,g,b));
        }
    }
    QImage ret = crop(tmp,nCol,nRow);
    return ret;

}

QVector<double> ImageFilter::normalization(QVector<double> mat)
{
    double kernalsum = 0;
    QVector<double> ret;
    for(int i = 0; i<mat.size(); i++){
        kernalsum += mat.at(i);
//        qDebug()<<"i:"<<mat.at(i);
    }

//    qDebug()<<"kernalsum:"<<kernalsum<<" matsize:"<<mat.size();
    if(kernalsum == 0 || kernalsum == 1.0){
        for(int i = 0;i < mat.size(); i++){
            ret.push_back(mat.at(i));
//            qDebug()<<"kernalsum 0 or 1"<<mat.at(i);
        }
    }
    else {
        for(int i = 0;i < mat.size(); i++){
//            qDebug()<<"kernalsum not 0 or 1:"<<mat.at(i);
            ret.push_back(mat.at(i)/kernalsum);
        }
    }
//    qDebug()<<"normalization:"<<ret;
    return ret;
}

QImage ImageFilter::GaussBlur(const QImage &img, double sigma, int size)
{
    QImage tmp;
    QVector<double>gauss_mat;
    gauss_mat = createGaussKernal(size,sigma);
    qDebug()<<"gauss_mat:"<<gauss_mat;
    tmp = LinearFilter(img,gauss_mat,size,size);
    return tmp;

}

int ImageFilter::calculate(int color){
    if(color < 0)
        return 0;
    else if(color > 255)
        return 255;
    else
        return color;

}

QImage ImageFilter::LinearFilter_forGrey(const QImage &img, QVector<double> mat, int nCol, int nRow)
{
    QImage grey = ImageProcessor::rgb2gray(img);
    QImage tmp = paddingzero(grey,nCol,nRow);
    int width = tmp.width();
    int height = tmp.height();
    QVector<double>filter_mat;
    QVector<int>Rpatch,Bpatch,Gpatch;
    filter_mat = normalization(mat);
    double sumR,sumG,sumB;
    int r,g,b;
    for(int i = nCol/2; i< width - nCol/2; i++){
        for(int j = nRow/2; j<height - nRow/2; j++){
            sumR = 0; sumG = 0; sumB = 0;
            r = 0; g = 0; b = 0;
            Rpatch = getPatch(tmp,i,j,nCol,nRow,'r');
            Gpatch = getPatch(tmp,i,j,nCol,nRow,'g');
            Bpatch = getPatch(tmp,i,j,nCol,nRow,'b');
//            qDebug()<<"Rpatch:"<<Rpatch<<" Gpatch:"<<Gpatch<<" Bpatch:"<<Bpatch;
            for(int k = 0; k < filter_mat.size(); k++){
                sumR += filter_mat.at(k) * Rpatch.at(k);
                sumG += filter_mat.at(k) * Gpatch.at(k);
                sumB += filter_mat.at(k) * Bpatch.at(k);
            }
            r = calculate((int)sumR);
            g = calculate((int)sumG);
            b = calculate((int)sumB);
            tmp.setPixel(i,j,qRgb(r,g,b));
        }
    }
    QImage ret = crop(tmp,nCol,nRow);
    return ret;
}





QImage ImageFilter::GaussBlur_forGrey(const QImage &img, double sigma, int size)
{
    QImage grey = ImageProcessor::rgb2gray(img);
    QVector<double>gauss_mat;
    gauss_mat = createGaussKernal(size,sigma);
    qDebug()<<"gauss_mat:"<<gauss_mat;
    QImage tmp = LinearFilter(grey,gauss_mat,size,size);
    return tmp;
}

QVector<double> ImageFilter::createBilateralKernal(int size,double sigma_r)
{
    QVector<double> bilateral_mat;
    for(int i=0; i<256; i++){
        bilateral_mat.push_back(exp((-i*i)/(2*sigma_r*sigma_r)));
    }
    return bilateral_mat;
}

QVector<double> ImageFilter::createGaussKernal(int size,double sigma)
{
    double num;
    int center = size/2;
    QVector<double>gauss_mat;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
           num = exp(-1*((i-center)*(i-center)/(2*sigma*sigma) + (j-center)*(j-center)/(2*sigma*sigma)));
           gauss_mat.push_back(num);
        }
    }
    return gauss_mat;
}

QImage ImageFilter::BilateralFilter(const QImage &img, int size, double sigma_d, double sigma_r)
{
    QImage grey_img = ImageProcessor::rgb2gray(img);
    QImage padded = paddingzero(grey_img,size,size);

    int width = padded.width();
    int height = padded.height();
    int grey_near = 0 , grey_center = 0;
    double num = 0, gauss_num = 0 ;


    QVector<double> kernalSpace = createGaussKernal(size,sigma_d);
    QVector<double> kernalColor = createBilateralKernal(size,sigma_r);
    QVector<int> GreyPatch;
    QVector<double> mat;

    double sum;
    int grey;
    qDebug()<<"size:"<<size;
    for(int i = size/2; i< width - size/2; i++){
        for(int j = size/2; j < height - size/2; j++){
            GreyPatch = getPatch(padded,i,j,size,size,'r');
//            qDebug()<<"GreyPatch:"<<GreyPatch;
            grey_center = qRed(padded.pixel(i,j));//中心灰度值
//            qDebug()<<"greycenter:"<<grey_center;
            QVector<double> filter_mat;
//            centerNum = kernalColor.at(grey_center);//
            for(int k = 0 ;k < GreyPatch.size(); k++){
                grey_near = GreyPatch.at(k);//邻域灰度值
//                qDebug()<<"greycenter:"<<grey_center<<" greynear:"<<grey_near;
                num = kernalColor.at(abs(grey_near - grey_center));
                gauss_num = kernalSpace.at(k);
                filter_mat.push_back(num * gauss_num);
            }
            mat = normalization(filter_mat);
            sum = 0; grey = 0;
            for(int k = 0; k < mat.size(); k++){
                sum += mat.at(k) * GreyPatch.at(k);

            }
            grey = calculate((int)sum);
            padded.setPixel(i,j,qRgb(grey,grey,grey));

        }
    }
    QImage ret = crop(padded,size,size);
    return ret;
}

QImage ImageFilter::MedianBlur(const QImage &img, int size)
{
    QImage padded = paddingzero(img,size,size);
    int width = padded.width();
    int height = padded.height();

    QVector<int>Rpatch,Bpatch,Gpatch;
    int r,g,b;

    for(int i = size/2; i< width - size/2; i++){
        for(int j = size/2; j<height - size/2; j++){
//            rMed = 0; gMed = 0; bMed = 0;
            r = 0; g = 0; b = 0;
            Rpatch = getPatch(padded,i,j,size,size,'r');
            Gpatch = getPatch(padded,i,j,size,size,'g');
            Bpatch = getPatch(padded,i,j,size,size,'b');
//            qDebug()<<"Rpatch:"<<Rpatch<<" Gpatch:"<<Gpatch<<" Bpatch:"<<Bpatch;
            qSort(Rpatch.begin(),Rpatch.end());
            qSort(Gpatch.begin(),Gpatch.end());
            qSort(Bpatch.begin(),Bpatch.end());
//            int rtmp = 0, btmp = 0, gtmp = 0;
            r = calculate((int)Rpatch.at(size*size/2));
            g = calculate((int)Gpatch.at(size*size/2));
            b = calculate((int)Bpatch.at(size*size/2));
            padded.setPixel(i,j,qRgb(r,g,b));
        }
    }
    QImage ret = crop(padded,size,size);
    return ret;
}

QImage ImageFilter::MedianBlurforGrey(const QImage &img, int size)
{
    QImage greyimg = ImageProcessor::rgb2gray(img);
    QImage padded = paddingzero(greyimg,size,size);
    int width = padded.width();
    int height = padded.height();

    QVector<int>greyPatch;
    int grey;
    for(int i = size/2; i< width - size/2; i++){
        for(int j = size/2; j<height - size/2; j++){
//            rMed = 0; gMed = 0; bMed = 0;
            grey = 0;
            greyPatch = getPatch(padded,i,j,size,size,'r');
            qSort(greyPatch.begin(),greyPatch.end());
//            int rtmp = 0, btmp = 0, gtmp = 0;
            grey = calculate((int)greyPatch.at(size*size/2));
            padded.setPixel(i,j,qRgb(grey,grey,grey));
        }
    }
    QImage ret = crop(padded,size,size);
    return ret;
}

QImage ImageFilter::expanding(const QImage &img, int size)
{
    QImage greyimg = ImageProcessor::rgb2gray(img);
    QImage padded = paddingzero(greyimg,size,size);

    int width = padded.width();
    int height = padded.height();

    QImage tmp(width,height,padded.format());

    QVector<int>greyPatch;
//    vector<int>v;
    int grey;
    for(int i = size/2; i< width - size/2; i++){
        for(int j = size/2; j<height - size/2; j++){
//            rMed = 0; gMed = 0; bMed = 0;
            grey = 0;
            greyPatch = getPatch(padded,i,j,size,size,'r');
//            v = greyPatch.toStdVector();
            grey = *max_element(begin(greyPatch),end(greyPatch));
            tmp.setPixel(i,j,qRgb(grey,grey,grey));
        }
    }
    QImage ret = crop(tmp,size,size);
    return ret;
}

QImage ImageFilter::corroding(const QImage &img, int size)
{
    QImage greyimg = ImageProcessor::rgb2gray(img);
    QImage padded = paddingzero(greyimg,size,size);

    int width = padded.width();
    int height = padded.height();

    QImage tmp(width,height,padded.format());


    QVector<int>greyPatch;
//    vector<int>v;
    int grey;
    for(int i = size/2; i< width - size/2; i++){
        for(int j = size/2; j<height - size/2; j++){
//            rMed = 0; gMed = 0; bMed = 0;
            grey = 0;
            greyPatch = getPatch(padded,i,j,size,size,'r');
//            v = greyPatch.toStdVector();
            grey = *min_element(begin(greyPatch),end(greyPatch));
            tmp.setPixel(i,j,qRgb(grey,grey,grey));
        }
    }
    QImage ret = crop(tmp,size,size);
    return ret;
}


QImage ImageFilter::crop(const QImage &img, int nCol, int nRow)
{
    QImage tmp(img.width() - nCol + 1, img.height() - nRow + 1, img.format());
//    int kernelsize = nCol * nRow;
    int width = tmp.width();
    int height = tmp.height();

    for(int i = 0; i < width ; i++){
        for(int j = 0; j<height ; j++){
            tmp.setPixel(i,j,img.pixel(i+nCol/2,j+nRow/2));
        }
    }
    return tmp;
}
