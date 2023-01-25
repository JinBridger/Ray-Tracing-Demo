#include "Sphere.h"

Sphere::Sphere(cv::Vec3f center, float radius, Material material)
    : m_center(center), m_radius(radius), m_material(material) {}

IntersectResult Sphere::intersect(Ray ray) const {
    IntersectResult result;

    cv::Vec3f vecS = ray.startPoint;
    cv::Vec3f vecd = ray.direction;

    cv::Vec3f vecSO = m_center - vecS;

    float lenSO = VecNorm(vecSO);
    float lenSH = vecSO.dot(vecd);
    float lenOH = sqrt(lenSO * lenSO - lenSH * lenSH);

    if (lenOH > m_radius)
        return result;

    if (lenSH < 0)
        return result;

    float lenPH = sqrt(m_radius * m_radius - lenOH * lenOH);

    float t_1 = abs(lenSH) - lenPH;
    float t_2 = abs(lenSH) + lenPH;
    float t   = (t_1 < 0) ? t_2 : t_1;

    cv::Vec3f vecP  = vecS + t * vecd;
    cv::Vec3f vecOP = vecP - m_center;

    if (abs(t_1) < 0.0005f || abs(t_2) < 0.0005f)
        return result;

    result.isIntersect        = true;
    result.distance           = t;
    result.intersectPoint     = vecP;
    result.material           = m_material;
    result.material.normalVec = cv::normalize(vecOP);

    return result;
}