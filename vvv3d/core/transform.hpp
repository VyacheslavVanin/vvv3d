#pragma once
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {

class Transform {
public:
    Transform();
    ~Transform() = default;

    inline void setPosition(const vvv::vector3f& value);
    inline void setPosition(float x, float y, float z);
    void setRotationEuler(float pitch, float yaw, float roll);
    inline void setRotation(float angle, float ax, float ay, float az);
    void setRotation(float angle, const vvv::vector3f& axis);
    inline void setRotation(const vvv::quaternion<float>& quaternion);
    inline void setScale(float x, float y, float z);
    inline void setScale(const vvv::vector3f& s);
    inline void setScale(float scale);

    inline void move(const vvv::vector3f& v);
    inline void move(float x, float y, float z);
    inline void rotate(float angle, const vvv::vector3f& axis);
    inline void rotate(float angle, float ax, float ay, float az);
    inline void rotate(const vvv::quaternion<float>& quaternion);
    inline void scale(float x, float y, float z);
    inline void scale(const vvv::vector3f& s);
    inline void scale(float s);

    inline const vvv::vector3f& getPosition() const;
    inline const vvv::vector3f& getScale() const;
    const vvv::matrix44f& getModelMatrix() const;
    const vvv::matrix44f& getRotationMatrix() const;

    vvv::vector3f getDirection() const;
    vvv::vector3f getUpVector() const;

    void setDirection(const vvv::vector3f& v);
    inline void setDirection(float x, float y, float z);

    void setUp(const vvv::vector3f& v);
    inline void setUp(float x, float y, float z);

    inline const vvv::quaternion<float>& getQuaternion() const;

private:
    vvv::quaternion<float> q;
    vvv::vector3f position;
    vvv::vector3f m_scale;
    mutable vvv::vector3f front; // delete?
    mutable vvv::vector3f up;    // delete?
    mutable vvv::matrix44f modelMatrix;
    mutable vvv::matrix44f rotationMatrix;
    mutable bool positionChanged;
    mutable bool rotationChanged;
    mutable bool scaleChanged;

    inline void updateRotationMatrix() const;
    inline void updateScaleMatrix() const;
};

inline Transform::Transform()
    : q(0, 0, 0, 1), position(0.0f), m_scale(1, 1, 1), front(0, 0, 1),
      up(0, 1, 0), modelMatrix(), rotationMatrix(), positionChanged(true),
      rotationChanged(true), scaleChanged(true)
{
    q.setAngleAxis(0, vvv::vector3f(0, 0, 1));
}

inline const vvv::vector3f& Transform::getPosition() const { return position; }

const vvv::vector3f& Transform::getScale() const { return m_scale; }

inline void Transform::setDirection(float x, float y, float z)
{
    setDirection(vvv::vector3f{x, y, z});
}

inline void Transform::setUp(float x, float y, float z)
{
    setUp(vvv::vector3f{x, y, z});
}

inline void Transform::setPosition(const vvv::vector3f& value)
{
    position = value;
    positionChanged = true;
}

inline void Transform::setPosition(float x, float y, float z)
{
    setPosition(vvv::vector3f(x, y, z));
}

inline void Transform::setRotation(float angle, float ax, float ay, float az)
{
    setRotation(angle, vvv::vector3f(ax, ay, az));
}

inline void Transform::setRotation(const vvv::quaternion<float>& q)
{
    this->q = q;
    rotationChanged = true;
}

void Transform::setScale(float x, float y, float z)
{
    m_scale.set(x, y, z);
    scaleChanged = true;
}

void Transform::setScale(const vvv::vector3f& s)
{
    m_scale = s;
    scaleChanged = true;
}

void Transform::setScale(float scale) { setScale(scale, scale, scale); }

inline void Transform::move(const vvv::vector3f& v)
{
    position += v;
    positionChanged = true;
}

inline void Transform::move(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
    positionChanged = true;
}

inline void Transform::rotate(const vvv::quaternion<float>& quaternion)
{
    q *= quaternion;
    rotationChanged = true;
}

inline void Transform::rotate(float angle, const vvv::vector3f& axis)
{
    rotate(vvv::quaternion<float>(angle, axis));
}

inline void Transform::rotate(float angle, float ax, float ay, float az)
{
    rotate(angle, vvv::vector3f(ax, ay, az));
}

inline void Transform::scale(float x, float y, float z)
{
    scale(vvv::vector3f(x, y, z));
}

inline void Transform::scale(const vvv::vector3f& s) { m_scale *= s; }

inline void Transform::scale(float s) { m_scale *= s; }

inline const vvv::quaternion<float>& Transform::getQuaternion() const
{
    return q;
}
} // namespace vvv3d
