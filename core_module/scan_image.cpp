#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <random>

using namespace std;
using namespace cv;

Mat& ScanImageAndReduceC(Mat& I, const uchar* table)
{
    CV_Assert(I.depth() == CV_8U);
    
    int channels = I.channels();
    int nRows = I.rows;
    int nCols = I.cols * channels;

    if(I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    uchar* p;
    for(i=0; i<nRows; i++)
    {
        p = I.ptr<uchar>(i);
        for(j=0; j<nCols; j++)
        {
            p[j] = table[p[j]];
        }
    }
    return I;
}

Mat& ScanImageAndReduceIterator(Mat &I, const uchar* const table)
{
    CV_Assert(I.depth() == CV_8U);
    const int channels = I.channels();
    switch (channels)
    {
    case 1:
        /* code */
        {
            MatIterator_<uchar> it, end;
            for(it=I.begin<uchar>(), end=I.end<uchar>(); it!=end; ++it)
                *it = table[*it];
            break;
        }
    case 3:
        {
            MatIterator_<Vec3b> it, end;
            for(it=I.begin<Vec3b>(), end=I.end<Vec3b>(); it!=end; ++it)
            {
                (*it)[0] = table[(*it)[0]];
                (*it)[1] = table[(*it)[1]];
                (*it)[2] = table[(*it)[2]];
            }
        }
    default:
        break;
    }
    return I;
}
Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table)
{
    CV_Assert(I.depth() == CV_8U);

    const int channels = I.channels();
    switch (channels)
    {
    case 1:
        /* code */
        {
            for(int i=0; i<I.rows; i++)
                for(int j=0; j<I.cols; j++)
                    I.at<uchar>(i,j) = table[I.at<uchar>(i,j)];
            break;
        }
    case 3:
        {
            Mat_<Vec3b> _I = I;
            for(int i=0; i<I.rows; i++)
                for(int j=0; j<I.cols; j++)
                {
                    _I(i,j)[0] = table[_I(i,j)[0]];
                    _I(i,j)[1] = table[_I(i,j)[1]];
                    _I(i,j)[2] = table[_I(i,j)[2]];
                }
            I = _I;
            break;
        }
    default:
        break;
    }
    return I;
}

int main()
{
    Mat I = imread("1.jpeg",IMREAD_COLOR);
    if(!I.data)
    {
        printf("No image data\n");
        return -1;
    }
    double start = (double)getTickCount();
    // my code
    int divideWith = 30;
    uchar table[256];
    for(int i=0; i<256; i++)
        table[i] = (uchar)(divideWith * (i/divideWith));

    const int times = 100;
    Mat J;
    for(int i=0; i<times; ++i)
    {
        Mat clone_i = I.clone();
        J = ScanImageAndReduceC(clone_i, table);
    }
    double duration = (getTickCount() - start) / getTickFrequency();
    cout <<"C pointer:" << duration << endl;

    start = (double)getTickCount();
    for(int i=0; i<times; ++i)
    {
        Mat clone_i = I.clone();
        J = ScanImageAndReduceIterator(clone_i, table);
    }
    duration = (getTickCount() - start) / getTickFrequency();
    cout << "Iterator:" <<duration << endl;


    start = (double)getTickCount();
    for(int i=0; i<times; ++i)
    {
        Mat clone_i = I.clone();
        J = ScanImageAndReduceRandomAccess(clone_i, table);
    }
    duration = (getTickCount() - start) / getTickFrequency();
    cout << "RA:" << duration << endl;


    start = (double)getTickCount();
    Mat lookUpTable(1,256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for(int i=0; i<256; i++)
        p[i] = table[i];
    for(int i=0; i<times;i++)
        LUT(I, lookUpTable, J);
    duration = (getTickCount() - start) / getTickFrequency();
    cout << "LUT init:" << duration << endl;

    imwrite("2.jpeg", J);
    
    return 0;
}