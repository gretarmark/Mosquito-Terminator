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

    imshow("Background", Background);

    cout << "Channels: " << Background.channels() << endl;

    //Mat bgr_Background[3];
    //split(Background, bgr_Background);
    //cout << "Channel B: " << bgr_Background[0].size() << endl;
    //cout << "Channel G: " << bgr_Background[1].size() << endl;
    //cout << "Channel R: " << bgr_Background[2].size() << endl;
    //cout << "Channels size: " << Background.size() << endl;

    //cout << "d_Channel B: " << bgr_Background[0].depth() << endl;
    //cout << "d_Channel G: " << bgr_Background[1].depth() << endl;
    //cout << "d_Channel R: " << bgr_Background[2].depth() << endl;
    //cout << "d_Channels size: " << Background.depth() << endl;


    cout << "N: " << count_sum << endl;
    cout << "Background Mean; " << Background_mean << endl;
    cout << "Background Rows: " << Background.rows << endl;
    cout << "Background Columns: " << Background.cols << endl;
    system("Pause");

    count_sum = 0;
    count = 0;

    double check1, check2, check3;

    // Start the machine
    while (1) {
        Mat CurrentFrame;
        cap >> CurrentFrame;                    // Capture frame-by-frame
        
                                                // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        if (Background.empty()) break;


        //// read next frame if any
        //if (!capture.read(frame))
        //    break;

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





        // mean value of the background and current frame
        for (int u = 1; u < CurrentFrame.rows - 1; ++u) {
            for (int v = 1; v < (CurrentFrame.channels() * CurrentFrame.cols) - 1; ++v) {
                /*check1 = (double)CurrentFrame.at<Vec3b>(u, v)[0] - (double)Background.at<Vec3b>(u, v)[0];
                check2 = (double)CurrentFrame.at<Vec3b>(u, v)[1] - (double)Background.at<Vec3b>(u, v)[1];
                check3 = (double)CurrentFrame.at<Vec3b>(u, v)[2] - (double)Background.at<Vec3b>(u, v)[2];


                if (check1 > blue_threshold) { CurrentFrame.at<Vec3b>(u, v)[0] = (uchar)255; }
                else { CurrentFrame.at<Vec3b>(u, v)[0] = (uchar)0; }

                if (check2 > green_threshold) { CurrentFrame.at<Vec3b>(u, v)[1] = (uchar)255; }
                else { CurrentFrame.at<Vec3b>(u, v)[1] = (uchar)0; }

                if (check3 > red_threshold) { CurrentFrame.at<Vec3b>(u, v)[2] = (uchar)255; }
                else { CurrentFrame.at<Vec3b>(u, v)[2] = (uchar)0; }*/

                cout << "Val: " << (double)CurrentFrame.at<Vec3b>(u, v)[0] << endl;
                counter3 += 1;
                cout << "Counter: " << counter3 << endl;



                //if ((u == 200) && (v == (400))) {
                //    double x = (double)CurrentFrame.at<Vec3b>(10, 29)[2];
                //    cout << "bgr_Current[0] value: " << x <<  endl;
                //    //cout << "bgr_Current[0] value: " << CurrentFrame.at<Vec3b>(u, v)[0] << endl;
                //    system("Pause");
                //}




                //if (abs((double)CurrentFrame.at<uchar>(u, v) - (double)Background.at<uchar>(u, v)) > Threshold2) {
                //    /*CurrentFrame.at<uchar>(u, v) = (uchar)2 * 256;*/
                //    cout << "B: " << (double)CurrentFrame.at<Vec3b>(u, v)[0] << endl;
                //    cout << "G: " << (double)CurrentFrame.at<Vec3b>(u, v)[1] << endl;
                //    cout << "R: " << (double)CurrentFrame.at<Vec3b>(u, v)[2] << endl;
                //}
                //else {
                //    //CurrentFrame.at<uchar>(u, v) = (char)((double)CurrentFrame.at<uchar>(u,v) - (double)Background.at<uchar>(u,v));
                //    CurrentFrame.at<uchar>(u, v) = (uchar)50;
                //}

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