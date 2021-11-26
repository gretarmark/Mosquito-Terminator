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



int main(int argc, char** argv)
{
	String fileName = "PEN.pgm";
	Mat image1_RGB = imread(fileName);
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

	String windowName1 = "Grayscale image of pen 1"; //Name of the window
	namedWindow(windowName1); // Create a window
	imshow(windowName1, image1); // Show our image inside the created window.

	// Draw contours using OpenCV -------------------------------------------------------------
	int thresh = 100;
	Mat canny_output;

	Canny(image1, canny_output, thresh, 2* thresh);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	//findContours(input_image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	findContours(canny_output, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	
	for (int i = 0; i < (int)contours.size(); i++) {
		drawContours(image1_RGB, contours, (int)i, cv::Scalar(0, 255, 0), 2, 8, hierarchy, 1);
	}
	cout << "contour test1" << endl;

	String windowName2 = "Colour image of pen 1 with contour lines"; //Name of the window
	namedWindow(windowName2); // Create a window
	imshow(windowName2, image1_RGB); // Show our image inside the created window.

	waitKey(0);	

return 0;
}