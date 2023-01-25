#pragma once

#include "Utils.h"

class Shape {
public:
    virtual IntersectResult intersect(Ray) const = 0;
};