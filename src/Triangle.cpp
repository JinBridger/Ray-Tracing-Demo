#include "Triangle.h"

Triangle::Triangle(cv::Vec3f point_a, cv::Vec3f point_b, cv::Vec3f point_c, Material material)
    : m_pointA(point_a), m_pointB(point_b), m_pointC(point_c) {
    m_material = material;

    cv::Vec3f vecAB = point_b - point_a;
    cv::Vec3f vecAC = point_c - point_a;

    cv::Vec3f normalVec  = cv::normalize(vecAB.cross(vecAC));
    m_material.normalVec = normalVec;
}

IntersectResult Triangle::intersect(Ray ray) const {
    IntersectResult result;

    cv::Vec3f vecS = ray.startPoint;
    cv::Vec3f vecd = ray.direction;
    cv::Vec3f vecN = m_material.normalVec;

    if (vecN.dot(vecd) > 0.0f)
        vecN = -vecN;

    if (abs(vecN.dot(vecd)) < 0.0001f)
        return result;

    float t = (vecN.dot(m_pointA) - vecS.dot(vecN)) / vecd.dot(vecN);
    if (t < 0.0005f)
        return result;

    cv::Vec3f vecP = vecS + vecd * t;

    cv::Vec3f vecAB = m_pointB - m_pointA;
    cv::Vec3f vecBC = m_pointC - m_pointB;
    cv::Vec3f vecCA = m_pointA - m_pointC;

    cv::Vec3f vecAP = vecP - m_pointA;
    cv::Vec3f vecBP = vecP - m_pointB;
    cv::Vec3f vecCP = vecP - m_pointC;

    cv::Vec3f vecC1 = vecAB.cross(vecAP);
    cv::Vec3f vecC2 = vecBC.cross(vecBP);
    cv::Vec3f vecC3 = vecCA.cross(vecCP);

    cv::Vec3f vecn = m_material.normalVec;

    if (vecC1.dot(vecn) < 0 || vecC2.dot(vecn) < 0 || vecC3.dot(vecn) < 0) {
        return result;
    }

    result.isIntersect        = true;
    result.distance           = t;
    result.intersectPoint     = vecP;
    result.material           = m_material;
    result.material.normalVec = vecN;

    return result;
}
