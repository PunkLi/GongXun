/**
 * Copyright (c) 2018, Xidian University Robomaster Team, li chunpeng
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "zbar.h"
#include "detect.hpp"

int main(int argc, char* argv[])
{

    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    cv::Mat image = cv::imread("../code.png");
    QR_mul::QR_detecter qr123(image);
    qr123.detectQR(image);
    if (!image.data)
    {
        std::cout << "image is null " << std::endl;
        return -1;
    }
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
        return -1;
    }
    for (; symbol != imageZbar.symbol_end(); ++symbol)
    {
        std::cout << "Type: " << std::endl
                  << symbol->get_type_name() << std::endl;
        std::cout << "Data: " << std::endl
                  << symbol->get_data() << std::endl;
    }
    cv::imshow("Source Image", image);
    cv::waitKey();
    imageZbar.set_data(NULL,0);
    return 0;
}
