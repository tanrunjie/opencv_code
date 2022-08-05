#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

int main()
{

    const int64 start = getTickCount();
    // my code
    Mat A, C;
    A = imread("1.jpeg", IMREAD_COLOR);
    Mat B(A); // copy  shadow copy
    C = A;    // Assign
    Mat D(A, Rect(10, 10, 100, 100));
    Mat F = A.clone(); // deep copy
    Mat G;
    A.copyTo(G); // deep copy
    // cout << G << endl;
    // color space: RGB, HSV/HLS, YCrCB/YUV , CIE LAB

    // CV_[bits num][Singed or Unsigned][Type]C[channel num]
    Mat M(2, 2, CV_8UC3, Scalar(0, 0, 255));
    cout << M << endl;

    M.create(4, 4, CV_8UC(2));
    cout << M << endl;

    Mat E = Mat::eye(4, 4, CV_64F);
    cout << E << endl;

    Mat O = Mat::ones(2, 2, CV_32F);
    cout << O << endl;

    Mat Z = Mat::zeros(3, 3, CV_8UC1);
    cout << Z << endl;

    Mat H = (Mat_<double>(3, 3) << 0, -1, 0,
             -1, -5, -1, 0, -1, 0);
    cout << H << endl;

    H = (Mat_<double>({0, -1, 0,
                       -1, 5, -1, 0, -1, 0}))
            .reshape(3);
    cout << H << endl;

    Mat rowClone = H.row(1).clone();
    cout << rowClone << endl;

    Mat R = Mat(3, 2, CV_8UC3);
    randu(R, Scalar::all(0), Scalar::all(255));
    cout << R << endl;

    cout << format(R, Formatter::FMT_PYTHON) << endl;

    cout << format(R, Formatter::FMT_CSV) << endl;

    cout << format(R, Formatter::FMT_NUMPY) << endl;

    cout << format(R, Formatter::FMT_C) << endl;

    Point2f P(5, 1);
    cout << P << endl;

    Point3f p3f(2, 6, 7);
    cout << p3f << endl;

    vector<float> v;
    v.push_back((float)CV_PI);
    v.push_back(2);
    v.push_back(3.01f);
    cout << Mat(v) << endl;

    vector<Point2f> vPoints(20);
    for(size_t i = 0; i< vPoints.size(); i++)
        vPoints[i] = Point2f((float)(i*5), (float)(i%7));
    cout << vPoints << endl;

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << "s" <<  endl;

    return 0;
}