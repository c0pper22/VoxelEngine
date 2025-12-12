#include "Application.h"

void Application::HandleMouseMove(double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = 400.0f;
	static float lastY = 300.0f;

	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos);

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void Application::HandleScroll(double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Application::HandleResize(int width, int height)
{
	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
}

void Application::HandleKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window->getGLFWwindow(), true);
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		gui->toggleGui();
	}
}

Application::Application()
{
	window = std::make_unique<Window>(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE);

	window->onMouseMove = std::bind(&Application::HandleMouseMove, this, std::placeholders::_1, std::placeholders::_2);
	window->onScroll = std::bind(&Application::HandleScroll, this, std::placeholders::_1, std::placeholders::_2);
	window->onResize = std::bind(&Application::HandleResize, this, std::placeholders::_1, std::placeholders::_2);
	window->onKey = std::bind(&Application::HandleKey, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	gui = std::make_unique<Gui>(window.get());
	
	default_shader = std::make_unique<Shader>("Shaders/default.vert", "Shaders/default.frag", "Shaders/default.geom");
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 2.0f));
	m_lastFrameTime = 0.0f;
	world = std::make_unique<World>();
}

void Application::run()
{
	while (!window->shouldClose())
	{
		float currentFrame = glfwGetTime();
		dt = currentFrame - m_lastFrameTime;
		m_lastFrameTime = currentFrame;

		gui->startFrame();
		CTX ctx;
		ctx.camera = camera.get();
		ctx.world = world.get();
		gui->renderGui(ctx);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		default_shader->use();

		processInput();

		projection = glm::perspective(glm::radians(camera->Zoom), (float)window->getWidth() / (float)window->getHeight(), 0.1f, 100.0f);
		view = camera->GetViewMatrix();

		default_shader->setMat4("projection", projection);
		default_shader->setMat4("view", view);


		glm::mat4 model = glm::mat4(1.0f);
		default_shader->setMat4("model", model);
		default_shader->setVec3("uEdgeColor", 1.0f, 1.0f, 0.0f);
		default_shader->setVec3("uFaceColor", 0.1f, 0.1f, 0.1f);
		default_shader->setFloat("uLineWidth", 0.005f);

		world->draw(*default_shader);

		gui->finishFrame();

		window->swapBuffers();
		window->pollEvents();
	}
}

void Application::processInput()
{
	if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, dt);
	if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, dt);
	if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, dt);
	if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, dt);
	if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, dt);
	if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, dt);
}

