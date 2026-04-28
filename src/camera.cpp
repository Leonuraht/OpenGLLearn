#include "misc/camera.hpp"
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(float FOV, float asprat, float near, float far, glm::vec3 campos,
               glm::vec3 camdir) {
    this->FOV = FOV;
    this->asprat = asprat;
    this->near = near;
    this->far = far;
    this->camerapos = campos;
    this->cameradir = camdir;
    this->cameraright =
        glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camdir));
    this->cameraup = glm::cross(camdir, cameraright);
}
void Camera::updatevec() {
    if (!isDirty)
        return;
    if (cameradir != glm::vec3(0.0f, 1.0f, 0.0f))
        this->cameraright =
            glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameradir));
    this->cameraup = glm::cross(cameradir, cameraright);
}
void Camera::updatemat(int projloc, int viewloc) {
    if (!isDirty)
        return;
    proj = glm::perspective(FOV, asprat, near, far);
    view = glm::lookAt(camerapos, camerapos - cameradir, cameraup);

    glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
    isDirty = false;
}
