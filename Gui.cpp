#include "Gui.h"
#include "Application.h"

Gui::Gui(Window* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui_ImplGlfw_InitForOpenGL(window->getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::startFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::renderGui(CTX& ctx)
{
    if (isDisplayed())
    {
        static int counter = 0;
        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");

        ImGui::SliderFloat("View Distance", ctx.viewDistance, 0.1f, 2000.0f);
        ImGui::SliderInt("Render Distance", ctx.chunkRenderDistance, 0, 48);

        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::SliderFloat("Zoom", &ctx.camera->Zoom, 1.0f, 45.0f);

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
        ImGui::Render();
}

void Gui::finishFrame()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
