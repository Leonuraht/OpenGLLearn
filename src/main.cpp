#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#define GLFW_INCLUDE_NONE
#include "Misc/shaders.hpp"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include <string>

int width = 800, height = 600;

void framebuffercallback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

float vertices[] = {
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, // top right f
    0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, // bottom right f
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, // top left f
    -0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f, 0.0f,
    0.0f, // bottom left f
    0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, // top right b
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, // bottom right b
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, // top left b
    -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.0f,
    0.0f // bottom left b
};
unsigned int indices[] = {0, 1, 2, 1, 3, 2, 4, 5, 6, 5, 7, 6, 4, 0, 6, 0, 2, 6,
                          1, 5, 3, 5, 7, 3, 4, 5, 0, 5, 1, 0, 2, 3, 6, 3, 7, 6};

void checkshader(unsigned int i) {
    GLint succ;
    glGetShaderiv(i, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        char logdata[512];
        glGetShaderInfoLog(i, 512, NULL, logdata);
        printf("Shader Compilation failed : %s\n", logdata);
    }
}

void checkprogram(unsigned int i) {
    GLint succ;
    glGetProgramiv(i, GL_LINK_STATUS, &succ);
    if (!succ) {
        char logdata[512];
        glGetProgramInfoLog(i, 512, NULL, logdata);
        printf("Program Compilation failed : %s\n", logdata);
    }
}

int twidth, theight, tnrcolorch;
int twidth2, theight2, tnrcolorch2;
float movem = 0;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(width, height, "Ray Tracer", NULL, NULL);
    if (window == NULL) {
        printf("Window Creation failed\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Glad init failed\n");
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffercallback);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unsigned int Vertshader, FragShader, program;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *texturedata =
        stbi_load("/home/leonuraht/storage/Downloads/vim1.png", &twidth,
                  &theight, &tnrcolorch, 3);

    unsigned char *texturedata2 =
        stbi_load("/home/leonuraht/Downloads/rei Ayanami.jpg", &twidth2,
                  &theight2, &tnrcolorch2, 3);

    unsigned int texture, texture2;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, texturedata);
    glGenerateMipmap(GL_TEXTURE_2D);
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth2, theight2, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, texturedata2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texturedata);
    stbi_image_free(texturedata2);

    std::string imgpath1 =
        "/home/leonuraht/storage/CFiles/OpenGL/src/shaders/vertex.glsl";
    std::string imgpath2 =
        "/home/leonuraht/storage/CFiles/OpenGL/src/shaders/fragment.glsl";
    Shader shader = {(char *)imgpath1.c_str(), (char *)imgpath2.c_str()};

    char *vertexsource = openpath(shader.vertexpath);
    char *fragmentsource = openpath(shader.fragmentpath);

    Vertshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertshader, 1, ((const char *const *)&vertexsource), NULL);
    glCompileShader(Vertshader);
    checkshader(Vertshader);

    FragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragShader, 1, (const char *const *)&fragmentsource, NULL);
    glCompileShader(FragShader);
    checkshader(FragShader);

    program = glCreateProgram();
    glAttachShader(program, Vertshader);
    glAttachShader(program, FragShader);
    glLinkProgram(program);
    checkprogram(program);
    glDeleteShader(FragShader);
    glDeleteShader(Vertshader);

    freebuff(vertexsource);
    freebuff(fragmentsource);

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texll"), 0);
    glUniform1i(glGetUniformLocation(program, "texdata"), 2);
    int loc = glGetUniformLocation(program, "color");
    int movloc = glGetUniformLocation(program, "mov");

    glm::mat4 projec;
    projec = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f,
                              100.0f);
    glm::mat4 view;
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f, -3.0f));
    glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE,
                       glm::value_ptr(projec));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    float k = 0.0f;
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        float gval = sin(time) / 2.0f;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            movem += 0.1;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            movem -= 0.1;
        // mats = glm::translate(mats, glm::vec3(0.0f, 0.0f, 0.0f));
        // mats = glm::scale(mats, glm::vec3(gval, gval, 0.5f));
        k++;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        glUniform1f(loc, gval);
        glUniform1f(movloc, movem);
        for (int i = 0; i < 10; i++) {
            glm::mat4 mats = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            mats =
                glm::rotate(mats, glm::radians(k), glm::vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1,
                               GL_FALSE, glm::value_ptr(mats));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
