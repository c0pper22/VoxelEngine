#include "Application.h"

void Application::HandleMouseMove(double xpos, double ypos)
{
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

		if(!gui->isDisplayed())
			camera->ProcessMouseMovement(xoffset, yoffset);
}

void Application::HandleScroll(double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Application::toggleCursor()
{
	if (gui->isDisplayed())
	{
		glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Application::HandleResize(int width, int height)
{
	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, viewDistance);
}

void Application::HandleKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window->getGLFWwindow(), true);
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		toggleCursor();
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
	
	default_shader = std::make_unique<Shader>("Shaders/texture.vert", "Shaders/texture.frag");
	crosshair_shader = std::make_unique<Shader>("Shaders/crosshair.vert", "Shaders/crosshair.frag");
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 2.0f));
	m_lastFrameTime = 0.0f;
	world = std::make_unique<World>();
	atlas = std::make_unique<Texture>("assets/atlas_1.png");
	player = std::make_unique<Player>(camera.get(), world.get());
	crosshair = std::make_unique<Crosshair>();
}

void Application::run()
{
	while (!window->shouldClose())
	{
		float currentFrame = glfwGetTime();
		dt = currentFrame - m_lastFrameTime;
		m_lastFrameTime = currentFrame;

		gui->startFrame();
		world->update(player->getPosition());
		CTX ctx;
		ctx.camera = camera.get();
		ctx.world = world.get();
		ctx.viewDistance = &viewDistance;
		ctx.chunkRenderDistance = &world->renderDistance;
		gui->renderGui(ctx);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		default_shader->use();

		default_shader->setInt("texture1", 0);
		atlas->bind(0);

		player->update(dt);
		processInput();

		projection = glm::perspective(glm::radians(camera->Zoom), (float)window->getWidth() / (float)window->getHeight(), 0.1f, viewDistance);
		view = camera->GetViewMatrix();

		default_shader->setMat4("projection", projection);
		default_shader->setMat4("view", view);


		glm::mat4 model = glm::mat4(1.0f);
		default_shader->setMat4("model", model);
		//default_shader->setVec3("uEdgeColor", 1.0f, 1.0f, 0.0f);
		//default_shader->setVec3("uFaceColor", 0.1f, 0.1f, 0.1f);
		//default_shader->setFloat("uLineWidth", 0.005f);

		world->draw(*default_shader);

		crosshair->draw(*crosshair_shader, window->getWidth(), window->getHeight());

		gui->finishFrame();

		window->swapBuffers();
		window->pollEvents();
	}
}

void Application::processInput()
{
	player->handleInput(window.get(), dt);
}

