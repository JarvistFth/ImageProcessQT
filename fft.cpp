#include "fft.h"
#define PI 3.1415926
#include "iostream"
using std::cout;
using std::endl;
using std::cerr;
using std::complex;
using std::vector;


size_t calcN(size_t length) {
  // check if length is power of 2
  // if it is, just return length
  // if not, get the correct N and return
    if (length == 1 || length == 0) {
        return 2;
    }
    else if ((length & (length - 1) )== 0) {
        return length;
    }
    else {

        size_t N = length;
        while (N >>= 1) {
            length |= N;
        }
        return length + 1;
    }
}

complex<double> complexExpPow(size_t N,int k) {
  // return base^{exponent}
    complex<double> tmp(0.0, -2 * PI / N * k);
//    cout<<"N:"<<N<<" K: "<<k<<" tmp:"<<tmp<<" exptmp:"<<exp(tmp)<<endl;
    return std::exp(tmp);

}

vector<complex<double> >
fft(vector<int> data, size_t N)
{
  // convert data to vector<complex<double> >
  // call:
  // vector<complex<double> >
  // fft(vector<complex<double> > data, size_t N)
    vector<complex<double>> temp,ret;
    for (int i = 0; i < data.size(); i++) {
        std::complex<double> tmp((double)data.at(i), 0.0);
        temp.push_back(tmp);
    }
    ret = fft(temp,N);
    return ret;
}

vector<complex<double> >
fft(vector<double> data, size_t N)
{

  // convert data to vector<complex<double> >
  // call:
  // vector<complex<double> >
  // fft(vector<complex<double> > data, size_t N)
    vector<complex<double>> temp,ret;
    for (int i = 0; i < data.size(); i++) {
        std::complex<double> tmp(data.at(i), 0.0);
        temp.push_back(tmp);
    }
    ret = fft(temp, N);
    return ret;
}

vector<complex<double> >
fft(vector<complex<double> > data, size_t N) {
    if(N == 0)
        N = calcN(data.size());
    if ((N & (N - 1)) != 0) {
        cerr << "error N!";
        return data;
    }
    size_t size = data.size();
  // check length of data, and append 0 if neccessary
    while(size<N) {
        complex<double> zero(0.0, 0.0);
        data.push_back(zero);
        size++;
    }
  // start fft
  // check if N is 0, 1, 2
  // if N is 0 or 1, just return data
  // if N is 2, do dft on them
  // if N > 2, do fft
    // 1. split input into two parts
    // 2. do fft on them seperately
    // 3. construct result from output
    vector<complex<double>> ret, temp;

    if (N == 0 || N == 1) {
        return data;
    }
    else if (N == 2) {
        ret.push_back(data.at(0) + data.at(1));
        ret.push_back(data.at(0) - data.at(1));
        return ret;
    }
    else {
            vector<complex<double>> oddpart, evenpart,x1,x2;
            for (int r = 0; r < data.size(); r+=2) {
                oddpart.push_back(data.at(r+1));
                evenpart.push_back(data.at(r));
            }
            x1 = fft(evenpart);
            x2 = fft(oddpart);


            for (size_t r = 0; r < N/2; ++r) {
                complex<double> wnk = complexExpPow(N,r);
//                cout<<"r:"<<r<<" wnk:"<<wnk<<endl;
                ret.push_back(x1.at(r) + x2.at(r)*wnk);
            }
            for (size_t r = N / 2; r < N; ++r) {
                size_t ri = r - N / 2;
                complex<double> wnk = complexExpPow(N, ri);
//                cout<<"r:"<<r<<" wnk:"<<wnk<<endl;
                ret.push_back(x1.at(ri) - x2.at(ri)*wnk);
            }
        return ret;
    }
}


Matrix<complex<double> > fft2d(const Matrix<int>& mat, size_t M, size_t N){
    complex<double> init(0.0,0.0);
    Matrix<complex<double> > temp(mat.getNRow(),mat.getNCol(),init);
    for (size_t i = 0; i < mat.getNRow(); i++) {
        for(size_t j = 0 ; j < mat.getNCol(); j++){
            try{
                std::complex<double> tmp(double(mat(i,j)), 0.0);
                temp(i,j) = tmp;
            }catch(range_error e){
                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
    Matrix<complex<double> > ret = fft2d(temp, M, N);
    return ret;
}
Matrix<complex<double> > fft2d(const Matrix<double>& mat, size_t M, size_t N){
    complex<double> init(0.0,0.0);
    Matrix<complex<double> > temp(mat.getNRow(),mat.getNCol(),init);
    for (size_t i = 0; i < mat.getNRow(); i++) {
        for(size_t j = 0 ; j < mat.getNCol(); j++){
            try{
                std::complex<double> tmp(mat(i,j), 0.0);
                temp(i,j) = tmp;
            }catch(range_error e){
                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
    Matrix<complex<double> > ret = fft2d(temp,M, N);
    return ret;
}


Matrix<complex<double> > fft2d(const Matrix<complex<double> > &mat, size_t M, size_t N)
{
    if(M == 0)
        M = calcN(mat.getNRow());
    if(N == 0)
        N = calcN(mat.getNCol());
    cout<<M<<" N:"<<N<<endl;
    if(((M & (M-1)) != 0) | (N & (N-1)) != 0)
    {
        cerr<<"M or N is not 2^n!"<<endl;
        return mat;
    }

    Matrix<complex<double> > tmp(M,N,complex<double>(0.0,0.0));
//    for(int i=0;i<mat.getNRow();i++){
//        for(int j=0;j<mat.getNCol();j++){
//            try{
//                tmp(i,j) = mat(i,j);
//            }
//            catch(range_error e){
//                cerr<<e.what()<<endl;
//                continue;
//            }
//        }
//    }

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            try{
                tmp(i,j) = mat(i,j);
            }
            catch(range_error e){
//                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
    return fftRow(fftRow(tmp).transpose()).transpose();
}





vector<complex<double> > ifft(vector<complex<double> > data, size_t N)
{
    if(N == 0)
        N = calcN(data.size());
    if ((N & (N - 1)) != 0) {
        cerr << "error N!";
        return data;
    }
    size_t size = data.size();
  // check length of data, and append 0 if neccessary
    while(size<N) {
        complex<double> zero(0.0, 0.0);
        data.push_back(zero);
        size++;
    }
  // start fft
  // check if N is 0, 1, 2
  // if N is 0 or 1, just return data
  // if N is 2, do dft on them
  // if N > 2, do fft
    // 1. split input into two parts
    // 2. do fft on them seperately
    // 3. construct result from output
    vector<complex<double>> ret, temp;

    if (N == 0 || N == 1) {
        return data;
    }
    else if (N == 2) {
        complex<double> n(1.0/N,0.0);
        ret.push_back((data.at(0)*n + data.at(1)*n));
        ret.push_back((data.at(0)*n - data.at(1)*n));
//        cout<<ret.at(1)<<endl;
        return ret;
    }
    else {
            vector<complex<double>> oddpart, evenpart,X1,X2;

            for (size_t r = 0; r < data.size(); r+=2) {
                complex<double> tmpodd(data.at(r+1).real(),-1.0 * data.at(r+1).imag()),
                        tmpeve(data.at(r).real(),-1.0 * data.at(r).imag());
                oddpart.push_back(tmpodd);
                evenpart.push_back(tmpeve);
            }
            X1 = fft(evenpart);
            X2 = fft(oddpart);


            for (size_t r = 0; r < N/2; ++r) {
                complex<double> wnk = complexExpPow(N,r);
                complex<double> n(1.0/N,0.0);
//                cout<<"r:"<<r<<" wnk:"<<wnk<<endl;
                ret.push_back((X1.at(r)*n + X2.at(r)*wnk*n));
            }
            for (size_t r = N / 2; r < N; ++r) {
                size_t ri = r - N / 2;
                complex<double> n(1.0/N,0.0);
                complex<double> wnk = complexExpPow(N, ri);
//                cout<<"r:"<<r<<" wnk:"<<wnk<<endl;
                ret.push_back((X1.at(ri)*n - X2.at(ri)*wnk*n));
            }
        return ret;
    }
}

vector<complex<double> > ifft(vector<int> data, size_t N)
{
    vector<complex<double>> temp,ret;
    for (int i = 0; i < data.size(); i++) {
        std::complex<double> tmp((double)data.at(i), 0.0);
        temp.push_back(tmp);
    }
    ret = ifft(temp,N);
    return ret;
}

vector<complex<double> > ifft(vector<double> data, size_t N)
{
    vector<complex<double>> temp,ret;
    for (int i = 0; i < data.size(); i++) {
        std::complex<double> tmp((double)data.at(i), 0.0);
        temp.push_back(tmp);
    }
    ret = ifft(temp,N);
    return ret;
}

Matrix<complex<double> > ifft2d(const Matrix<int> &mat, size_t M, size_t N)
{
    complex<double> init(0.0,0.0);
    Matrix<complex<double> > temp(mat.getNRow(),mat.getNCol(),init);
    for (size_t i = 0; i < mat.getNRow(); i++) {
        for(size_t j = 0 ; j < mat.getNCol(); j++){
            try{
                std::complex<double> tmp(mat(i,j), 0.0);
                temp(i,j) = tmp;
            }catch(range_error e){
                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
    Matrix<complex<double> > ret = ifft2d(temp, M, N);
    return ret;
}

Matrix<complex<double> > ifft2d(const Matrix<double> &mat, size_t M, size_t N)
{
    complex<double> init(0.0,0.0);
    Matrix<complex<double> > temp(mat.getNRow(),mat.getNCol(),init);
    for (size_t i = 0; i < mat.getNRow(); i++) {
        for(size_t j = 0 ; j < mat.getNCol(); j++){
            try{
                std::complex<double> tmp(mat(i,j), 0.0);
                temp(i,j) = tmp;
            }catch(range_error e){
                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
    Matrix<complex<double> > ret = ifft2d(temp, M, N);
    return ret;
}

Matrix<complex<double> > ifft2d(const Matrix<complex<double> > &mat, size_t M, size_t N)
{
    if(M == 0)
        M = calcN(mat.getNRow());
    if(N == 0)
        N = calcN(mat.getNCol());
    if(((M & (M-1)) != 0) | (N & (N-1)) != 0)
    {
        cerr<<"M or N is not 2^n!"<<endl;
        return mat;
    }

    Matrix<complex<double> > tmp(M,N,complex<double>(0.0,0.0));
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            try{
                tmp(i,j) = mat(i,j);
            }
            catch(range_error e){
//                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
    return ifftRow(ifftRow(tmp).transpose()).transpose();
}
