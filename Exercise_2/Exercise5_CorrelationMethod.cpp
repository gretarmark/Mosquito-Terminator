#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>
#include <chrono>
#include <thread>
#include <array>
#include <math.h>
#include <vector>
#include <opencv2/imgcodecs/imgcodecs.hpp>

using namespace cv;
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void GradientMethod_x(Mat& image_x, int threshold) {
	int counter = 0;
	int counter2 = 0;

	unsigned char* pic = image_x.data; // placeholder for image data
	//int lastIntensity;

	for (int y = 1; y < image_x.rows; ++y) {

		for (int x = 1; x < image_x.cols; ++x) {

			//*pic = imageOriginal.data[y+x+counter2];

			if (abs(image_x.data[y + x + counter2] - image_x.data[x + y + counter2 + 1]) > threshold) {
				pic[y + x + counter2] = 255;
				/*	cout << "255" << endl;
					sleep_for(milliseconds(100));*/
			}
			else {
				pic[y + x + counter2] = 0;
				/*cout << "1" << endl;
				sleep_for(milliseconds(100));*/
			}
			counter++;
		}
		counter2 = counter;
	}
}

void GradientMethod_y(Mat& image_y, int threshold) {
	int counter = 0;
	int counter2 = 0;
	//int lastIntensity;

	Mat transposed_img;
	transpose(image_y, transposed_img);

	unsigned char* pic = transposed_img.data; // placeholder for image data

	//imshow("transposed", transposed_img); // Show our image inside the created window.

	for (int y = 1; y < transposed_img.rows; ++y) {

		for (int x = 1; x < transposed_img.cols; ++x) {

			//*pic = imageOriginal.data[y+x+counter2];

			if (abs(transposed_img.data[y + x + counter2] - transposed_img.data[x + y + counter2 + 1]) > threshold) {
				pic[y + x + counter2] = 255;
				/*	cout << "255" << endl;
					sleep_for(milliseconds(100));*/
			}
			else {
				pic[y + x + counter2] = 0;
				/*cout << "1" << endl;
				sleep_for(milliseconds(100));*/
			}
			counter++;
		}
		counter2 = counter;
	}
	transpose(transposed_img, image_y);
}

//void GaussianSmoothing(Mat imageOriginal, Mat imageFiltered) {
	// It is better to iterate over a image before edge detection is used on it.
//}

//void SobelKernelIteration(Mat imageOriginal, Mat imageFiltered) {
	/*
	* Instead of using difficult math, we can use convolution with e.g. sobel kernel to find edges.
	*/
	//}

void CorrelationMethod(Mat& image1, Mat& image2) {
	Mat kernel;
	unsigned char* pic = image2.data; // placeholder for image data

	int i = 0;

	//for (int p = 0; p < ; ++p) {
	//	for (int y = 0 + i; y < 10; ++y) {
	//		for (int x = 0; x < 10; ++x) {
	//			kernel.at<uchar>(y, x) = image1.at<uchar>(y, x);
	//		}
	//	}
	//	i++;
	//}

	for (int y = 13; y < 13+10; ++y) {
		for (int x = 13; x < 13+10 ; ++x) {
			kernel.at<uchar>(y - 13, x - 13) = image1.at<uchar>(y - 13, x - 13);
		}
	}

	int m1 = 0;
	int m1_tmp = 0;
	int m_final = 255*10;
	int x_co, y_co;

	for (int i = 5; i < image1.rows - 5; i++)
	{
		for (int j = 5; j < image1.cols - 5; j++)
		{
			for (int k = i - 5; k < i + 6; k++)
			{
				for (int l = j - 5; l < j + 6; l++)
				{
					m1 += abs(static_cast<int>(image2.at<uchar>(k, l) - static_cast<int>(image1.at<uchar>(k, l));
				}
			}
			if (m1 < m_final) { 
				m_final = m1;
				x_co = i;
				y_co = j;
				cout << m_final << endl;
			}
			m1 = 0;
		}
	}
}

Mat Laplacian_filter(Mat& image) {

	int laplace_mask[9] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 }; // Laplacian kernel
	int tmp = 0;
	int counter = 0;
	Mat laplaceImage = Mat::ones(image.rows - 2, image.cols - 2, CV_32F);

	for (int i = 1; i < image.rows - 1; i++)
	{
		for (int j = 1; j < image.cols - 1; j++)
		{

			for (int k = i - 1; k < i + 2; k++)
			{
				for (int l = j - 1; l < j + 2; l++)
				{
					tmp += laplace_mask[counter] * static_cast<int>(image.at<uchar>(k, l));
					counter++;
				}
			}
			//std::cout << tmp << std::endl;
			laplaceImage.at<float>(i - 1, j - 1) = tmp;
			tmp = 0;
			counter = 0;
		}
	}
	return laplaceImage;
}

int main(int argc, char** argv)
{
	//String fileName = "PEN.pgm";
	String fileName1 = "PIC1_L.png";
	String fileName2 = "PIC1_R.png";

	Mat image1_L_RGB = imread(fileName1);
	Mat image2_R_RGB = imread(fileName2);

	//Mat image1 = imread(fileName);
	Mat image1_L; // Grayscale image
	Mat image2_R; // Grayscale image

	cvtColor(image1_L_RGB, image1_L, COLOR_BGR2GRAY);
	cvtColor(image1_L_RGB, image2_R, COLOR_BGR2GRAY);

	imshow("Grayscale image of PIC1_L.png", image1_L); // Show our image inside the created window.
	imshow("Grayscale image of PIC1_R.png", image2_R); // Show our image inside the created window.

	//Mat image1_x = image1.clone();
	//Mat image1_y = image1.clone();

	//// Check for failure
	//if (image1.empty())
	//{
	//	cout << "Could not open or find the image" << endl;
	//	cin.get(); //wait for any key press
	//	return -1;
	//}

	//GradientMethod_x(image1_x, 15);
	//GradientMethod_y(image1_y, 15);

	//String windowName2 = "image1_x. Grayscale image of pen 1"; //Name of the window
	//imshow(windowName2, image1_x); // Show our image inside the created window.

	//String windowName3 = "image1_y. Grayscale image of pen 1"; //Name of the window
	//imshow(windowName3, image1_y); // Show our image inside the created window.

	//Mat img;
	////hconcat(image1_x, image1_y, img);

	//img = image1_x + image1_y;
	//String windowName1 = "image1_x+image1_y. Grayscale image of pen 1"; //Name of the window
	//imshow(windowName1, img); // Show our image inside the created window.

	waitKey(0);

	return 0;
}