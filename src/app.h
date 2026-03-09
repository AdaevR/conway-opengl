#pragma once

#include "camera.h"
#include "gameoflife.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION


struct MouseInputState {
    bool isActive = false;
    bool fillType = false;
};

class App
{
public:
    static App* instance;

    const unsigned int screenWidth = 800;
    const unsigned int screenHeight = 600;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    GLFWwindow* window;
    Camera camera;
    GameOfLife gameOfLife;

    int lastSpaceKeyState = GLFW_RELEASE;
    MouseInputState mouseInputState;

    App() : camera(glm::vec3(0.0f, 0.0f, 3.0f))
    {
        instance = this;
    };
    ~App();

    void run();
    int initWindow();

    void processInput(GLFWwindow *window);
    glm::vec2 screenToWorld(float mouseX, float mouseY);
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void cursorCallback(GLFWwindow* window, double xoffset, double yoffset);
