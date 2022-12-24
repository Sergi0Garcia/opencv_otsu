// IMPLEMENTATION OF OTSU.H
#include "otsu.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

Otsu::Otsu(cv::Mat image)
{
	image_ = image;
}

// Se implementa metodo customGray de clase pasada
// Para poder transformar imagen a escala de grises
// @returns cv::Mat

cv::Mat Otsu::customGray()
{
	int height = image_.rows;
	int width = image_.cols;
	int channels = image_.channels();
	int rows = image_.rows;
	int cols = image_.cols;

	cv::Mat gray(rows, cols, CV_8UC1);

	uchar *pixelPtr = (uchar*)image_.data;
	uchar *pixelPtrGray = (uchar*)gray.data;
	for(int irows = 0; irows < rows; irows ++)
	{
		for(int icols = 0; icols < cols; icols ++)
		{
			uchar blue = pixelPtr[irows*cols*channels + icols*channels + 0];
			uchar green = pixelPtr[irows*cols*channels + icols*channels + 1];
			uchar red = pixelPtr[irows*cols*channels + icols*channels + 2];
			double gray_value = 0.2*blue + 0.7*green + 0.1*red;
			pixelPtrGray[irows*cols + icols] = gray_value;
		}
	}

	image_ = gray;
	return image_;
}

// Se implementa metodo threshold para poder setear la imagen gris
// a imagen blanca y negra con el threshosld como parametro 
// @returns cv::Mat

cv::Mat Otsu::threshsold(cv::Mat grayImg, int threshsold)
{
	int rows = grayImg.rows;
	int cols = grayImg.cols;

	cv::Mat img_bw(rows, cols, CV_8UC1);

	uchar *pixelPtr = (uchar*)grayImg.data;
	uchar *pixelPtrOtsu = (uchar*)img_bw.data;
	for(int irows = 0; irows < rows; irows ++)
	{
		for(int icols = 0; icols < cols; icols ++)
		{
			double valuePix = pixelPtr[irows*cols + icols];
			if( valuePix <= threshsold)
			{
				pixelPtrOtsu[irows*cols + icols] = 0;
			}else{
				pixelPtrOtsu[irows*cols + icols] = 255;
			}
		}
	}
	return img_bw;
}	

// Metodo para calcular el valor del threshold ideal
// Se utiliza la logiac matematica de varianzas y pesos de Otsu para encontrar
// El valor del threshold ideal, el cual se retorna como int
// @returns int 

int Otsu::binarization(cv::Mat img)
{
	int rows = img.rows;
	int cols = img.cols;
	int total_pixels = rows*cols;
	int total_gray_pixels = 256;

	int histo[total_gray_pixels]={};
	float bcvariance[total_gray_pixels]={};
	float bcvarianceCopy[total_gray_pixels]={};

	uchar *pixelGrayPointer = (uchar*)img.data;

	// En este for se obtiene el histograma de la escala de grises de la imagen
	// Se guarda en un arreglo  

	for(int irows = 0; irows < rows; irows ++)
	{
		for(int icols = 0; icols < cols; icols ++)
		{
			int r = pixelGrayPointer[irows*cols + icols];
			histo[r] = histo[r]+1;
		}
	}

	double variance = 0;
	double otsu_Wb = 0;
	double otus_Wf = 0;
	double wbcounter = 0;
	double wfcounter = 0;
	double miub = 0;
	double miuf = 0;

	// Se calcula las virianzas y los pesos haciendo el corte en cada pixel 
	// del histograma, el indice del valor maximo de la varianza es el mejor para el otsu

	for(int i = 0; i < total_gray_pixels; i++)
	{
		variance = 0;
		otsu_Wb = 0;
		otus_Wf = 0;
		wbcounter = 0;
		wfcounter = 0;
		miub = 0;
		miuf = 0;

		for (int j = 0; j < total_gray_pixels; j++)
		{
			if(j <= i)
			{
				wbcounter = wbcounter + histo[j];
				miub = miub + (histo[j]*j);
			}else
			{
				wfcounter = wfcounter + histo[j];
				miuf = miuf + (histo[j]*j);
			}
		}
		miub = miub/wbcounter;
		miuf = miuf/wfcounter;
		otsu_Wb = wbcounter/total_pixels;
		otus_Wf = wfcounter/total_pixels;
		variance = ((otsu_Wb*otus_Wf)*((miub-miuf)*(miub-miuf)));
		// Se agrega este if por seguridad
		if (isnan(variance))
		{
			variance = 0;
		}
		bcvariance[i] = variance;
	}

	// Se hace una copia del arreglo original del histograma para poder encontrar
	// el indice del valor maximo

	for(int x = 0; x < total_gray_pixels ; x++)
	{
		bcvarianceCopy[x] = bcvariance[x];
	}

	// Se hace un loop iterativo para poder ordenar el arreglo de 256 valores para 
	// poder obtener el valor maximo de la varianza al cuadrado

	for(int idx=0; idx < total_gray_pixels; idx++)
	{
		for(int idy = idx+1; idy<total_gray_pixels; idy++)
		{
			if (bcvariance[idx] > bcvariance[idy])
			{
				float temporal = bcvariance[idx];
				bcvariance[idx] = bcvariance[idy];
				bcvariance[idy] = temporal;
			}
		}
	}

	// El valor maximo es el del ultimo indice, el cual esta en la posicion 255
	double maxValueArray = bcvariance[255];
	int indexMax = 0;

	// Se hace un loop entre el copy del array para poder saber en que indice tenemos el valor
	// maximo de la varianza, el cual nos indica otsu que es el threshold perfecto. 

	for(int x = 0; x < total_gray_pixels ; x++)
	{
		if(indexMax == 0){
			if (bcvarianceCopy[x] == bcvariance[255])
			{
			indexMax = x;
		  	}
		}
	}

	return indexMax;
}


