#include "animation.hpp"
#include <numeric>

Animation::Animation() : frames(), m_duration(0) {}

void Animation::addFrame(vvv3d::Texture* tex, double duration)
{
    frames.push_back({tex, duration, m_duration});
    m_duration += duration;
}

double Animation::duration() const { return m_duration; }

size_t Animation::size() const { return frames.size(); }

const vvv3d::Texture* Animation::operator[](size_t index) const
{
    return frames[index].frame;
}

const vvv3d::Texture* Animation::getFrame(double t) const
{
    if (t == frames.front().timestamp)
        return frames.front().frame;
    if (t > frames.back().timestamp)
        return frames.back().frame;
    if (t < frames.front().timestamp)
        t = duration() + t;

    const auto l = std::lower_bound(frames.begin(), frames.end(), t,
                                    [](const AnimationFrame& f, double t) {
                                        return f.timestamp < t;
                                    }) -
                   1;
    return l->frame;
}
