#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

class ColorDetector{
private:
    int maxDist;
    Vec3b target;
    Mat converted;
    Mat result;
public:
    ColorDetector():maxDist(100), target(0,0,0){}
    
    void setColorDistanceThreshold(int distance){
        if(distance <0)
            distance = 0;
        maxDist = distance;
    }
    int getColorDistanceThreshold() const{
        return maxDist;
    }
    void setTargetColor(uchar blue, uchar green, uchar red){
        target = Vec3b(blue, green, red);
    }
    void setTargetColor(Vec3b color){
        target = color;
    }
    Vec3b getTargetColor() const{
        return target;
    }
    int getDistanceToTargetColor(const Vec3b& color) const{
        return getColorDistance(color, target);
    }
    int getColorDistance(const Vec3b& color1, const Vec3b& color2) const{
        return abs(color1[0] - color2[0]) +
        abs(color1[1] - color2[1]) + abs(color1[2] - color2[2]);
    }
    Mat process(const Mat& image)
    {
        result.create(image.size(), CV_8U);
        Mat_<Vec3b>::const_iterator it = image.begin<Vec3b>();
        Mat_<Vec3b>::const_iterator itend = image.end<Vec3b>();
        Mat_<uchar>::iterator itout = result.begin<uchar>();
        for(; it!=itend; ++it, ++itout){
            if(getDistanceToTargetColor(*it) <= maxDist){
                *itout = 255;
            } else{
                *itout = 0;
            }
        }
        return result;
    }
  };

void detectHScolor(const Mat& image, double minHue, double maxHue,
double minSat, double maxSat, Mat& mask){
    Mat hsv;
    cvtColor(image, hsv, CV_BGR2HSV);
    vector<Mat> channels;
    split(hsv, channels); // 0: hue, 1:saturate, 2:value

    Mat mask1;
    threshold(channels[0], mask1, maxHue, 255, THRESH_BINARY_INV);
    Mat mask2;
    threshold(channels[0], mask2, minHue, 255, THRESH_BINARY);

    Mat hueMask;
    if(minHue < maxHue)
        hueMask = mask1 & mask2;
    else
        hueMask = mask1 | mask2;
    
    Mat satMask;
    inRange(channels[1], minSat, maxSat, satMask);

    mask = hueMask & satMask;
}

int main()
{
    Mat image = imread("2.jpeg");
    Mat result;
    const int64 start = getTickCount();
    // method 1: color detect
    ColorDetector cdetect;
    cdetect.setTargetColor(210, 210, 210);
    result = cdetect.process(image);

    // method 2: grab cut
    // Rect rectangle(800,800,400,400);
    // Mat bgModel, fgModel;
    // grabCut(image, result, rectangle, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
    // compare(result, GC_PR_FGD,result, CMP_EQ);
    // Mat foreground(image.size(), CV_8UC3, Scalar(255,255,255));
    // image.copyTo(foreground, result);
    // result = result & 1;
    
    // method 3: HSV op
    /*
    Mat hs(128, 360, CV_8UC3);
    for(int h=0; h<360; h++){
        for(int s=0; s<128; s++){
            hs.at<Vec3b>(s,h)[0] = h/2;
            hs.at<Vec3b>(s,h)[1] = 255 - s*2;
            hs.at<Vec3b>(s,h)[2] = 255;
        }
    }

    Mat hsv;
    cvtColor(image, hsv, CV_BGR2HSV);
    vector<Mat> channels;
    split(hsv, channels);
    channels[2] = 255;
    merge(channels, hsv);
    Mat newImage;
    cvtColor(hsv, newImage, CV_HSV2BGR);
    */
    Mat mask;
    detectHScolor(image, 160, 10, 25,166,mask);
    Mat detected(image.size(), CV_8UC3, Scalar(0,0,0));
    image.copyTo(detected, mask);
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    imwrite("2_2.jpeg", detected);
    
    return 0;
}