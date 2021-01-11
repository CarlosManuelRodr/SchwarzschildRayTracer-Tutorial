#pragma once
#include "Hitable.h"

/**
 * @class Sphere
 * @brief Objeto colisionable esférico.
 */
class Sphere : public Hitable
{
public:
    Vector3 center;
    float radius;

    Sphere(Vector3 cen, float r) : center(cen), radius(r) {};
    virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
};

bool Sphere::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
    // Calcula la intersección del rayo con la esfera.
    Vector3 oc = r.Origin() - center;
    float a = dot(r.Direction(), r.Direction());
    float b = dot(oc, r.Direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    // Si el discriminante es > 0 existe una intersección.
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.p = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}