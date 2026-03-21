#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

struct RGB
{
    uint8_t r, g, b;

    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0)
        : r(r)
        , g(g)
        , b(b)
    {}

    RGB operator+(const RGB &other) const;
    RGB operator*(const RGB &other) const;
    RGB operator*(double scalar) const;
    RGB operator/(double scalar) const;
};

class Image
{
public:
    int width;
    int height;

    Image(int width, int height)
        : width(width)
        , height(height)
        , pixels_(width * height)
    {}

    Image(int width, int height, const std::vector<RGB> &pixels)
        : width(width)
        , height(height)
        , pixels_(pixels)
    {}

    RGB &get(int x, int y);
    const RGB &get(int x, int y) const;
    void set(int x, int y, const RGB &value);
    void save(const std::string &filepath) const;

private:
    std::vector<RGB> pixels_;

    void checkBounds(int x, int y) const;
};