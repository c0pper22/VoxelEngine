#include "Application.h"
#include "Input.h"

void Application::HandleMouseMove(double xpos, double ypos)
{
    if (!gui->isDisplayed()) {
        glm::vec2 delta = Input::GetMouseDelta();
        camera->ProcessMouseMovement(delta.x, delta.y);
    }
}

void Application::HandleScroll(double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
    updateProjection();
}

void Application::toggleCursor()
{
    if (gui->isDisplayed()) {
        glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    }
    else {
        glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Application::HandleResize(int width, int height)
{
    glViewport(0, 0, width, height);
    updateProjection();
}

void Application::HandleKey(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window->getGLFWwindow(), true);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        toggleCursor();
        gui->toggleGui();
    }
}

void Application::updateProjection()
{
    int w = window->getWidth();
    int h = window->getHeight();

    if (h == 0) h = 1;

    projection = glm::perspective(glm::radians(camera->Zoom), (float)w / (float)h, 0.1f, viewDistance);
}

Application::Application()
{
    window = std::make_unique<Window>(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE);
    gui = std::make_unique<Gui>(window.get());

    Input::Init();
    BlockRegistry::Get().Init();

    window->onMouseMove = std::bind(&Application::HandleMouseMove, this, std::placeholders::_1, std::placeholders::_2);
    window->onScroll = std::bind(&Application::HandleScroll, this, std::placeholders::_1, std::placeholders::_2);
    window->onResize = std::bind(&Application::HandleResize, this, std::placeholders::_1, std::placeholders::_2);
    window->onKey = std::bind(&Application::HandleKey, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

    default_shader = std::make_unique<Shader>("Shaders/texture.vert", "Shaders/texture.frag");
    crosshair_shader = std::make_unique<Shader>("Shaders/crosshair.vert", "Shaders/crosshair.frag");

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 100.0f, 0.0f)); // Start higher up
    world = std::make_unique<World>();
    atlas = std::make_unique<Texture>("assets/atlas_1.png");

    player = std::make_unique<Player>(camera.get(), world.get(), glm::vec3(0, 100, 0));
    crosshair = std::make_unique<Crosshair>();

    updateProjection();
}

// -------------------------------------------------------------------------
// 3. CORE LOOP
// -------------------------------------------------------------------------

void Application::run()
{
    while (!window->shouldClose())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float frameTime = currentFrame - m_lastFrameTime;
        m_lastFrameTime = currentFrame;

        if (frameTime > 0.25f) frameTime = 0.25f;

        m_accumulator += frameTime;

        Input::Update();
        window->pollEvents();

        while (m_accumulator >= FIXED_TIMESTEP)
        {
            player->handleInput(FIXED_TIMESTEP);
            player->update(FIXED_TIMESTEP, *world);

            world->update(player->getPosition(), FIXED_TIMESTEP);
            world->updateFluids(FIXED_TIMESTEP);

            Input::ClearPendingKeys();

            m_accumulator -= FIXED_TIMESTEP;
        }

        float alpha = m_accumulator / FIXED_TIMESTEP;
        player->interpolate(alpha);

        // --- 1. Start GUI Frame ---
        gui->startFrame();

        // --- 2. Update GUI Data ---
        CTX ctx;
        ctx.camera = camera.get();
        ctx.world = world.get();
        ctx.player = player.get();
        ctx.viewDistance = &viewDistance;
        ctx.chunkRenderDistance = &world->renderDistance;

        gui->renderGui(ctx);

        // --- 3. Render World & Finish GUI ---
        render();
        window->swapBuffers();
    }
}

void Application::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    default_shader->use();
    atlas->bind(0);
    default_shader->setInt("texture1", 0);

    view = camera->GetViewMatrix();
    default_shader->setMat4("view", view);

    default_shader->setMat4("projection", projection);

    world->draw(*default_shader);

    crosshair->draw(*crosshair_shader, window->getWidth(), window->getHeight());
    gui->finishFrame();
}