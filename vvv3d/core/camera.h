#ifndef CAMERA_H
#define CAMERA_H
#include <vvv3d/vvvmath/linalg.h>

namespace vvv3d {

class Camera {
public:
    Camera();

    const vvv::vector3f& getPosition() const;
    Camera& setPosition(const vvv::vector3f& value);
    Camera& setPosition(float x, float y, float z);
    Camera& move(float dx, float dy, float dz);
    Camera& move(const vvv::vector3f& d);

    const vvv::vector3f& getDirection() const;
    Camera& setDirection(const vvv::vector3f& value);

    const vvv::vector3f& getUp() const;
    Camera& setUp(const vvv::vector3f& value);

    const vvv::matrix44f& getProjectionMatrix() const;

    const vvv::matrix44f& getViewMatrix() const;

    const vvv::matrix44f& getViewProjection() const;

    Camera& lookAt(const vvv::vector3f& position, const vvv::vector3f& target,
                   const vvv::vector3f& up);

    Camera& setOrtho(float left, float right, float bottom, float top,
                     float zNear, float zFar);

    Camera& setPerspective(float fovy, float aspect, float zNear, float zFar);

private:
    vvv::vector3f position;
    vvv::vector3f forward;
    vvv::vector3f up;

    enum class PROJECTION_TYPE { ORTHO, PERSPECTIVE };

    PROJECTION_TYPE projectionType = PROJECTION_TYPE::ORTHO;
    float zNear                    = 0.1f;
    float zFar                     = 100.0f;
    float aspect                   = 1.0f;
    float fovy                     = 90.0f;
    float ortho_left               = -1.0f;
    float ortho_right              = 1.0f;
    float ortho_bottom             = -1.0f;
    float ortho_top                = 1.0f;

    mutable vvv::matrix44f projectionMatrix;
    mutable bool projectionChanged;

    mutable vvv::matrix44f viewMatrix;
    mutable bool viewChanged;

    mutable vvv::matrix44f viewProjection;

    void updateProjectionMatrix() const;
    void updateViewMatrix() const;
    void updateViewProjectionMatrix() const;
};
}

#endif // CAMERA_H
