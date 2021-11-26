#include <stdio.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;


class tracker {
public:
	int tracker_id;
	Mat full_image;
	Rect image_limits;
	Mat cropped_img;
	tracker(int id, Mat current_frame, Rect limits) {//Rect_ (_Tp _x, _Tp _y, _Tp _width, _Tp _height)
		tracker_id = id;
		full_image = current_frame;
		image_limits = limits;
		cropped_img = full_image(limits);
	}

	void update_position() {

	}

	void update_frame(Rect limits) {
		image_limits = limits;
		cropped_img = full_image(limits);
	}
};