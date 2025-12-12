#include "Window.h"

static Window* g_CurrentWindow = nullptr;

Window::Window(int width, int height, const char* title)
    : width(width), height(height)
{
    g_CurrentWindow = this;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (glfw_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(glfw_window, mouse_callback);
    glfwSetScrollCallback(glfw_window, scroll_callback);
    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwDestroyWindow(glfw_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(glfw_window);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (g_CurrentWindow)
    {
        g_CurrentWindow->width = width;
        g_CurrentWindow->height = height;

        if (g_CurrentWindow->onResize)
            g_CurrentWindow->onResize(width, height);
    }
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (g_CurrentWindow && g_CurrentWindow->onMouseMove)
        g_CurrentWindow->onMouseMove(xpos, ypos);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (g_CurrentWindow && g_CurrentWindow->onScroll)
        g_CurrentWindow->onScroll(xoffset, yoffset);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check if the current window exists and if an application function is attached
    if (g_CurrentWindow && g_CurrentWindow->onKey)
    {
        g_CurrentWindow->onKey(key, scancode, action, mods);
    }
}
