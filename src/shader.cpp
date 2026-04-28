#include "misc/shader.hpp"
#include <iostream>

void filesize(FILE *fptr, long *n) {
    fseek(fptr, 0, SEEK_END);
    *n = ftell(fptr);
    rewind(fptr);
}

char *openpath(const char *path) {
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL) {
        std::cout << "Shader file loading failed" << std::endl;
        return NULL;
    }
    int k, ptr;
    long len;
    filesize(fptr, &len);
    if (fptr == NULL) {
        printf("File opening failed...\n");
        return NULL;
    }
    char *buffer = new char[len + 1];
    ptr = 0;
    while ((k = fgetc(fptr)) != EOF) {
        buffer[ptr++] = (char)k;
    }
    buffer[ptr++] = '\0';
    fclose(fptr);
    return buffer;
}

void freebuff(char *buffer) { delete[] buffer; }

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

Shader::Shader(std::string vertpath, std::string fragpath) {
    vertbuff = openpath(vertpath.c_str());
    fragbuff = openpath(fragpath.c_str());
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *intervert = vertbuff;
    const char *interfrag = fragbuff;
    glShaderSource(vertex, 1, &intervert, NULL);
    glShaderSource(fragment, 1, &interfrag, NULL);
    glCompileShader(vertex);
    checkshader(vertex);
    glCompileShader(fragment);
    checkshader(fragment);
    freebuff(vertbuff);
    freebuff(fragbuff);
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    checkprogram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
