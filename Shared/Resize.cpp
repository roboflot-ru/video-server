#include "Resize.h"

#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<unsigned char> Resize(unsigned srcWidth, unsigned srcHeight, unsigned dstWidth, unsigned dstHeight, const unsigned char* src)
{
  try
  {
    void* row = (void*)(src);
    cv::Mat src(srcHeight + srcHeight / 2, srcWidth, CV_8UC1, row);
    cv::Mat dst;
    cv::resize(src, dst, cvSize(dstWidth, dstHeight + dstHeight / 2));
    std::vector<unsigned char> result(dst.rows*dst.cols);
    if (dst.isContinuous())
    {
      memcpy(&result[0], dst.data, result.size());
    }
    return result;
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}
