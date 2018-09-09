#include "camera.hpp"

using namespace vvv;
using namespace vvv3d;

Camera::Camera()
    : position(0, 0, 1), forward(0, 0, -1), up(0, 1, 0), projectionMatrix(),
      projectionChanged(true), viewMatrix(), viewChanged(true), viewProjection()
{
}

const vector3f& Camera::getPosition() const { return position; }

Camera& Camera::setPosition(const vector3f& value)
{
    viewChanged = true;
    position = value;
    return *this;
}

Camera& Camera::setPosition(float x, float y, float z)
{
    viewChanged = true;
    position.set(x, y, z);
    return *this;
}

Camera& Camera::move(float dx, float dy, float dz)
{
    return this->move(vector3f(dx, dy, dz));
}

Camera& Camera::move(const vector3f& d)
{
    return setPosition(getPosition() + d);
}

const vector3f& Camera::getDirection() const { return forward; }

Camera& Camera::setDirection(const vector3f& value)
{
    viewChanged = true;
    forward = value;
    return *this;
}

Camera& Camera::setDirection(float x, float y, float z)
{
    return setDirection(vector3f(x, y, z));
}

const vector3f& Camera::getUp() const { return up; }

Camera& Camera::setUp(const vector3f& value)
{
    viewChanged = true;
    up = value;
    return *this;
}

Camera& Camera::setUp(float x, float y, float z)
{
    return setUp(vector3f(x, y, z));
}

const matrix44f& Camera::getProjectionMatrix() const
{
    if (projectionChanged)
        updateProjectionMatrix();
    return projectionMatrix;
}

const matrix44f& Camera::getViewMatrix() const
{
    if (viewChanged)
        updateViewMatrix();
    return viewMatrix;
}

const matrix44f& Camera::getViewProjection() const
{
    if (viewChanged || projectionChanged)
        updateViewProjectionMatrix();
    return viewProjection;
}

Camera& Camera::lookAt(const vector3f& position, const vector3f& target,
                       const vector3f& up)
{
    this->position = position;
    this->forward = (target - position).normalize();
    this->up = up.normalized();
    viewChanged = true;
    return *this;
}

Camera& Camera::setOrtho(float left, float right, float bottom, float top,
                         float zNear, float zFar)
{
    projectionType = PROJECTION_TYPE::ORTHO;
    ortho_left = left;
    ortho_right = right;
    ortho_bottom = bottom;
    ortho_top = top;
    this->zNear = zNear;
    this->zFar = zFar;
    projectionChanged = true;
    return *this;
}

Camera& Camera::setOrtho(const Viewport& viewport, float zNear, float zFar)
{
    const auto& w = viewport.getWidth() / 2.0f;
    const auto& h = viewport.getHeight() / 2.0f;
    return setOrtho(-w, w, -h, h, zNear, zFar);
}

Camera& Camera::setOrthoHeight(const Viewport& viewport, float height,
                               float zNear, float zFar)
{
    const auto& h = height / 2;
    const auto& w = h * viewport.getAspect();
    return setOrtho(-w, w, -h, h, zNear, zFar);
}

Camera& Camera::setOrthoWidth(const Viewport& viewport, float width,
                              float zNear, float zFar)
{
    const auto& w = width / 2;
    const auto& h = w / viewport.getAspect();
    return setOrtho(-w, w, -h, h, zNear, zFar);
}

Camera& Camera::setOrtho(const Viewport& viewport, float width, float height,
                         float zNear, float zFar)
{
    const Viewport field(width, height);
    const auto field_aspect = field.getAspect();
    const auto viewport_aspect = viewport.getAspect();
    if (field_aspect < viewport_aspect)
        return setOrthoHeight(viewport, height, zNear, zFar);
    else
        return setOrthoWidth(viewport, width, zNear, zFar);
}

Camera& Camera::setPerspective(float fovy, float aspect, float zNear,
                               float zFar)
{
    projectionType = PROJECTION_TYPE::PERSPECTIVE;
    this->fovy = fovy;
    this->aspect = aspect;
    this->zNear = zNear;
    this->zFar = zFar;
    projectionChanged = true;
    return *this;
}

float Camera::getAspectRatio() const
{
    switch (projectionType) {
    case PROJECTION_TYPE::PERSPECTIVE: return aspect;
    case PROJECTION_TYPE::ORTHO:
        return fabs((ortho_left - ortho_right) / (ortho_top - ortho_bottom));
    }
    return 1.0;
}

void Camera::updateProjectionMatrix() const
{
    projectionChanged = false;
    switch (projectionType) {
    case PROJECTION_TYPE::ORTHO:
        projectionMatrix.loadOrtho(ortho_left, ortho_right, ortho_bottom,
                                   ortho_top, zNear, zFar);
        break;
    case PROJECTION_TYPE::PERSPECTIVE:
        projectionMatrix.loadPerspective(fovy, aspect, zNear, zFar);
        break;
    }
}

void Camera::updateViewMatrix() const
{
    viewChanged = false;
    viewMatrix.loadLookAt(position, position + forward, up);
}

void Camera::updateViewProjectionMatrix() const
{
    viewProjection = getProjectionMatrix() * getViewMatrix();
}
