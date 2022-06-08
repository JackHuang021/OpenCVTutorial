#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


using namespace std;
using namespace cv;


namespace {

void conv_seq(Mat src, Mat &dst, const Mat kernel)
{
    int rows = src.rows;
    int cols = src.cols;

    dst = Mat(rows, cols, src.type());
    int sz = kernel.rows / 2;
    copyMakeBorder(src, src, sz, sz, sz, sz, BORDER_REPLICATE);

    for (int i = 0; i < rows; i++)
    {
        uint8_t *dst_ptr = dst.ptr(i);
        for (int j = 0; j < cols; j++)
        {
            double value = 0;
            for (int k = -sz; k <= sz; k++)
            {
                uint8_t *src_ptr = src.ptr(i + sz + k);
                for (int l = -sz; l <= sz; l++)
                {
                    value += kernel.ptr<double>(k + sz)[l + sz] * src_ptr[j + sz + l];
                }
            }
            dst_ptr[j] = saturate_cast<uint8_t>(value);
        }
    }
}

#ifdef CV_CXX11

void conv_parallel(Mat src, Mat &dst, Mat kernel)
{
   int rows = src.rows;
   int cols = src.cols;

   dst = Mat(rows, cols, CV_8UC1, Scalar(0));
   int sz = kernel.rows / 2;
   copyMakeBorder(src, src, sz, sz, sz, sz, BORDER_REPLICATE);

   parallel_for_(Range(0, rows * cols), [&](const Range &range)
   {
       for (int r = range.start; r < range.end; r++)
       {
           int i = r / cols;
           int j = r % cols;

           double value = 0;
           for (int k = -sz; k <= sz; k++)
           {
               uint8_t *src_ptr = src.ptr(i + sz + k);
               for (int l = -sz; l <= sz; l++)
               {
                   value += kernel.ptr<double>(k + sz)[l + sz] * src_ptr[l + sz + j];
               }
           }
           dst.ptr(i)[j] = saturate_cast<uint8_t>(value);
       }
   });
}
#else


#endif

}



