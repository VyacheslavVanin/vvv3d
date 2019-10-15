#pragma once
#include <vvv3d/core/viewport.hpp>
#include <vvv3d/vvvmath/linalg.hpp>

namespace vvv3d {

class Camera {
public:
    enum class PROJECTION_TYPE { ORTHO, PERSPECTIVE };

    Camera();

    const vvv::vector3f& getPosition() const;
    Camera& setPosition(const vvv::vector3f& value);
    Camera& setPosition(float x, float y, float z);
    Camera& move(float dx, float dy, float dz);
    Camera& move(const vvv::vector3f& d);

    const vvv::vector3f& getDirection() const;
    Camera& setDirection(const vvv::vector3f& value);
    Camera& setDirection(float x, float y, float z);

    const vvv::vector3f& getUp() const;
    Camera& setUp(const vvv::vector3f& value);
    Camera& setUp(float x, float y, float z);

    const vvv::matrix44f& getProjectionMatrix() const;

    const vvv::matrix44f& getViewMatrix() const;

    const vvv::matrix44f& getViewProjection() const;

    Camera& lookAt(const vvv::vector3f& position, const vvv::vector3f& target,
                   const vvv::vector3f& up);

    Camera& setOrtho(float left, float right, float bottom, float top,
                     float zNear, float zFar);
    Camera& setOrtho(const Viewport& viewport, float zNear = 0, float zFar = 1);

    /**
     * @brief Set ortho projection. Preserve aspect ratio and height.
     * h = height / 2;
     * w = h * viewport.getAspect();
     * same as setOrtho(-w, w, -h, h, zNear, zFar)*/
    Camera& setOrthoHeight(const Viewport& viewport, float height,
                           float zNear = 0, float zFar = 1);
    /**
     * @brief Set ortho projection. Preserve aspect ratio and height.
     * w = width / 2;
     * h = w / aspectRatio;
     * same as setOrtho(-w, w, -h, h, zNear, zFar)*/
    Camera& setOrthoWidth(const Viewport& viewport, float width,
                          float zNear = 0, float zFar = 1);

    /**
     * @brief Set ortho projection/ Preserve width, height and aspect ratio */
    Camera& setOrthoWindow(const Viewport& viewport, float width, float height,
                           float zNear = 0, float zFar = 1);

    Camera& setPerspective(float fovy, float aspect, float zNear, float zFar);

    /**
     * @brief Update projection matrix to fix aspect ratio to match new viewport
     * @param viewport new Viewport */
    Camera& updateViewport(const Viewport& viewport);

    PROJECTION_TYPE getProjectionType() const { return projectionType; }
    float getAspectRatio() const;
    float getZNear() const { return zNear; }
    float getZFar() const { return zFar; }
    float getFOVy() const { return fovy; }
    Camera& setZNear(float zNear);
    Camera& setZFar(float zFar);
    Camera& setFOVy(float fovy);

private:
    void updateProjectionMatrix() const;
    void updateViewMatrix() const;
    void updateViewProjectionMatrix() const;
    Camera& setOrthoImpl(float left, float right, float bottom, float top,
                         float zNear, float zFar);
    Camera& setOrthoHeightImpl(const Viewport& viewport, float height,
                               float zNear = 0, float zFar = 1);
    Camera& setOrthoWidthImpl(const Viewport& viewport, float width,
                              float zNear = 0, float zFar = 1);
    Camera& setOrthoWindowImpl(const Viewport& viewport, float width,
                               float height, float zNear = 0, float zFar = 1);

    struct OrthoSettings {
        enum class Type { None, Height, Width, Window };

        void reset()
        {
            width = 1.0f;
            height = 1.0f;
            type = Type::None;
        }
        void setPreserveHeight(float height)
        {
            type = Type::Height;
            this->height = height;
        }
        void setPreserveWidth(float width)
        {
            type = Type::Width;
            this->width = width;
        }
        void setPreserveBoth(float width, float height)
        {
            type = Type::Window;
            this->width = width;
            this->height = height;
        }

        float width = 1.0f;
        float height = 1.0f;
        Type type = Type::None;
    };

    vvv::vector3f position;
    vvv::vector3f forward;
    vvv::vector3f up;

    PROJECTION_TYPE projectionType = PROJECTION_TYPE::ORTHO;
    float zNear = 0.1f;
    float zFar = 100.0f;
    float aspect = 1.0f;
    float fovy = 90.0f;
    float ortho_left = -1.0f;
    float ortho_right = 1.0f;
    float ortho_bottom = -1.0f;
    float ortho_top = 1.0f;

    OrthoSettings projection_settings;

    mutable vvv::matrix44f projectionMatrix;
    mutable bool projectionChanged;

    mutable vvv::matrix44f viewMatrix;
    mutable bool viewChanged;

    mutable vvv::matrix44f viewProjection;
};
} // namespace vvv3d
