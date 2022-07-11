#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;
Mat src, dst1, dst2;
int medianValue = 10;
int bilateralValue = 10;

static void on_MedianBlur(int, void *)
{
    medianBlur(src, dst1, medianValue * 2 + 1);
    imshow("median", dst1);
}

static void on_BilateralFilter(int, void *)
{
    bilateralFilter(src, dst2, bilateralValue, bilateralValue * 2, bilateralValue / 2);
    imshow("bilateral", dst2);

}

int main()
{
    src = imread("1.jpeg");
    dst1 = src.clone();
    dst2 = src.clone();

    const int64 start = getTickCount();
    // my code
    imshow("ori", src);
    namedWindow("median", 1);
    createTrackbar("value:", "median", &medianValue, 50, on_MedianBlur);
    on_MedianBlur(medianValue, 0);

    namedWindow("bilateral", 1);
    createTrackbar("value:", "bilateral", &bilateralValue, 50, on_BilateralFilter);
    on_MedianBlur(bilateralValue, 0);

    // imshow("blur", dst);
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;

    waitKey();
    return 0;
}