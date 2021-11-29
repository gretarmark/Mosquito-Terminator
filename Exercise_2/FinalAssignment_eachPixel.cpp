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
    double count = 0, count_sum = 0, count_abs = 0, counter1 = 0, counter2 = 0;

    double Background_sum = 0, CurrentFrame_sum = 0, OldFrame_sum = 0;
    double Background_mean = 0, CurrentFrame_mean = 0, OldFrame_mean = 0, var = 0;
    bool isObject = false;

    char wk;

    count_sum = 0;
    count = 0;

    double check1 = 0, check2 = 0, check3 = 0;

    // Lower and upper bounds
    Scalar lowerBound(120, 80, 100); // Lower Threshold Bounds
    Scalar upperBound(140, 85, 110); // Upper Threshold Bounds

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

        if (count_sum == 1) {
            Background_mean = var / count_sum;
            break;
        }

        Background_sum = 0;
        count = 0;
    }


    string ty = type2str(Background.type());
    printf("Matrix: %s %dx%d \n", ty.c_str(), Background.cols, Background.rows);

    cout << "Channels: " << Background.channels() << endl;


    cout << "N: " << count_sum << endl;
    cout << "Background Mean; " << Background_mean << endl;
    cout << "Background Rows: " << Background.rows << endl;
    cout << "Background Columns: " << Background.cols << endl;
    system("Pause");


    // Start the machine
    while (1) {
        Mat CurrentFrame;
        cap >> CurrentFrame;                    // Capture frame-by-frame

        imshow("CurrentFrame Before Changes", CurrentFrame);


        // https://cppsecrets.com/users/203110310511410511510410011599115495764103109971051084699111109/C00-OpenCV-cvsplit.php
        // https://www.youtube.com/watch?v=VjYtoL0wZMc
        //Mat bgr_CurrentFrame[3];
        //Mat bgr_Background[3];
        //split(CurrentFrame, bgr_CurrentFrame);
        //split(Background, bgr_Background);

        // Thresholding each channel
        //void inRange(CurrentFrame, Scalar(120, 80, 100), Scalar(140, 85, 110), Background);



        //imshow("B", bgr_CurrentFrame[0]);
        //imshow("G", bgr_CurrentFrame[1]);
        //imshow("R", bgr_CurrentFrame[2]);


        /*
        
        */


        // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        if (Background.empty()) break;


        // mean value of the background and current frame
        for (int u = 1; u < CurrentFrame.rows - 1; ++u) {
            //cout << "u: " << u << endl;
            for (int v = 1; v < 3 * CurrentFrame.cols - 1; ++v) {
                //cout << "v: " << v << endl;

                //check1 = (double)CurrentFrame.at<Vec3b>(u, v)[0] - (double)Background.at<Vec3b>(u, v)[0];
                //check2 = (double)CurrentFrame.at<Vec3b>(u, v)[1] - (double)Background.at<Vec3b>(u, v)[1];
                //check3 = (double)CurrentFrame.at<Vec3b>(u, v)[2] - (double)Background.at<Vec3b>(u, v)[2];
                


                //if (check1 > Threshold2) { CurrentFrame.at<Vec3b>(u, v)[0] = (uchar)255; }
                //else { CurrentFrame.at<Vec3b>(u, v)[0] = (uchar)0; }

                //if (check2 > Threshold2) { CurrentFrame.at<Vec3b>(u, v)[1] = (uchar)255; }
                //else { CurrentFrame.at<Vec3b>(u, v)[1] = (uchar)0; }

                //if (check3 > Threshold2) { CurrentFrame.at<Vec3b>(u, v)[2] = (uchar)255; }
                //else { CurrentFrame.at<Vec3b>(u, v)[2] = (uchar)0; }

                
                //if ((u == 200) && (v == (400))) {
                //    double x = (double)CurrentFrame.at<Vec3b>(10, 29)[0];
                //    cout << "bgr_Current[0] value: " << x <<  endl;
                //    //cout << "bgr_Current[0] value: " << CurrentFrame.at<Vec3b>(u, v)[0] << endl;
                //    system("Pause");
                //}
                

                


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
        //Mat CurrentFrame_out;
        //Mat Background_out;
        //merge(bgr_CurrentFrame, 3, CurrentFrame_out);
        //merge(bgr_Background, 3, Background_out);
        //imshow("Merged Frame", CurrentFrame_out);

        //imshow("Background", Background);
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