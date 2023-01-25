#pragma once

#include "ImageGenerator.h"
#include "RandomGenerator.h"
#include "Sphere.h"
#include "Triangle.h"

#include <memory>
#include <vector>

class RayTracer {
public:
    RayTracer(unsigned int img_width, unsigned int img_height, float screen_z, float cam_x, float cam_y, float cam_z, unsigned sample);

    void addShape(Shape* ptr);
    void rayTrace();

private:
    IntersectResult emitRay(Ray);
    cv::Vec3f       genRandomVec(cv::Vec3f);
    cv::Vec3f       genReflectVec(cv::Vec3f, cv::Vec3f);
    cv::Vec3f       pathTrace(Ray, int);
    cv::Vec3b       colorF2B(cv::Vec3f);
    cv::Vec3f       colorB2F(cv::Vec3b);
    cv::Vec3f       mixColor(cv::Vec3f, cv::Vec3f, float);

    ImageGenerator  m_imageGenerator;
    RandomGenerator m_randomGenerator;

    float     m_screenZ;
    cv::Vec3f m_cameraCoord;

    std::vector<Shape*> m_shapes;
    unsigned int        m_sample;
    float               m_brightness;
};