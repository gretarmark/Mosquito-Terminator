#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>
#include <chrono>
#include <thread>
#include <array>
#include <math.h>

using namespace cv;
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void histDisplay(int histogram[], const char* name, int width, int height, int t)
{
	// Width: columns
	// Height: rows
	int hist[256];
	for (int i = 0; i < 256; i++)
	{
		hist[i] = histogram[i];
	}
	// draw the histograms
	int hist_w = width; int hist_h = height;
	int bin_w = cvRound((double)hist_w / 256);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = hist[0];
	for (int i = 1; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows
	for (int i = 0; i < 256; i++)
	{
		hist[i] = ((double)hist[i] / max) * histImage.rows;
	}


	// draw the intensity line for histogram
	for (int i = 0; i < 256; i++)
	{
		line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(0, 0, 0), 1, 8, 0);
	}

	unsigned int position = t;
	line(histImage, Point(position, 0), Point(position, height), Scalar(50, 50, 255), 1, 8, 0);

	// display histogram
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, histImage);
}



	void centerOfMass(Mat image, int imageWidth, int imageHeight, int arr[])
	{
		int SumX = 0;
		int SumY = 0;
		int num = 0;
		for (int i = 0; i < imageWidth; i++)
		{
			for (int j = 0; j < imageHeight; j++)
			{
				if (image.at<uchar>(j, i) == 0)
				{
					SumX = SumX + i;
					SumY = SumY + j;
					num = num + 1; // The black area
				}
			}
		}

		SumX = SumX / num;
		SumY = SumY / num;
		arr[0] = SumX;
		arr[1] = SumY;
		// The coordinate (SumX,SumY) is the center of the image mass
	}

	void orientation(Mat image, int imageWidth, int imageHeight, double orientation_angle[])
	{
		double mu_00 = 0;
		double mu_20 = 0;
		double mu_02 = 0;
		double mu_11 = 0;

		double SumX = 0;
		double SumY = 0;
		int num = 0;
		for (int i = 0; i < imageWidth; i++)
		{
			for (int j = 0; j < imageHeight; j++)
			{
				if (image.at<uchar>(j, i) == 0)
				{
					SumX = SumX + i;
					SumY = SumY + j;
					num = num + 1;
				}
			}
		}

		SumX = SumX / num;
		SumY = SumY / num;

		for (int i = 0; i < imageWidth; i++)
		{
			for (int j = 0; j < imageHeight; j++)
			{
				if (image.at<uchar>(j, i) == 0)
				{

					mu_20 = mu_20 + pow((double(i) - double(SumX)), 2);
					mu_02 = mu_02 + pow((double(j) - double(SumY)), 2);
					mu_11 = mu_11 + (double(i) - double(SumX)) * (double(j) - double(SumY));
				}
			}
		}
		mu_00 = double(num);
		mu_11 = mu_11 / mu_00;
		mu_20 = mu_20 / mu_00;
		mu_02 = mu_02 / mu_00;
		//orientation_angle[0] = atan(2 * double(mu_11) / ((double(mu_20) - double(mu_02))))/2;
		orientation_angle[0] = atan2(2 * double(mu_11), ((double(mu_20) - double(mu_02)))) / 2;
		cout << "Angle: " << orientation_angle[0] << endl;
		//cout << "center X: " << SumX << endl;
		//cout << "center Y: " << SumY << endl;
		//cout << "mu_11: " << mu_11 << endl;
		//cout << "mu_02: " << mu_02 << endl;
		//cout << "mu_20: " << mu_20 << endl;
	}

int main(int argc, char** argv)
{
	String fileName;
	fileName = "PEN.pgm";

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

	String windowName = "Image"; //Name of the window
	namedWindow(windowName); // Create a window
	imshow(windowName, image1); // Show our image inside the created window.
	//waitKey(0); // Wait for any keystroke in the window
	//destroyWindow(windowName); //destroy the created window

	// Histogram
	unsigned char* channel = image1.data;  // pre-allocated array holding image data for 
							 // the color channel or the grayscale image.
	unsigned char value = 0; // index value for the histogram (not really needed)
	int histogram[256];	   	 // histogram array - remember to set to zero initially
	int width = image1.cols;				 // say, 320
	int height = image1.rows;				 // say, 240
	int k = 256;

	while (k-- > 0)
		histogram[k] = 0; // reset histogram entry

	for (int i = 0; i < width * height; i++) // Here we need to multiply by 3 if we have RGB image
												 // because we have 3 values for each pixel. If we use
												 // grayscale image we don't need to multiply it by 3.
	{
		value = channel[i];
		histogram[value] += 1;
	}
	

	//cout << "Image channel: " << image_grays.channels() << endl;
	//cout << "Image depth: " << image_grays.depth() << endl;
	//cout << "Image type: " << image_grays.type() << endl;

	

	int t = 0;
	char wk;
	int arr[2];
	double orientation_angle[1];
	while (1) {
		//if (waitKey(10) == 'f')
		//	wk = 'f';
		//else if()
		
		wk = waitKey(0);
		if (wk == 'g')
			t = t + 1;
		if (wk == 'f')
			t = t - 1;
		if (wk == 'q')
			break;
		//if (waitKey(10) == wk) // f: forward
		//{
		if (t > 194) t = 194;
		if (t <= 0) t = 1;

		Mat image1_new;
		cvtColor(image1, image1_new, COLOR_GRAY2RGB);

		cout << "t = " << t << endl;

			histDisplay(histogram, "Histogram of the image", width, height,t);
			Mat imageCopy = image1.clone();
			unsigned char* channel2 = imageCopy.data;

			for (int i = 0; i < width * height; i++) {
				if (channel[i] >= t)
					channel2[i] = 255;
				else 
					channel2[i] = 0;
			}

			String windowName2 = "Copy of the image";
			namedWindow(windowName2);
			imshow(windowName2, imageCopy);
			centerOfMass(imageCopy, width, height, arr);
			orientation(imageCopy, width, height, orientation_angle);
			
			cout << "Center of mass: (x,y) = (" << arr[0] << ", " << arr[1] << ")" << endl;
			//line(image1_new, Point(arr[0], arr[1]), Point(arr[0]+10, arr[1]+10), Scalar(50, 50, 255), 2, 8, 0);
			line(image1_new, Point(arr[0]-5, arr[1]-5), Point(arr[0]+5, arr[1]+5), Scalar(50, 50, 255), 2, 8, 0);
			//line(image1_new, Point(arr[0]+10, arr[1]), Point(arr[0], arr[1]+10), Scalar(50, 50, 255), 2, 8, 0);
			line(image1_new, Point(arr[0]+5, arr[1]-5), Point(arr[0]-5, arr[1]+5), Scalar(50, 50, 255), 2, 8, 0);
			
			line(image1_new, Point(arr[0] - 100 * cos(orientation_angle[0]), arr[1] - 100 * sin(orientation_angle[0])), Point(arr[0] + 100 * cos(orientation_angle[0]), arr[1] + 100 * sin(orientation_angle[0])), Scalar(50, 50, 255), 2, 8, 0);

			imshow(windowName, image1_new); // Show our image inside the created window.
			
			//if (wk == 'u')
			//	cout << "Dark area: " << Dark_Area;
			//t = t + 5;
		//}
	}	

	waitKey(0); // Wait for any keystroke in the window

	return 0;
}