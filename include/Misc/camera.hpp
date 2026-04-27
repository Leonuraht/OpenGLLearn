#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

class Camera {
  private:
    bool firstmouse = true;
    float FOV, aspectratio, nearplane, farplane,
        xlast = 0.0f, ylast = 0.0f, yaw = -90.0f, pitch = 0.0f, sens = 0.2f;
    glm::vec3 camerapos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameradir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraright =
        glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameradir));
    glm::vec3 cameraup = glm::cross(cameradir, cameraright);
    glm::mat4 projection, view;

  public:
    Camera(float fov, float asp, float np, float fp)
        : FOV(fov), aspectratio(asp), nearplane(np), farplane(fp) {
        projection = glm::perspective(FOV, aspectratio, nearplane, farplane);
    }
    void scroll_cb(GLFWwindow *window, double x, double y) {
        FOV -= (float)y;
        if (FOV < 1.0f)
            FOV = 1.0f;
        else if (FOV > 90.0f)
            FOV = 90.0f;
    }
    void mouse_cb(GLFWwindow *window, double x, double y) {
        if (firstmouse) {
            xlast = x;
            ylast = y;
            firstmouse = false;
        }
        float xdiff = (x - xlast) * sens;
        float ydiff = (y - ylast) * sens;
        yaw += xdiff;
        pitch -= ydiff;

        if (pitch < -90.0f)
            pitch = -90.0f;
        else if (pitch > 90.0f)
            pitch = 90.0f;

        glm::vec3 dir;
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameradir = glm::normalize(dir);
    }
    void updatematrices(unsigned int program) {
        projection = glm::perspective(FOV, aspectratio, nearplane, farplane);
        glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE,
                           glm::value_ptr(projection));
        view = glm::lookAt(camerapos, camerapos + cameradir, cameraup);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));
    }
    void updatefeilds() {
        cameraright =
            glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameradir));
        cameraup = glm::cross(cameradir, cameraright);
    }

    void updatemovement(GLFWwindow *window, double pasttime) {
        double time = glfwGetTime();
        double delta = time - pasttime;
        float camspd = 5.0f * delta;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camerapos -= camspd * cameradir;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camerapos += camspd * cameradir;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camerapos -= cameraright * camspd;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camerapos += cameraright * camspd;
        }
    }
};
