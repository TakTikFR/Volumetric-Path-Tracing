#include "image.hh"

#include <cmath>
#include <fstream>
#include <stdexcept>

void Image::checkBounds(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::out_of_range(
            "Image::checkBounds: coordinates out of range.");
}

RGB &Image::get(int x, int y)
{
    checkBounds(x, y);
    return pixels_[y * width + x];
}

const RGB &Image::get(int x, int y) const
{
    checkBounds(x, y);
    return pixels_[y * width + x];
}

void Image::set(int x, int y, const RGB &value)
{
    checkBounds(x, y);
    pixels_[y * width + x] = value;
}

void Image::save(const std::string &filepath) const
{
    std::ofstream file(filepath);
    if (!file)
        throw std::runtime_error("Image::save: cannot open file \"" + filepath
                                 + "\".");

    file << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            const RGB &p = get(i, j);
            double r_norm = p.r / 255.0;
            double g_norm = p.g / 255.0;
            double b_norm = p.b / 255.0;

            // gamma correction
            r_norm = std::sqrt(std::max(0.0, r_norm));
            g_norm = std::sqrt(std::max(0.0, g_norm));
            b_norm = std::sqrt(std::max(0.0, b_norm));

            int ir = static_cast<int>(
                std::max(0.0, std::min(255.0, r_norm * 255.0)));
            int ig = static_cast<int>(
                std::max(0.0, std::min(255.0, g_norm * 255.0)));
            int ib = static_cast<int>(
                std::max(0.0, std::min(255.0, b_norm * 255.0)));

            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}

RGB RGB::operator+(const RGB &other) const
{
    return RGB(r + other.r, g + other.g, b + other.b);
}

RGB RGB::operator*(const RGB &other) const
{
    return RGB((r * other.r) / 255.0, (g * other.g) / 255.0,
               (b * other.b) / 255.0);
}

RGB RGB::operator*(double scalar) const
{
    return RGB(r * scalar, g * scalar, b * scalar);
}

RGB RGB::operator/(double scalar) const
{
    if (scalar == 0.0)
        return RGB(0, 0, 0);
    return RGB(r / scalar, g / scalar, b / scalar);
}
