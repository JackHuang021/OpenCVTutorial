#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static void showWaitDestory(const char* winName, const Mat& img);


int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, "{@input | notes.png | input image}");

    Mat src = imread(samples::findFile(parser.get<String>("@input")));
    if (src.empty())
    {
        cout << "could not open or find the image" << endl;
        cout << "Usage: " << argv[0] << " <input image>" << endl;
        return -1;
    }

    imshow("Original", src);


    Mat gray;
    if (src.channels() == 3)
        cvtColor(src, gray, COLOR_BGR2GRAY);
    else
        gray = src;

    showWaitDestory("Gray", gray);

    Mat bw;
    /* 将原始灰度图像进行取反操作后再进行二值化操作 */
    /* 自适应阈值函数 */
    /* ADAPTIVE_THRESH_MEAN_C方法，阈值T为 bloskSize*blockSize 邻域内(x, y)减去第七个参数C的平均值 */
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

    showWaitDestory("Binary", bw);

    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();

    int horizontalSize = horizontal.cols / 10;
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalSize, 1));

    /* 使用一个水平线结构提取当前图片的水平直线，使用开操作提取，先腐蚀后膨胀，下面两种方法均可 */
//    erode(horizontal, horizontal, horizontalStructure);
//    dilate(horizontal, horizontal, horizontalStructure);
    morphologyEx(horizontal, horizontal, MORPH_OPEN, horizontalStructure);

    showWaitDestory("Horizontal", horizontal);

    int verticalSize = vertical.rows / 30;
    Mat  verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalSize));

//    erode(vertical, vertical, verticalStructure);
//    dilate(vertical, vertical, verticalStructure);
    morphologyEx(vertical, vertical, MORPH_OPEN, verticalStructure);

    showWaitDestory("Vertical", vertical);

    bitwise_not(vertical, vertical);
    showWaitDestory("Vertical Inverse", vertical);

    Mat edges;
    adaptiveThreshold(vertical, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    showWaitDestory("Edge", edges);

    Mat kernel = Mat::ones(2, 2, CV_8UC1);
    dilate(edges, edges, kernel);
    showWaitDestory("Edge Dilate", edges);

    Mat smooth;
    vertical.copyTo(smooth);

    blur(smooth, smooth, Size(2, 2));

    showWaitDestory("Smooth", smooth);

    smooth.copyTo(vertical, edges);

    showWaitDestory("Smooth Final", vertical);

    return 0;
}

void showWaitDestory(const char* winName, const Mat& img)
{
    imshow(winName, img);
    moveWindow(winName, 500, 0);
    waitKey(0);
    destroyWindow(winName);
}
