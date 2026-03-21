#include "image.hh"

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
            file << static_cast<int>(p.r) << ' ' << static_cast<int>(p.g) << ' '
                 << static_cast<int>(p.b) << '\n';
        }
    }
}

RGB RGB::operator+(const RGB &other) const
{
    return RGB(r + other.r, g + other.g, b + other.b);
}

RGB RGB::operator*(const RGB &other) const
{
    return RGB(static_cast<uint8_t>(r * other.r / 255.0),
               static_cast<uint8_t>(g * other.g / 255.0),
               static_cast<uint8_t>(b * other.b / 255.0));
}

RGB RGB::operator*(double scalar) const
{
    return RGB(static_cast<uint8_t>(r * scalar),
               static_cast<uint8_t>(g * scalar),
               static_cast<uint8_t>(b * scalar));
}

RGB RGB::operator/(double scalar) const
{
    return RGB(static_cast<uint8_t>(r / scalar),
               static_cast<uint8_t>(g / scalar),
               static_cast<uint8_t>(b / scalar));
}
