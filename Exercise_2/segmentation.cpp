#include <stdio.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;


class segmentation {
public:
	Mat current_frame;
	Mat source_img;
	Mat dist;
	Mat dist_8u;
	Mat markers;
	Mat markers8u;
	Mat mark;
	Mat dst;
	vector<vector<Point> > contours;
	segmentation(Mat src,Mat bw) {
		current_frame = bw;
		source_img = src;
		distanceTransform(current_frame, dist, DIST_L2, 3);
		dist.convertTo(dist_8u, CV_8U);
		findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		
		markers = Mat::zeros(dist.size(), CV_32S);
		markers.convertTo(markers8u, CV_8U, 10);
		
		watershed(source_img, markers);
		markers.convertTo(mark, CV_8U);
		bitwise_not(mark, mark);
		
		
		//dst = Mat::zeros(markers.size(), CV_8UC3);
	}
	
};

