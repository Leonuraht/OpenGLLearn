#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
  public:
    glm::vec3 camerapos, cameradir, cameraright, cameraup, cameratarget;
    float FOV, asprat, near, far, sens = 0.3f,yaw = -90.0f,pitch = 0.0f,xlast = 0.0f,ylast = 0.0f;
    bool firstmouse = true,isDirty = true;
    glm::mat4 proj, view;
    Camera(float FOV, float asprat, float near, float far, glm::vec3 campos,
           glm::vec3 camdir);
    void updatevec();
    void updatemat(int projloc,int viewloc);
};

#endif
