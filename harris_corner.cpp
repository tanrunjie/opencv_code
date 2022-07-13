#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

#define WIN1 "win1"
#define WIN2 "win2"


Mat src, mid, gray;
int thresh = 30;
int max_thresh = 175;

void on_Harris(int, void *)
{
    Mat dst, norm, scaled;

    dst = Mat::zeros(src.size(), CV_32FC1);
    mid = src.clone();

    cornerHarris(gray, dst, 2, 3, 0.04, BORDER_DEFAULT);

    normalize(dst, norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(norm, scaled);

    for(int j=0; j<norm.rows; j++)
    {
        for(int i=0; i<norm.cols; i++)
        {
            if((int)norm.at<float>(j,i) > thresh + 80)
            {
                circle(mid, Point(i,j), 5, Scalar(10, 10, 255), 2, 8, 0);
                circle(scaled, Point(i,j), 5, Scalar(0, 10, 255), 2, 8 ,0);
            }
        }
    }
    imshow(WIN1, mid);
    imshow(WIN2, scaled);
}

int main()
{
    src = imread("2.png", 1);

    mid = src.clone();

    cvtColor(mid, gray, COLOR_BGR2GRAY);

    namedWindow(WIN1, WINDOW_AUTOSIZE);
    createTrackbar("thresh: " , WIN1, &thresh, max_thresh, on_Harris);
    on_Harris(0,0);
    waitKey();

    return 0;
}