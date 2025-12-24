#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include <iostream>
#include <functional>

constexpr unsigned int DEFAULT_SCREEN_WIDTH = 800;
constexpr unsigned int DEFAULT_SCREEN_HEIGHT = 800;
constexpr const char* WINDOW_TITLE = "Voxel Engine";

class Window 
{
private:
    GLFWwindow* glfw_window;
    unsigned int width;
    unsigned int height;

public:
	Window(int width, int height, const char* title);
	~Window();

    bool shouldClose() { return glfwWindowShouldClose(glfw_window); }
    unsigned int getWidth() { return width; }
    unsigned int getHeight() { return height; }
    GLFWwindow* getGLFWwindow() { return glfw_window; }

    void pollEvents();
    void swapBuffers();

    std::function<void(int, int)> onResize;
    std::function<void(double, double)> onMouseMove;
    std::function<void(double, double)> onScroll;
    std::function<void(int, int, int, int)> onKey;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};