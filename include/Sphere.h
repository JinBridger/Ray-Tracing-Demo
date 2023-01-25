#pragma once

#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere(cv::Vec3f center, float radius, Material material);

    IntersectResult intersect(Ray) const override;

private:
    cv::Vec3f m_center;
    float     m_radius;
    Material  m_material;
};