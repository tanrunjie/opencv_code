#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

int g_nContrastValue;
int g_nBrightValue;
Mat g_srcImage, g_dstImage;
static void ContrastAndBright(int, void *)
{
    namedWindow("origin", 1);

    for(int y=0; y<g_srcImage.rows; y++)
    {
        for(int x=0; x<g_srcImage.cols; x++)
        {
            for(int c=0; c<3; c++)
            {
                g_dstImage.at<Vec3b>(y,x)[c] = static_cast<uchar>(
                    (g_nContrastValue*0.01)*(g_srcImage.at<Vec3b>(y,x)[c]) +
                    g_nBrightValue
                );
            }
        }
    }
    imshow("origin", g_srcImage);
    imshow("effect", g_dstImage);
}

Mat img;
int threshval = 160;

static void on_trackbar(int, void *)
{
    Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);
    // namedWindow("bw");
    // imshow("bw", bw);
    // waitKey();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(bw, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

    Mat dst = Mat::zeros(img.size(), CV_8UC3);

    if (!contours.empty() && !hierarchy.empty())
    {

        for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
        {
            Scalar color((rand() & 255), (rand() & 255), (rand() & 255));
            drawContours(dst, contours, idx, color, FILLED, 8, hierarchy);
        }
    }
    imshow("components", dst);
}

bool MultiChannelBlending()
{
    Mat src = imread("1.jpeg");
    Mat logo = imread("2.png", 0); //gray
    vector<Mat> channels;
    split(src, channels);

    Mat blue = channels.at(0); // 0:blue, 1:green, 2:red
    addWeighted(blue(Rect(500, 250, logo.cols, logo.rows)), 1.0,
                logo, 0.5, 0, blue(Rect(500, 250, logo.cols, logo.rows)));

    merge(channels, src);
    namedWindow("blend in blue");
    imshow("blend in blue", src);
    waitKey();

    return true;
}
int main()
{
    const int64 start = getTickCount();
    // my code
    g_srcImage = imread("1.jpeg");
    g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

    g_nContrastValue = 80;
    g_nBrightValue = 80;

    namedWindow("effect", 1);

    createTrackbar("contrass:", "effect", &g_nContrastValue, 300, ContrastAndBright);
    createTrackbar("brightness:", "effect", &g_nBrightValue, 200, ContrastAndBright);

    ContrastAndBright(g_nContrastValue, 0);
    ContrastAndBright(g_nBrightValue, 0);
    while (char(waitKey(1)) != 'q')
    {
    }

    // MultiChannelBlending();

    // example: trackbar
    /***  
    img = imread("1.jpeg",0);

    namedWindow("components", 1);
    imshow("components", img);

    createTrackbar("Thres", "components", &threshval, 255, on_trackbar);
    on_trackbar(threshval, 0);
    
    ***/

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    return 0;
}