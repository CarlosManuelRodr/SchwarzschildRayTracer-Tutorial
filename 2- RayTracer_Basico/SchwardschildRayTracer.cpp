#include <iostream>
#include <vector>
#include "RayTracer.h"
#include "BmpWriter.h"
using namespace std;

int main()
{
    const int nx = 200;
    const int ny = 100;
    Vector3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    Vector3 horizontal(4.0f, 0.0f, 0.0f);
    Vector3 vertical(0.0f, 2.0f, 0.0f);
    Vector3 origin(0.0f, 0.0f, 0.0f);

    // Inicializa el mundo
    vector<Hitable*> world;
    world.push_back(new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f));
    world.push_back(new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f));

    BMPWriter bmpwriter("test.bmp", nx, ny);

    for (int j = 0; j < ny; j++)
    {
        vector<BMPPixel> line;
        for (int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

            Vector3 color = ray_trace(r, world);

            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);

            BMPPixel pixel;
            pixel.r = ir;
            pixel.g = ig;
            pixel.b = ib;

            line.push_back(pixel);
        }
        bmpwriter.WriteLine(line.data());
    }
    bmpwriter.CloseBMP();
    return 0;
}