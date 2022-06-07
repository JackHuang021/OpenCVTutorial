#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>


using std::cin;
using std::cout;
using std::endl;

using namespace cv;


int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, "{@input | lena.jpg | input image}");
    Mat image = imread(samples::findFile(parser.get<String>( "@input" )));

    if (image.empty())
    {
        cout << "could not open or find the image!.\n" << endl;
        cout << "Usage: " << argv[0] << " <Input Image>" << endl;
        return -1;
    }

    Mat new_image = Mat::zeros(image.size(), image.type());

    double alpha = 1.0;
    int beta = 0;

    cout << "Basic Linear Transform " << endl;
    cout << "-----------------------" << endl;
    cout << "*Enter the alpha value [1.0-3.0]: ";
    cin >> alpha;
    cout << "*Enter the beta value [0-100]: ";
    cin >> beta;

    for (int x = 0; x < image.rows; x++)
        for (int y = 0; y < image.cols; y ++)
           for (int c = 0; c < image.channels(); c++)
               new_image.at<Vec3b>(y, x)[c] =
                       saturate_cast<uint8_t>(alpha * image.at<Vec3b>(y, x)[c] + beta);

    imshow("Original Image", image);
    imshow("New Image", new_image);

    waitKey();
    destroyAllWindows();

    return 0;
}
