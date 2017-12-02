#pragma once
#include <vector>
#include "vvv3d/core/graphics/textures/texture.h"

class Animation {
public:
    Animation();
    void addFrame(vvv3d::Texture* frame, double frameDuraton);
    const vvv3d::Texture* getFrame(double t) const;
    double duration() const;
    size_t size() const;
    const vvv3d::Texture* operator[](size_t index) const;

private:
    struct AnimationFrame {
        vvv3d::Texture* frame;
        double duration;
        double timestamp;
    };

    std::vector<AnimationFrame> frames;
    double m_duration;
};

