#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H
#include "animation.h"

class AnimationPlayer {
    enum class AnimationState {
        PLAY,
        PAUSE,
    };
public:
    AnimationPlayer();

    void setAnimation(const Animation* animation);
    const vvv3d::Texture* getFrame(double time) const;
    const vvv3d::Texture* getFrame() const;

    void play();

    void setSpeed(double speed);
    double getSpeed() const;

    void setLooped(bool loop);
    bool isLooped() const;

private:
    const Animation* animation;
    double startTime;
    double speed;
    AnimationState state;
    bool looped;

    double getFrameTime(double now) const;
};

#endif

