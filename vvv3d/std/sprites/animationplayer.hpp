#pragma once
#include "animation.hpp"

namespace vvv3d {

class AnimationPlayer {
public:
    AnimationPlayer();

    void setAnimation(const Animation* animation);
    void setAnimation(const Animation& animation);
    void setAnimation(const Animation* animation, double startTime);
    void setAnimation(const Animation& animation, double startTime);
    const vvv3d::Texture* getFrame(double time) const;
    const vvv3d::Texture* getFrame() const;

    void setSpeed(double speed);
    void setSpeed(double speed, double speedChangeTime);
    double getSpeed() const;

    void setLooped(bool loop);
    bool isLooped() const;

private:
    const Animation* animation;
    double startTime;
    double speed;
    bool looped;

    double getFrameTime(double now) const;
};
} // namespace vvv3d
