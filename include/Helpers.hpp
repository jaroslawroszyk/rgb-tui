#pragma once

#include <iostream>
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

auto colorTile(int r, int g, int b) -> Element;

auto hexLetter(int x) -> wchar_t;

auto hexColor(int r, int g, int b) -> std::wstring;

auto hexaElement(int r, int g, int b) -> Element;

auto toRGB(int h,
           int s,
           int v,
           int &r,
           int &g,
           int &b) -> void;

auto toHSV(int r, int g, int b, int &h, int &s, int &v) -> void;
