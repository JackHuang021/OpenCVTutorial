#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

namespace
{
int alpha = 100;
int beta = 100;
int gamma_cor = 100;

Mat img_original;
Mat img_corrected;
Mat img_gamma_corrected;

void basicLinearTransform(const Mat& img, const double alpha_, const int beta_)
{
    Mat res;

    img.convertTo(res, -1, alpha_, beta_);
    hconcat(img, res, img_corrected);
    imshow("Brightness and contrast adjustments", img_corrected);
}

void gammaCorrection(const Mat& img, const double gamma_)
{
    CV_Assert(gamma_ >= 0);

    Mat lookUpTable(1, 256, CV_8U);
    uint8_t* p = lookUpTable.ptr();

    for (int i = 0; i < 256; i++)
        p[i] = saturate_cast<uint8_t>(pow(i / 255.0, gamma_) * 255.0 );

    Mat res = img.clone();
    LUT(img, lookUpTable, res);
    hconcat(img, res, img_gamma_corrected);
    imshow("Gamma correction", img_gamma_corrected);
}

void on_linear_transform_alpha_trackbar(int, void *)
{
    double alpha_value = alpha / 100.0;
    int beta_value = beta - 100;
    basicLinearTransform(img_original, alpha_value, beta_value);
}

void on_linear_transform_beta_trackbar(int, void *)
{
    double alpha_vlue = alpha / 100.0;
    int beta_value = beta - 100;
    basicLinearTransform(img_original, alpha_vlue, beta_value);
}

void on_gamma_correction_trackbar(int, void *)
{
    double gamma_value = gamma_cor / 100.0;
    gammaCorrection(img_original, gamma_value);
}

}

int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, "{@input | lena.jpg | input image}");
    img_original = imread(samples::findFile(parser.get<String>("@input")));
    if (img_original.empty())
    {
        cout << "could not open or find the image." << endl;
        cout << "Usage: " << argv[0] << " <input_image>" << endl;
        return -1;
    }

    img_corrected = Mat(img_original.rows, img_original.cols * 2, img_original.type());
    img_gamma_corrected = Mat(img_corrected.rows, img_corrected.cols, img_corrected.type());

    hconcat(img_original, img_original, img_corrected);
    hconcat(img_original, img_original, img_gamma_corrected);

    namedWindow("Brightness and contrast adjustments");
    namedWindow("Gamma correction");

//    createTrackbar("Alpha gain (contrast)", "Brightness and contrast adjustments",
//                   &alpha, 500)
}
