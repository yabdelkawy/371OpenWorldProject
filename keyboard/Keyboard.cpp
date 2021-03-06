//
// Created by danseremet on 2020-04-04.
//

#include "Keyboard.h"

Keyboard::Keyboard() = default;

GLboolean Keyboard::monitorCounted = false;
bool Keyboard::oKeyReleased = true;
bool Keyboard::fpKeyReleased = true;

void Keyboard::processInput(Game *game) {
    // Handle keyboard inputs

    GLFWwindow *window = game->getWindow();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Camera *camera = game->getCamera();
    Player* player = game->getPlayer();
    GLfloat dt = game->dt;


    int newWidth;
    int newHeight;
    glfwGetWindowSize(window, &newWidth, &newHeight);
    if ((newWidth - static_cast<int>(game->getScrWidth())) || (newHeight - static_cast<int>(game->getScrHeight()))) {
        game->setScrWidth(newWidth);
        game->setScrHeight(newHeight);
        game->updateAspectRatio();
        game->updateProjectionMatrix();
        glViewport(0, 0, newWidth, newHeight);
    }

    // On program launch set speed multiplier based on monitor count
    static constexpr float DEV_SLOW{1.0f};
    static constexpr float PROD_FAST{60.0f};
    constexpr int MONITOR_NUMBER_PROD{1};
    static float SPEED_MULTIPLIER{DEV_SLOW};
    if (!monitorCounted) {
        int count;
        glfwGetMonitors(&count);
        if (count == MONITOR_NUMBER_PROD) {
            SPEED_MULTIPLIER = PROD_FAST;
        }
    }

    const float currentCameraSpeed = camera->cameraSpeed;
    float currentPlayerSpeed = player->playerSpeed;
    float currentPlayerHeight = player->height;

    // Camera logic from a lab (modified for separating panning/tilting/zooming)
    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    double dx = mousePosX - game->lastMousePosX;
    double dy = mousePosY - game->lastMousePosY;
    game->lastMousePosX = mousePosX;
    game->lastMousePosY = mousePosY;

    float cameraAngularSpeed = SPEED_MULTIPLIER * 6.0f;


    glm::vec3 cameraSideVector = glm::cross(camera->cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

    if (game->cameraFirstPerson)
    {
        cameraAngularSpeed = SPEED_MULTIPLIER * 3.0f;

        camera->cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        camera->cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        camera->cameraVerticalAngle = std::fmax(-85.0f, std::fmin(85.0f, camera->cameraVerticalAngle));
        if (camera->cameraHorizontalAngle > 360)
        {
            camera->cameraHorizontalAngle -= 360;
        }
        else if (camera->cameraHorizontalAngle < -360)
        {
            camera->cameraHorizontalAngle += 360;
        }

        float theta = glm::radians(camera->cameraHorizontalAngle);
        float phi = glm::radians(camera->cameraVerticalAngle);

        camera->cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        cameraSideVector = glm::cross(camera->cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { // shift
            currentPlayerSpeed = player->playerSpeed * 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { // control
            currentPlayerHeight = player->height / 2;
            currentPlayerSpeed = player->playerSpeed * 0.4f;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // forward W
            player->x += camera->cameraLookAt[0] * dt * currentPlayerSpeed;
            player->z += camera->cameraLookAt[2] * dt * currentPlayerSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // backward S
            player->x -= camera->cameraLookAt[0] * dt * currentPlayerSpeed;
            player->z -= camera->cameraLookAt[2] * dt * currentPlayerSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // left A
            player->x -= cameraSideVector[0] * dt * currentPlayerSpeed;
            player->z -= cameraSideVector[2] * dt * currentPlayerSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // right D
            player->x += cameraSideVector[0] * dt * currentPlayerSpeed;
            player->z += cameraSideVector[2] * dt * currentPlayerSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (game->playerOnGround())
            {
                player->verticalVelocity = player->jumpSpeed;
            }
        }

        camera->cameraPosition = glm::vec3(player->x, player->y + currentPlayerHeight, player->z);

    }
    else
    {
        cameraAngularSpeed = SPEED_MULTIPLIER * 3.0f;

            camera->cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
            camera->cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

            // Clamp vertical angle to [-85, 85] degrees
            camera->cameraVerticalAngle = std::fmax(-85.0f, std::fmin(85.0f, camera->cameraVerticalAngle));
            if (camera->cameraHorizontalAngle > 360)
            {
                camera->cameraHorizontalAngle -= 360;
            }
            else if (camera->cameraHorizontalAngle < -360)
            {
                camera->cameraHorizontalAngle += 360;
            }

            float theta = glm::radians(camera->cameraHorizontalAngle);
            float phi = glm::radians(camera->cameraVerticalAngle);

            camera->cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
            cameraSideVector = glm::cross(camera->cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::normalize(cameraSideVector);

        // LEFT MOUSE BUTTON ZOOM IN AND OUT of scene
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && dy > 0) {
            game->setFovy(game->getFovy() + 0.01f);
            game->updateProjectionMatrix();
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && dy < 0) {
            game->setFovy(game->getFovy() - 0.01f);
            game->updateProjectionMatrix();
        }

        // Extra camera movement keys (not a requirement, but easier to work with)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // forward W
            camera->cameraPosition += camera->cameraLookAt * dt * currentCameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // backward S
            camera->cameraPosition -= camera->cameraLookAt * dt * currentCameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // right D
            camera->cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // left A
            camera->cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }

        player->movePlayer(camera->cameraPosition[0], game->findTerrainYat(camera->cameraPosition[2], camera->cameraPosition[0]), camera->cameraPosition[2]);

        // Return Home
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
            camera->initCamera(camera->getDefaultPosition());
            game->setFovy(70.0f);
            game->updateProjectionMatrix();
        }
    }
    
    // Show Points
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        game->polygonMode = GL_POINT;
    }
    // Show Lines
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        game->polygonMode = GL_LINE;
    }
    // Show Triangles
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        game->polygonMode = GL_FILL;
    }

    // Refresh rock
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (oKeyReleased)
        {
            auto rocks = game->getRocks();

            std::map<int, std::map<int, Model*>>::iterator itr;
            std::map<int, Model*>::iterator ptr;

            for (itr = rocks.begin(); itr != rocks.end(); itr++) {
                for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
                    auto rock = (RockModel*) ptr->second;
                    rock->resetRock();
                }
            }
            oKeyReleased = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) {
        oKeyReleased = true;
    }

    // Toggle first person view
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (fpKeyReleased)
        {
            game->cameraFirstPerson = !game->cameraFirstPerson;
        }
        fpKeyReleased = false;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
        fpKeyReleased = true;
    }
}
