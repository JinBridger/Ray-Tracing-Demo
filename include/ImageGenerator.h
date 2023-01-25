#pragma once

#include <stdint.h>
#include <opencv2/opencv.hpp>

class ImageGenerator {
public:
    ImageGenerator(unsigned int width, unsigned int height);

    void setPixelColor(unsigned int x, unsigned int y, uint8_t R, uint8_t G, uint8_t B);
    void showImage();

    inline unsigned int getWidth() const {
        return m_width;
    }
    inline unsigned int getHeight() const {
        return m_height;
    }

private:
    unsigned int m_width;
    unsigned int m_height;

    cv::Mat m_img;
};