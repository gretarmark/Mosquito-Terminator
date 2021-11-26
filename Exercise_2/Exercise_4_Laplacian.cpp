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
	//String fileName = "PEN.pgm";
	String fileName = "PIC1_R.png";
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

	Mat img = Laplacian_filter(image1);
	//hconcat(image1_x, image1_y, img);

	String windowName1 = "Laplacian filtered grayscale image of pen 1"; //Name of the window
	imshow(windowName1, img); // Show our image inside the created window.

	waitKey(0);

	return 0;
}