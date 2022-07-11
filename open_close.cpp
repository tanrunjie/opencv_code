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
    Mat dst;
    Mat element = getStructuringElement(MORPH_RECT, Size(15,15));
    const int64 start = getTickCount();
    // my code
    // DIALATE/ERODE/OPEN/CLOSE/TOPHAT/HIT MISS
    morphologyEx(src, dst, MORPH_BLACKHAT, element);
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    
    imshow("dst", dst);
    waitKey();
    return 0;
}