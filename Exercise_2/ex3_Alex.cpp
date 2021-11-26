
#include <stdio.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
using namespace cv;
using namespace std;


int q(Mat image, int n, int m, int k, int l,int ** kernel);



void lowpassfilter(Mat image, Mat imagef, int imageWidth, int imageHeight, int f00, int f01, int f02, int f10, int f11, int f12, int f20, int f21, int f22) {
	//Mat imagef = image.clone();
	unsigned char* channel = imagef.data;
	int n = 1;
	//int fkernel[3][3] = { {1,1,1},{1,1,1},{1,1,1} };
	int** fkernel;
	fkernel = new int* [3];
	for (int i = 0; i < 3; i++) {
		fkernel[i] = new int[3];
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			fkernel[i][j] = 0;
			cout << fkernel[i][j] << ",";
		}
		cout << endl;
	}
	fkernel[0][0] = f00;
	fkernel[0][1] = f01;
	fkernel[0][2] = f02;
	fkernel[1][0] = f10;
	fkernel[1][1] = f11;
	fkernel[1][2] = f12;
	fkernel[2][0] = f21;
	fkernel[2][1] = f21;
	fkernel[2][2] = f22;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << fkernel[i][j] << ",";
		}
		cout << endl;
	}
	cout << endl;
	int m = 1;

	
	for (int i = n; i < imageWidth - n; i++)
	{
		for (int j = n; j < imageHeight - n; j++)
		{
			
			//channel[(i +1)*(j+1)] = int(q(image,i+n,j+n,i-n,j-n,fkernel));
			imagef.at<uchar>(j, i) = (uchar)int(q(image, i+n, j+n, i - n, j - n, fkernel));
			//cout << "i = :" << i << " j = :" << j << endl;
			
		}
		
		
	}
	
	


}

int q(Mat image, int n, int m, int k, int l, int  **fkernel) {
	int retval = 0;
	int xc = 0;
	int yc = 0;
	for (int i = k; i < n; i++) {
		for (int j = l; j < m; j++) {
			//cout << "X counter = " << xc << " Y counter = " << yc << endl;
			retval = retval + fkernel[xc][yc] * image.at<uchar>(j, i);
			yc = yc + 1;
			
		}
		yc = 0;
		xc = xc + 1;
	}//
	return retval/9;
}

void orientation(Mat image, int imageWidth, int imageHeight, double orientation_angle[])
{
	double m_00 = 0;
	double m_20 = 0;
	double m_02 = 0;
	double m_11 = 0;

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
				
				m_20 = m_20 + pow(((double(i) - double(SumX))),2);
				m_02 = m_02 + pow((double(j) - double(SumY)),2);
				m_11 = m_11 + (double(i) - double(SumX)) * (double(j) - double(SumY));
			}
		}
	}
	m_00 = double(num);
	m_11 = m_11 / m_00;
	m_20 = m_20 / m_00;
	m_02 = m_02 / m_00;
	//orientation_angle[0] = atan(2 * double(m_11) / ((double(m_20) - double(m_02))))/2;
	orientation_angle[0] = atan2(2*double(m_11), ((double(m_20) - double(m_02))))/2;
	//cout << "Angle: " << orientation_angle[0] << endl;
	//cout << "center X: " << SumX << endl;
	//cout << "center Y: " << SumY << endl;
	//cout << "m_11: " << m_11 << endl;
	//cout << "m_02: " << m_02 << endl;
	//cout << "m_20: " << m_20 << endl;
}


void centerOfMass(Mat image, int imageWidth, int imageHeight, int center[])
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
				num = num + 1;
			}
		}
	}

	SumX = SumX / num;
	SumY = SumY / num;
	//cout << "X = " << SumX << endl;
	//cout << "Y = " << SumY << endl;
	
	center[0] = SumX;
	center[1] = SumY;
	
	// The coordinate (SumX,SumY) is the center of the image mass
}




void histDisplay(int histogram[], const char* name, int width, int height)
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

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

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


	// display histogram
	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, histImage);
}
int main(int argc, char** argv)
{
	int center[2];
	double orientation_angle[1];
	String fileName;
	fileName = "C:\\Users\\allib\\source\\repos\\ConsoleApplication1\\PIC1_R.png";
	String image_in = "ariane5_1b.jpg";
	
	Mat image2 = imread(fileName);
    Mat image1;
	cvtColor(image2, image1, COLOR_BGR2GRAY);
	Mat imagef = image1.clone();
	
	if(image1.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}
	String windowName = "original"; //Name of the window
	namedWindow(windowName); // Create a window
	String windowName2 = "filtered"; //Name of the window
	namedWindow(windowName2); // Create a window
	//imshow(windowName, image1); // Show our image inside the created window.
	
	int width = image1.cols;				 // say, 320
	int height = image1.rows;				 // say, 240
	int k = 256;
	
	cout << "Image channel: " << image1.channels() << endl;
	cout << "Image depth: " << image1.depth() << endl;
	cout << "Image type: " << image1.type() << endl;
	cout << "image width" << width << endl;
	cout << "image height" << height << endl;
	
	imshow(windowName, image1);
	char wk;
	
	int f00 = 0;
	int f01 = 0;
	int f02 = 0;
	int f10 = 0;
	int f11 = 0;
	int f12 = 0;
	int f20 = 0;
	int f21 = 0;
	int f22 = 0;
	while (1) {
		wk = waitKey(0);
		if (wk == '1')f00++;
		if (wk == '2')f01++;
		if (wk == '3')f02++;
		if (wk == '4')f10++;
		if (wk == '5')f11++;
		if (wk == '6')f12++;
		if (wk == '7')f20++;
		if (wk == '8')f21++;
		if (wk == '9')f22++;
		if (wk == 'q')f00--;
		if (wk == 'w')f01--;
		if (wk == 'e')f02--;
		if (wk == 'r')f10--;
		if (wk == 't')f11--;
		if (wk == 'y')f12--;
		if (wk == 'u')f20--;
		if (wk == 'i')f21--;
		if (wk == 'o')f22--;
		
		lowpassfilter(image1,imagef, width, height,f00,f01,f02,f10,f11,f12,f20,f21,f22);
		imshow(windowName2, imagef); // Show our image inside the created window.
		
		
		

	}
	return 0;
}

