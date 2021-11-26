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
    double Threshold = 1;
    double count = 0, count_sum = 0, count_abs = 0;

    double Background_sum = 0, CurrentFrame_sum = 0, OldFrame_sum = 0;
    double Background_mean = 0, CurrentFrame_mean = 0, OldFrame_mean = 0, var = 0;
    bool isObject = false;

    char wk;

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
   
    //VideoCapture cap("Bolti.mp4");
    //VideoCapture cap2("Bolti.mp4");
    VideoCapture cap("MovTin.mp4");
    VideoCapture cap2("MovTin.mp4");
    //VideoCapture cap("TingHighQual.mp4");
    //VideoCapture cap2("TingHighQual.mp4");

    // Check if camera opened successfully
    if (!cap.isOpened()) { cout << "Error opening video stream or file" << endl;    return -1; }
    if (!cap2.isOpened()) { cout << "Error opening video stream or file" << endl;    return -1; }

    // Initialize the frame
    Mat Background;
    while(1) {
        
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
        
        if (count_sum == 100) {
            Background_mean = var / count_sum;
            break;
        }

        Background_sum = 0;
        count = 0;
    }


    cout << "N: " << count_sum << endl;
    cout << "Background Mean; " << Background_mean << endl;
    cout << "Background Rows: " << Background.rows << endl;
    cout << "Background Columns: " << Background.cols << endl;
    system("Pause");

    count_sum = 0;
    count = 0;
    

    // Start the machine
    while (1) {
        Mat OldFrame;
        Mat CurrentFrame;
        

        if (count == 0) {
            Background.copyTo(OldFrame);
            count += 1;
        }

        // Capture frame-by-frame
        cap >> CurrentFrame;





        // If the frame is empty, break immediately
        if (CurrentFrame.empty()) break;
        if (Background.empty()) break;
        //if (OldFrame.empty()) break;







        ////Analyse the image - Is object in the image?
        //for (int u = 1; u < CurrentFrame.rows; ++u) {
        //    for (int v = 1; v < CurrentFrame.cols; ++v) {
        //        if (Threshold < frame.at<uchar>(u, v)) {
        //            cout << "There is a object in the video" << endl;
        //            isObject = true;
        //            break;
        //        }
        //    }
        //}







        cout << "Current Frame Rows: " << CurrentFrame.rows << endl;
        cout << "Current Frame Columns: " << CurrentFrame.cols << endl;
        cout << "Channels CurrentFrame: " << CurrentFrame.channels() << endl;

        // mean value of the background and current frame
        for (int u = 1; u < CurrentFrame.rows - 1; ++u) {
            for (int v = 1; v < 3*CurrentFrame.cols - 1; ++v) {
                CurrentFrame_sum += (double)CurrentFrame.at<uchar>(u, v);
                //Background_sum += (double)Background.at<uchar>(u, v);
                count_sum += 1;
            }
        }


        CurrentFrame_mean = CurrentFrame_sum / count_sum;
        //Background_mean = Background_sum / count;


        cout << "Mean of current frame: " << CurrentFrame_mean << endl;
        cout << "Mean of background: " << Background_mean << endl;
        cout << "Absolute difference: " << abs(CurrentFrame_mean - Background_mean) << endl;
        cout << "Threshold: " << Threshold << endl;

        if (abs(CurrentFrame_mean - Background_mean) > Threshold) {
            cout << "Absolute difference greater than threshold: " << abs(CurrentFrame_mean - Background_mean) << endl;
            cout << "There is a moving object in the image" << endl;
            system("Pause");
        }


        count_abs += 1;
        if (count_abs == 50) {
            cout << "Absolute difference (count_abs): " << abs(CurrentFrame_mean - Background_mean) << endl;
            system("Pause");
            count_abs = 0;
        }


        CurrentFrame_sum = 0;
        OldFrame_sum = 0;
        CurrentFrame_mean = 0;
        OldFrame_mean = 0;
        count_sum = 0;








        // Display the resulting frame
        imshow("Frame", CurrentFrame);

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

