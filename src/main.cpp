#include <glad/glad.h>
#include <glm/ext/quaternion_geometric.hpp>
#define GLFW_INCLUDE_NONE
#include "misc/camera.hpp"
#include "misc/shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int width = 800, height = 600;

float vertices[] = {
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // top right f
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, // bottom right f
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // top left f
    -0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f, // bottom left f
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, // top right b
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right b
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, // top left b
    -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f  // bottom left b
};
unsigned int indices[] = {0, 1, 2, 1, 3, 2, 4, 5, 6, 5, 7, 6, 4, 0, 6, 0, 2, 6,
                          1, 5, 3, 5, 7, 3, 4, 5, 0, 5, 1, 0, 2, 3, 6, 3, 7, 6};

Camera camera(45.0f, (float)width / height, 0.1f, 100.0f,
              glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, -1.0f));

void framebuffer_cb(GLFWwindow *window, int w, int h);
void mousepos_cb(GLFWwindow *window, double x, double y);
void scroll_cb(GLFWwindow *window, double x, double y);

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

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader shader(
        "/home/leonuraht/storage/CFiles/OpenGLLight/src/shaders/vertex.glsl",
        "/home/leonuraht/storage/CFiles/OpenGLLight/src/shaders/fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
                                      glm::vec3(1.0f, 1.0f, 1.0f));

    int projloc = glGetUniformLocation(shader.program, "projection"),
        viewloc = glGetUniformLocation(shader.program, "view"),
        traloc = glGetUniformLocation(shader.program, "transform");

    glUseProgram(shader.program);
    double pasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double delta = time - pasttime;
        pasttime = time;

        float camspd = 5.0f * (float)delta;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.isDirty = true;
            camera.camerapos -= camera.cameradir * camspd;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.isDirty = true;
            camera.camerapos -= camera.cameraright * camspd;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.isDirty = true;
            camera.camerapos += camera.cameradir * camspd;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.isDirty = true;
            camera.camerapos += camera.cameraright * camspd;
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            camera.isDirty = true;
            camera.camerapos += camera.cameraup * camspd;
        }
        if(glfwGetKey(window,GLFW_KEY_COMMA) == GLFW_PRESS){
            camera.isDirty = true;
            camera.camerapos -= camera.cameraup * camspd;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        transform = glm::rotate(transform,(float)delta * glm::radians(60.0f) * 1.2f,
                                glm::vec3(2.0f, 3.0f, 1.0f));
        glUniformMatrix4fv(traloc, 1, GL_FALSE, glm::value_ptr(transform));

        camera.updatevec();
        camera.updatemat(projloc, viewloc);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
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
    camera.pitch += yoff;
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
