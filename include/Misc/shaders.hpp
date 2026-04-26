#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include <cstdio>

typedef struct {
    char *vertexpath, *fragmentpath;
} Shader;

void filesize(FILE *fptr, long *n);

char *openpath(char *path);

void freebuff(char *buffer);

#endif
