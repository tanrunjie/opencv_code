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

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    
    return 0;
}