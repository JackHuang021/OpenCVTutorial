#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const char* winName = "Gaussian Pyramids Demo";

int main(int argc, char* argv[])
{
    cout << "Zoom In-Out demo" << endl
         << "----------------" << endl
         << "* [i] -> Zoom in" << endl
         << "* [o] -> Zoom out" << endl
         << "* [ESC] -> Close program" << endl;

    const char* filename = argc >= 2 ? argv[1] : "chicky_512.png";
    Mat src = imread(samples::findFile(filename));
    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        cout << "Usage: " << argv[0] << " <input image>" << endl;
        return -1;
    }

    while (1)
    {
        imshow(winName, src);
        char c = (char)waitKey(0);

        if (c == 27)
            break;
        else if (c == 'i')
        {
            pyrUp(src, src, Size(src.cols * 2, src.rows * 2));
            cout << "** Zoom In: Image x 2" << endl;
        }
        else if (c == 'o')
        {
            pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
            cout << "** Zoom Out: Image / 2" << endl;
        }
    }

    destroyAllWindows();
    return 0;
}
