#include "ImageGenerator.h"
#include <iostream>
#include <format>

ImageGenerator::ImageGenerator(unsigned int width, unsigned int height) : 
    m_width(width), m_height(height),
    m_img(cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0))) {}

void ImageGenerator::setPixelColor(unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B) {
    if (x >= m_width || y >= m_height) {
        auto errMsg = std::format("[Error] Try to set ({}, {}) on size of ({}, {})\n", x, y, m_width, m_height);
        std::cerr << errMsg;
        return;
    }

    m_img.at<cv::Vec3b>(m_height - y - 1, x)[0] = B;
    m_img.at<cv::Vec3b>(m_height - y - 1, x)[1] = G;
    m_img.at<cv::Vec3b>(m_height - y - 1, x)[2] = R;
}

void ImageGenerator::showImage() {
    cv::imwrite("./res/image.jpg", m_img);
    cv::imshow("ImageGenerator", m_img);
    cv::waitKey(0);
}