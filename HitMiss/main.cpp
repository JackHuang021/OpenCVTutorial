#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    Mat inputImg = (Mat_<uint8_t>(8, 8) <<
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 255, 255, 255, 0, 0, 0, 255,
                    0, 255, 255, 255, 0, 0, 0, 0,
                    0, 255, 255, 255, 0, 255, 0, 0,
                    0, 0, 255, 0, 0, 0, 0, 0,
                    0, 0, 255, 0, 0, 255, 255, 0,
                    0, 255, 0, 255, 0, 0, 255, 0,
                    0, 255, 255, 255, 0, 0, 0, 0);

    Mat kernel = (Mat_<int>(3, 3) << 0, 1, 0, 1, -1, 1, 0, 1, 0);
    Mat outImg;
    morphologyEx(inputImg, outImg, MORPH_HITMISS, kernel);

    const int rate = 50;
    kernel = (kernel + 1) * 127;
    kernel.convertTo(kernel, CV_8U);

    resize(kernel, kernel, Size(), rate, rate, INTER_NEAREST);
    imshow("kernel", kernel);
    moveWindow("kernel", 0, 0);

    resize(inputImg, inputImg, Size(), rate, rate, INTER_NEAREST);
    imshow("Original", inputImg);
    moveWindow("Original", 0, 200);

    resize(outImg, outImg, Size(), rate, rate, INTER_NEAREST);
    imshow("Hit or Miss", outImg);
    moveWindow("Hit or Miss", 500, 200);

    waitKey();
    destroyAllWindows();

    return 0;
}
