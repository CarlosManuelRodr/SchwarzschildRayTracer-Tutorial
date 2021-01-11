#pragma once
#include "Ray.h"

/**
 * @struct HitRecord
 * @brief Estructura que guarda los parámetros de una colisión.
 */
struct HitRecord
{
    Vector3 p;          // Posición donde ocurre la colisión
    Vector3 normal;     // Vector normal de la superficie colisionada
    float t;            // Parámetro donde ocurrió la colisión
};

/**
 * @class Hitable
 * @brief Clase abstracta que representa un objeto colisionable.
 */
class Hitable
{
public:
    virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};