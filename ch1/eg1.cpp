#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void colorReduce(Mat image, int div = 64)
{
    for(int j=0; j<image.rows; j++){
        uchar* data = image.ptr<uchar>(j);
        for(int i=0; i<image.cols * image.channels(); i++)
        {
            data[i] = data[i]/div *div + div/2;
        }
    }
}
void colorReduce2(Mat image, int div=64)
{
    int n = static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
    uchar mask = 0XFF << n;
    uchar div2 = div >> 1;
    Mat_<Vec3b>::iterator it = image.begin<Vec3b>();
    Mat_<Vec3b>::iterator itend = image.end<Vec3b>();

    for(; it!= itend; it++)
    {
        (*it)[0] &= mask;
        (*it)[0] += div2;
        (*it)[1] &= mask;
        (*it)[1] += div2;
        (*it)[2] &= mask;
        (*it)[2] += div2;
    }
}

void sharpen(const Mat &image, Mat &result){
    result.create(image.size(), image.type());
    int nc = image.channels();
    for(int j=1; j<image.rows-1; j++)
    {
        const uchar* previous = image.ptr<const uchar>(j-1);
        const uchar* current = image.ptr<const uchar>(j);
        const uchar* next = image.ptr<const uchar>(j+1);

        uchar* output = result.ptr<uchar>(j);

        for(int i=nc; i<(image.cols-1) * nc; i++)
        {
            *output++ = saturate_cast<uchar>(
                5 * current[i] - current[i - nc] - current[i+nc] - 
                    previous[i] - next[i]);
        }
    }
    result.row(0).setTo(Scalar(0));
    result.row(result.rows-1).setTo(Scalar(0));
    result.col(0).setTo(Scalar(0));
    result.col(result.cols-1).setTo(Scalar(0));
}

void sharpen2D(const Mat &image, Mat &result)
{
    Mat kernel(3,3, CV_32F, Scalar(0));

    kernel.at<float>(1,1) = 5.0;
    kernel.at<float>(0,1) = -1.0;
    kernel.at<float>(1,0) = -1.0;
    kernel.at<float>(2,1) = -1.0;
    kernel.at<float>(1,2) = -1.0;

    filter2D(image, result, image.depth(), kernel);
}

int main()
{
    Mat image = imread("1.jpeg");
    Mat result;
    const int64 start = getTickCount();
    sharpen2D(image, result);
    // colorReduce2(image);
    double duration = (getTickCount() - start) / getTickFrequency();
    cout << duration << endl;
    // flip(result, result, 0); // vertical
    flip(result, result, 1); // horizental

    imwrite("2.jpeg", result);
    
    return 0;
}