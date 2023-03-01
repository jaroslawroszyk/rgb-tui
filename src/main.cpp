#include "../include/MainComponent.hpp"

using namespace ftxui;

int main()
{
    int r = 255;
    int g = 0;
    int b = 0;
    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(Make<MainComponent>(r, g, b));

    return 0;
}
