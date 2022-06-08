#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;


static void help(char ** argv)
{
    cout << endl
        <<  "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl
        <<  "The dft of an image is taken and it's power spectrum is displayed."  << endl << endl
        <<  "Usage:"                                                                      << endl
        << argv[0] << " [image_name -- default lena.jpg]" << endl << endl;
}

int main(int argc, char* argv[])
{
    help(argv);

    const char* filename = argc > 2 ? argv[1] : "lena.jpg";
    Mat img = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    if (img.empty())
    {
        cout << "error open image." << endl;
        return EXIT_FAILURE;
    }

    Mat padded;
    // 计算最适合进行离散傅里叶变换的图像尺寸
    int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);
    cout << "m = " << m << ", n = " << n << endl;
    // 填充图像扩展部分
    copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // planes表示傅里叶变换的结果，实数部分和复数部分用两个Mat表示
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;

    // 将planes数组中的数据进行合并到一个Mat中
    merge(planes, 2, complexImg);

    // 进行傅里叶变换
    dft(complexImg, complexImg);

    // 将变换后的结果分开到planes数组中进行存储
    split(complexImg, planes);

    // 将复数转化为距离，并存储到planes[0]中
    magnitude(planes[0], planes[1], planes[0]);

    Mat magImg = planes[0];

    // 将傅里叶变换后的结果进行缩小
    magImg += Scalar::all(1);
    log(magImg, magImg);

    // 调整图像尺寸为偶数行和列
    magImg = magImg(Rect(0, 0, magImg.cols & -2, magImg.rows & -2));
    int cx = magImg.cols / 2;
    int cy = magImg.rows / 2;

    Mat q0(magImg, Rect(0, 0, cx, cy));
    Mat q1(magImg, Rect(cx, 0, cx, cy));
    Mat q2(magImg, Rect(0, cy, cx, cy));
    Mat q3(magImg, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magImg, magImg, 0, 1, NORM_MINMAX);

    imshow("Input Image", img);
    imshow("Spectrum Magnitude", magImg);
    waitKey();
    destroyAllWindows();

    return 0;
}
