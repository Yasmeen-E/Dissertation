#pragma once

struct GLFWwindow;

class ImGuiLayer {
public:
    ImGuiLayer() = default;
    ~ImGuiLayer();

    void Init(GLFWwindow* window, const char* glslVersion = "#version 130");
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void DrawUI();

private:
    bool m_initialized = false;
};
