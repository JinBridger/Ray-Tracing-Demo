#include "RayTracer.h"

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>

void rayTraceDemo() {
    auto t = RayTracer(1024, 1024, 1.1, 0, 0, 4, 4096);

    Material mDBGCL, mWHITE, mLIGHT, mCYAN, mBLUE, mRED;
    mDBGCL.color       = WHITE;
    mDBGCL.specularRate = 1.0f;
    mCYAN.color        = CYAN;
    mBLUE.color        = BLUE;
    mRED.color         = RED;
    mWHITE.color       = WHITE;
    mLIGHT.color       = WHITE;
    mLIGHT.isEmmissive = true;


    t.addShape(new Sphere(cv::Vec3f(-0.6, -0.8, 0.6), 0.2, mDBGCL));
    t.addShape(new Sphere(cv::Vec3f(-0.1, -0.7, 0.2), 0.3, mDBGCL));
    t.addShape(new Sphere(cv::Vec3f(0.5, -0.6, -0.5), 0.4, mDBGCL));

    t.addShape(new Triangle(cv::Vec3f(0.4, 0.99, 0.4), cv::Vec3f(-0.4, 0.99, -0.4), cv::Vec3f(-0.4, 0.99, 0.4), mLIGHT));
    t.addShape(new Triangle(cv::Vec3f(0.4, 0.99, 0.4), cv::Vec3f(0.4, 0.99, -0.4), cv::Vec3f(-0.4, 0.99, -0.4), mLIGHT));
    // Box
    // Bottom
    t.addShape(new Triangle(cv::Vec3f(1, -1, 1), cv::Vec3f(-1, -1, -1), cv::Vec3f(-1, -1, 1), mWHITE));
    t.addShape(new Triangle(cv::Vec3f(1, -1, 1), cv::Vec3f(1, -1, -1), cv::Vec3f(-1, -1, -1), mWHITE));
    // Top
    t.addShape(new Triangle(cv::Vec3f(1, 1, 1), cv::Vec3f(-1, 1, 1), cv::Vec3f(-1, 1, -1), mWHITE));
    t.addShape(new Triangle(cv::Vec3f(1, 1, 1), cv::Vec3f(-1, 1, -1), cv::Vec3f(1, 1, -1), mWHITE));
    // Back
    t.addShape(new Triangle(cv::Vec3f(1, -1, -1), cv::Vec3f(-1, 1, -1), cv::Vec3f(-1, -1, -1), mCYAN));
    t.addShape(new Triangle(cv::Vec3f(1, -1, -1), cv::Vec3f(1, 1, -1), cv::Vec3f(-1, 1, -1), mCYAN));
    // Left
    t.addShape(new Triangle(cv::Vec3f(-1, -1, -1), cv::Vec3f(-1, 1, 1), cv::Vec3f(-1, -1, 1), mBLUE));
    t.addShape(new Triangle(cv::Vec3f(-1, -1, -1), cv::Vec3f(-1, 1, -1), cv::Vec3f(-1, 1, 1), mBLUE));
    // Right
    t.addShape(new Triangle(cv::Vec3f(1, 1, 1), cv::Vec3f(1, -1, -1), cv::Vec3f(1, -1, 1), mRED));
    t.addShape(new Triangle(cv::Vec3f(1, -1, -1), cv::Vec3f(1, 1, 1), cv::Vec3f(1, 1, -1), mRED));

    t.rayTrace();
}

void unitTest() {}

int main() {
    // Disable log output
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

    rayTraceDemo();
    // unitTest();
}