/**
 * Copyright (c) 2018, Xidian University Robomaster Team, li chunpeng
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "my_v4l2.h"
#include "task.h"

int main(int argc, char* argv[])
{
    cv::VideoCapture cap;
    //cap.set(CV_CAP_PROP_FPS,120);
    cap.open("/dev/video1"); // open camera of ptz
    if(!cap.isOpened())
    {
        std::cout<<"Cannot open cap!"<<std::endl;
        return -1;
    }
    //cap.set(CV_CAP_PROP_EXPOSURE, 500);

    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    set_camera_exposure("/dev/video1", 100);
    
    cv::Mat image;

    std::vector<int> key;

    int mode = 1;

    while(true)
    {
        key.clear();
        cap >> image;
        if (!image.data)
        {
            std::cout << "No image. \n " << std::endl;
            cv::waitKey(1);
        }
        if(mode == 1)
        {
            if (task1(image, key)); //mode = 2;  // lian xu shi bie
            cv::waitKey(1);
        }
        else{
            task2(image, key);
        }
        cv::imshow("Source Image", image);
    }

    return 0;
}
