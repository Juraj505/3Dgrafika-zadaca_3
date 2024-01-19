#include <iostream>
#include <cmath>
#include "tgaimage.h"
using namespace std;

// compile with:
// g++ rasterizer.cpp tgaimage.cpp -o out.exe

// dimenzije slike
const int width = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

float f(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void draw_triangle_2d(float x0, float y0, float x1, float y1, float x2, float y2, TGAImage &image, TGAColor colour)
{
    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = f(x1, y1, x2, y2, x, y) / f(x1, y1, x2, y2, x0, y0);

            float beta = f(x2, y2, x0, y0, x, y) / f(x2, y2, x0, y0, x1, y1);

            float gamma = f(x0, y0, x1, y1, x, y) / f(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
                image.set(x, y, colour);
        }
    }
}

void draw_triangle_2d_gouraud(float x0, float y0, TGAColor c0, float x1, float y1, TGAColor c1, float x2, float y2, TGAColor c2, TGAImage &image)
{
    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float xmax = max(max(ceil(x0), ceil(x1)), ceil(x2));

    float ymin = min(min(floor(y0), floor(y1)), floor(y2));
    float ymax = max(max(ceil(y0), ceil(y1)), ceil(y2));

    for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = f(x1, y1, x2, y2, x, y) / f(x1, y1, x2, y2, x0, y0);

            float beta = f(x2, y2, x0, y0, x, y) / f(x2, y2, x0, y0, x1, y1);

            float gamma = f(x0, y0, x1, y1, x, y) / f(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
                image.set(x, y, TGAColor((alpha * c0.r + beta * c1.r + gamma * c2.r), (alpha * c0.g + beta * c1.g + gamma * c2.g), (alpha * c0.b + beta * c1.b + gamma * c2.b), 255));
        }
    }
}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    // nacrtaj 2 trokuta proizvoljnih boja
    draw_triangle_2d(160, 190, 310, 220, 270, 340, image, green);
    draw_triangle_2d(130, 180, 210, 120, 140, 200, image, blue);

    // dodatan trokut obojan Gourardovom interpolacijom
    draw_triangle_2d_gouraud(370, 400, red, 480, 390, blue, 450, 340, green, image);

    // podešavanje koordinatnog sustava
    image.flip_vertically();

    // spremi sliku
    image.write_tga_file("problem2.tga");
}