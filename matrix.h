#ifndef MATRIX__H
#define MATRIX__H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <QImage>
#include <complex>
using std::range_error;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;
using std::cerr;
using std::complex;


template<typename T>
class Matrix
{
private:
	// point to the data
    T** _data;
	// number of row and column
    size_t _nRow, _nCol;
    size_t _startR,_startC;
	// flag to indicate row-first or column-first
    int _t;
    bool isOutOfRange(size_t r, size_t c) const;
    void swap(Matrix<T>& mat);

public:
	// construct a r-row and c-column matrix with all init element
    Matrix(size_t r, size_t c, T init);
	// copy constructor
	Matrix(const Matrix<T>& mat);
	Matrix& operator=(Matrix<T> mat);
	~Matrix();
	// get element at r-row and c-column
	// return reference to make it modifiable
    T& operator()(size_t r, size_t c) const;



    Matrix<T> subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const;
    // get a submatrix [rs:re,cs:ce]
	// output the matrix


	size_t getNRow() const;
	size_t getNCol() const;
	std::vector<T> getRow(size_t r) const;
	std::vector<T> getCol(size_t c) const;
	bool setRow(size_t r, const std::vector<T>& d);
	bool setCol(size_t c, const std::vector<T>& d);
	// transpose it in-place
    Matrix<T>& transpose();
    static Matrix<int> fromQImage(const QImage& img,char s);
    static QImage toQImage(const Matrix<int>& mat);
    static QImage toQImage(const Matrix<int>& matR, const Matrix<int>&matG, const Matrix<int>&matB);


    static Matrix<double> complexabs(const Matrix<complex<double>> mat);
    static Matrix<int> normalization(Matrix<double> mat);
    static Matrix<T> fftshift(Matrix<T> mat);
    static Matrix<double> complexlog(Matrix<double> mat);




    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs);
    template<typename A,typename B>
    friend Matrix<B> operator +(const A &addnum, Matrix<B> &mat);
    template<typename A,typename B>
    friend Matrix<B> operator+(Matrix<B> &mat,const A &addnum);
    template<typename A,typename B>
    friend Matrix<B> operator+(Matrix<B> &mat1, Matrix<A> &mat2);

    template<typename A,typename B>
    friend Matrix<B> operator-(const A &minusnum, Matrix<B> &mat);
    template<typename A,typename B>
    friend Matrix<B> operator-(Matrix<B> &mat,const A &minusnum);
    template<typename A,typename B>
    friend Matrix<B> operator-(Matrix<B> &mat1, Matrix<A> &mat2);

    template<typename A,typename B>
    friend Matrix<B> operator*(const A &mulnum, Matrix<B>& mat);
    template<typename A,typename B>
    friend Matrix<B> operator*(Matrix<B> &mat, const A &mulnum);
    template<typename T>
    friend Matrix<T> operator*(Matrix<T> &mat1, Matrix<T> &mat2);

    template<typename A,typename B>
    friend Matrix<B> operator/(Matrix<B> &mat,const A &divnum);

};

template <typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init):
_nRow(r),_nCol(c),_t(0),_startC(0),_startR(0)
{
    _data = new T *[r];
    for(int i=0;i<r;i++){
        _data[i] = new T[c];
    }

    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            _data[i][j] = init;
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& mat):
_nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t),_startC(mat._startC),_startR(mat._startR)
{
    size_t r = mat._nRow;
    size_t c = mat._nCol;
    _data = new T *[r];
    for(int i=0;i<r;i++){
        _data[i] = new T[c];
    }

    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            _data[i][j] = mat._data[i][j];
        }
    }
}

template <typename T>
void Matrix<T>::swap(Matrix<T>& mat) {
    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_t, mat._t);
    std::swap(_startR,mat._startR);
    std::swap(_startC,mat._startC);
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    swap(mat);
    return *this;
}




template <typename T>
size_t Matrix<T>::getNRow() const
{
    // return the number of row
    return _nRow;
}

template <typename T>
size_t Matrix<T>::getNCol() const
{
    // return the number of column
    return _nCol;
}

template <typename T>
bool Matrix<T>::isOutOfRange(size_t r, size_t c) const
{
//    bool flag = true;
    if(_t == 0){

        if(r>getNRow() || r<0 || c>getNCol() || c<0)
        {
            return true;
        }
        else
            return false;
    }
    else if(_t == 1){
        if(r>getNCol() || r<0 || c>getNRow() || c<0)
        {
            return true;
        }
        else
            return false;
    }
    // check if r-row and c-column is out of range
        return true;

}


template <typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    // get element in (r,c)
    // remember check first
    // different of _t has different order
//    qDebug()<<"_t:"<<_t;
    if(!isOutOfRange(r,c)){

        if(_t == 0)
            return _data[(r+_startR)%_nRow][(c+_startC)%_nCol];
        else if(_t == 1)
            return _data[(c+_startC)%_nCol][(r+_startR)%_nRow];
    }

    else{
//        cerr<<"matrix index out of range!\n";
        throw(range_error("matrix index out of range!\n"));
    }


}



template <typename T>
Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
{
    // note the validity of value:rs, re, cs, ce
    // try to use "except" to broadcast the exception
    size_t r,c;
    r = re-rs+1;
    c = ce-cs+1;
    size_t x,y,j,i;
    x=0;y=0;i=rs;j=cs;
    Matrix<T> ret(r,c,0);
    for(x=0;x<r;x++){
        for(y=0;y<c;y++){
            ret(x,y) = this->operator ()(x,y);
            j++;
//            cout<<i<<" "<<j<<endl;
        }
        i++;
        j=cs;
    }
    return ret;
}


template <typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    // get all element in r-row and push it back in a vector
    // remember check range first
    vector<T> row;
    if(!isOutOfRange(r,0)){
        for(int i=0; i<_nCol;i++){
                row.push_back(this->operator ()(r,i));
        }
    }


    return row;
}


template <typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    // get all element in c-column and push it back in a vector
    // remember check range first
    vector<T> col;
    if(!isOutOfRange(0,c)){
        for(int i=0; i<_nRow;i++){
            col.push_back(this->operator ()(i,c));
        }
    }

    return col;
}

template <typename T>
bool Matrix<T>::setRow(size_t r, const vector<T>& d)
{
    // set the element of the r-row
    // remember check range first

    for(int i=0; i<getNCol(); i++){
            if(!isOutOfRange(r,i))
                this->operator ()(r,i) = d.at(i);
            else{
                cerr<<"setting error!"<<endl;
                return false;
            }
    }
    return true;
}

template <typename T>
bool Matrix<T>::setCol(size_t c, const vector<T>& d)
{
    // set the element of the c-column
    // remember check range first

    for(int i=0; i<getNRow(); i++){
            if(!isOutOfRange(i,c))
                this->operator ()(i,c) = d.at(i);
            else{
                cerr<<"setting error!"<<endl;
                return false;
            }
    }
    return true;
}

template <typename T>
Matrix<T>::~Matrix()
{
    // delete _data[i] first
    // then delete _data
    // note the value of _t
    for(int i=0;i<this->getNRow();i++)
        delete []_data[i];
    _data = NULL;
}

template <typename T>
Matrix<T>& Matrix<T>::transpose()
{
    // change _t
    // swap _nRow and _nCol

    if (_t == 0)
        _t = 1;
    else
        _t = 0;
    size_t temp = _nRow;
    _nRow = _nCol;
    _nCol = temp;
    return *this;

}




template<typename A,typename B>
Matrix<B> operator +(Matrix<B> &mat,const A &addnum)
{
    Matrix<B>ret(mat);
    for(int i=0;i<mat._nRow;i++){
        for(int j=0;j<mat._nCol;j++){
            ret(i,j) = mat(i,j) + addnum;
        }
    }
    return ret;
}


template<typename A,typename B>
Matrix<B> operator+ (const A &addnum, Matrix<B> &mat)
{
    Matrix<B>ret(mat);
    for(int i=0;i<mat._nRow;i++){
        for(int j=0;j<mat._nCol;j++){
            ret(i,j) = mat(i,j) + addnum;
        }
    }
    return ret;
}
template<typename A,typename B>
Matrix<B> operator+ (Matrix<B> &mat1, Matrix<A> &mat2)
{
    Matrix<B>ret(mat1);
    size_t r1 = mat1._nRow; size_t r2 = mat2.getNRow();
    size_t c1 = mat1.getNCol(); size_t c2 = mat2.getNCol();
    if(r1 == r2 && c1 == c2){
        for(int i=0;i<r1;i++){
            for(int j=0;j<c1;j++){
                ret(i,j) = mat1(i,j) + mat2(i,j);
            }
        }
        return ret;
    }
    else{
        cerr<<"matrixs' shape is not the same!";
        return mat1;
    }
}

template<typename A,typename B>
Matrix<B> operator-(const A &minusnum,Matrix<B> &mat)
{
    Matrix<B>ret(mat);
    for(int i=0;i<mat._nRow;i++){
        for(int j=0;j<mat._nCol;j++){
            ret(i,j) = minusnum - mat(i,j) ;
        }
    }
    return ret;
}

template<typename A,typename B>
Matrix<B> operator-(Matrix<B> &mat,const A &minusnum)
{
    Matrix<B>ret(mat);
    for(int i=0;i<mat._nRow;i++){
        for(int j=0;j<mat._nCol;j++){
            ret(i,j) = mat(i,j) - minusnum;
        }
    }
    return ret;
}

template<typename A,typename B>
Matrix<B> operator-(Matrix<B> &mat1,Matrix<A> &mat2)
{
    Matrix<B>ret(mat1);
    size_t r1 = mat1.getNRow(); size_t r2 = mat2.getNRow();
    size_t c1 = mat1.getNCol(); size_t c2 = mat2.getNCol();
    if(r1 == r2 && c1 == c2){
        for(int i=0;i<r1;i++){
            for(int j=0;j<c1;j++){
                ret(i,j) = mat1(i,j) - mat2(i,j);
            }
        }
        return ret;
    }
    else{
        cerr<<"matrixs' shape is not the same!";
        return mat1;
    }
}

template<typename A,typename B>
Matrix<B> operator*(const A &mulnum,Matrix<B> &mat)
{
    Matrix<B>ret(mat);
    for(int i=0;i<mat._nRow;i++){
        for(int j=0;j<mat._nCol;j++){
            ret(i,j) = mat(i,j) * mulnum;
        }
    }
    return ret;
}

template<typename T>
Matrix<T> operator*(Matrix<T> &mat1, Matrix<T> &mat2)
{
    Matrix<T> temp(mat1._nRow, mat2._nCol, 0);
    for (size_t i = 0;i < mat1._nRow;i++)
    {
        for (size_t j = 0;j < mat2._nCol;j++)
        {
            std::vector<T> a = mat1.getRow(i);
            std::vector<T> b = mat2.getCol(j);
            T sum = 0;
            for (int k = 0;k < a.size();k++)
            {
                sum += a[k] * b[k];
            }
            temp(i, j) = sum;
        }
    }
    return temp;
}

template<typename A,typename B>
Matrix<B> operator*(Matrix<B> &mat1, Matrix<A> &mat2)
{
    size_t r1 = mat1.getNRow(); size_t r2 = mat2.getNRow();
    size_t c1 = mat1.getNCol(); size_t c2 = mat2.getNCol();
    if(c1 == r2){
        Matrix<B> tmp(r1,c2,0.0);
        vector<B> colvec,rowvec;
        for(int x=0;x<r1;x++){
            for(int y=0;y<c2;y++){
               rowvec = mat1.getRow(x);
               colvec = mat2.getCol(y);
               for(int i=0;i<rowvec.size();i++){
                   tmp(x,y) += rowvec.at(i) * colvec.at(i);
               }
            }
        }

        return tmp;
    }
    else{
        cerr<<"matrixs' shape is not the same!";
        return mat1;
    }
}

template<typename A,typename B>
Matrix<B> operator/(Matrix<B> &mat,const A& divnum)
{
    Matrix<B>ret(mat);
    for(int i=0;i<mat._nRow;i++){
        for(int j=0;j<mat._nCol;j++){
            ret(i,j) = mat(i,j) / divnum;
        }
    }
    return ret;
}



template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& rhs)
{
    for(size_t i=0; i<rhs._nRow; ++i) {
        for(size_t j=0; j<rhs._nCol; ++j) {
            out << rhs(i,j) << "\t";
        }
        out << endl;
    }
    return out;
}

template<typename T>
Matrix<int> Matrix<T>::fromQImage(const QImage &img, char s){
    //width -- col  height -- row
    Matrix<int> ret(img.height(),img.width(),0);
    size_t row = ret.getNRow();
    size_t col = ret.getNCol();
    cout<<row<<" col:"<<col<<endl;
    switch (s) {
    case 'r':
        for(int i=0;i<col;++i){
            for(int j=0;j<row;++j){
                ret(j,i) = qRed(img.pixel(i,j));
            }
        }
        return ret;
        break;
    case 'g':
        for(int i=0;i<col;++i){
            for(int j=0;j<row;++j){
               ret(j,i) = qGreen(img.pixel(i,j));
            }
        }
        return ret;
        break;
    case 'b':
        for(int i=0;i<col;++i){
            for(int j=0;j<row;++j){
                ret(j,i) = qBlue(img.pixel(i,j));
            }
        }
        return ret;
        break;
    case 'G':
        for(int i=0;i<col;++i){
            for(int j=0;j<row;++j){
                //0.2989R+ 0.5870G + 0.1140B
                ret(j,i) = 0.2989 * qRed(img.pixel(i,j))
                         + 0.587 * qGreen(img.pixel(i,j))
                         + 0.114 * qBlue(img.pixel(i,j));
            }
        }
        return ret;
        break;
    default:
        return ret;
        break;
    }
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int> &mat)
{
    size_t col = mat.getNCol();
    size_t row = mat.getNRow();
    cout<<col<<" "<<row<<endl;
    QImage ret(col,row,QImage::Format_RGB32);
    for(int i=0;i<ret.width();++i){
        for(int j=0;j<ret.height();++j){
            ret.setPixel(i,j,qRgb(mat(j,i),mat(j,i),mat(j,i)));
        }
    }
    return ret;
}


template<typename T>
Matrix<double> Matrix<T>::complexabs(const Matrix<complex<double>> mat)
{
    size_t r=mat.getNRow(),c=mat.getNCol();
    Matrix<double> ret(r,c,0.0);
    for(size_t i=0;i<r;++i){
        for(size_t j=0;j<c;++j){
            complex<double> tmp = mat(i,j);
            ret(i,j) = std::sqrt(tmp.imag()*tmp.imag() + tmp.real()*tmp.real());
        }
    }
    return ret;
}


template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int> &matR, const Matrix<int> &matG, const Matrix<int> &matB)
{
    size_t col = matR.getNCol();
    size_t row = matR.getNRow();
    QImage ret(row,col,QImage::Format_RGB32);
    for(int i=0;i<ret.width();++i){
        for(int j=0;j<ret.height();++j){
            ret.setPixel(i,j,qRgb(matR(i,j),matG(i,j),matB(i,j)));
        }
    }
    return ret;


}

template<typename T>
Matrix<int> Matrix<T>::normalization(Matrix<double> mat)
{
    vector<complex<double>> tmp;
    size_t r=mat.getNRow(),c=mat.getNCol();
    double min=mat(0,0),max=mat(0,0);
    for(size_t i=0;i<r;++i){
        for(size_t j=0;j<c;++j){
            if(mat(i,j)<min){
                min = mat(i,j);
            }
            else if(mat(i,j)>max){
                max = mat(i,j);
            }
        }
    }

    for(size_t i=0;i<r;++i){
        for(size_t j=0;j<c;++j){
            mat(i,j) = (mat(i,j) - min) / (max - min);
        }
    }
    Matrix<int> ret(r,c,0);
    for(size_t i=0;i<r;++i){
        for(size_t j=0;j<c;++j){
            ret(i,j) = mat(i,j)*255;
        }
    }

    return ret;


}

template<typename T>
Matrix<T> Matrix<T>::fftshift(Matrix<T> mat)
{
    Matrix<T> ret(mat);
    ret._startR = mat._nRow/2;
    ret._startC = mat._nCol/2;

    return ret;

}

template<typename T>
Matrix<double> Matrix<T>::complexlog(Matrix<double> mat){
    Matrix<double> ret(mat);
    for(int i=0;i<mat.getNRow();++i){
        for(int j=0;j<mat.getNCol();++j){
            ret(i,j) = 1.0 + log(mat(i,j));
        }
    }
    return ret;

}

#endif
