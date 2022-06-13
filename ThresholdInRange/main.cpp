#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const int maxValueH = 180;
const int maxValue = 255;

const String winCaptureName = "VideoCapture";
const String winDectionName = "ObjectDetectio";
int lowH = 0;
int lowS = 0;
int lowV = 0;
int highH = maxValueH;
int highS = maxValue;
int highV = maxValue;


int main(int argc, char* argv[])
{

}
