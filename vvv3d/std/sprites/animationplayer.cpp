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
    const auto now = vvv3d::Engine::time();
    const auto sinceStartOfAnimation = getFrameTime(now) / this->speed;
    this->startTime = now - sinceStartOfAnimation * this->speed / speed;
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
