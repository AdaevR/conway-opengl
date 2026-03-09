#pragma once

#include "imgui_impl_glfw.h"

class Gui
{
public:
    ~Gui();
    void init(GLFWwindow* window);
    void startFrame();
    void render();
};
