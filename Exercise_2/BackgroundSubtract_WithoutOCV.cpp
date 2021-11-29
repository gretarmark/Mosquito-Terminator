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

#include "segmentation.cpp"


// https://en.wikipedia.org/wiki/Moving_object_detection

using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds














int main(int argc, char** argv) {
    double Threshold = 50, Threshold2 = 70;
    double count = 0, count_sum = 0, count_abs = 0, counter = 0, counter1 = 0, counter2 = 0;

    double Background_sum = 0, CurrentFrame_sum = 0, OldFrame_sum = 0;
    double Background_mean = 0, CurrentFrame_mean = 0, OldFrame_mean = 0, var = 0;
    bool isObject = false;

    char wk;


    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name

    //VideoCapture cap("Bolti.mp4");
    //VideoCapture cap2("Bolti.mp4");
    //VideoCapture cap("MovTin.mp4");
    //VideoCapture cap2("MovTin.mp4");
    //VideoCapture cap("TingHighQual.mp4");
    //VideoCapture cap2("TingHighQual.mp4");
   /* VideoCapture cap("TwoObjects.mp4");
    VideoCapture cap2("TwoObjects.mp4");*/
    VideoCapture cap(0);
    VideoCapture cap2(0);

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
        for (int u = 0; u < Background.rows; ++u) {
            for (int v = 0; v < 3 * Background.cols; ++v) {
                Background_sum += (double)Background.at<uchar>(u, v);
                count += 1;
            }
        }
        var += (Background_sum / count);

        count_sum += 1;

        if (count_sum == 5) {
            Background_mean = var / count_sum;
            break;
        }

        Background_sum = 0;
        count = 0;
    }
    Mat LastFrame;
    LastFrame = Background.clone();

    Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2(1, 100, false);
    double nLabels = 0;
    // Start the machine
    while (1) {




        Mat CurrentFrame;
        //Map Foreground;
        cap >> CurrentFrame;                    // Capture frame-by-frame
        Mat foreground;
        //cv::namedWindow("Extracted Foreground");



        imshow("CurrentFrame", CurrentFrame);
        imshow("LastFrame", LastFrame);

        LastFrame = CurrentFrame.clone();






        counter2 += 1;

        // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        if (Background.empty()) break;

        //cout << "CurrentVal: " << (double)CurrentFrame.at<Vec3b>(339, 639)[0] << endl;
        //cout << "Count: " << counter2 << endl;
        //cout << "Current Frame Rows: " << CurrentFrame.rows << endl;
        //cout << "Current Frame Columns: " << CurrentFrame.cols << endl;
        //system("Pause");

        //// mean value of the background and current frame
        //for (int u = 0; u < CurrentFrame.rows-1; ++u) { // Rows are 360
        //    for (int v = 0; v < CurrentFrame.cols-1; ++v) { // Columns are 640
        //        if (   abs((double)CurrentFrame.at<Vec3b>(u, v)[0] - (double)Background.at<Vec3b>(u, v)[0]) > Threshold
        //            || abs((double)CurrentFrame.at<Vec3b>(u, v)[1] - (double)Background.at<Vec3b>(u, v)[1]) > Threshold
        //            || abs((double)CurrentFrame.at<Vec3b>(u, v)[2] - (double)Background.at<Vec3b>(u, v)[2]) > Threshold) {
        //                CurrentFrame.at<Vec3b>(u, v)[0] = 255;
        //                CurrentFrame.at<Vec3b>(u, v)[1] = 255;
        //                CurrentFrame.at<Vec3b>(u, v)[2] = 255;
        //        }
        //        else {
        //            CurrentFrame.at<Vec3b>(u, v)[0] = 0;
        //            CurrentFrame.at<Vec3b>(u, v)[1] = 0;
        //            CurrentFrame.at<Vec3b>(u, v)[2] = 0;
        //        }



        //        //if (CurrentFrame.at<Vec3b>(u,v)[0] )
        //        // 
        //        // 
        //        //if (abs((double)CurrentFrame.at<uchar>(u, v) - (double)Background.at<uchar>(u, v)) > Threshold2) {
        //        //    //CurrentFrame.at<uchar>(u, v) = (uchar)255;
        //        //    CurrentFrame.at<Vec3b>(u, v)[0] = 255;
        //        //    CurrentFrame.at<Vec3b>(u, v)[1] = 255;
        //        //    CurrentFrame.at<Vec3b>(u, v)[2] = 255;
        //        //}
        //        //else {
        //        //    //CurrentFrame.at<uchar>(u, v) = (char)((double)CurrentFrame.at<uchar>(u,v) - (double)Background.at<uchar>(u,v));
        //        //    //CurrentFrame.at<uchar>(u, v) = (uchar)0;
        //        //    CurrentFrame.at<Vec3b>(u, v)[0] = 0;
        //        //    CurrentFrame.at<Vec3b>(u, v)[1] = 0;
        //        //    CurrentFrame.at<Vec3b>(u, v)[2] = 0;
        //        //}
        //        
        //    }
        //}


        GaussianBlur(CurrentFrame, foreground, Size(9, 9), 0);

        mog->apply(foreground, foreground, 0.01);
        //mog->apply(CurrentFrame, foreground, 0.01);



        //Mat bin_mat(gray_mat.size(), gray_mat.type());
        //adaptiveThreshold(foreground, foreground, 255,ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 9, 9);
        adaptiveThreshold(foreground, foreground, 255, ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 12);


        Mat labels;
        Mat stats;
        Mat centroids;
        nLabels = connectedComponentsWithStats(foreground, labels, stats, centroids, 8);

        //cout << "Labels: " << labels << std::endl;
        //cout << "Stats size: " << stats.size() << std::endl;
        cout << "Centroids: " << centroids << std::endl;

        cout << "Amount of objects: " << nLabels - 1 << endl;



        // Display the resulting frame
        //rgbchannel[2] = Mat::zeros(rgbchannel[2].size(), CV_8UC1); // This can be used to get rid of the red channel

        imshow("Foreground", CurrentFrame);
        imshow("Background", foreground);
        //imshow("Background", Foreground);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if (c == 27)
            break;





        //CurrentFrame.copyTo(OldFrame);
        //if (OldFrame.empty()) break;







        //sleep_for(nanoseconds(50));
        //sleep_until(system_clock::now() + seconds(1));


    }

    // When everything done, release the video capture object
    cap.release();
    cap2.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}







