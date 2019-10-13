#include "transform.hpp"

namespace vvv3d {

const vvv::matrix44f& vvv3d::Transform::getModelMatrix() const
{
    if (rotationChanged || scaleChanged || positionChanged) {
        // rotation
        modelMatrix = getRotationMatrix();

        // scale
        modelMatrix *= vvv::matrix44f::createScaleMatrix(m_scale);
        // translate
        modelMatrix.matrix[3][0] = position.x;
        modelMatrix.matrix[3][1] = position.y;
        modelMatrix.matrix[3][2] = position.z;
        scaleChanged = positionChanged = false;
    }
    return modelMatrix;
}

void vvv3d::Transform::setDirection(const vvv::vector3f& v)
{
    static const auto default_front = vvv::vector3f(0, 0, 1);
    const auto& v_norm = v.normalized();
    const auto& current_front = default_front;
    const auto& axis = vvv::vector3f::cross(current_front, v_norm);
    const auto& angle = vvv::vector3f::angle(current_front, v_norm);
    setRotation(angle, axis);
}

void vvv3d::Transform::setUp(const vvv::vector3f& v)
{
    const auto& current_up = getUpVector();
    const auto& v_norm = v.normalized();
    const auto& n = getDirection();
    // projection of v to plane with normal n
    const auto& v_proj = v_norm - v_norm.dot(n) * n;
    // rotate
    const auto& cross = current_up.cross(v_proj);
    auto angle = vvv::vector3f::angle(current_up, v_proj);
    if (std::signbit(cross.dot(n)))
        angle = -angle;

    rotate(angle, n);
}

vvv::vector3f vvv3d::Transform::getDirection() const
{
    static const auto default_front = vvv::vector3f(0, 0, 1);
    return getRotationMatrix() * default_front;
}

vvv::vector3f vvv3d::Transform::getUpVector() const
{
    static const auto default_up = vvv::vector3f(0, 1, 0);
    return getRotationMatrix() * default_up;
}

const vvv::matrix44f& vvv3d::Transform::getRotationMatrix() const
{
    if (rotationChanged)
        updateRotationMatrix();
    return rotationMatrix;
}

void vvv3d::Transform::updateRotationMatrix() const
{
    q.toMatrix44(rotationMatrix);
    rotationChanged = false;
}

void vvv3d::Transform::setRotationEuler(float pitch, float yaw, float roll)
{
    q.setEuler(pitch, yaw, roll);
    rotationChanged = true;
}

void vvv3d::Transform::setRotation(float angle, const vvv::vector3f& axis)
{
    q.setAngleAxis(angle, axis);
    rotationChanged = true;
}

} // namespace vvv3d
