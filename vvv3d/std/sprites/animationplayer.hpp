#pragma once
#include "animation.hpp"

class AnimationPlayer {
public:
    AnimationPlayer();

    void setAnimation(const Animation* animation);
    const vvv3d::Texture* getFrame(double time) const;
    const vvv3d::Texture* getFrame() const;

    void setSpeed(double speed);
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
