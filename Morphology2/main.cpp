#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat src;
Mat dst;


int morphElem = 0;
int morphSize = 0;
int morphOperator = 0;

const int maxOperator = 4;
const int maxElem = 2;
const int maxKernelSize = 21;

const char* winName = "Morphology Transformations Demo";
const char* morphologyOperationTrackbarName = "Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat";
const char* kernelTypeTrackbarName = "Element:\n 0: Rect - 1: Cross - 2: Ellipse";
const char* kernelSizeTrackbarName = "Kernel size :\n 2n + 1";

static void morphologyOperations(int, void*);


int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, "{@input | baboon.jpg | input image}");
    src = imread(samples::findFile(parser.get<String>("@input")));

    if (src.empty())
    {
        cout << "could not open or find the image" << endl;
        cout << "Usage: " << argv[0] << " <input image>" << endl;
        return -1;
    }

    namedWindow(winName);

    createTrackbar(morphologyOperationTrackbarName, winName, &morphOperator,
                   maxOperator, morphologyOperations);

    createTrackbar(kernelTypeTrackbarName, winName, &morphElem, maxElem, morphologyOperations);

    createTrackbar(kernelSizeTrackbarName, winName, &morphSize, maxKernelSize, morphologyOperations);

    morphologyOperations(0, 0);

    waitKey();
    destroyAllWindows();

    return 0;
}

static void morphologyOperations(int, void*)
{
    int operation = morphOperator + 2;

    /* 生成对应尺寸和形状的kernel */
    Mat element = getStructuringElement(morphElem, Size(2 * morphSize + 1,
                                                        2 * morphSize + 1),
                                        Point(morphSize, morphSize));

    morphologyEx(src, dst, operation, element);
    imshow(winName, dst);
}

