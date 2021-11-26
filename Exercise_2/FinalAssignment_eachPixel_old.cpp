#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>  
#include <stdlib.h> 
#include <chrono>
#include <thread>
#include <Windows.h>


// https://en.wikipedia.org/wiki/Moving_object_detection

using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

int main(int argc, char** argv) {
    double Threshold = 1, Threshold2 = 55;
    double count = 0, count_sum = 0, count_abs = 0, counter1 = 0, counter2 = 0;

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

        if (count_sum == 5) {
            Background_mean = var / count_sum;
            break;
        }

        Background_sum = 0;
        count = 0;
    }

    cout << "Channels: " << Background.channels() << endl;


    cout << "N: " << count_sum << endl;
    cout << "Background Mean; " << Background_mean << endl;
    cout << "Background Rows: " << Background.rows << endl;
    cout << "Background Columns: " << Background.cols << endl;

    
    system("Pause");

    count_sum = 0;
    count = 0;


    // Start the machine
    while (1) {
        Mat CurrentFrame;
        cap >> CurrentFrame;                    // Capture frame-by-frame
        


        // https://cppsecrets.com/users/203110310511410511510410011599115495764103109971051084699111109/C00-OpenCV-cvsplit.php
        // https://www.youtube.com/watch?v=VjYtoL0wZMc
        Mat bgr_channel[3];
        split(CurrentFrame, bgr_channel);



        //imshow("B", rgbchannel[0]);
        //imshow("G", rgbchannel[1]);
        //imshow("R", rgbchannel[2]);



        // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        if (Background.empty()) break;


        // mean value of the background and current frame
        for (int u = 1; u < CurrentFrame.rows - 1; ++u) {
            for (int v = 1; v < 3 * CurrentFrame.cols - 1; ++v) {

                //if (bgr_channel) 
                
                if (abs((double)CurrentFrame.at<uchar>(u, v) - (double)Background.at<uchar>(u, v)) > Threshold2) {
                    CurrentFrame.at<uchar>(u, v) = (uchar)2*256;
                    
                }
                else {
                    //CurrentFrame.at<uchar>(u, v) = (char)((double)CurrentFrame.at<uchar>(u,v) - (double)Background.at<uchar>(u,v));
                    CurrentFrame.at<uchar>(u, v) = (uchar)50;
                }


                //// Thresholding each color channel (B, G, R)
                //rgbchannel[0]
                //rgbchannel[1]
                //rgbchannel[2]


            }
        }


        counter2 += 1;


        // Display the resulting frame
        //rgbchannel[2] = Mat::zeros(rgbchannel[2].size(), CV_8UC1); // This can be used to get rid of the red channel
        Mat output;
        merge(bgr_channel, 3, output);
        imshow("Merged Frame", output);

        imshow("CurrentFrame", CurrentFrame);

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