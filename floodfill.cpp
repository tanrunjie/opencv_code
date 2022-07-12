#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat src, dst, gray, mask;

int fillMode = 1;
int lowDiff = 20, upDiff = 20;
int connectivity = 4;
int isColor = true;
bool useMask = false;
int newMaskVal = 255;

static void onMouse(int event, int x, int y, int, void *)
{
    if (event != EVENT_LBUTTONDOWN)
        return;

    Point seed = Point(x, y);
    int lowD = fillMode == 0 ? 0 : lowDiff;
    int upD = fillMode == 0 ? 0 : upDiff;
    int flags = connectivity + (newMaskVal << 8) +
                (fillMode == 1 ? FLOODFILL_FIXED_RANGE : 0);

    int b = (unsigned)theRNG() & 255;
    int g = (unsigned)theRNG() & 255;
    int r = (unsigned)theRNG() & 255;

    Rect ccomp;
    Scalar newVal = isColor ? Scalar(b, g, r) : Scalar(r * 0.299 + g * 0.587 + b * 0.114);

    Mat tmp_dst = isColor ? dst : gray;
    int area;

    if (useMask)
    {
        threshold(mask, mask, 1, 128, THRESH_BINARY);
        area = floodFill(tmp_dst, mask, seed, newVal, &ccomp,
                         Scalar(lowD, lowD, lowD),
                         Scalar(upD, upD, upD), flags);
        imshow("mask", mask);
    }
    else
    {
        area = floodFill(tmp_dst, seed, newVal, &ccomp,
                         Scalar(lowD, lowD, lowD),
                         Scalar(upD, upD, upD), flags);
    }
    imshow("effect", tmp_dst);
    cout << area << " pixels were redraw.\n";
}

int main()
{
    src = imread("1.jpeg", 1);

    src.copyTo(dst);
    cvtColor(src, gray, COLOR_BGR2GRAY);
    mask.create(src.rows + 2, src.cols + 2, CV_8UC1);

    namedWindow("effect", WINDOW_AUTOSIZE);

    createTrackbar("lowD", "effect", &lowDiff, 255, 0);
    createTrackbar("upD", "effect", &upDiff, 255, 0);

    setMouseCallback("effect", onMouse, 0);

    while (1)
    {
        imshow("effect", isColor ? dst : gray);

        int c = waitKey(0);

        if((c&255)==27) // esc
        {
            cout <<"exit" << endl;
            break;
        }

        switch ((char)c)
        {
        case '1':
            if(isColor)
            {
                cvtColor(src, gray, COLOR_BGR2GRAY);
                mask = Scalar::all(0);
                isColor = false;
            }
            else
            {
                src.copyTo(dst);
                mask = Scalar::all(0);
                isColor = true;
            }
            break;
        case '2':
            if(useMask)
            {
                destroyWindow("mask");
                useMask = false;
            }
            else
            {
                namedWindow("mask", 0);
                mask = Scalar::all(0);
                imshow("mask", mask);
                useMask = true;
            }
            break;
        case '3':
            src.copyTo(dst);
            cvtColor(dst, gray, COLOR_BGR2GRAY);
            mask = Scalar::all(0);
            break;
        case '4':
            fillMode = 0;
            break;
        case '5':
            fillMode = 1;
            break;
        case '6':
            fillMode = 2;
            break;
        case '7':
            connectivity = 4;
            break;
        case '8':
            connectivity = 8;
            break;
        default:
            break;
        }
    }

    // method : floodfill
    // const int64 start = getTickCount();
    // my code
    // Rect ccomp;
    // floodFill(src, Point(50,300), Scalar(155,255,55), &ccomp,
    //     Scalar(20,20,20), Scalar(20,20,20));
    // double duration = (getTickCount() - start) / getTickFrequency();
    // cout << duration << endl;
    // imshow("eff", src);
    // waitKey();
    return 0;
}