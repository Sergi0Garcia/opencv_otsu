// OTSU CLASS HEADER
#ifndef OTSU_H
#define OTSU_H

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

class Otsu 
{
private: 
	cv::Mat image_;

public: 
	Otsu(cv::Mat image);
	cv::Mat customGray();
	cv::Mat threshsold(cv::Mat grayImg, int threshsold);	
	int binarization(cv::Mat img);
};
#endif 