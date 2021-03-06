//
// Created by jayohanna on 2020-04-15.
//

#ifndef OPENWORLD_PLAYER_H
#define OPENWORLD_PLAYER_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player {
public:

    Player(glm::vec3 pos);

    void initPlayer(glm::vec3 pos);

    void movePlayer(GLfloat x, GLfloat y, GLfloat z);

    GLfloat x;
    GLfloat y;
    GLfloat z;

    glm::vec3 defaultPosition;

    GLfloat playerSpeed;
    GLfloat jumpSpeed;
    GLfloat height;
    GLfloat verticalVelocity;

    const glm::vec3& getDefaultPosition() const;
};


#endif //OPENWORLD_PLAYER_H
