/**
 * Copyright (c) 2018, Xidian University Robomaster Team, li chunpeng
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "zbar.h"
#include "detect.hpp"
#include "my_v4l2.h"



bool task1(cv::Mat image, std::vector<int>& key)
{
    QR_mul::QR_detecter qr123(image);
    qr123.detectQR(image);
    if (!image.data)
    {
        std::cout << "No QR code. \n " << std::endl;
        return false;
    }
    else{
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        cv::Mat imageGray;
        cv::cvtColor(image, imageGray, CV_RGB2GRAY);
        int width = imageGray.cols;
        int height = imageGray.rows;
        uchar* raw = (uchar*)imageGray.data;
        zbar::Image imageZbar(width, height, "Y800", raw, width * height);
        scanner.scan(imageZbar);
        zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
        
        if (imageZbar.symbol_begin()==imageZbar.symbol_end())
        {
            std::cout << "scan QR is failed " << std::endl;

            imageZbar.set_data(NULL,0);
            return false;
        }
        else{
            for (; symbol != imageZbar.symbol_end(); ++symbol)
            {
                std::cout << "Type: " << std::endl << symbol->get_type_name() << std::endl;
                std::cout << "Data: " << std::endl << symbol->get_data() << std::endl;
                std::string result = symbol->get_data();
        
                // To-do 不够鲁棒
                for(int i = 0; i < 3; ++i)
                    key.push_back(static_cast<int>(result[i]-'0'));

                imageZbar.set_data(NULL,0);
                return true;
            }
        }
    }
}

void task2(cv::Mat image, std::vector<int>& key )
{

    cv::waitKey(1);
}

int main(int argc, char* argv[])
{
    cv::VideoCapture cap;
    //cap.set(CV_CAP_PROP_FPS,120);
    cap.open("/dev/video0"); // open camera of ptz
    if(!cap.isOpened())
    {
        std::cout<<"Cannot open cap!"<<std::endl;
        return -1;
    }
    //cap.set(CV_CAP_PROP_EXPOSURE, 500);

    //cap.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    //set_camera_exposure("/dev/video0", 100);
    

    cv::Mat image;

    std::vector<int> key;

    int mode = 1;

    while(true)
    {
        cap >> image;
        if (!image.data)
        {
            std::cout << "No image. \n " << std::endl;
            cv::waitKey(1);
        }
        if(mode == 1)
        {
            if (task1(image, key)) mode = 2;  // lian xu shi bie
            cv::waitKey(1);
        }
        else{
            task2(image, key);
        }
        cv::imshow("Source Image", image);
    }

    return 0;
}
