#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

static void help();
static Mat& scanImageAndReduceC(Mat& img, const uint8_t* const table);

int main(int argc, char* argv[])
{
    help();

    if (argc < 3)
    {
        cout << "not enough parameters" << endl;
        return -1;
    }

    Mat img;
    Mat dst;
    if (argc == 4 && !(strcmp(argv[3], "G")))
        img = imread(argv[1], IMREAD_GRAYSCALE);
    else
        img = imread(argv[1], IMREAD_COLOR);

    if (img.empty())
    {
        cout << "The image" << argv[1] << " could not be loaded." << endl;
        return -1;
    }

    int divideWith = 0;
    stringstream s;
    s << argv[2];
    s >> divideWith;
    if (!s || !divideWith)
    {
        cout << "invalid number entered fro dividing." << endl;
        return -1;
    }

    uint8_t table[256];
    for (int i = 0; i < 256; i++)
        table[i] = (uint8_t)(divideWith * (i / divideWith));

    const int times = 100;
    double t = 0;

    t = (double)getTickCount();

    for (int i = 0; i < times; i++)
    {
        Mat clone_i = img.clone();
        dst = scanImageAndReduceC(clone_i, table);
    }
    t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
    t /= times;

    cout << "time of reducing wiht C operator [] (average for "
         << times << " runs): " << t << " milliseconds." << endl;

    return 0;
}

static void help()
{
    cout
        << "\n--------------------------------------------------------------------------" << endl
        << "This program shows how to scan image objects in OpenCV (cv::Mat). As use case"
        << " we take an input image and divide the native color palette (255) with the "  << endl
        << "input. Shows C operator[] method, iterators and at function for on-the-fly item address calculation."<< endl
        << "Usage:"                                                                       << endl
        << "./how_to_scan_images <imageNameToUse> <divideWith> [G]"                       << endl
        << "if you add a G parameter the image is processed in gray scale"                << endl
        << "--------------------------------------------------------------------------"   << endl
        << endl;
}

static Mat& scanImageAndReduceC(Mat& img, const uint8_t* const table)
{
    CV_Assert(img.depth() == CV_8U);

    int channels = img.channels();
    int nRows = img.rows;
    int nCols = img.cols * channels;

    if (img.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i = 0;
    int j = 0;
    uint8_t* p;

    for (i = 0; i < nRows; i++)
    {
        p = img.ptr<uint8_t>(i);
        for (j = 0; j < nCols; j++)
        {
            p[j] = table[p[j]];
        }
    }

    return img;
}








