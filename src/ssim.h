#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

std::pair<cv::Mat, cv::Scalar> getMSSIM(const cv::Mat &i1, const cv::Mat &i2);