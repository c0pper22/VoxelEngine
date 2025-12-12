#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Window.h"

class Application;
struct CTX;

class Gui
{
private:
	bool displayed = false;
public:
	Gui(Window* window);
	~Gui();

	bool isDisplayed() { return displayed; };
	void toggleGui() { displayed = !displayed; };

	void startFrame();
	void renderGui(CTX& ctx);
	void finishFrame();
};