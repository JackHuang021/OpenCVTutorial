/**
 * @file Morphology_3(Extract_Lines).cpp
 * @brief Use morphology transformations for extracting horizontal and vertical lines sample code
 * @author OpenCV team
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

void show_wait_destroy(const char* winname, cv::Mat img);

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //! [load_image]
    CommandLineParser parser(argc, argv, "{@input | notes.png | input image}");
    Mat src = imread( samples::findFile( parser.get<String>("@input") ), IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    imshow("src", src);

    // Transform source image to gray if it is not already
    Mat gray;

    if (src.channels() == 3)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }

    // Show gray image
    show_wait_destroy("gray", gray);

    /* 先对像素颜色进行反转然后再二值化 */
    // ADAPTIVE_THRESH_MEAN_C 采取blocksize * blocksize区域的像素均值减去C作为阈值
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

    show_wait_destroy("binary", bw);

    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();

    // Specify size on horizontal axis
    int horizontal_size = horizontal.cols / 30;

    // 创建Kernel
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));

    // 形态学开操作提取水平直线，以下两种方法都可以用
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    // morphologyEx(horizontal, horizontal, MORPH_OPEN, horizontalStructure);

    // Show extracted horizontal lines
    show_wait_destroy("horizontal", horizontal);

    // Specify size on vertical axis
    int vertical_size = vertical.rows / 30;

    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));

    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
//    morphologyEx(vertical, vertical, MORPH_OPEN, verticalStructure);

    // Show extracted vertical lines
    show_wait_destroy("vertical", vertical);

    // Inverse vertical image
    bitwise_not(vertical, vertical);
    show_wait_destroy("vertical_bit", vertical);

    // Extract edges and smooth image according to the logic
    // 1. extract edges
    // 2. dilate(edges)
    // 3. src.copyTo(smooth)
    // 4. blur smooth img
    // 5. smooth.copyTo(src, edges)

    // Step 1
    Mat edges;
    adaptiveThreshold(vertical, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    show_wait_destroy("edges", edges);

    // Step 2
    Mat kernel = Mat::ones(2, 2, CV_8UC1);
    dilate(edges, edges, kernel);
    show_wait_destroy("dilate", edges);

    // Step 3
    Mat smooth;
    vertical.copyTo(smooth);

    // Step 4
    blur(smooth, smooth, Size(2, 2));

    // Step 5
    smooth.copyTo(vertical, edges);

    // Show final result
    show_wait_destroy("smooth - final", vertical);

    return 0;
}

void show_wait_destroy(const char* winname, cv::Mat img) {
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(0);
    destroyWindow(winname);
}
