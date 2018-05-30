#include "mainwindow.h"
#include <QApplication>
#include <matrix.h>
#include <fft.h>
using std::cout;
using std::endl;

void test_fft()
{
    vector<double> a;
    a.push_back(4);
    a.push_back(8);

    a.push_back(10);
    a.push_back(7);
//    a.push_back(2.);
//    a.push_back(4.);
//    a.push_back(3.);
//    a.push_back(4.);
//    a.push_back(2.);
//    a.push_back(4.);
//    a.push_back(3.);
//    a.push_back(4.);
    cout<<"a:_____________________________"<<endl;
    for(size_t i=0; i<a.size(); ++i)
        cout << a.at(i) << "\t";
    cout<<endl;
    vector<complex<double> > r = fft(a);
    cout<<"fft1d:_____________________________"<<endl;
    for(size_t i=0; i<r.size(); ++i)
        cout << r.at(i) << "\t";
    cout << endl;
    cout<<"ifft1d:_____________________________"<<endl;
    vector<complex<double> > k = ifft(r);
    for(size_t i=0; i<k.size(); ++i)
        cout << k.at(i) << "\t";
    cout << endl;
//    //int r = calcN(5);
//    //cout << r << endl;
    Matrix<double> m(4,4,2);
    m(0,1) = 1.4;
    m(1,1) = 3.2;
//    m(1,2) = 6;
//    m(2,2) = 8;
    cout<<"m:_____________________________"<<endl;
    cout<<m<<endl;
    Matrix<complex<double> > ret = fft2d(m);
    cout<<"fft2d:_____________________________"<<endl;
    cout<< ret <<endl;
    Matrix<complex<double> > ret2 = ifft2d(ret);
    cout<<"ifft2d:_____________________________"<<endl;
    cout<< ret2 <<endl;
}




void test_matrix()
{
    cout << "------------------------------------" << endl;
    cout << "test of constructor:" << endl;
    cout << "------------------------------------" << endl;
    Matrix<double> m(3,3,1);
    cout << "*******    m:\n" << m << endl;;
    Matrix<double> mC(m);
    cout << "*******    mC:\n" << mC << endl;
    Matrix<double> mE = m;
    cout << "*******    mE:\n" << mE << endl;
    mC(1,2) = 2;
    cout << "------------------------------------" << endl;
    cout << "*******    mC:\n" << mC << endl;
//    cout << "*******  mC:\n" << mC << endl;
//    cout << "*******  mE:\n" << mE << endl;
//    cout << "------------------------------------" << endl;
//    cout << "test of transpose()" << endl;
//    cout << "------------------------------------" << endl;
//    cout << mE.transpose()<<endl;

////    cout << mE <<endl;
//    cout << "test of get and set" << endl;
//    cout << "------------------------------------" << endl;
//    vector<double> vd = mE.getCol(0);
//    cout << "get the first row of mE:" << endl;
//    for(size_t i = 0; i<vd.size(); ++i)
//        cout << vd.at(i) << "  **  ";
//    cout << endl;
//    cout << "set the last column of mC:" << endl;
//    vd.at(2) = -3.3;
//    mC.setRow(2, vd);
//    cout << "*******  mC:\n" << mC << endl;
//        cout << "------------------------------------" << endl;
//    cout << "test of subMatrix" << endl;
//    cout << "------------------------------------" << endl;
//    Matrix<double> sub = mE.subMatrix(0,1,0,1);
//    cout << "*******  mE.subMatrix(1,3,1,3):\n" << sub << endl;
//        cout << "test of operator+" << endl;
//        cout << "------------------------------------" << endl;
//        cout << "*******  m+mC:\n" << m+mC << endl;
//        cout << "*******  9+mC:\n" << 9+mC<< endl;
//        cout << "------------------------------------" << endl;
//        cout <<"mC:"<<mC<<endl;
//        cout << "test of operator-" << endl;
//        cout << "------------------------------------" << endl;
//        cout << "*******  m-mC:\n" << m-mC << endl;
//        cout << "*******  10-mC:\n" << 10-mC << endl;
//        cout <<"mC:"<<mC<<endl;
//        cout << "------------------------------------" << endl;
        cout << "test of operator*" << endl;
        cout << "------------------------------------" << endl;
        cout << "*******  mE*mC:\n" << mE*mC << endl;
//        cout << "*******  10*mC:\n" << 10*mC << endl;
//        cout <<"mC:"<<mC<<endl;
//        cout << "------------------------------------" << endl;
//        cout << "test of operator/" << endl;
//        cout << "------------------------------------" << endl;
//        cout << "*******  mC/2:\n" << mC/2 << endl;
//        cout <<"mC:"<<mC<<endl;

//    cout << "test of exception" << endl;
//        cout << "------------------------------------" << endl;
//        try {
//            cout << mC(0,2) << endl;
//            cout << mE(5,6) << endl;
//        } catch (range_error e) {
//            cout << e.what() << endl;
//        }
}

int main(int argc, char *argv[])
{
//    test_fft();
//        test_matrix();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
