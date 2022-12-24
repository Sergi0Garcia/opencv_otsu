/*
-------------------
ASSIGNMENT 4
OTSU BINARIZATION
--------------------
SERGIO GARCIA 
A01634768
27 ABRIL 2021
--------------------
*/

#include "otsu.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

// GLOBAL DECLARATIONS
const char* g_window_name = "image pipeline";
const int g_camera_id = 0;
const int g_width = 640;
const int g_heigth = 480;
const int g_fps = 30;


int main()
{	
	cv::VideoCapture input_video;
	cv::Mat input_frame;
	cv::Mat output_frame;
	cv::Mat gray;
	cv::Mat output;

	input_video.open(g_camera_id);

	if(!input_video.isOpened())
	{
		std::cout << "Could not open camera ... \n";
		return -1;
	}
	
	input_video.set(CV_CAP_PROP_FRAME_WIDTH, g_width);
	input_video.set(CV_CAP_PROP_FRAME_HEIGHT, g_heigth);
	input_video.set(CV_CAP_PROP_FPS, g_fps);
	cv::namedWindow(g_window_name, CV_WINDOW_AUTOSIZE);

	while(1)
	{
		input_video.read(input_frame);
		Otsu otsuImg(input_frame);
	   	gray = otsuImg.customGray();
		int thresh = otsuImg.binarization(gray);
		output = otsuImg.threshsold(gray, thresh);
		output_frame = output;
		imshow(g_window_name, output_frame);
		cv::waitKey(1);
	}
	input_video.release();
	return 0;
}