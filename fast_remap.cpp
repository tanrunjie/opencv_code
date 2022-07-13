#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat src;
int thres = 40;

// fast detector
void on_fast(int, void *)
{
    vector<KeyPoint> keypoints;
    Mat dst = src.clone();
    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(thres);
    detector->detect(src, keypoints);
    drawKeypoints(dst, keypoints, dst, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);

    imshow("src", dst);
}

int main()
{
    Mat dst, map_x, map_y;

    src = imread("1.jpeg", 1);
    const int64 start = getTickCount();
    // my code
    dst.create(src.size(), src.type());
    map_x.create(src.size(), CV_32FC1);
    map_y.create(src.size(), CV_32FC1);

    for (int j = 0; j < src.rows; j++)
    {
        for (int i = 0; i < src.cols; i++)
        {
            // method 1 : scale down
            if (i > src.cols * 0.25 && i < src.cols * 0.75 &&
                j > src.rows * 0.25 && j < src.rows * 0.75)
            {
                map_x.at<float>(j, i) = static_cast<float>(2 * (i - src.cols * 0.25) + 0.5);
                map_y.at<float>(j, i) = static_cast<float>(2 * (j - src.rows * 0.25) + 0.5);
            }
            else
            {
                map_x.at<float>(j, i) = 0;
                map_y.at<float>(j, i) = 0;
            }

            // method 2 : horizontal flip
            // map_x.at<float>(j,i) = static_cast<float>(i);
            // map_y.at<float>(j,i) = static_cast<float>(src.rows - j);

            // method 3 : vertical flip
            // map_x.at<float>(j,i) = static_cast<float>(src.cols - i);
            // map_y.at<float>(j,i) = static_cast<float>(j);

            // method 4 : vertical and horizontal flip
            // map_x.at<float>(j,i) = static_cast<float>(src.cols - i);
            // map_y.at<float>(j,i) = static_cast<float>(src.rows - j);
        }
    }

    remap(src, dst, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    imshow("dst", dst);
    waitKey();

    namedWindow("src", WINDOW_AUTOSIZE);
    createTrackbar("thres:", "src", &thres, 255, on_fast);
    on_fast(0, 0);
    waitKey();

    return 0;
}