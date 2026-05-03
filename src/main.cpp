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
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int width = 800, height = 600;
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    Shader shader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);

    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
                                      glm::vec3(1.0f, 1.0f, 1.0f));
    glm::vec3 objcolor = glm::vec3(1.0f, 0.5f, 0.31f);

    int projloc = glGetUniformLocation(shader.program, "projection"),
        viewloc = glGetUniformLocation(shader.program, "view"),
        traloc = glGetUniformLocation(shader.program, "transform"),
        islig = glGetUniformLocation(shader.program, "islight"),
        colorb = glGetUniformLocation(shader.program, "color"),
        invtrans = glGetUniformLocation(shader.program, "invtrans"),
        matamb = glGetUniformLocation(shader.program, "material.ambient"),
        matdiff = glGetUniformLocation(shader.program, "material.diffuse"),
        matspe = glGetUniformLocation(shader.program, "material.specular"),
        matshi = glGetUniformLocation(shader.program, "material.shineness");

    glUseProgram(shader.program);
    glUniform3f(colorb, objcolor.x, objcolor.y, objcolor.z);
    glUniform3f(matamb, 0.0f, 1.0f, 0.6f);
    glUniform1i(matdiff, 0);
    glUniform1i(matspe, 1);
    glUniform1f(matshi, 25.0f);

    glUniform3f(glGetUniformLocation(shader.program, "Dirlight.ambient"), 0.05f,
                0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(shader.program, "Dirlight.diffuse"), 0.4f,
                0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(shader.program, "Dirlight.specular"), 0.5f,
                0.5f, 0.5f);

    for (int i = 0; i < 4; i++) {
        std::string number = std::to_string(i);
        glUniform3f(
            glGetUniformLocation(shader.program,
                                 ("light[" + number + "].ambient").c_str()),
            0.05f, 0.05f, 0.05f);
        glUniform3f(
            glGetUniformLocation(shader.program,
                                 ("light[" + number + "].diffuse").c_str()),
            0.8f, 0.8f, 0.8f);
        glUniform3f(
            glGetUniformLocation(shader.program,
                                 ("light[" + number + "].specular").c_str()),
            1.0f, 1.0f, 1.0f);
    }
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

    glActiveTexture(GL_TEXTURE0);
    int tw, th, tch, tmw, tmh, tmch;
    unsigned int diffmap, specmap;
    glGenTextures(1, &diffmap);
    glBindTexture(GL_TEXTURE_2D, diffmap);
    unsigned char *diffmapval =
        stbi_load("/home/leonuraht/Downloads/container.png", &tw, &th, &tch, 3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 diffmapval);
    glGenerateTextureMipmap(diffmap);
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &specmap);
    glBindTexture(GL_TEXTURE_2D, specmap);
    unsigned char *spdata =
        stbi_load("/home/leonuraht/Downloads/container_specular.png", &tmw,
                  &tmh, &tmch, 3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmw, tmh, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, spdata);
    glGenerateTextureMipmap(specmap);

    stbi_image_free(spdata);
    stbi_image_free(diffmapval);

    double pasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double delta = time - pasttime;
        pasttime = time;

        process_inp(window, delta);
        glUniform1i(islig, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transform =
            glm::rotate(transform, (float)delta * glm::radians(10.0f) * 1.2f,
                        glm::vec3(2.0f, 3.0f, 1.0f));

        camera.updatevec();
        camera.updatemat(projloc, viewloc);

        glm::vec3 dirLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
        glm::vec3 viewDirLight =
            glm::vec3(camera.view * glm::vec4(dirLightDirection, 0.0f));
        glUniform3f(glGetUniformLocation(shader.program, "Dirlight.dir"),
                    viewDirLight.x, viewDirLight.y, viewDirLight.z);

        for (int i = 0; i < 4; i++) {
            std::string number = std::to_string(i);
            glm::vec3 viewLightPos = glm::vec3(
                camera.view * glm::vec4(pointLightPositions[i], 1.0f));
            glUniform3f(
                glGetUniformLocation(shader.program,
                                     ("light[" + number + "].pos").c_str()),
                viewLightPos.x, viewLightPos.y, viewLightPos.z);
        }
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(traloc, 1, GL_FALSE,
                               glm::value_ptr(model * transform));
            glUniformMatrix4fv(
                invtrans, 1, GL_TRUE,
                glm::value_ptr(glm::inverse(camera.view * model * transform)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glUniform1i(islig, 1);
        for (unsigned int i = 0; i < 4; i++) {
            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, pointLightPositions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));
            glUniformMatrix4fv(traloc, 1, GL_FALSE, glm::value_ptr(lightModel));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
