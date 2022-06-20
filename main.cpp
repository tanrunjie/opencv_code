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
    Mat result;
    const int64 start = getTickCount();
    // my code

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    imwrite("2.jpeg", result);
    
    return 0;
}