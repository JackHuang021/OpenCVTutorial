#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static Mat src;
static Mat srcGray;
static Mat dst;
static Mat detectedEdges;

static int lowerThreshold = 0;
static const int maxThreshold = 100;
static const int thresholdRatio = 3;
const int kernelSize = 5;
const char* winName = "Edge Map";

static void cannyThreshold(int, void*)
{
    GaussianBlur(srcGray, detectedEdges, Size(3, 3), 0, 0);
    Canny(detectedEdges, detectedEdges, lowerThreshold, lowerThreshold * thresholdRatio, kernelSize);
    dst = Scalar::all(0);
    src.copyTo(dst, detectedEdges);
    imshow("Original Image", src);
    imshow("Edge Detect", detectedEdges);
    imshow(winName, dst);
}


int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, "{@input | fruits.jpg | input image}");

    src = imread(samples::findFile(parser.get<String>("@input")));
    if (src.empty())
    {
        cout << "can not open or find the image" << endl;
        cout << "Usage: " << argv[0] << " [input image]" << endl;
        return -1;
    }

    dst.create(src.size(), src.type());
    cvtColor(src, srcGray, COLOR_BGR2GRAY);
    namedWindow(winName, WINDOW_AUTOSIZE);
    createTrackbar("Min Threshold:", winName, &lowerThreshold, maxThreshold, cannyThreshold);

    waitKey(0);

    return 0;
}
