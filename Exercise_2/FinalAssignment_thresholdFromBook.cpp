#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>  
#include <stdlib.h> 
#include <chrono>
#include <thread>
#include <Windows.h>
#include <string>
#include <vector>


// https://en.wikipedia.org/wiki/Moving_object_detection

using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


// Functions
string type2str(int type);



int main(int argc, char** argv) {
    double Threshold = 1, Threshold2 = 84;
    double red_threshold = 160, green_threshold = 140, blue_threshold = 120; // B goes lowest, sometimes 85 or 94, (B:125)(G:148),(R:167)
    double count = 0, count_sum = 0, count_abs = 0, counter1 = 0, counter2 = 0, counter3 = 0;

    double Background_sum = 0, CurrentFrame_sum = 0, OldFrame_sum = 0;
    double Background_mean = 0, CurrentFrame_mean = 0, OldFrame_mean = 0, var = 0;
    bool isObject = false;

    char wk;

    // Lower and upper bounds, used for thresholding
    //Scalar lowerBound(120, 80, 100); // Lower Threshold Bounds
    //Scalar upperBound(140, 85, 110); // Upper Threshold Bounds

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap("TwoObjects.mp4");
    VideoCapture cap2("TwoObjects.mp4");

    // Check if camera opened successfully
    if (!cap.isOpened()) { cout << "Error opening video stream or file" << endl;    return -1; }
    if (!cap2.isOpened()) { cout << "Error opening video stream or file" << endl;    return -1; }

    // Initialize the frame
    Mat Background;
    while (1) {

        //wk = waitKey(0);
        //if (wk == 'g')

        cap2 >> Background;
        if (Background.empty()) break;

        // mean value of the background and current frame
        for (int u = 1; u < Background.rows - 1; ++u) {
            for (int v = 1; v < 3 * Background.cols - 1; ++v) {
                Background_sum += (double)Background.at<uchar>(u, v);
                count += 1;
            }
        }
        var += (Background_sum / count);

        count_sum += 1;

        if (count_sum == 1) {
            Background_mean = var / count_sum;
            break;
        }

        Background_sum = 0;
        count = 0;
    }


    string ty = type2str(Background.type());
    printf("Matrix: %s %dx%d \n", ty.c_str(), Background.cols, Background.rows);


    // Start the machine
    while (1) {
        Mat Foreground;
        Mat CurrentFrame;
        cap >> CurrentFrame;                    // Capture frame-by-frame 
                                                // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        if (Background.empty()) break;

        absdiff(Background, CurrentFrame, Foreground); // compute difference between image and background


        namedWindow("Foreground", WINDOW_AUTOSIZE);// Create a window for display.
        imshow("Foreground", Foreground);



        //imshow("Background", Background);
        namedWindow("CurrentFrame", WINDOW_AUTOSIZE);// Create a window for display.
        imshow("CurrentFrame", CurrentFrame);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if (c == 27)
            break;

    }

    // When everything done, release the video capture object
    cap.release();
    cap2.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
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