#include <iostream>
#include <vector>
#include "BmpWriter.h"
using namespace std;

int main()
{
    const int nx = 200;
    const int ny = 100;
    BMPWriter bmpwriter("test.bmp", nx, ny);

    for (int j = 0; j < ny; j++)
    {
        vector<BMPPixel> line;
        for (int i = 0; i < nx; i++)
        {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

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