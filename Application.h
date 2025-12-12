#pragma once
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "World.h"
#include "Gui.h"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CTX
{
	Camera* camera;
	World* world;
};

class Application 
{
private:
	std::unique_ptr<Window> window;
	float m_lastFrameTime;
    float dt;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	void HandleResize(int width, int height);
	void HandleMouseMove(double xpos, double ypos);
	void HandleScroll(double xoffset, double yoffset);
	void HandleKey(int key, int scancode, int action, int mods);
public:
	std::unique_ptr<Shader> default_shader;
    std::unique_ptr<Camera> camera;
	std::unique_ptr<World> world;
	std::unique_ptr<Gui> gui;

	Application();

	void run();
    void processInput();
};