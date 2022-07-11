#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat src, dst1, dst2, dst3;
int meanValue = 3;
int boxValue = 3;
int gaussianValue = 3;

static void on_MeanBlur(int, void *)
{
    blur(src, dst1, Size(meanValue + 1, meanValue + 1));
    imshow("win1", dst1);
}

static void on_BoxBlur(int, void *)
{
    boxFilter(src, dst2, -1, Size(meanValue + 1, meanValue + 1));
    imshow("win2", dst2);
}

static void on_GaussianBlur(int, void *)
{
    GaussianBlur(src, dst3, Size(gaussianValue * 2 + 1, gaussianValue * 2 + 1), 0, 0);
    imshow("win3", dst3);
}

int main()
{
    src = imread("1.jpeg");

    dst1 = src.clone();
    dst2 = src.clone();
    dst3 = src.clone();

    // mean
    namedWindow("win1", 1);
    createTrackbar("meanValue", "win1", &meanValue, 255, on_MeanBlur, 0);
    on_MeanBlur(meanValue, 0);

    // box
    namedWindow("win2", 1);
    createTrackbar("boxValue", "win2", &boxValue, 255, on_BoxBlur, 0);
    on_MeanBlur(boxValue, 0);

    // gaussian
    namedWindow("win3", 1);
    createTrackbar("gaussianValue", "win3", &gaussianValue, 255, on_GaussianBlur, 0);
    on_MeanBlur(gaussianValue, 0);

    imshow("ori", src);

    while (char(waitKey(1)) != 'q')
    {
    }

    return 0;
}