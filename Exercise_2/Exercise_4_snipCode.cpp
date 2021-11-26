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

// https://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#the-efficient-way

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

	//Mat& ScanImage(image1);

	// ----Using ptr--------------------------------------
	
	String isit;
	
	if (image1.isContinuous()) isit = "Yes"; else isit = "No";
	
	cout << "Is the image continuous? " << isit << endl; 
	// if it is continuous, we should treat the arrays as 1D vectors.
	
	int countTo = 20;
	int widthX = image1.rows;
	int heightY = image1.cols;
	int img_size = widthX * heightY;

	unsigned char* pic; // placeholder for image data
	pic = image1.ptr<uchar>(0);

	//for (int i = 0; i < nRows; i++) {
	for (int i = 0; i < 20; i++) {

		cout << static_cast<double>(pic[i]);
		//cout << pic[i];
		
		if (i != countTo - 1)
			cout << ", ";
	}

	cout << endl;

	for (int i = 0; i < 20; i++) {
		cout << static_cast<unsigned>(image1.data[i]);
		if (i != countTo - 1)
			cout << ", ";
	}

	cout << endl;
	cout << "This is it: " << static_cast<unsigned>(image1.data[0]) << endl;

	// ---------Before function-------------------------------------------------------------------------------------------------------------
	//int countTo = 20;
	//unsigned char* pic = image1.data; // placeholder for image data
	//for (int i = 0; i < countTo; ++i) {
	//	*pic = image1.data[i];
	//	//*pic = image1.ptr<uchar>(i);
	//	cout << static_cast<unsigned>(*pic);
	//	if (i != countTo - 1)
	//		cout << ", ";
	//}
	//cout << endl;
	//for (int i = 0; i < countTo; i++) {
	//	cout << static_cast<unsigned>(image1.data[i]);
	//	if (i != countTo - 1)
	//		cout << ", ";
	//}
	// ------------------------------------------------------------------------------------------------------------------------
	
	unsigned char* pic = image1.data; // placeholder for image data
	
	// Variables that I define
	const int MAX_RAND = 10; // image1.rows * image1.cols
	int pos = 0;  // y* image1.cols+x;
	int count = 0;
	int rimx[1];
	int rimy[1];
	int B = image1.step;
	int end = image1.rows * image1.cols;

	// Given variables from the exercise
	int randx[MAX_RAND];
	int randy[MAX_RAND];
	int newpos = pos; // pos equals the starting position in the image ( = y* Width + x)
	int local_tresh = 0;
	int draw_type = 0;

	
	// 0L: type Long = 0
	while (newpos >= 0L && newpos < end)
	{
		rimx[count] = newpos % B; // save current position in list
		rimy[count] = newpos / B;
		count++;
		draw_type = (draw_type + 6) % 8; // Select next search direction

		cout << "Tester" << endl;
		
		switch (draw_type) // If draw_type = 0, do case 0, if draw_type = 5, do case 5...
		{
		case 0:  if (pic[newpos + 1    ] > local_tresh) { newpos += 1;     draw_type = 0; break; }
		case 1:  if (pic[newpos + B + 1] > local_tresh) { newpos += B + 1; draw_type = 1; break; }
		case 2:  if (pic[newpos + B    ] > local_tresh) { newpos += B;     draw_type = 2; break; }
		case 3:  if (pic[newpos + B - 1] > local_tresh) { newpos += B - 1; draw_type = 3; break; }
		case 4:  if (pic[newpos - 1    ] > local_tresh) { newpos -= 1;     draw_type = 4; break; }
		case 5:  if (pic[newpos - B - 1] > local_tresh) { newpos -= B + 1; draw_type = 5; break; }
		case 6:  if (pic[newpos - B    ] > local_tresh) { newpos -= B;     draw_type = 6; break; }
		case 7:  if (pic[newpos - B + 1] > local_tresh) { newpos -= B - 1; draw_type = 7; break; }
		case 8:  if (pic[newpos + 1    ] > local_tresh) { newpos += 1;	   draw_type = 0; break; }
		case 9:  if (pic[newpos + B + 1] > local_tresh) { newpos += B + 1; draw_type = 1; break; }
		case 10: if (pic[newpos + B    ] > local_tresh) { newpos += B;     draw_type = 2; break; }
		case 11: if (pic[newpos + B - 1] > local_tresh) { newpos += B - 1; draw_type = 3; break; }
		case 12: if (pic[newpos - 1    ] > local_tresh) { newpos -= 1;     draw_type = 4; break; }
		case 13: if (pic[newpos - B - 1] > local_tresh) { newpos -= B + 1; draw_type = 5; break; }
		case 14: if (pic[newpos - B    ] > local_tresh) { newpos -= B;     draw_type = 6; break; }
		}

		cout << "Tester2" << endl;
		// If we are back at the beginning, we declare success
		if (newpos == pos) {
			break;
			cout << "We went back at the beginning so the loop was ended." << endl;
		}
		// Abort if the contour is too complex.
		if (count >= MAX_RAND) {
			break;
			cout << "The contour is to complex so the loop was ended." << endl;
		}
	}

	cout << "Tester3" << endl;

	return 0;
}