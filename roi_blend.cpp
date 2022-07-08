#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

bool LinearBlending()
{
    double alpha = 0.5;
    double beta = 1.0 - alpha;
    Mat a = imread("2.png", 1);
    Mat b = imread("roi.png", 1); // same size with a
    cout << a.size() << endl;
    cout << b.size() << endl;

    cout << a.channels() << endl;
    cout << b.channels() << endl;

    Mat dst;
    addWeighted(a, alpha, b, beta, 0.0, dst);
    namedWindow("blend");
    imshow("blend", dst);
    waitKey();

    return true;
}

bool ROI_Add()
{
    Mat image = imread("1.jpeg");
    Mat logo = imread("2.png");
    Mat imgROI = image(Rect(200, 250, logo.cols, logo.rows));
    Mat mask = imread("2.png", 0);
    logo.copyTo(imgROI, mask);
    namedWindow("ROI_ADD");
    imshow("ROI_ADD", image);
    waitKey();

    return true;
}

bool ROI_Blending()
{
    Mat image = imread("1.jpeg");
    Mat logo = imread("2.png");
    Mat roi = image(Rect(200,250, logo.cols, logo.rows));

    addWeighted(roi, 0.5, logo, 0.3, 0.0, roi);

    namedWindow("ROI_BLEND");
    imshow("ROI_BLEND", image);
    waitKey();
    return true;
}
int main()
{
    // my code
    const int64 start = getTickCount();
    LinearBlending();
    ROI_Add();
    ROI_Blending();
    // ROI coverage
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;

    return 0;
}