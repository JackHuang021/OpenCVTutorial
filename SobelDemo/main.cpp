#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, "{@input | lena.jpg | input image}"
                                         "{ksize k | 1 | ksize (hit 'K' to increase its value)}"
                                         "{scale s | 1 | scale (hit 'S' to increase its value)}"
                                         "{delta d | 0 | delta (hit 'D' to increase its value)}"
                                         "{help h | false | show help message}");

    cout << "the sample use Sobel or Scharr OpenCV functions for edge detection" << endl;
    parser.printMessage();
    cout << "press 'ESC' to exit program" << endl;
    cout << "press 'R' to reset values" << endl;

    Mat image;
    Mat src;
    Mat src_gray;
    Mat grad;
    Mat grad_threshold;
    const String winName = "Sobel Demo - Simple Edge Detector";
    int ksize = parser.get<int>("ksize");
    int scale = parser.get<int>("scale");
    int delta = parser.get<int>("delta");
    int ddepth = CV_16S;

    String filename = parser.get<String>("@input");
    image = imread(samples::findFile(filename));
    if (image.empty())
    {
        cout << "could not open or find the image" << filename << endl;
        return -1;
    }

    while (1)
    {
        GaussianBlur(image, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

        cvtColor(src, src_gray, COLOR_BGR2GRAY);

        Mat gradX;
        Mat gradY;
        Mat absGradX;
        Mat absGradY;

        /* dx dy 指求导的阶数 */
        Sobel(src_gray, gradX, ddepth, 1, 0, ksize, scale, delta);
        Sobel(src_gray, gradY, ddepth, 0, 1, ksize, scale, delta);

        convertScaleAbs(gradX, absGradX);
        convertScaleAbs(gradY, absGradY);

        imshow("grad x", absGradX);
        imshow("grad y", absGradY);

        addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

        imshow(winName, grad);

        /* 边缘图像二值化 */
        adaptiveThreshold(grad, grad_threshold, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
        Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
        morphologyEx(grad_threshold, grad_threshold, MORPH_OPEN, element);
        imshow("Edge Binary", grad_threshold);

        char key = waitKey(0);

        if(key == 27)
            break;


        if (key == 'k' || key == 'K')
        {
            ksize = ksize < 30 ? ksize+2 : -1;
        }

        if (key == 's' || key == 'S')
        {
            scale++;
        }

        if (key == 'd' || key == 'D')
        {
            delta++;
        }

        if (key == 'r' || key == 'R')
        {
            scale =  1;
            ksize = -1;
            delta =  0;
        }

        cout << "scale: " << scale
             << ", ksize: " << ksize
             << ", delta: " << delta << endl;
    }

    destroyAllWindows();
    return 0;

}
