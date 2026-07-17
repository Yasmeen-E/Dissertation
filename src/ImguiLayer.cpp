#include "ImguiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>


ImGuiLayer::~ImGuiLayer() {
    if (m_initialized) {
        Shutdown();
    }
}

void ImGuiLayer::Init(GLFWwindow* window, const char* glslVersion) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init(glslVersion);
    m_initialized = true;
}

void ImGuiLayer::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void ImGuiLayer::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();
}

void ImGuiLayer::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::DrawUI(Settings& settings) {

    ImGui::Begin("Controls");
    ImGui::Checkbox("Snow", &settings.snow);
    ImGui::SliderFloat("Time", &settings.time, 0.0f, 1.0f);
    ImGui::Checkbox("Wind", &settings.wind);
    if (settings.wind) {
    ImGui::Indent();
    ImGui::SliderFloat("Wind Angle", &settings.windAngle, 0.0f, 360.0f);
    ImGui::Unindent();
    }
    ImGui::End(); 
}
