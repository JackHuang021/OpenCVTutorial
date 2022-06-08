#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void help(char* progName)
{
    cout << endl
         << "This program shows how to filter images with mask: the write it "
            "youself and the filter2d way." << endl
         << "Usage: " << progName << " [image_path -- default lena.jpg] [G -- "
                                     "grayscale]" << endl;
}

static void sharpen(const Mat& src, Mat& dst);


int main(int argc, char* argv[])
{
    help(argv[0]);
    const char* filename = argc >= 2 ? argv[1] : "lena.jpg";

    Mat src;
    Mat dst1;
    Mat dst2;

    if (argc >= 3 && !(strcmp("G", argv[2])))
        src = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    else
        src = imread(samples::findFile(filename), IMREAD_COLOR);

    if (src.empty())
    {
        cerr << "Can't open image [" << filename << "]" << endl;
        return EXIT_FAILURE;
    }

    namedWindow("Input");
    namedWindow("Output");

    imshow("Input", src);
    double  t = (double)getTickCount();
    sharpen(src, dst1);
    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "hand written function time passed in seconds: " << t << endl;
    imshow("Output", dst1);
    waitKey();

    Mat kernel = (Mat_<int8_t>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    t = (double)getTickCount();

    filter2D(src, dst2, src.depth(), kernel);
    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "built-in filter2D time passed in seconds: " << t << endl;
    imshow("Output", dst2);
    waitKey();

    return EXIT_SUCCESS;
}

static void sharpen(const Mat& src, Mat& dst)
{
    CV_Assert(src.depth() == CV_8U);
    
    const int channels = src.channels();
    dst.create(src.size(), src.type());
    
    for (int i = 1; i < src.rows - 1; i++)
    {
        const uint8_t* previous = src.ptr<uint8_t>(i - 1);
        const uint8_t* current = src.ptr<uint8_t>(i);
        const uint8_t* next = src.ptr<uint8_t>(i + 1);
        
        uint8_t* out = dst.ptr<uint8_t>(i);
        for (int j = channels; j < channels * (src.cols - 1); j++)
        {
            *out = saturate_cast<uint8_t>(5 * current[j] - current[j - channels]
                    - current[j + channels] - previous[j] - next[j]);
            out++;
        }
    }

    dst.row(0).setTo(Scalar(0));
    dst.row(dst.rows - 1).setTo(Scalar(0));
    dst.col(0).setTo(Scalar(0));
    dst.col(dst.cols - 1).setTo(Scalar(0));
}
