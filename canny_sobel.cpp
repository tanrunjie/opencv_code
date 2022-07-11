#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat src, gray, dst;

// canny variant
Mat gCannyDtectedEdges;
int gCannyLowThreshold = 1;

// sobel variant
Mat gSobelGradientX, gSobelGradientY;
Mat gSobelAbsGradientX, gSobelAbsGradientY;
int gSobelKernelSize = 1;

static void on_canny(int, void *)
{
    blur(gray, gCannyDtectedEdges, Size(3, 3));
    Canny(gCannyDtectedEdges, gCannyDtectedEdges, gCannyLowThreshold, gCannyLowThreshold * 3);
    dst = Scalar::all(0);
    src.copyTo(dst, gCannyDtectedEdges);
    imshow("canny", dst);
}

static void on_sobel(int, void *)
{
    Sobel(src, gSobelGradientX, CV_16S, 1, 0, (2 * gSobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
    convertScaleAbs(gSobelGradientX, gSobelAbsGradientX);

    Sobel(src, gSobelGradientY, CV_16S, 0, 1, (2 * gSobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
    convertScaleAbs(gSobelGradientY, gSobelAbsGradientY);

    addWeighted(gSobelAbsGradientX, 0.5, gSobelAbsGradientY, 0.5, 0, dst);

    imshow("sobel", dst);
}

int main()
{
    src = imread("1.jpeg");
    const int64 start = getTickCount();
    // my code
    // method 1: canny
    // Mat gray, edge, dst;
    // dst.create(src.size(), src.type());
    // dst = Scalar::all(0);
    // cvtColor(src, gray, COLOR_BGR2GRAY);
    // blur(gray, edge, Size(3,3));
    // Canny(edge, edge, 3, 9, 3);
    // src.copyTo(dst, edge);

    // method 2: Sobel
    // Mat grad_x, grad_y;
    // Mat abs_grad_x, abs_grad_y, dst;

    // Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
    // convertScaleAbs(grad_x, abs_grad_x);
    // imshow("x sobel", abs_grad_x);

    // Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    // convertScaleAbs(grad_y, abs_grad_y);
    // imshow("y sobel", abs_grad_y);

    // addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    // imshow("sobel", dst);

    // method 3: Laplacian
    // Mat gray, dst;
    // GaussianBlur(src, src, Size(3,3), 0, 0, BORDER_DEFAULT);
    // cvtColor(src, gray, COLOR_RGB2GRAY);
    // Laplacian(gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    // convertScaleAbs(dst, dst);
    // imshow("laplacian", dst);

    // method 4: trackBar with canny
    imshow("ori", src);

    dst.create(src.size(), src.type());
    cvtColor(src, gray, COLOR_BGR2GRAY);

    namedWindow("canny", WINDOW_AUTOSIZE);
    namedWindow("sobel", WINDOW_AUTOSIZE);

    createTrackbar("value:", "canny", &gCannyLowThreshold, 120, on_canny);
    createTrackbar("value:", "sobel", &gSobelKernelSize, 3, on_sobel);

    on_canny(0, 0);
    on_sobel(0, 0);

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    while ((char(waitKey(1)) != 'q'))
    {
    }
    return 0;
}