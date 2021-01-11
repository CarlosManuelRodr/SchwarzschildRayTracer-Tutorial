#include <iostream>
#include <vector>
#include "RayTracer.h"
#include "Camera.h"
#include "BmpWriter.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

using namespace std;

int main()
{
    RandomGen::Seed();
    const int nx = 200; // Tamaño horizontal
    const int ny = 100; // Tamaño vertical
    const int ns = 100; // Muestras por cada pixel
    
    // Configuración de la cámara
    Vector3 cameraPosition(0.0f, 0.0f, 0.0f);
    Vector3 cameraLookAt(1.0f, 0.0f, -1.0f);
    Camera camera = Camera(cameraPosition, cameraLookAt, Vector3(0.0f, 1.0f, 0.0f), 90.0f, (float)nx / (float)ny);

    // Carga las texturas
    int earthTextWidth, earthTextHeight, earthTextChannels;
    unsigned char* earthTexture = stbi_load("Textures\\earthmap.jpg", &earthTextWidth, &earthTextHeight, &earthTextChannels, 0);

    // Inicializa el mundo
    vector<Hitable*> world;
    world.push_back(new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(new ImageTexture(earthTexture, earthTextWidth, earthTextHeight))));
    world.push_back(new Sphere(Vector3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(new ConstantTexture(Vector3(0.8f, 0.8f, 0.0f)))));
    world.push_back(new Sphere(Vector3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vector3(0.8f, 0.6f, 0.2f), 0.5f)));
    world.push_back(new Sphere(Vector3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f)));

    BMPWriter bmpwriter("test.bmp", nx, ny);

    for (int j = 0; j < ny; j++)
    {
        vector<BMPPixel> line;
        for (int i = 0; i < nx; i++)
        {
            Vector3 color = Vector3(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + RandomGen::Getfloat()) / float(nx);
                float v = float(j + RandomGen::Getfloat()) / float(ny);
                Ray r = camera.GetRay(u, v);

                color += ray_trace(r, world, 0);
            }
            color /= float(ns);
            color = Vector3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

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