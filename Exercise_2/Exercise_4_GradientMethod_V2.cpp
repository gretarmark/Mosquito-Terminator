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

// https://soubhihadri.medium.com/image-processing-best-practices-c-part-2-c0988b2d3e0c



void GradientMethod(Mat& imageOriginal, int threshold) {
	int img_size = imageOriginal.rows * imageOriginal.cols * imageOriginal.channels();
	
	unsigned char* pic = imageOriginal.data; // placeholder for image data
	//int lastIntensity;

	/*for (int x = 0; x < imageOriginal.rows; ++x) {
		for (int y = 0; y < imageOriginal.cols; ++y) {

		}

	}*/

	for (int i = 0; i < img_size; i++) {
		*pic = imageOriginal.data[i];

		//cout << abs(image1.data[i] - image1.data[i + 1]) << endl;
		//sleep_for(milliseconds(100));

		//cout << static_cast<unsigned>(*pic) << endl;
		if (abs(imageOriginal.data[i] - imageOriginal.data[i + 1]) > threshold) {
			pic[i] = 255;
			/*	cout << "255" << endl;
				sleep_for(milliseconds(100));*/
		}
		else {
			pic[i] = 0;
			/*cout << "1" << endl;
			sleep_for(milliseconds(100));*/
		}

	}
}

//void GaussianSmoothing(Mat imageOriginal, Mat imageFiltered) {
	// It is better to iterate over a image before edge detection is used on it.
//}

//void SobelKernelIteration(Mat imageOriginal, Mat imageFiltered) {
	/*
	* Instead of using difficult math, we can use convolution with e.g. sobel kernel to find edges.
	*/
	//}


int main(int argc, char** argv)
{
	//String fileName = "PIC1_R.png";
	String fileName = "PEN.pgm";
	Mat image1_RGB = imread(fileName);
	//Mat image1 = imread(fileName);
	Mat image1; // Grayscale image

	cvtColor(image1_RGB, image1, COLOR_BGR2GRAY);

	// Check for failure
	if (image1.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	GradientMethod(image1, 15);

	String windowName1 = "Grayscale image of pen 1"; //Name of the window
	imshow(windowName1, image1); // Show our image inside the created window.

	waitKey(0);

	return 0;
}