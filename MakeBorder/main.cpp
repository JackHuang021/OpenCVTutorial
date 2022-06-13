#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


using namespace cv;


Mat src;
Mat dst;
int top = 0;
int bottom = 0;
int left = 0;
int right = 0;
int borderType = BORDER_CONSTANT;
const char* winName = "copyMakeBorder demo";
RNG rng(12345);

int main(int argc, char* argv[])
{
    const char* fileName = argc >= 2 ? argv[1] : "lena.jpg";

    src = imread(samples::findFile(fileName));
    if (src.empty())
    {
        std::cout << "could not open or find the image " << fileName << std::endl;
        std::cout << "Usage: " << argv[0] << " <input image>" << std::endl;
        return -1;
    }

    printf( "\n \t copyMakeBorder Demo: \n" );
    printf( "\t -------------------- \n" );
    printf( " ** Press 'c' to set the border to a random constant value \n");
    printf( " ** Press 'r' to set the border to be replicated \n");
    printf( " ** Press 'ESC' to exit the program \n");

    namedWindow(winName);

    top = (int)(0.1 * src.rows);
    bottom = top;
    left = (int)(0.1 * src.cols);
    right = left;

    while (1)
    {
        Scalar value(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        copyMakeBorder(src, dst, top, bottom, left, right, borderType, value);
        std::cout << "update image" << std::endl;
        imshow(winName, dst);
        char c = (char)waitKey(0);
        if( c == 27 )
            break;
        else if( c == 'c' )
            borderType = BORDER_CONSTANT;
        else if( c == 'r' )
            borderType = BORDER_REPLICATE;
    }

    destroyAllWindows();
    return 0;
}
