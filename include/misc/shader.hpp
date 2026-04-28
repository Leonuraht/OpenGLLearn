#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <glad/glad.h>
#include <string>

void filesize(FILE *fptr, long *n);

char *openpath(const char *path);

void freebuff(char *buffer);

void checkshader(unsigned int i);

void checkprogram(unsigned int i);

class Shader {
  public:
    unsigned int vertex, fragment, program;
    char *vertbuff, *fragbuff;
    Shader(std::string vertpath, std::string fragpath);
};

#endif
