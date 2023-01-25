#pragma once

#include <opencv2/opencv.hpp>

const cv::Vec3b RED     = cv::Vec3b(255, 128, 128);
const cv::Vec3b GREEN   = cv::Vec3b(128, 255, 128);
const cv::Vec3b BLUE    = cv::Vec3b(128, 128, 255);
const cv::Vec3b YELLOW  = cv::Vec3b(255, 255,  25);
const cv::Vec3b CYAN    = cv::Vec3b( 25, 255, 255);
const cv::Vec3b MAGENTA = cv::Vec3b(255,  25, 255);
const cv::Vec3b GRAY    = cv::Vec3b(128, 128, 128);
const cv::Vec3b WHITE   = cv::Vec3b(255, 255, 255);

const cv::Vec3b DBGCL   = cv::Vec3b(255,   0,   0);

struct Ray {
    cv::Vec3f startPoint = cv::Vec3f(0, 0, 0);
    cv::Vec3f direction  = cv::Vec3f(0, 0, 0);
};

struct Material {
    cv::Vec3f normalVec        = cv::Vec3f(0, 0, 0);
    cv::Vec3b color            = cv::Vec3b(0, 0, 0);
    float     specularRate     = 0;
    float     roughness        = 1;
    float     refractRate      = 0;
    float     refractAngle     = 1;
    float     refraceRoughness = 0;
    bool      isEmmissive      = false;
};

struct IntersectResult {
    bool      isIntersect    = false;
    cv::Vec3f intersectPoint = cv::Vec3f(0, 0, 0);
    float     distance       = 0;
    Material  material;
};

inline float VecNorm(cv::Vec3f v) {
    float res = 0;
    for (int i = 0; i < 3; ++i) {
        res += v[i] * v[i];
    }
    return sqrt(res);
}
