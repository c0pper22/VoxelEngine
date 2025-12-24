#pragma once
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "World.h"
#include "Gui.h"
#include "Player.h"
#include "Crosshair.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CTX
{
    Camera* camera;
    World* world;
    Player* player;
    float* viewDistance;
    int* chunkRenderDistance;
};

class Application
{
private:
    float m_lastFrameTime = 0.0f;
    float dt = 0.0f;

    // Cursor state
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = true;

    float viewDistance = 500.0f;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    void toggleCursor();
    void updateProjection();

    void HandleResize(int width, int height);
    void HandleMouseMove(double xpos, double ypos);
    void HandleScroll(double xoffset, double yoffset);
    void HandleKey(int key, int scancode, int action, int mods);

public:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> default_shader;
    std::unique_ptr<Shader> crosshair_shader;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<World> world;
    std::unique_ptr<Gui> gui;
    std::unique_ptr<Texture> atlas;
    std::unique_ptr<Player> player;
    std::unique_ptr<Crosshair> crosshair;

    Application();

    void run();
    void update(float dt);
    void render();
};