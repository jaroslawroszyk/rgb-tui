#include "../include/MainComponent.hpp"

MainComponent::MainComponent(int &r, int &g, int &b) : r_(r), g_(g), b_(b)
{
    Add(container_);
    toHSV(r_, g_, b_, h_, s_, v_);
    box_color_.x_min = 0;
    box_color_.y_min = 0;
    box_color_.x_max = 80;
    box_color_.y_max = 1;
}

Element MainComponent::Render()
{
    std::string rgb_txt = fmt::format("{:3} , {:3} , {:3} ", r_, g_, b_);
    std::string hsv_txt = fmt::format("{:3}°, {:3}%, {:3}%", //
            int(h_ * 360. / 255.), //
            int(s_ * 100. / 255.), //
            int(v_ * 100. / 255.)  //
    );

    int hue = h_;
    Elements array {};
    int x_length = std::max(10, box_color_.x_max - box_color_.x_min) + 1;
    int y_length = 15;

    int h, s, v;
    toHSV(r_, g_, b_, h, s, v);
    int target_x = std::max(0, std::min(x_length - 1, (v * x_length) / 255));
    int target_y =
            std::max(0, std::min(2 * y_length - 1, (s * 2 * y_length) / 255));

    for(int y = 0; y < y_length; ++y)
    {
        Elements line;
        for(int x = 0; x < x_length; ++x)
        {
            int saturation_1 = 255 * (y + 0.0f) / float(y_length);
            int saturation_2 = 255 * (y + 0.5f) / float(y_length);
            int value = 255 * x / float(x_length);
            if(x == target_x)
            {
                if(2 * y == target_y)
                {
                    line.push_back(text(L"▀")                                    //
                            | color(Color::HSV(hue, saturation_1, value)) //
                            | bgcolor(Color::Black));                     //
                    continue;
                }
                if(2 * y == target_y + 1)
                {
                    line.push_back(text(L"▀")            //
                            | color(Color::Black) //
                            | bgcolor(Color::HSV(hue, saturation_2, value)));
                    continue;
                }
            }
            line.push_back(text(L"▀")                                    //
                    | color(Color::HSV(hue, saturation_1, value)) //
                    | bgcolor(Color::HSV(hue, saturation_2, value)));
        }
        array.push_back(hbox(std::move(line)));
    }
    getSaturation(array);

    return vbox({
            window(text(L"[ rgb-tui ]") | center, //
                    vbox({
                            hbox({
                                    vbox(std::move(array)) | flex | reflect(box_color_),
                            }),
                            separator(),
                            hbox({
                                    colorTile(r_, g_, b_),
                                    separator(),
                                    vbox({
                                            color_hue_->Render(),
                                            color_saturation_->Render(),
                                            color_value_->Render(),
                                            separator(),
                                            color_red_->Render(),
                                            color_green_->Render(),
                                            color_blue_->Render(),
                                    }) | flex,
                            }),
                    })),
            hbox({
                    window(text(L" Hexa ") | center, hexaElement(r_, g_, b_)),
                    window(text(L" RGB ") | center, text(to_wstring(rgb_txt))),
                    window(text(L" HSV ") | center, text(to_wstring(hsv_txt))),
            }),
    }) |
            size(WIDTH, LESS_THAN, 80);
}

void MainComponent::getSaturation(Elements &array) const
{
    for(int saturation = 0; saturation < 255; saturation += 20)
    {
        Elements line;
        array.push_back(hbox(std::move(line)));
    }
}

bool MainComponent::OnEvent(Event event)
{
    int r = r_;
    int g = g_;
    int b = b_;
    int h = h_;
    int s = s_;
    int v = v_;

    bool out = false;

    if(event.is_mouse())
        out |= OnMouseEvent(std::move(event));
    out |= ComponentBase::OnEvent(std::move(event));

    if(h != h_ || s != s_ || v != v_)
        toRGB(h_, s_, v_, //
                r_, g_, b_);

    else if(r != r_ || g != g_ || b != b_)
        toHSV(r_, g_, b_, //
                h_, s_, v_);
    return out;
}

bool MainComponent::OnMouseEvent(Event event)
{
    if(event.mouse().motion == Mouse::Released)
    {
        captured_mouse_ = nullptr;
        return true;
    }

    if(box_color_.Contain(event.mouse().x, event.mouse().y) &&
            CaptureMouse(event))
    {
        TakeFocus();
    }

    if(event.mouse().button == Mouse::Left &&
            event.mouse().motion == Mouse::Pressed &&
            box_color_.Contain(event.mouse().x, event.mouse().y) &&
            !captured_mouse_)
    {
        captured_mouse_ = CaptureMouse(event);
    }

    if(captured_mouse_)
    {
        return isEventMouse(event);
    }
    return false;
}

bool MainComponent::isEventMouse(Event &event)
{
    v_ = (event.mouse().x - box_color_.x_min) * 255 /
            (box_color_.x_max - box_color_.x_min);
    s_ = (event.mouse().y - box_color_.y_min) * 255 /
            (box_color_.y_max - box_color_.y_min);
    v_ = std::max(0, std::min(255, v_));
    s_ = std::max(0, std::min(255, s_));
    return true;
}
