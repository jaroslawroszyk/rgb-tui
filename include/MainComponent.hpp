#pragma once

#include <fmt/core.h>
#include <cstddef>
#include <iostream>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "../include/Helpers.hpp"

using namespace ftxui;

class MainComponent : public ComponentBase
{
public:
    MainComponent(int &r, int &g, int &b);

private:
    auto Render() -> Element final;

    auto OnEvent(Event event) -> bool final;

    bool OnMouseEvent(Event event);

    int &r_;
    int &g_;
    int &b_;
    int h_;
    int s_;
    int v_;
    Component color_hue_ = Slider(L"Hue:        ", &h_, 0, 255, 1);
    Component color_saturation_ = Slider(L"Saturation: ", &s_, 0, 255, 1);
    Component color_value_ = Slider(L"Value:      ", &v_, 0, 255, 1);
    Component color_red_ = Slider(L"Red:        ", &r_, 0, 255, 1);
    Component color_green_ = Slider(L"Green:      ", &g_, 0, 255, 1);
    Component color_blue_ = Slider(L"Blue:       ", &b_, 0, 255, 1);
    Component container_ = Container::Vertical({
            color_hue_,
            color_saturation_,
            color_value_,
            color_red_,
            color_green_,
            color_blue_,
    });

    Box box_color_;
    CapturedMouse captured_mouse_;

    bool isEventMouse(Event &event);

    void getSaturation(Elements &array) const;
};