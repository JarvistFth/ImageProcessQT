#ifndef FFT__H
#define FFT__H

#include <complex>
#include <vector>
#include <matrix.h>

using std::vector;
using std::complex;

const double PI = 3.1415926;

size_t calcN(size_t length);
complex<double> complexExpPow(size_t N, int k);

// different function with different input
vector<complex<double> > fft(vector<int> data, size_t N=0);
vector<complex<double> > fft(vector<double> data, size_t N=0);
vector<complex<double> > fft(vector<std::complex<double> > data, size_t N=0) ;

vector<complex<double> > ifft(vector<int> data, size_t N=0);
vector<complex<double> > ifft(vector<double> data, size_t N=0);
vector<complex<double> > ifft(vector<std::complex<double> > data, size_t N=0) ;

Matrix<complex<double> > fft2d(const Matrix<int>& mat, size_t M=0, size_t N=0);
Matrix<complex<double> > fft2d(const Matrix<double>& mat, size_t M=0, size_t N=0);
Matrix<complex<double> > fft2d(const Matrix<complex<double> >& mat, size_t M=0, size_t N=0);

Matrix<complex<double> > ifft2d(const Matrix<int>& mat, size_t M=0, size_t N=0);
Matrix<complex<double> > ifft2d(const Matrix<double>& mat, size_t M=0, size_t N=0);
Matrix<complex<double> > ifft2d(const Matrix<complex<double> >& mat, size_t M=0, size_t N=0);

template <typename T>
Matrix<T> fftRow(const Matrix<T>& mat);

template <typename T>
Matrix<T> ifftRow(const Matrix<T>& mat);


template <typename T>
Matrix<T> fftRow(const Matrix<T> &mat)
{
    Matrix<T> ret(mat);
    vector<complex<double> > vc,vd;
    cout<<endl;
//    cout<<" mat:<<"<<mat<<endl;
    for(size_t i=0; i<ret.getNRow();i++){
//        vd = mat.getRow(i);

//        vc = fft(mat.getRow(i));
//        ret.setRow(i,vc);
//        for(int j = 0; j<vd.size();j++){
//            cout<<"i:"<<i<<" vd:"<<vd.at(j)<<endl;
//            cout<<"vc:"<<vc.at(j)<<" "<<endl;
//        }



        ret.setRow(i,fft(mat.getRow(i)));
    }
//    cout<<"ret:"<<ret;
    return ret;
}

template <typename T>
Matrix<T> ifftRow(const Matrix<T> &mat){
    Matrix<T> ret(mat);
    for(size_t i=0;i<ret.getNRow();i++){
        ret.setRow(i,ifft(mat.getRow(i)));
    }
    return ret;
}

#endif
