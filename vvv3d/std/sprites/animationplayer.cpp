#include "animationplayer.h"
#include "vvv3d/core/engine.h"

AnimationPlayer::AnimationPlayer()
    : animation(nullptr), startTime(0.0), speed(1.0), looped(false)
{
}

void AnimationPlayer::setAnimation(const Animation* animation)
{
    this->animation = animation;
    this->startTime = vvv3d::Engine::time();
}

void AnimationPlayer::setSpeed(double speed)
{
    const auto newSpeed = speed;
    const auto oldSpeed = this->speed;

    if (newSpeed == oldSpeed)
        return;

    const auto now = vvv3d::Engine::time();
    // Special case to avoid division to zero
    if (newSpeed == 0) {
        const auto sinceStartOfAnimation = getFrameTime(now);
        this->startTime = sinceStartOfAnimation;
        this->speed = 0;
        return;
    }

    if (oldSpeed == 0) {
        this->startTime = now - this->startTime / newSpeed;
        this->speed = newSpeed;
        return;
    }

    const auto sinceStartOfAnimation = getFrameTime(now) / oldSpeed;
    this->startTime = now - sinceStartOfAnimation * oldSpeed / newSpeed;
    this->speed = speed;
}

double AnimationPlayer::getSpeed() const
{
    return speed;
}

void AnimationPlayer::setLooped(bool loop)
{
    this->looped = loop;
}

bool AnimationPlayer::isLooped() const
{
    return looped;
}

double AnimationPlayer::getFrameTime(double now) const
{
    if (this->speed == 0)
        return this->startTime; // Special meaning in this case

    const auto sinceStartOfAnimation = (now - startTime) * speed;
    return isLooped() ? fmod(sinceStartOfAnimation, animation->duration())
                      : sinceStartOfAnimation;
}

const vvv3d::Texture* AnimationPlayer::getFrame(double time) const
{
    if (animation == nullptr)
        return nullptr;

    const auto now       = vvv3d::Engine::time();
    const auto frameTime = getFrameTime(now);
    return animation->getFrame(frameTime);
}

const vvv3d::Texture* AnimationPlayer::getFrame() const
{
    const auto& now = vvv3d::Engine::time();
    return getFrame(now);
}
