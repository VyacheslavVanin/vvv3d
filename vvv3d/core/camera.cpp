#include "camera.h"

using namespace vvv;
using namespace vvv3d;

Camera::Camera() :  position(0,0,1),
                    forward(0,0,-1),
                    up( 0, 1, 0),
                    projectionMatrix(), projectionChanged(true),
                    viewMatrix(),       viewChanged(true),
                    viewProjection()
{
}

const vector3f& Camera::getPosition() const
{
    return position;
}

Camera& Camera::setPosition(const vector3f& value)
{
    viewChanged = true;
    position = value;
    return *this;
}

Camera& Camera::setPosition(float x, float y, float z)
{
    viewChanged = true;
    position.set(x,y,z);
    return *this;
}

const vector3f& Camera::getDirection() const
{
    return forward;
}

Camera& Camera::setDirection(const vector3f& value)
{
    viewChanged = true;
    forward = value;
    return *this;
}

const vector3f& Camera::getUp() const
{
    return up;
}

Camera& Camera::setUp(const vector3f& value)
{
    viewChanged = true;
    up = value;
    return *this;
}

const matrix44f& Camera::getProjectionMatrix() const
{
    if( projectionChanged )
        updateProjectionMatrix();
    return projectionMatrix;
}

const matrix44f& Camera::getViewMatrix() const
{
    if( viewChanged )
        updateViewMatrix();
    return viewMatrix;
}

const matrix44f& Camera::getViewProjection() const
{
    if( viewChanged || projectionChanged)
        updateViewProjectionMatrix();
    return viewProjection;
}

Camera& Camera::lookAt(const vector3f& position, const vector3f& target, const vector3f& up)
{
    this->position = position;
    this->forward  = (target-position).normalize();
    this->up       = up.normalized();
    viewChanged = true;
    return *this;
}

Camera& Camera::setOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    projectionType      = PROJECTION_TYPE::ORTHO;
    ortho_left          = left;
    ortho_right         = right;
    ortho_bottom        = bottom;
    ortho_top           = top;
    this->zNear         = zNear;
    this->zFar          = zFar;
    projectionChanged   = true;
    return *this;
}

Camera& Camera::setPerspective(float fovy, float aspect, float zNear, float zFar)
{
    projectionType      = PROJECTION_TYPE::PERSPECTIVE;
    this->fovy          = fovy;
    this->aspect        = aspect;
    this->zNear         = zNear;
    this->zFar          = zFar;
    projectionChanged   = true;
    return *this;
}

void Camera::updateProjectionMatrix() const
{
    projectionChanged = false;
    switch(projectionType){
        case PROJECTION_TYPE::ORTHO:
            projectionMatrix.loadOrtho( ortho_left, ortho_right,
                                        ortho_bottom, ortho_top,
                                        zNear, zFar);break;
        case PROJECTION_TYPE::PERSPECTIVE:
            projectionMatrix.loadPerspective( fovy, aspect, zNear, zFar);break;}
}

void Camera::updateViewMatrix() const
{
    viewChanged = false;
    viewMatrix.loadLookAt(position, position + forward, up );
}

void Camera::updateViewProjectionMatrix() const
{
    viewProjection = getProjectionMatrix()*getViewMatrix();
}



