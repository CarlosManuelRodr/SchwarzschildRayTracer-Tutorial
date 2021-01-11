#pragma once
#include <cfloat>
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"

Vector3 ray_trace(const Ray& r, HitableList world)
{
    HitRecord rec;
    if (world.Hit(r, 0.0f, FLT_MAX, rec))
    {
        return 0.5f * Vector3(rec.normal.x() + 1.0f, rec.normal.y() + 1.0f, rec.normal.z() + 1.0f);
    }
    else
    {
        Vector3 unitDirection = unit_vector(r.Direction());
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * Vector3(1.0f, 1.0f, 1.0f) + t * Vector3(0.5f, 0.7f, 1.0f);
    }
}