#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

#define WIN1 "ori"
#define WIN2 "wrap"
#define WIN3 "wrap and rotate"

int main()
{
    Point2f srcTri[3];
    Point2f dstTri[3];

    Mat rotMat(2, 3, CV_32FC1);
    Mat warpMat(2, 3, CV_32FC1);
    Mat src, dst, dst2;

    src = imread("1.jpeg" , 1);
    imshow(WIN1, src);
    dst = Mat::zeros(src.size(), src.type());

    // calc affine
    srcTri[0] = Point2f(0,0);
    srcTri[1] = Point2f(static_cast<float>(src.cols-1),0);
    srcTri[2] = Point2f(0, static_cast<float>(src.rows-1));

    dstTri[0] = Point2f(static_cast<float>(src.cols*0.0), static_cast<float>(src.rows*0.3));
    dstTri[1] = Point2f(static_cast<float>(src.cols*0.65), static_cast<float>(src.rows*0.35));
    dstTri[2] = Point2f(static_cast<float>(src.cols*0.15), static_cast<float>(src.rows*0.65));

    warpMat = getAffineTransform(srcTri, dstTri);

    warpAffine(src, dst, warpMat, dst.size());
    imshow(WIN2, dst);

    Point center = Point(dst.cols/2, dst.rows/2);
    double angle = -30.0;
    double scale = 0.8;

    rotMat = getRotationMatrix2D(center, angle, scale);

    warpAffine(dst, dst2, rotMat, dst2.size());

    imshow(WIN3, dst2);
    waitKey();

    return 0;
}