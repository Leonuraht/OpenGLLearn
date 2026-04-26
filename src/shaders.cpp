
#include "Misc/shaders.hpp"
#include <cstdlib>

void filesize(FILE *fptr, long *n) {
    fseek(fptr, 0, SEEK_END);
    *n = ftell(fptr);
    rewind(fptr);
}

char *openpath(char *path) {
    FILE *fptr = fopen(path, "r");
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
