#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat src, dst;
int gElementSize = 3;
int gTrackbarNum = 0;

void process()
{
    Mat element = getStructuringElement(MORPH_RECT, Size(2*gElementSize+1,2*gElementSize+1));
    if(gTrackbarNum == 0)
        erode(src, dst, element);
    else
        dilate(src, dst, element);
    
    imshow("dst", dst);
}
static void on_TrackbarNumChange(int , void *)
{
    process();
}

static void on_ElementSizeChange(int , void*)
{
    process();
}

int main()
{
    src = imread("1.jpeg");
    imshow("src", src);
    const int64 start = getTickCount();
    // my code

    namedWindow("dst");
    createTrackbar("dialte/erode:", "dst", &gTrackbarNum, 1, on_TrackbarNumChange);
    createTrackbar("kernel:", "dst", &gElementSize,21, on_ElementSizeChange);

    while(char(waitKey(1))!='q'){}

    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
  
    return 0;
}