#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>
#include <array>
#include <math.h>
#include <vector>

using namespace cv;
using namespace std;


void imageIteration(Mat imageOriginal, Mat imageFiltered);
void imageIteration2(Mat imageOriginal, Mat imageFiltered);
void imageIteration3(Mat imageOriginal, Mat imageFiltered);
double LowPassFilter(Mat imageOriginal, int i, int j);
double HighPassFilter(Mat imageOriginal, int i, int j);
int MedianFilter(Mat imageOriginal, int i, int j);

void imageIteration(Mat imageOriginal, Mat imageFiltered) {
	/*
	     This is a function to iterate over grayscale images
		 and add a filter to a copy of it.
	*/
	int width  = imageOriginal.rows;
	int height = imageOriginal.cols;
	//cout << "width: " << width << endl;
	//cout << "height: " << height << endl;
	
	for (int j = 1; j < height-1; j++) {
		for (int i = 1; i < width-1; i++) {
			imageFiltered.at<uchar>(i, j) = (uchar)LowPassFilter(imageOriginal, i, j);
			//cout << "imIt j: " << j << endl;
			//cout << "imIt i: " << i << endl;
		}
	}
}

void imageIteration2(Mat imageOriginal, Mat imageFiltered) {
	/*
		 This is a function to iterate over grayscale images
		 and add a filter to a copy of it.
	*/
	int width = imageOriginal.rows;
	int height = imageOriginal.cols;
	//cout << "width: " << width << endl;
	//cout << "height: " << height << endl;

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			imageFiltered.at<uchar>(i, j) = (uchar)HighPassFilter(imageOriginal, i, j);
			//cout << "imIt j: " << j << endl;
			//cout << "imIt i: " << i << endl;
		}
	}
}

void imageIteration3(Mat imageOriginal, Mat imageFiltered) {
	/*
		 This is a function to iterate over grayscale images
		 and add a filter to a copy of it.
	*/
	int width = imageOriginal.rows;
	int height = imageOriginal.cols;
	//cout << "width: " << width << endl;
	//cout << "height: " << height << endl;

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			imageFiltered.at<uchar>(i, j) = (uchar)MedianFilter(imageOriginal, i, j);
			//cout << "imIt j: " << j << endl;
			//cout << "imIt i: " << i << endl;
		}
	}
}

double LowPassFilter(Mat imageOriginal, int i, int j) {
	/*
	* low-pass filters: a mean (‘average’) filter and a binomial filter that smooth the image.
	                    Compared to the simple averaging image, edge enhancement with the binomial 
						filter is notably smoother; 

	p_{ij} = C_kl * sum_{k=i-n -> i+n} * sum_{l=j-n -> j+n} q_kl
	
	where C_kl = 1/(2n+1)^2
	      q is the original image
		  p is the filtered image
		  i and j are the (x,y) coordinates of the specific pixel
		  "i-1 + i+1 = 3"
	*/

	int n = 1.0;          int m = 1;
	int k = i - n;        int l = j - n; 
	int k_top = i + n;    int l_top = j + n;

	double num = (2 * (double)n) + 1;
	double num2 = num * num;
	double C_kl = 1.0 / num2;
	double p_sum = 0;

	for (int p = k; p <= k_top; p++) {
		for (int q = l; q <= l_top; q++) {
			//cout << "p: " << p << endl;
			//cout << "q: " << q << endl;
			p_sum = p_sum + imageOriginal.at<uchar>(p, q);
			//cout << "C_kl: " << C_kl << endl;
		}
	}

	return C_kl*p_sum;
}

double HighPassFilter(Mat imageOriginal, int i, int j) {
	/*
	* low-pass filters: a mean (‘average’) filter and a binomial filter that smooth the image.
						Compared to the simple averaging image, edge enhancement with the binomial
						filter is notably smoother;

	p_{ij} = C_kl * sum_{k=i-n -> i+n} * sum_{l=j-n -> j+n} q_kl

	where C_kl = 1/(2n+1)^2
		  q is the original image
		  p is the filtered image
		  i and j are the (x,y) coordinates of the specific pixel
		  "i-1 + i+1 = 3"
	*/


	 
	//double[3][3] kernel = { {0,-1,0},{-1,4,-1},{0,-1,0} };
	
	double** kernel;
	kernel = new double* [3];
	for (int i = 0; i < 3; i++) {
		kernel[i] = new double[3];
	}
	
	kernel[0][0] = 0;   kernel[0][1] = -1;   kernel[0][2] = 0;
	kernel[1][0] = -1;  kernel[1][1] = 4;    kernel[1][2] = -1;
	kernel[2][0] = 0;   kernel[2][1] = -1;   kernel[2][2] = 0;
	
	int n = 1.0;          int m = 1;
	int k = i - n;        int l = j - n;
	int k_top = i + n;    int l_top = j + n;

	double num = (2 * (double)n) + 1;
	double num2 = num * num;
	double C_kl = 1.0 / num2;
	double p_sum = 0;

	int xc = 0, yc = 0;

	for (int p = k; p <= k_top; p++) {
		for (int q = l; q <= l_top; q++) {
			//cout << "p: " << p << endl;
			//cout << "q: " << q << endl;
			p_sum = p_sum + kernel[xc][yc] * imageOriginal.at<uchar>(p, q);
			yc++;
			//cout << "C_kl: " << C_kl << endl;
		}
		xc++;
		yc = 0;
	}

	return abs(p_sum);
}

int MedianFilter(Mat imageOriginal, int i, int j) {
	/*
	* Median filter = quantile filter = fractile filter = percentile filter
	*/

	int median;
	int nn = 9;
	int * sortArr = new int[nn];
	//vector<int> sortArr;

	int width = imageOriginal.rows;
	int height = imageOriginal.cols;

	int n = 1.0;          int m = 1;
	int k = i - n;        int l = j - n;
	int k_top = i + n;    int l_top = j + n;

	//double num = (2 * (double)n) + 1;
	//double num2 = num * num;
	//double C_kl = 1.0 / num2;
	//double p_sum = 0;

	//int xc = 0, yc = 0;
	int count = 0;

	for (int p = k; p <= k_top; p++) {
		for (int q = l; q <= l_top; q++) {
			//cout << "p: " << p << endl;
			//cout << "q: " << q << endl;
			
			sortArr[count] = imageOriginal.at<uchar>(p, q);
			count++;
			//yc++;
			//cout << "C_kl: " << C_kl << endl;
		}
		//xc++;
		//yc = 0;
	}
	/*for (int i = 0; i < size(sortArr); i++) {
		cout << "Arr: " << sortArr[i] << endl;
	}*/

	//int length = sizeof(sortArr) / sizeof(sortArr[0]);
	//cout << "length: " << length << endl;


	sort(sortArr,sortArr+nn);
	median = sortArr[4];

	//for (int i = 0; i < size(sortArr); i++) {
	//	cout << "sortArr: " << sortArr[i] << endl;
	//}
	//cout << "median: " << median << endl;

	delete[] sortArr;
	//sortArr = 0;

	return median;
}

int main(int argc, char** argv)
{
	//String Pen1_string = "Gray_Image_2_1_r.pgm";
	String Pen1_string = "PIC1_R.png";
	Mat Pen_1 = imread(Pen1_string);
	Mat Pen_1_GRAY;

	cvtColor(Pen_1, Pen_1_GRAY, COLOR_RGB2GRAY); //Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	Mat Pen_1_filtered  = Pen_1_GRAY.clone();
	Mat Pen_1_filtered2 = Pen_1_GRAY.clone();

	// Check for failure
	if (Pen_1_GRAY.empty()) {
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	String windowName1 = "Grayscale image of pen 1, Original"; //Name of the window
	namedWindow(windowName1); // Create a window
	imshow(windowName1, Pen_1_GRAY); // Show our image inside the created window.

	//float BB = 1 / pow((2 * 1 + 1), (2 * 1 + 1));
	//cout << "BB: " << BB << endl;

	char wk;					  // Initialise the waitKey
	
	double e1, e2, e3, e4;
	double time1, time2;
	double diff;
	String text1 = "Manual implementation is faster.";
	String text2 = "OpenCV is faster";
	String text3;

	/*
	int FilterChoise;

	cout << "0: Low Pass Filter" << endl;
	cout << "1: High Pass Filter";
	cin >> FilterChoise;
	cout << endl;
	cout << "You chose: " << FilterChoise << endl;
	*/
	while (1) {
		wk = waitKey(0);	      // 0 is the delay in milliseconds, how long button is holded down
		if (wk == 'a') {
			e1 = getTickCount();
			imageIteration(Pen_1_GRAY, Pen_1_filtered);
			e2 = getTickCount();
			String windowName2 = "Grayscale image of pen 1, average filtered"; //Name of the window
			namedWindow(windowName2); // Create a window
			imshow(windowName2, Pen_1_filtered); // Show our image inside the created window.

			e3 = getTickCount();
			float sobelx_data[9] = { 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111 };
			cv::Mat kernel = cv::Mat(3, 3, CV_32F, sobelx_data);
			filter2D(Pen_1_GRAY, Pen_1_filtered2 , -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
			e4 = getTickCount();
			imshow("filter2D", Pen_1_filtered2);

			time1 = (e2 - e1) / getTickFrequency();
			time2 = (e4 - e3) / getTickFrequency();
			cout << "The time using manual implementation: " << time1 << endl;
			cout << "The time using openCV: " << time2 << endl;
			if (time1 > time2) {
				diff = time1 - time2; 
				text3 = text2;
			}
			else {
				diff = time2 - time1;
				text3 = text1;
			}
			cout << "Difference: " << diff << endl;
			cout << "Result: " << text3 << endl;
		}
		if (wk == 's') {
			imageIteration2(Pen_1_GRAY, Pen_1_filtered);
			String windowName2 = "Grayscale image of pen 1, high pass filtered"; //Name of the window
			namedWindow(windowName2); // Create a window
			imshow(windowName2, Pen_1_filtered); // Show our image inside the created window.

			float sobelx_data[9] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
			cv::Mat kernel = cv::Mat(3, 3, CV_32F, sobelx_data);
			filter2D(Pen_1_GRAY, Pen_1_filtered2, -1, kernel, Point(-1, -1), 0, cv::BORDER_DEFAULT);
			imshow("filter2D", Pen_1_filtered2);
		}

		if (wk == 'd') {
			imageIteration3(Pen_1_GRAY, Pen_1_filtered);
			String windowName2 = "Grayscale image of pen 1, median filtered"; //Name of the window
			namedWindow(windowName2); // Create a window
			imshow(windowName2, Pen_1_filtered); // Show our image inside the created window.

			//float sobelx_data[9] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
			//cv::Mat kernel = cv::Mat(3, 3, CV_32F, sobelx_data);
			//filter2D(Pen_1_GRAY, Pen_1_filtered2, -1, kernel, Point(-1, -1), 0, cv::BORDER_DEFAULT);
			//imshow("filter2D", Pen_1_filtered2);
		}

		if (wk == 'q') {
			break;
		}
	}

	waitKey(0); // Wait for any keystroke in the window
	//delete[];
	return 0;
}