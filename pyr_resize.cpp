#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

int main()
{
    Mat src = imread("1.jpeg");
    const int64 start = getTickCount();
    // my code

    // method 1: resize
    // Mat dst1, dst2, tmp;
    // tmp = src;
    // resize(tmp, dst1, Size(tmp.cols / 2, tmp.rows / 2), (0, 0), (0, 0), 3);
    // resize(tmp, dst2, Size(tmp.cols * 2, tmp.rows * 2), (0, 0), (0, 0), 3);
    // imshow("dst1", dst1);
    // imshow("dst2", dst2);
    // waitKey();

    // method 2: pyr
    Mat dst;
    // pyrUp(src, dst, Size(src.cols *2, src.rows * 2));
    pyrDown(src, dst, Size(src.cols / 2, src.rows / 2));


    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    
    imshow("dst", dst);
    waitKey();

    return 0;
}