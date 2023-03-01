#include "../include/Helpers.hpp"

using namespace ftxui;

auto colorTile(int r, int g, int b) -> Element
{
    return text(L"")                       //
            | size(WIDTH, EQUAL, 20)        //
            | size(HEIGHT, EQUAL, 7)        //
            | bgcolor(Color::RGB(r, g, b)); //
}

auto hexLetter(int x) -> wchar_t
{
    if(x < 0 or x > 15)
        throw std::out_of_range("invalid hex digit");

    if(x <= 9)
        return U'0' + x;
    else
        return U'A' + x - 0xA;
};

auto hexColor(int r, int g, int b) -> std::wstring
{
    std::wstring out {};
    out += L"#";
    out += hexLetter(r / 16);
    out += hexLetter(r % 16);
    out += hexLetter(g / 16);
    out += hexLetter(g % 16);
    out += hexLetter(b / 16);
    out += hexLetter(b % 16);
    return out;
}

auto hexaElement(int r, int g, int b) -> Element
{
    return text(hexColor(r, g, b));
}

auto toRGB(int h,
           int s,
           int v,
           int &r,
           int &g,
           int &b) -> void
{
    if(s == 0)
    {
        r = v;
        g = v;
        b = v;
        return;
    }

    uint8_t region = h / 43;
    uint8_t remainder = (h - (region * 43)) * 6;
    uint8_t p = (v * (255 - s)) >> 8;
    uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    // clang-format off
    switch (region)
    {
    case 0:
        r = v, g = t, b = p;
        return;
    case 1:
        r = q, g = v, b = p;
        return;
    case 2:
        r = p, g = v, b = t;
        return;
    case 3:
        r = p, g = q, b = v;
        return;
    case 4:
        r = t, g = p, b = v;
        return;
    case 5:
        r = v, g = p, b = q;
        return;
    }
    // clang-format on
}

auto toHSV(int r, int g, int b, int &h, int &s, int &v) -> void
{
    int rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
    int rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

    v = rgbMax;
    if(v == 0)
    {
        h = 0;
        s = 0;
        return;
    }

    s = 255 * int(rgbMax - rgbMin) / v;
    if(s == 0)
    {
        h = 0;
        return;
    }

    if(rgbMax == r)
        h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
    else if(rgbMax == g)
        h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
    else
        h = 171 + 43 * (r - g) / (rgbMax - rgbMin);
}