#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat src, mid, dst;
vector<Vec4i> lines;
int thres = 100;

static void on_houghlines(int, void*)
{
    

    vector<Vec4i> mylines;
    HoughLinesP(mid, mylines, 1, CV_PI/180, thres + 1, 50, 10);

    for(size_t i=0; i<mylines.size(); i++)
    {
        Vec4i l = mylines[i];
        line(dst,  Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23,180,55), 1, LINE_AA );
    }
    imshow("effect", dst);
}

int main()
{
    src = imread("1.jpeg");
    const int64 start = getTickCount();
    // my code

    Canny(src, mid, 50, 200);
    cvtColor(mid, dst, COLOR_GRAY2BGR);

    // method 1: hough line
    // vector<Vec2f> lines;
    // HoughLines(mid, lines, 1, CV_PI/180, 150, 0, 0);
    // for(size_t i=0; i<lines.size(); i++)
    // {
    //     float rho = lines[i][0], theta = lines[i][1];
    //     Point pt1, pt2;
    //     double a = cos(theta), b = sin(theta);
    //     double x0 = a*rho, y0 = b*rho;
    //     pt1.x = cvRound(x0 + 1000*(-b));
    //     pt1.y = cvRound(y0 + 1000*(a));
    //     pt2.x = cvRound(x0 - 1000*(-b));
    //     pt2.y = cvRound(y0 - 1000 * (a));
    //     line(dst, pt1, pt2, Scalar(55,100, 195), 1, LINE_AA);
    // }

    // method 2: hough line progressive
    // vector<Vec4i> lines;
    // HoughLinesP(mid, lines, 1, CV_PI/180, 80, 50, 10);

    // for(size_t i =0; i<lines.size(); i++)
    // {
    //     Vec4i l = lines[i];
    //     line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA);
    // }

    // method 3: hough circles
    // GaussianBlur(mid, mid, Size(9,9), 2, 2);

    // vector<Vec3f> circles;
    // HoughCircles(mid, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);

    // for(size_t i=0 ;i < circles.size(); i++)
    // {
    //     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    //     int radius = cvRound(circles[i][2]);

    //     circle(src, center, 3, Scalar(0,255,0), -1, 8, 0);
    //     circle(src, center, radius, Scalar(155, 50,255), 3, 8, 0);
    // }

    // method 4: trackbar hough
    namedWindow("effect", 1);
    createTrackbar("value:", "effect", &thres, 200, on_houghlines);
    on_houghlines(thres, 0);
    HoughLinesP(mid, lines, 1, CV_PI/180, 80, 50, 10);

    imshow("ori", src);
    imshow("edge", mid);
    // imshow("effect", dst);
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    while(char(waitKey())!='q'){}
    return 0;
}