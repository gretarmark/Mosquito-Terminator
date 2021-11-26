#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>
#include <array>
#include <math.h>

using namespace cv;
using namespace std;

void histDisplay(int histogram[], const char* name, int width, int height, int t)
{
	// Width: columns
	// Height: rows
	int hist[256];
	for (int i = 0; i < 256; i++) {
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
	
	/*
	cout << "SIZEOF hist: " << sizeof(hist) / sizeof(hist[0]) << endl;
	cout << "SIZEOF histImage: " << histImage.size() << endl;
	cout << "height histImage: " << histImage.rows << endl;
	cout << "width histImage: " << histImage.cols << endl;
	*/

	// display histogram
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, histImage);
}




void centerOfMass(Mat image, int imageWidth, int imageHeight, int arr[]) {
	int SumX = 0; int SumY = 0; int num = 0;
	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {
			if (image.at<uchar>(j, i) == 0) {
				SumX = SumX + i;
				SumY = SumY + j;
				num = num + 1; // The black area
			}
		}
	}
	if (num < 1) num = 1;
	SumX = SumX / num; 
	SumY = SumY / num;
	arr[0] = SumX; // x coordinate for center of mass
	arr[1] = SumY; // y coordinate for center of mass
}

void orientation(Mat image, int imageWidth, int imageHeight, double orientation_angle[], double eta_value[], double i_value[])
{
	double mu_00 = 0;  double mu_20 = 0;
	double mu_02 = 0;  double mu_11 = 0;
	double SumX = 0;   double SumY = 0;  int num = 0;
	double eta_00 = 0; double eta_20 = 0;
	double eta_02 = 0; double eta_11 = 0; double i_val = 0;
	
	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {
			if (image.at<uchar>(j, i) == 0) {
				SumX = SumX + i;
				SumY = SumY + j;
				num = num + 1;
			}
		}
	}

	SumX = SumX / num;
	SumY = SumY / num;

	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {
			if (image.at<uchar>(j, i) == 0) {
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

	eta_00 = 1;
	eta_11 = mu_11 / (mu_00 * 2);
	eta_20 = mu_20 / (mu_00 * 2);
	eta_02 = mu_02 / (mu_00 * 2);

	i_val = eta_20 + eta_02;

	//orientation_angle[0] = atan(2 * double(mu_11) / ((double(mu_20) - double(mu_02))))/2;
	orientation_angle[0] = atan2(2 * double(mu_11), ((double(mu_20) - double(mu_02)))) / 2;
	cout << "Angle: " << orientation_angle[0] << endl;
	eta_value[0] = eta_00;
	eta_value[1] = eta_11;
	eta_value[2] = eta_20;
	eta_value[3] = eta_02;
	i_value[0] = i_val;
}

 
string type2str(int type) {
	// type2str is used to check what type of Mat matrix is used.
	// int type should be e.g. image.type() where
	// image is a Mat object: Mat image;
	// 
	// To print out the informations, it is used as follow:
	// string ty = type2str(image.type());
	// printf("Matrix: %s %dx%d \n", ty.c_str(), Pen_1_Mat.cols, Pen_1_Mat.rows);
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int main(int argc, char** argv)
{
	String Pen_1; Pen_1 = "Gray_Image_2_1_r.pgm";
	String Pen_2; Pen_2 = "Gray_Image_2_2_r.pgm";

	Mat Pen_1_Mat_GRAY; // Grayscale image
	Mat Pen_2_Mat_GRAY; // Grayscale image

	Mat Pen_1_Mat_RGB = imread(Pen_1); // Pen for orientation, Problem 2.4, image type: 8UC3 
	Mat Pen_2_Mat_RGB = imread(Pen_2); // Pen for orientation, Problem 2.4, image type: 8UC3

	Mat Pen_1_Mat; // Pen for orientation, Problem 2.4, image type: 8UC3 
	Mat Pen_2_Mat; // Pen for orientation, Problem 2.4, image type: 8UC3

	//Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	cvtColor(Pen_1_Mat_RGB, Pen_1_Mat, COLOR_RGB2GRAY);
	cvtColor(Pen_2_Mat_RGB, Pen_2_Mat, COLOR_RGB2GRAY);

	// Check for failure
	if (Pen_1_Mat.empty() || Pen_2_Mat.empty()) {
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	// Pen for orientation, Problem 2.4
	String windowName1 = "Grayscale image of pen 1"; //Name of the window
	namedWindow(windowName1); // Create a window
	imshow(windowName1, Pen_1_Mat); // Show our image inside the created window.
	
	// Pen for orientation, Problem 2.4
	String windowName2 = "Grayscale image of pen 2"; //Name of the window
	namedWindow(windowName2); // Create a window
	imshow(windowName2, Pen_2_Mat); // Show our image inside the created window.

	// Histogram
	// pre-allocated array holding image data for the color channel or the grayscale image.
	unsigned char* channel1_1 = Pen_1_Mat.data;    
	unsigned char* channel2_1 = Pen_2_Mat.data;    
	
	int histogram1[256];	   	                   // histogram array - remember to set to zero initially
	int histogram2[256];

	unsigned char value1 = 0;					   // index value for the histogram 
	unsigned char value2 = 0;

	int width1 = Pen_1_Mat.cols; // Data for Pen oriontation, Problem 2.4
	int height1 = Pen_1_Mat.rows;// Data for Pen oriontation, Problem 2.4
	int width2 = Pen_2_Mat.cols; // Data for Pen oriontation, Problem 2.4
	int height2 = Pen_2_Mat.rows;// Data for Pen oriontation, Problem 2.4
	
	int k1 = 256;
	int k2 = 256;

	while (k1-- > 0)
		histogram1[k1] = 0; // reset histogram entry
	while (k2-- > 0)
		histogram2[k2] = 0; // reset histogram entry
	
	/*
	Here we need to multiply by 3 if we have RGB image because we have 3 values for each pixel. 
	If we use grayscale image we don't need to multiply it by 3.
	*/
	for (int i = 0; i < width1 * height1; i++) {
		value1 = channel1_1[i];
		histogram1[value1] += 1;
	}
	for (int i = 0; i < width2 * height2; i++) {
		value2 = channel2_1[i];
		histogram2[value2] += 1;
	}

	int t1 = 0;					  // Used to count set the position of the red line on the histogram
	int t2 = 0;					  // Used to count set the position of the red line on the histogram

	int center_of_mass1[2];		  // Coordinates for center of mass
	int center_of_mass2[2];		  // Coordinates for center of mass
	
	double orientation_angle1[1]; // Angle for the Moment
	double orientation_angle2[1]; // Angle for the Moment

	double eta_value1[4];
	double eta_value2[4];
	double i_value1[1];
	double i_value2[1];

	char wk;					  // Initialise the waitKey

	while (1) {
		wk = waitKey(0);	      // 0 is the delay in milliseconds, how long button is holded down
		if (wk == 't') t1 = t1 + 1; if (wk == 'r') t1 = t1 - 1; if (wk == 'q') break;
		if (wk == 'g') t2 = t2 + 1; if (wk == 'f') t2 = t2 - 1;

		//if (t1 > 255) t1 = 255;   // if t > 194 we exceed the array bounds
		if (t1 <= 0)  t1 = 1;	  // if t < 1 we exceed the array bounds
		//if (t2 > 255) t2 = 255; 
		if (t2 <= 0)  t2 = 1;	  

		Mat Pen_1_Mat_GRAY2RGB; //image1_new
		cvtColor(Pen_1_Mat, Pen_1_Mat_GRAY2RGB, COLOR_GRAY2RGB);

		Mat Pen_2_Mat_GRAY2RGB; //image1_new
		cvtColor(Pen_2_Mat, Pen_2_Mat_GRAY2RGB, COLOR_GRAY2RGB);

		cout << "t1 = " << t1 << endl;
		cout << "t2 = " << t2 << endl;

		histDisplay(histogram1, "Histogram, Pen 1", width1, height1, t1);
		histDisplay(histogram2, "Histogram, Pen 2", width2, height2, t2);

		Mat Pen_1_Mat_cloned = Pen_1_Mat.clone();		// Create a new cloned image, type 8UC1
		Mat Pen_2_Mat_cloned = Pen_2_Mat.clone();

		unsigned char* channel1_2 = Pen_1_Mat_cloned.data; // we get the width and height
		unsigned char* channel2_2 = Pen_2_Mat_cloned.data;

		for (int i = 0; i < width1 * height1; i++) {
			if (channel1_1[i] >= t1) channel1_2[i] = 255;
			else                     channel1_2[i] = 0;
		}

		for (int i = 0; i < width2 * height2; i++) {
			if (channel2_1[i] >= t2) channel2_2[i] = 255;
			else                     channel2_2[i] = 0;
		}

		String Pen_1_Mat_cloned_windowName = "Copy of Pen 1";
		namedWindow(Pen_1_Mat_cloned_windowName);
		imshow(Pen_1_Mat_cloned_windowName, Pen_1_Mat_cloned);

		String Pen_2_Mat_cloned_windowName = "Copy of Pen 2";
		namedWindow(Pen_2_Mat_cloned_windowName);
		imshow(Pen_2_Mat_cloned_windowName, Pen_2_Mat_cloned);

		centerOfMass(Pen_1_Mat_cloned, width1, height1, center_of_mass1);
		orientation(Pen_1_Mat_cloned, width1, height1, orientation_angle1, eta_value1, i_value1);

		centerOfMass(Pen_2_Mat_cloned, width2, height2, center_of_mass2);
		orientation(Pen_2_Mat_cloned, width2, height2, orientation_angle2, eta_value2, i_value2);

		cout << "Center of mass1: (x,y) = (" << center_of_mass1[0] << ", " << center_of_mass1[1] << ")" << endl;
		cout << "Center of mass2: (x,y) = (" << center_of_mass2[0] << ", " << center_of_mass2[1] << ")" << endl;

		cout << "Pen1_eta_00 = " << eta_value1[0] << endl;
		cout << "Pen1_eta_11 = " << eta_value1[1] << endl;
		cout << "Pen1_eta_20 = " << eta_value1[2] << endl;
		cout << "Pen1_eta_02 = " << eta_value1[3] << endl;
		cout << "Pen1_i_value = " << i_value1[0] << endl;

		cout << "Pen2_eta_00 = " << eta_value2[0] << endl;
		cout << "Pen2_eta_11 = " << eta_value2[1] << endl;
		cout << "Pen2_eta_20 = " << eta_value2[2] << endl;
		cout << "Pen2_eta_02 = " << eta_value2[3] << endl;
		cout << "Pen2_i_value = " << i_value2[0] << endl;

		line(Pen_1_Mat_GRAY2RGB, Point(center_of_mass1[0] - 5, center_of_mass1[1] - 5), Point(center_of_mass1[0] + 5, center_of_mass1[1] + 5), Scalar(50, 50, 255), 2, 8, 0);
		line(Pen_1_Mat_GRAY2RGB, Point(center_of_mass1[0] + 5, center_of_mass1[1] - 5), Point(center_of_mass1[0] - 5, center_of_mass1[1] + 5), Scalar(50, 50, 255), 2, 8, 0);
		line(Pen_1_Mat_GRAY2RGB, Point(center_of_mass1[0] - 100 * cos(orientation_angle1[0]), center_of_mass1[1] - 100 * sin(orientation_angle1[0])), Point(center_of_mass1[0] + 100 * cos(orientation_angle1[0]), center_of_mass1[1] + 100 * sin(orientation_angle1[0])), Scalar(50, 50, 255), 2, 8, 0);

		line(Pen_2_Mat_GRAY2RGB, Point(center_of_mass2[0] - 5, center_of_mass2[1] - 5), Point(center_of_mass2[0] + 5, center_of_mass2[1] + 5), Scalar(50, 50, 255), 2, 8, 0);
		line(Pen_2_Mat_GRAY2RGB, Point(center_of_mass2[0] + 5, center_of_mass2[1] - 5), Point(center_of_mass2[0] - 5, center_of_mass2[1] + 5), Scalar(50, 50, 255), 2, 8, 0);
		line(Pen_2_Mat_GRAY2RGB, Point(center_of_mass2[0] - 100 * cos(orientation_angle2[0]), center_of_mass2[1] - 100 * sin(orientation_angle2[0])), Point(center_of_mass2[0] + 100 * cos(orientation_angle2[0]), center_of_mass2[1] + 100 * sin(orientation_angle2[0])), Scalar(50, 50, 255), 2, 8, 0);

		imshow(windowName1, Pen_1_Mat_GRAY2RGB); // Show our image inside the created window.
		imshow(windowName2, Pen_2_Mat_GRAY2RGB); // Show our image inside the created window.
		/*
		cout << "size of Pen_1_Mat: " << Pen_1_Mat.size() << endl;
		cout << "size of Pen_2_Mat: " << Pen_2_Mat.size() << endl;
		cout << "size of channel1_1: " << sizeof(channel1_1)/sizeof(channel1_1[0]) << endl;
		cout << "size of channel1_2: " << sizeof(channel1_2) / sizeof(channel1_2[0]) << endl;
		cout << "size of channel2_1: " << sizeof(channel2_1) / sizeof(channel2_1[0]) << endl;
		cout << "size of channel2_2: " << sizeof(channel2_2) / sizeof(channel2_2[0]) << endl;
		cout << "size of histogram1: " << sizeof(histogram1) / sizeof(histogram1[0]) << endl;
		cout << "size of histogram2: " << sizeof(histogram2) / sizeof(histogram2[0]) << endl;
		*/
	}

	waitKey(0); // Wait for any keystroke in the window

	return 0;
}