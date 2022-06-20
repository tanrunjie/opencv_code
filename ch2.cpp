#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>
using namespace std;
using namespace cv;

void salt(Mat image, int n)
{
    default_random_engine generator;
    uniform_int_distribution<int> randomRow(0, image.rows - 1);
    uniform_int_distribution<int> randomCol(0, image.cols - 1);
    int i,j;
    for(int k=0; k<n; k++){
        i = randomCol(generator);
        j = randomRow(generator);
        if(image.type() == CV_8UC1){
            image.at<uchar>(j,i) = 255;
        } else if(image.type() == CV_8UC3){
            image.at<Vec3b>(j,i)[0] = 255;
            image.at<Vec3b>(j,i)[1] = 255;
            image.at<Vec3b>(j,i)[2] = 255;
        }
    }

}

void colorReduce(Mat image, int div=64)
{
    int nl = image.rows;
    int nc = image.cols * image.channels();

    if(image.isContinuous()){
        nc = nc * nl;
        nl = 1;
    }
    int n = static_cast<int>(log(
      static_cast<double>(div)) / log(2.0) + 0.5);

    uchar mask = 0xFF << n;
    uchar div2 = div >> 1;

    for(int j=0; j<nl; j++)
    {
        uchar * data = image.ptr<uchar>(j);
        for(int i=0; i<nc; i++){
            *data &= mask;
            *data++ += div2;
        }
    }
}

void colorReduce2(Mat image, int div=64)
{
    int nl = image.rows;
    int nc = image.cols * image.channels();

    if(image.isContinuous()){
        nc = nc * nl;
        nl = 1;
    }
    int n = static_cast<int>(log(
      static_cast<double>(div)) / log(2.0) + 0.5);

    uchar mask = 0xFF << n;
    uchar div2 = div >> 1;

    MatIterator_<Vec3b> it = image.begin<Vec3b>();
    MatIterator_<Vec3b> itend = image.end<Vec3b>();

    for(; it != itend; it++)
    {
        (*it)[0] &= mask;
        (*it)[0] +=div2;
        (*it)[1] &= mask;
        (*it)[1] +=div2;
        (*it)[2] &= mask;
        (*it)[2] +=div2;

    }
}


void colorReduce3(Mat image, int div=64)
{
    int n = static_cast<int>(log(
      static_cast<double>(div)) / log(2.0) + 0.5);

    uchar mask = 0xFF << n;
    uchar div2 = div >> 1;
    image = (image & Scalar(mask, mask, mask)) +
        Scalar(div/2, div/2, div/2);
}


void colorReduce(const Mat &image, Mat &result, int div = 64)
{
    result.create(image.rows, image.cols, image.type());
    for(int j=0; j< image.rows; j++)
    {
        const uchar* data_in = image.ptr<uchar>(j);
        uchar* data_out = result.ptr<uchar>(j);

        for(int i=0; i< image.cols * image.channels(); i++)
        {
            data_out[i] = data_in[i] / div *div + div/2;
        }
    }
}

void wave(const Mat &image, Mat &result)
{
    Mat srcX(image.rows, image.cols, CV_32F);
    Mat srcY(image.rows, image.cols, CV_32F);

    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++)
        {
            srcX.at<float>(i,j) = image.cols - j - 1;
            srcY.at<float>(i,j) = i;
        }
    }

    remap(image, result, srcX, srcY, INTER_LINEAR);
}


int main()
{
    Mat image = imread("1.jpeg");
    Mat result;
    const int64 start = getTickCount();
    salt(image, 3000);
    // method 1
    // colorReduce3(image);

    // method 2
    // colorReduce(image, result);

    wave(image, result);
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    imwrite("2.jpeg", result);
    
    return 0;
}