#include "app.h"
#include "gameoflife.h"
#include "gui.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

App* App::instance = nullptr;

App::~App()
{
    glfwTerminate();
}

void App::run()
{
    initWindow();

    Renderer renderer;
    renderer.init(window);

    Gui gui;
    gui.init(window);

    float elapsedTime = 0;

    gameOfLife.setCell({0, -1}, true);
    gameOfLife.setCell({-1, 0}, true);
    gameOfLife.setCell({0, 0}, true);
    gameOfLife.setCell({0, 1}, true);
    gameOfLife.setCell({1, 1}, true);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        gui.startFrame();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        elapsedTime += deltaTime;
        if (gameOfLife.isRunning && elapsedTime > gameOfLife.stepTime)
        {
            gameOfLife.step();
            elapsedTime = 0;
        }
        renderer.render(camera, gameOfLife.getCells());
        gui.render();

        glfwSwapBuffers(window);
    }
}

int App::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPA.T, GL_TRUE);
#endif

    window = glfwCreateWindow(screenWidth, screenHeight, "Conway", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw "Failed to create GLFW window";
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw "Failed to initialize GLAD";
    }

    return 0;
}

void App::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    int spaceKayState{glfwGetKey(window, GLFW_KEY_SPACE)};
    if (spaceKayState == GLFW_PRESS && lastSpaceKeyState == GLFW_RELEASE)
    {
        gameOfLife.isRunning = !gameOfLife.isRunning;
    }

    lastSpaceKeyState = spaceKayState;
}

glm::vec2 App::screenToWorld(float screenX, float screenY)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspectRatio = (float)width / (float)height;
    float orthoSize = camera.Zoom;
    glm::mat4 projection =
        glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, 0.1f, 100.0f);
    float x = (2.0f * screenX) / width - 1.0f;
    float y = 1.0f - (2.0f * screenY) / height; // Flip Y because OpenGL has Y up

    glm::vec4 ndcPos = glm::vec4(x, y, 0.0f, 1.0f);

    glm::mat4 viewProjection = projection * camera.GetViewMatrix();
    glm::mat4 inverseViewProjection = glm::inverse(viewProjection);

    glm::vec4 worldPos = inverseViewProjection * ndcPos;

    worldPos /= worldPos.w;

    return glm::vec2(worldPos.x, worldPos.y);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (io.WantCaptureMouse)
        {
            return;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec2 worldPos = App::instance->screenToWorld(xpos, ypos);
        int x = int(glm::round(worldPos.x));
        int y = int(glm::round(-worldPos.y));
        bool contains = App::instance->gameOfLife.getCells().contains({x,y});

        App::instance->mouseInputState.fillType = !contains;
        App::instance->mouseInputState.isActive = true;

        App::instance->gameOfLife.setCell({x, y}, !contains);

    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        App::instance->mouseInputState.isActive = false;
    }
}

void cursorCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(App::instance->mouseInputState.isActive)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec2 worldPos = App::instance->screenToWorld(xpos, ypos);
        int x = int(glm::round(worldPos.x));
        int y = int(glm::round(-worldPos.y));
        bool cellState = App::instance->mouseInputState.fillType;

        App::instance->gameOfLife.setCell({x, y}, cellState);
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    App::instance->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
