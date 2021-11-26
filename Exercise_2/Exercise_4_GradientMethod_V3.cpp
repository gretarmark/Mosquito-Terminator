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
// https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#the-efficient-way

void GradientMethod(Mat& imageOriginal, int threshold) {
	int counter = 0;
	int counter2 = 0;
	int img_size = imageOriginal.rows * imageOriginal.cols * imageOriginal.channels();

	unsigned char* pic = imageOriginal.data; // placeholder for image data
	//int lastIntensity;

	for (int y = 2; y < imageOriginal.rows; ++y) {
		
		for (int x = 2; x < imageOriginal.cols; ++x) {
			
			//*pic = imageOriginal.data[y+x+counter2];
			
			if (abs(imageOriginal.data[y+x+counter2] - imageOriginal.data[x+y+counter2+1]) > threshold) {
				pic[y+x+counter2] = 255;
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
	String fileName = "PEN.pgm";
	//String fileName = "PIC1_R.png";
	Mat image1_RGB = imread(fileName);
	
	String windowName = "Original RGB image of pen 1"; //Name of the window
	imshow(windowName, image1_RGB); // Show our image inside the created window.

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

	GradientMethod(image1, 20);
	
	String windowName1 = "Grayscale image of pen 1"; //Name of the window
	imshow(windowName1, image1); // Show our image inside the created window.

	waitKey(0);

	return 0;
}