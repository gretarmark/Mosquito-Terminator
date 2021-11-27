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



// https://www.pyimagesearch.com/2018/07/23/simple-object-tracking-with-opencv/

// https://en.wikipedia.org/wiki/Moving_object_detection

using namespace std;
using namespace cv;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds





int main(int argc, char** argv) {


    

    

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    //VideoCapture capture("TwoObjects.mp4");
    //VideoCapture cap2("TwoObjects.mp4");
    VideoCapture capture(0);


    // Open the video file
    //cv::VideoCapture capture("bike.avi");
    // check if video successfully opened
    if (!capture.isOpened())
        return 0;
    // current video frame
    cv::Mat frame;
    // foreground binary image
    cv::Mat foreground;
    cv::namedWindow("Extracted Foreground");
    // The Mixture of Gaussian object
    // used with all default parameters
    


        /*   createBackgroundSubtractorMOG2(history, varThreshold, detectShadow);   */
                             // history tells us how many frames should be used to model the background
                             // varThreshold correlates the value of the weight of the pixels on the current frame to model
                             //     - low values on varThreshold tend to create false objects.
    //BackgroundSubtractorMOG2 mog; // also BackgroundSubtractorMOG2 that removes shadows
    //Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();
    //Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2(20, 16, true);
    Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2(1, 100, false);
    //Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorKNN();
    //system("Pause");

    vector<vector<Point> > contours;
    
    

    int counter = 0;

    bool stop(false);
    // for all frames in video
    while (!stop) {
        Mat movie;
        capture >> movie;
        imshow("Video", movie);

        

        //counter += 1;
        //if (counter == 2) {
        //    system("Pause");
        //}
        // read next frame if any
        if (!capture.read(frame))
            break;

        // read next frame if any
        if (!capture.read(movie))
            break;

        // update the background and return the foreground
                  // - learningRate: Value between 0 and 1 indicates how fast the background model is learnt.
                  //                 -1 makes the algorithm use automatically chosen learning rate. 0 = no update at all
                  //                 
        mog->apply(frame, foreground, 0.05);

        // learning rate
        // Complement the image

        //threshold(foreground, foreground, 128, 255, cv::THRESH_BINARY_INV);
        //threshold(foreground, foreground, 190, 255, cv::THRESH_TRUNC);

        segmentation obj(frame,foreground);
        
        cv::imshow("Segmentation", obj.dist);
        //cout << "markers: " << obj.mark << endl;

        //Mat mask 
        // Color thresholding
        //inRange(frame, Scalar(100, 125, 100), Scalar(255, 200, 255), foreground);
        
        // Finding contours in the image
        //findContours(foreground, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        /*cout << "contours: " << contours[0][0].x << endl;*/

        // show foreground
        cv::imshow("Extracted Foreground", foreground);
        // introduce a delay
        // or press key to stop
        if (cv::waitKey(10) >= 0)
            stop = true;
    }



    return 0;
}


