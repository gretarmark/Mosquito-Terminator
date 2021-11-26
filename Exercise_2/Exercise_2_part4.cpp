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
	String Pen_1;
	Pen_1 = "Gray_Image_2_1_r.pgm";
	String Pen_2;
	Pen_2 = "Gray_Image_2_2_r.pgm";

	Mat Pen_1_Mat = imread(Pen_1);
	Mat Pen_2_Mat = imread(Pen_2);

	// Check for failure
	if (Pen_1_Mat.empty() || Pen_2_Mat.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	String windowName1 = "Pen 1"; //Name of the window
	namedWindow(windowName1); // Create a window
	imshow(windowName1, Pen_1_Mat); // Show our image inside the created window.
	
	String windowName2 = "Pen 2"; //Name of the window
	namedWindow(windowName2); // Create a window
	imshow(windowName2, Pen_2_Mat); // Show our image inside the created window.
	
	int t = 0;
	char wk;

	int com_1[2];
	int com_2[2];

	double orientation_angle[1];

	int width_1 = Pen_1_Mat.cols;				 
	int height_1 = Pen_1_Mat.rows;				 
	int width_2 = Pen_2_Mat.cols;
	int height_2 = Pen_2_Mat.rows;

	while (1) {
		wk = waitKey(0);
		if (wk == 'g')
			t = t + 1;
		if (wk == 'f')
			t = t - 1;
		if (wk == 'q')
			break;

		if (t > 194) t = 194;
		if (t <= 0) t = 1;

		cout << "t = " << t << endl;

		centerOfMass(Pen_1_Mat, width_1, height_1, com_1);
		centerOfMass(Pen_2_Mat, width_2, height_2, com_2);

		cout << "COM - x: " << com_1[0] << endl;
		cout << "COM - y: " << com_1[1] << endl;

		//orientation(imageCopy, width, height, orientation_angle);
		//orientation(Mat image, int imageWidth, int imageHeight, double orientation_angle[])

	}
	
	waitKey(0); // Wait for any keystroke in the window

	return 0;
}