#include <glad/glad.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/matrix.hpp>
#define GLFW_INCLUDE_NONE
#include "misc/camera.hpp"
#include "misc/shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int width = 800, height = 600;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

Camera camera(45.0f, (float)width / height, 0.1f, 100.0f,
              glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f));

void framebuffer_cb(GLFWwindow *window, int w, int h);
void mousepos_cb(GLFWwindow *window, double x, double y);
void scroll_cb(GLFWwindow *window, double x, double y);
void process_inp(GLFWwindow *window, double delta);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window =
        glfwCreateWindow(width, height, "Ray Tracer", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Window Creation failed" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Glad loading failed" << std::endl;
        return -1;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_cb);
    glfwSetCursorPosCallback(window, mousepos_cb);
    glfwSetScrollCallback(window, scroll_cb);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader shader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);

    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
                                      glm::vec3(1.0f, 1.0f, 1.0f));
    glm::vec3 lightpos = glm::vec3(-1.5f, 1.3f, -2.5f);
    glm::mat4 lightmove = glm::translate(glm::mat4(1.0f), lightpos);
    glm::vec3 objcolor = glm::vec3(1.0f, 0.5f, 0.31f);

    int projloc = glGetUniformLocation(shader.program, "projection"),
        viewloc = glGetUniformLocation(shader.program, "view"),
        traloc = glGetUniformLocation(shader.program, "transform"),
        islig = glGetUniformLocation(shader.program, "islight"),
        colorb = glGetUniformLocation(shader.program, "color"),
        lightloc = glGetUniformLocation(shader.program, "lightpos"),
        invtrans = glGetUniformLocation(shader.program, "invtrans"),
        viewpos = glGetUniformLocation(shader.program, "viewpos");

    glUseProgram(shader.program);
    glUniform3f(colorb, objcolor.x, objcolor.y, objcolor.z);
    glUniform3f(lightloc, lightpos.x, lightpos.y, lightpos.z);

    double pasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double delta = time - pasttime;
        pasttime = time;

        process_inp(window, delta);
        glUniform1i(islig, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // transform =
        //     glm::rotate(transform, (float)delta * glm::radians(60.0f) * 1.2f,
        //                 glm::vec3(2.0f, 3.0f, 1.0f));
        glUniformMatrix4fv(traloc, 1, GL_FALSE, glm::value_ptr(transform));

        camera.updatevec();
        camera.updatemat(projloc, viewloc);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniform1i(islig, 1);
        lightmove = glm::rotate(glm::mat4(1.0f),
                                (float)time * glm::radians(60.0f) * 1.2f,
                                glm::vec3(2.0f, 3.0f, 1.0f));
        lightmove = glm::translate(lightmove, glm::vec3(-1.5f, 1.3f, -2.5f));
        lightpos = glm::vec3(lightmove * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glUniform3f(lightloc, lightpos.x, lightpos.y, lightpos.z);
        glUniform3f(viewpos, camera.camerapos.x, camera.camerapos.y,
                    camera.camerapos.z);
        glUniformMatrix4fv(
            invtrans, 1, GL_TRUE,
            glm::value_ptr(glm::inverse(camera.view * transform)));
        glUniformMatrix4fv(traloc, 1, GL_FALSE, glm::value_ptr(lightmove));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_cb(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
    camera.asprat = (float)w / h;
    camera.isDirty = true;
}

void mousepos_cb(GLFWwindow *window, double x, double y) {
    camera.isDirty = true;
    if (camera.firstmouse) {
        camera.xlast = x;
        camera.ylast = y;
        camera.firstmouse = false;
    }
    float xoff = (x - camera.xlast) * camera.sens;
    float yoff = (y - camera.ylast) * camera.sens;
    camera.yaw += xoff;
    camera.pitch -= yoff;
    camera.xlast = x;
    camera.ylast = y;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x =
        cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z =
        sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

    camera.cameradir = glm::normalize(direction);
}

void scroll_cb(GLFWwindow *window, double xoffset, double yoffset) {
    camera.isDirty = true;
    camera.FOV -= (float)yoffset * 0.05f;
    if (camera.FOV < 1.0f)
        camera.FOV = 1.0f;
    if (camera.FOV > 90.0f)
        camera.FOV = 90.0f;
}

void process_inp(GLFWwindow *window, double delta) {

    float camspd = 5.0f * (float)delta;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.camerapos += camera.cameradir * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.camerapos += camera.cameraright * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.camerapos -= camera.cameradir * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.camerapos -= camera.cameraright * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.camerapos += camera.cameraup * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.camerapos -= camera.cameraup * camspd;
    }
}
