#pragma once
#include <cfloat>
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Material.h"

Vector3 ray_trace(const Ray& r, HitableList world, int depth)
{
    HitRecord rec;
    if (world.Hit(r, 0.001f, FLT_MAX, rec))
    {
        Ray scattered;
        Vector3 attenuation;

        if (depth < 50 && rec.matPtr->Scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_trace(scattered, world, depth + 1);
        }
        else
        {
            return Vector3(0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        Vector3 unitDirection = unit_vector(r.Direction());
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * Vector3(1.0f, 1.0f, 1.0f) + t * Vector3(0.5f, 0.7f, 1.0f);
    }
}