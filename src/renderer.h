#pragma once

#include "camera.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <set>
#include <vector>

struct Mesh
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class Renderer
{
public:
    GLFWwindow* window;
    Shader* sharedShader;

    unsigned int VBO, VAO, EBO;
    Mesh squareMesh = {
        .vertices = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        },
        .indices = {0, 1, 3, 1, 2, 3}
    };
    Renderer() : sharedShader(nullptr) {}
    ~Renderer();
    void init(GLFWwindow* window);
    void render(Camera camera, std::set<std::pair<int, int>> cells);
};
