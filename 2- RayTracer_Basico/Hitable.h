#pragma once
#include "Ray.h"

/**
 * @struct HitRecord
 * @brief Estructura que guarda los par�metros de una colisi�n.
 */
struct HitRecord
{
    Vector3 p;          // Posici�n donde ocurre la colisi�n
    Vector3 normal;     // Vector normal de la superficie colisionada
    float t;            // Par�metro donde ocurri� la colisi�n
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