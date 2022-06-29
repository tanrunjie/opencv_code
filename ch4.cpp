#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

class Histogram1D{
private:
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
public:
    Histogram1D(){
        histSize[0] = 256;
        hranges[0] = 0.0;
        hranges[1] = 256.0;
        ranges[0] = hranges;
        channels[0] = 0;
    }
    Mat getHistogram(const Mat& image){
        Mat hist;
        calcHist(&image, 1, 
        channels, 
        Mat(), 
        hist, 1, histSize, ranges);

        return hist;
    }
    static Mat getImageOfHistogram(const Mat& hist, int zoom){
        double maxVal = 0;
        double minVal = 0;
        minMaxLoc(hist, &minVal, &maxVal, 0, 0);

        int histSize = hist.rows;
        
        Mat histImg(histSize * zoom, histSize * zoom, CV_8U, Scalar(255));
        int hpt = static_cast<int>(0.9*histSize);

        for(int h=0; h<histSize; h++){
            float binVal = hist.at<float>(h);
            if(binVal>0){
                int intensity = static_cast<int>(binVal *hpt/maxVal);
                line(histImg, Point(h*zoom, histSize*zoom),
                  Point(h*zoom, (histSize - intensity)*zoom),
                  Scalar(0), zoom);
            }
        }
        return histImg;
    }
    Mat getHistogramImage(Mat & image, int zoom = 1){
        Mat hist = getHistogram(image);
        return getImageOfHistogram(hist, zoom);
    }
};

class ColorHistogram{
private:
    int histSize[3];
    float hranges[2];
    const float* ranges[3];
    int channels[3];
public:
    ColorHistogram(){
        histSize[0] = histSize[1] = histSize[2] = 256;
        hranges[0] = 0.0;
        hranges[1] = 256.0;
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
        channels[0] = 0;
        channels[1] = 0;
        channels[2] = 0;
    }
    Mat getHistogram(const Mat &image){
        Mat hist;
        calcHist(&image, 1, channels,
            Mat(), hist, 3, histSize, ranges);
        return hist;
    }
    SparseMat getSparseHistogram(const Mat &image){
        SparseMat hist(3, histSize, CV_32F);
        calcHist(&image, 1, channels, Mat(), hist, 3, histSize, ranges);
        return hist;
    }


};
void colorReduce(Mat &image, int div = 64){
    Mat lookup(1,256,CV_8U);
    for(int i=0; i<256;i++)
        lookup.at<uchar>(i) = i/div*div + div/2;
    LUT(image, lookup, image);
}
int main()
{
    Mat image = imread("1.jpeg", 0);
    Mat result;
    const int64 start = getTickCount();
    // my code
    // method 1: hist calc
    /*
    Histogram1D h;
    Mat histo = h.getHistogram(image);
    for(int i=0; i<256; i++)
        cout <<"Value : " << i << " = " 
            << histo.at<float>(i) << endl;
    // result = h.getHistogramImage(image);
    threshold(image, result, 110, 255, THRESH_BINARY);
    */

    Mat lut(1,256, CV_8U);
    for(int i=0; i<256; i++)
        lut.at<uchar>(i) = 255 -i;
    LUT(image, lut, result);   
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    // colorReduce(image);
    equalizeHist(image, result);

    Mat imageROI = image(Rect(100,100,100,100));
    Histogram1D h;
    Mat hist = h.getHistogram(imageROI);
    imwrite("2.jpeg", result);
    
    return 0;
}