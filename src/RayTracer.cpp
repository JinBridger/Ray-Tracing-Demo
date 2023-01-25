#include "RayTracer.h"

#include <omp.h>

RayTracer::RayTracer(unsigned int img_width, unsigned int img_height, float screen_z, float cam_x, float cam_y,
                     float cam_z, unsigned int sample)
    : m_imageGenerator(img_width, img_height), m_randomGenerator(), m_screenZ(screen_z),
      m_cameraCoord(cam_x, cam_y, cam_z), m_sample(sample), m_brightness(2.0f * 3.1415926f / float(m_sample)) {}

void RayTracer::addShape(Shape* ptr) {
    m_shapes.push_back(ptr);
}

void RayTracer::rayTrace() {
    unsigned int width  = m_imageGenerator.getWidth();
    unsigned int height = m_imageGenerator.getHeight();

    std::vector<std::vector<cv::Vec3f>> fColor;
    fColor.resize(width);
    for (unsigned int i = 0; i < width; ++i) {
        fColor[i].resize(height);
    }

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            fColor[i][j] = cv::Vec3f(0, 0, 0);
        }
    }

    int renderedCount = 0;

#pragma omp parallel for
    for (int k = 0; k < m_sample; ++k) {
        for (unsigned int i = 0; i < width; ++i) {
            for (unsigned int j = 0; j < height; ++j) {
                float x = 2.0f * float(i) / float(width) - 1.0f;
                float y = 2.0f * float(j) / float(height) - 1.0f;

                auto rayStartPoint = cv::Vec3f(x, y, m_screenZ);
                auto rayDirection  = cv::normalize(rayStartPoint - m_cameraCoord);

                Ray ray;
                ray.startPoint = rayStartPoint;
                ray.direction  = rayDirection;

                auto result = emitRay(ray);
                auto color  = cv::Vec3b(0, 0, 0);

                if (result.isIntersect) {
                    if (result.material.isEmmissive) {
                        auto emmissiveFColor = colorB2F(result.material.color);

                        fColor[i][j] += emmissiveFColor * m_brightness;
                    } else {
                        Ray randomRay;
                        randomRay.startPoint = result.intersectPoint;

                        float cosineVal;

                        cv::Vec3f mixedColor;

                        float randomFloat = m_randomGenerator.generate();
                        if (randomFloat < result.material.specularRate) {
                            randomRay.direction = genReflectVec(result.material.normalVec, ray.direction);
                            cosineVal = (cv::Vec3f(0, 0, 0) - randomRay.direction).dot(result.material.normalVec);

                            mixedColor = pathTrace(randomRay, 0) * (-cosineVal) * m_brightness;
                        } else {
                            randomRay.direction = genRandomVec(result.material.normalVec);
                            cosineVal = (cv::Vec3f(0, 0, 0) - randomRay.direction).dot(result.material.normalVec);

                            cv::Vec3b srcBColor = result.material.color;
                            cv::Vec3f srcFColor = colorB2F(srcBColor);
                            cv::Vec3f pthFColor = pathTrace(randomRay, 0) * (-cosineVal);

                            mixedColor = mixColor(srcFColor, pthFColor, m_brightness);
                        }

                        // Ray randomRay;
                        // randomRay.startPoint = result.intersectPoint;
                        // randomRay.direction  = genRandomVec(result.material.normalVec);
                        // 
                        // auto srcBColor = result.material.color;
                        // auto pthFColor = pathTrace(randomRay, 0);
                        // 
                        // auto srcFColor = colorB2F(srcBColor);
                        // 
                        // auto mixedFColor = mixColor(srcFColor, pthFColor, m_brightness);

                        fColor[i][j] += mixedColor;
                    }
                }
            }
        }
#pragma omp critical
        renderedCount++;

        printf("Rendered %d sample...\n", renderedCount);
    }

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            auto bColor = colorF2B(fColor[i][j]);

            uint8_t R = bColor[0];
            uint8_t G = bColor[1];
            uint8_t B = bColor[2];

            m_imageGenerator.setPixelColor(i, j, R, G, B);
        }
    }

    m_imageGenerator.showImage();
}

IntersectResult RayTracer::emitRay(Ray ray) {
    IntersectResult result, tmpResult;
    result.distance = 9999999.0f;

    for (auto& shape : m_shapes) {
        tmpResult = shape->intersect(ray);
        if (tmpResult.isIntersect && tmpResult.distance < result.distance) {
            result = tmpResult;
        }
    }

    return result;
}

cv::Vec3f RayTracer::genRandomVec(cv::Vec3f vec) {
    cv::Vec3f addVec;

    do {
        float ran_1 = m_randomGenerator.generate();
        float ran_2 = m_randomGenerator.generate();
        float ran_3 = m_randomGenerator.generate();

        addVec = 2.0f * cv::Vec3f(ran_1, ran_2, ran_3) - cv::Vec3f(1.0f, 1.0f, 1.0f);
    } while (VecNorm(addVec) > 1.0f);

    addVec = cv::normalize(addVec);
    vec    = cv::normalize(vec);

    auto result = cv::normalize(vec + addVec);
    return result;
}

cv::Vec3f RayTracer::genReflectVec(cv::Vec3f normalVec, cv::Vec3f inDirection) {
    cv::Vec3f vecN = cv::normalize(normalVec);
    cv::Vec3f vecS = cv::normalize(-inDirection);

    float ratio = vecN.dot(vecS);
    vecN *= ratio;

    cv::Vec3f vecSN = vecN - vecS;
    cv::Vec3f vecR  = vecN + vecSN;
    return cv::normalize(vecR);
}

cv::Vec3f RayTracer::pathTrace(Ray ray, int depth) {
    IntersectResult result = emitRay(ray);

    if (depth >= 8)
        return cv::Vec3f(0, 0, 0);

    if (!result.isIntersect)
        return cv::Vec3f(0, 0, 0);

    if (result.material.isEmmissive)
        return colorB2F(result.material.color);

    float randomFloat  = m_randomGenerator.generate();
    float endThreshold = 0.8;
    if (randomFloat > endThreshold) {
        return cv::Vec3f(0, 0, 0);
    }

    Ray randomRay;
    randomRay.startPoint = result.intersectPoint;

    float cosineVal;

    cv::Vec3f mixedColor;

    randomFloat = m_randomGenerator.generate();
    if (randomFloat < result.material.specularRate) {
        randomRay.direction = genReflectVec(result.material.normalVec, ray.direction);
        cosineVal           = (cv::Vec3f(0, 0, 0) - randomRay.direction).dot(result.material.normalVec);

        mixedColor = pathTrace(randomRay, depth + 1) * (-cosineVal);
    } else {
        randomRay.direction = genRandomVec(result.material.normalVec);
        cosineVal           = (cv::Vec3f(0, 0, 0) - randomRay.direction).dot(result.material.normalVec);

        cv::Vec3b srcBColor = result.material.color;
        cv::Vec3f srcFColor = colorB2F(srcBColor);
        cv::Vec3f pthFColor = pathTrace(randomRay, depth + 1) * (-cosineVal);

        mixedColor = mixColor(srcFColor, pthFColor, 1);
    }

    return mixedColor / endThreshold;
}

cv::Vec3b RayTracer::colorF2B(cv::Vec3f floatColor) {
    float color0 = floatColor[0];
    float color1 = floatColor[1];
    float color2 = floatColor[2];

    if (color0 > 1.0f)
        color0 = 1.0f;
    if (color0 < 0.0f)
        color0 = 0.0f;

    if (color1 > 1.0f)
        color1 = 1.0f;
    if (color1 < 0.0f)
        color1 = 0.0f;

    if (color2 > 1.0f)
        color2 = 1.0f;
    if (color2 < 0.0f)
        color2 = 0.0f;

    uint8_t cvtColor0 = 255 * color0;
    uint8_t cvtColor1 = 255 * color1;
    uint8_t cvtColor2 = 255 * color2;

    return cv::Vec3b(cvtColor0, cvtColor1, cvtColor2);
}

cv::Vec3f RayTracer::colorB2F(cv::Vec3b ucharColor) {
    uint8_t color0 = ucharColor[0];
    uint8_t color1 = ucharColor[1];
    uint8_t color2 = ucharColor[2];

    float cvtColor0 = float(color0) / 255.0f;
    float cvtColor1 = float(color1) / 255.0f;
    float cvtColor2 = float(color2) / 255.0f;

    return cv::Vec3f(cvtColor0, cvtColor1, cvtColor2);
}

cv::Vec3f RayTracer::mixColor(cv::Vec3f color1, cv::Vec3f color2, float brightness) {
    float res0 = color1[0] * color2[0] * brightness;
    float res1 = color1[1] * color2[1] * brightness;
    float res2 = color1[2] * color2[2] * brightness;

    return cv::Vec3f(res0, res1, res2);
}
