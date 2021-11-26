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

//void imageIteration(Mat imageOriginal, Mat imageFiltered) {
//	/*
//		 This is a function to iterate over grayscale images
//		 and add a filter to a copy of it.
//	*/
//	int width = imageOriginal.rows;
//	int height = imageOriginal.cols;
//	//cout << "width: " << width << endl;
//	//cout << "height: " << height << endl;
//
//	for (int j = 1; j < height - 1; j++) {
//		for (int i = 1; i < width - 1; i++) {
//			imageFiltered.at<uchar>(i, j) = (uchar)checkThresholdOfFirstDerivative(imageOriginal, i, j);
//			//cout << "imIt j: " << j << endl;
//			//cout << "imIt i: " << i << endl;
//		}
//	}
//}

//void checkThresholdOfFirstDerivative(Mat imageOriginal, int lastValue, int threshold) {
	/*
	* Instead of using difficult math, we can use convolution with e.g. sobel kernel to find edges.
	* 
	* When we check the threshold of a first derivative in x direction we are checking local extrema in one dimension.
	* We want to extend this idea for 2 dimensional images, therefore we use partial derivative from Calculus. This is
	* how we represent the amount of change along each dimension. This brings us to the Gradient operator
	*         delta I = [dI/dx , dI/dy] where I stands for Intensity
	* if we have delta I = [dI/dx , 0] the direction is along the x axis
	* if we have delta I = [0 , dI/dy] the direction is along the y axis
	* if we have delta I = [dI/dx, dI/dy] the direction can be somewhere else, it is a vector so it depend on the values.
	* 
	* The gradient magnitude is found by S = ||delta I|| = sqrt{(dI/dx)^2 + (dI/dy)^2}
	* The gradient orientation is found by theta = arctan{(dI/dy) / (dI/dx)}
	*
	* We can use finite difference approximation:
	*    dI/dx = 1/(2*epsilon) ((I_{i+1,j+1} - I_{i,j+1}) + (I_{i+1,j} - I_{i,j}))
	*    dI/dy = 1/(2*epsilon) ((I_{i+1,j+1} - I_{i+1,j}) + (I_{i,j+1} - I_{i,j})) where epsilon is the physical length between the pixels
	*
	* If ||delta I(x,y)|| <  T where T is a threshold, it is definitely not an edge
	* If ||delta I(x,y)|| >= T it is definitely an edge
	*
	* This can be extended for two thresholds, T0 < T1
	*
	* If ||delta I(x,y)|| <   T0 it is definitely not an edge
	* If ||delta I(x,y)|| >=  T1 it is definitely an edge
	* If T0 <= ||delta I(x,y)|| < T1 it is an edge if a neighboring pixel is definitely an edge
	*/

	//difference = dlastValue - currentValue  // Derivative of f, called df/dx. The local extrema of 1st derivative indicates edges
	//	if (abs(difference) > threshold) {  // We have to take the absolute value of the 1st derivative and use that
	//		// Value in this pixel = intensity of 255
	//	}
	//	else {
	//		// Value in this pixel = intensity of 0
	//	}

	//imageFiltered.at<uchar>(i, j)

//}

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

	//Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	cvtColor(image1_RGB, image1, COLOR_BGR2GRAY);

	// Check for failure
	if (image1.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	//String windowName1 = "Grayscale image of pen 1"; //Name of the window
	//imshow(windowName1, image1); // Show our image inside the created window.

	// ------------------------------------------
	cout << "Channels: " << image1.channels() << endl;
	int img_size = image1.rows * image1.cols * image1.channels();

	unsigned char* pic = image1.data; // placeholder for image data
	int lastIntensity;
	int thresH = 15;

	for (int x = 0; x < image1.rows; ++x) {
		for (int y = 0; y < image1.cols; ++y) {

		}

	}

	for (int i = 0; i < img_size; i++) { 
		*pic = image1.data[i];

		//cout << abs(image1.data[i] - image1.data[i + 1]) << endl;
		//sleep_for(milliseconds(100));

		//cout << static_cast<unsigned>(*pic) << endl;
		if (abs(image1.data[i] - image1.data[i + 1]) > thresH) {
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

	//for (int i = 0; i < img_size; ++i) {
	//	*pic = image1.data[i];

	//	cout << static_cast<unsigned>(*pic) << endl;
	//	
	//	sleep_for(milliseconds(100));
	//}

	String windowName1 = "Grayscale image of pen 1"; //Name of the window
	imshow(windowName1, image1); // Show our image inside the created window.

	/*cout << endl;
	
	for (int i = 0; i < img_size; i++) {
		cout << static_cast<unsigned>(image1.data[i]);
		if (i != countTo - 1)
			cout << ", ";
	}*/

	// --------------------------------------------

	//String windowName2 = "Colour image of pen 1 with contour lines"; //Name of the window
	//imshow(windowName2, image1_RGB); // Show our image inside the created window.

	waitKey(0);

	return 0;
}