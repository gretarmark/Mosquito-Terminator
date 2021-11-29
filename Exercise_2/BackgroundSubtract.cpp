#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>  
#include <stdlib.h> 
#include <chrono>
#include <thread>
#include <Windows.h>
#include <string>
#include <vector>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <sstream>



// https://en.wikipedia.org/wiki/Moving_object_detection

using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


int main(int argc, char** argv) {
    double Threshold = 50;
    double count = 0, count1 = 0;
    char wk;
    double nLabels;

    Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2(1, 100, false);


    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name

    //VideoCapture cap("Bolti.mp4");
    VideoCapture cap("TwoObjects.mp4");
    //VideoCapture cap(0);

    // Check if camera opened successfully
    if (!cap.isOpened()){cout << "Error opening video stream or file" << endl; return -1;}



    

    // Start the machine
    while (1) {
        
        // Initialize OpenCV Matrices to save current frame and the foreground object
        Mat CurrentFrame;
        Mat foreground;
        // Capture the video frame-by-frame
        cap >> CurrentFrame; 
        // Display current frame in a window
        imshow("CurrentFrame", CurrentFrame);
        // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        
        




        
    
        // Blur the currentframe with kernel of size 9x9
        GaussianBlur(CurrentFrame, foreground, Size(9, 9), 0);
        
        
        
        
        

        // Apply the MOG2 Background Subtractor Algorithm to get the foreground object
        mog->apply(foreground, foreground, 0.01);
        /*mog->apply(CurrentFrame, foreground, 0.01);*/
        




        // Apply adaptive thresholding algorithm on the foreground image (Gaussian or Mean)
        adaptiveThreshold(foreground, foreground, 255, ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 12);
        /*adaptiveThreshold(foreground, foreground, 255, ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 9, 9);*/









        // Initialize matrices for the CCT (with stats) algorithm
        Mat labels;
        Mat stats;
        Mat centroids;
        nLabels = connectedComponentsWithStats(foreground,labels,stats,centroids,8);
        //cout << "Labels: " << labels << std::endl;
        //cout << "Stats size: " << stats.size() << std::endl;



        count += 1;
        count1 += 1;
        if ((nLabels - 1) == 2 && count > 100 && count < 300 && count1 > 50) {

            cout << "Test: " << centroids << endl;;
            cout << "Centroids object 1:  (u1,v1) = (" << centroids.at<double>(1,0) << "," << centroids.at<double>(1,1) << ")" << std::endl;
            cout << "centroids object 2:  (u2,v2) = (" << centroids.at<double>(2,0) << "," << centroids.at<double>(2,1) << ")" << std::endl;
            cout << "Amount of objects: " << nLabels - 1 << endl;
            count1 = 0;
            system("Pause");
        }
        else {
            cout << "Amount of objects: " << nLabels - 1 << endl;
        }
        
        if ((nLabels - 1) >= 2) {
            Point p11((int)centroids.at<double>(1, 0) - 30 , (int)centroids.at<double>(1, 1));
            Point p12((int)centroids.at<double>(1, 0) + 30 , (int)centroids.at<double>(1, 1));
            Point p13((int)centroids.at<double>(1, 0)      , (int)centroids.at<double>(1, 1) - 30);
            Point p14((int)centroids.at<double>(1, 0)      , (int)centroids.at<double>(1, 1) + 30);

            Point p21((int)centroids.at<double>(2, 0) - 30 , (int)centroids.at<double>(2, 1));
            Point p22((int)centroids.at<double>(2, 0) + 30 , (int)centroids.at<double>(2, 1));
            Point p23((int)centroids.at<double>(2, 0)      , (int)centroids.at<double>(2, 1) - 30);
            Point p24((int)centroids.at<double>(2, 0)      , (int)centroids.at<double>(2, 1) + 30);

            line(foreground, p11, p12, Scalar(100, 0, 0), 2, 2, 0);
            line(foreground, p13, p14, Scalar(100, 0, 0), 2, 2, 0);

            line(foreground, p21, p22, Scalar(185, 0, 0), 2, 2, 0);
            line(foreground, p23, p24, Scalar(185, 0, 0), 2, 2, 0);
        }


        




        // Display the resulting frame
        //rgbchannel[2] = Mat::zeros(rgbchannel[2].size(), CV_8UC1); // This can be used to get rid of the red channel
        //count += 1;
        //cout << "Count: " << count << endl;
        imshow("Foreground", foreground);
        //imshow("Background", Foreground);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if (c == 27)
            break;


        if (foreground.empty())   break;

    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}







