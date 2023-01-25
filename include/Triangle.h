#pragma once

#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(cv::Vec3f, cv::Vec3f, cv::Vec3f, Material);
    
    IntersectResult intersect(Ray) const override;

private:
    cv::Vec3f m_pointA;
    cv::Vec3f m_pointB;
    cv::Vec3f m_pointC;
    Material  m_material;
};