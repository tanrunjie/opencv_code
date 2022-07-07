#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;


int main()
{
    Mat image = imread("1.jpeg");
    Mat logo = imread("2.png");

    // my code
    const int64 start = getTickCount();
    
    // ROI coverage
    Mat imgROI = image(Rect(200,250, logo.cols, logo.rows));
    Mat mask = imread("2.png", 0);
    logo.copyTo(imgROI, mask);
    Mat result;

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    namedWindow("img");
    imshow("img", image);
    imwrite("2.jpeg", mask);
    waitKey();
    return 0;
}