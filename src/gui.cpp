#include "gui.h"
#include "app.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void Gui::startFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();

    const bool isRunning = App::instance->gameOfLife.isRunning;
    if (ImGui::Button(isRunning ? "Play":"Pause")) {
        App::instance->gameOfLife.isRunning = !isRunning;
    }
    float* stepTime = &App::instance->gameOfLife.stepTime;
    ImGui::SliderFloat("stepTime", stepTime, 0.01f, 2.0f);
}

void Gui::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
