#include "animation.hpp"
#include <numeric>
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/graphics/textures/texturemanager.hpp>

namespace vvv3d {

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
                                        return f.timestamp <= t;
                                    }) -
                   1;
    return l->frame;
}

void Animation::setName(const std::string& name) { this->name = name; }

const std::string& Animation::getName() const { return name; }

AnimationManager::AnimationManager() {}

void AnimationManager::add(const std::string& name, const Animation& animation)
{
    animations[name] = animation;
}

void AnimationManager::add(const std::string& name, Animation&& animation)
{
    animations[name] = std::move(animation);
}

const Animation& AnimationManager::get(const std::string& name) const
{
    auto i = animations.find(name);
    if (i != animations.end())
        return i->second;

    return animations.at("default");
}

namespace {
Animation make_animation(const vvv::CfgNode& node) {
    auto& texman = vvv3d::getTextureManager();

    Animation ret;
    const auto& animation_length = node.getPropertyAsDouble("length", 1);
    const auto& frames = node.getProperty("frames").asStringList();
    const auto& frames_count = frames.size();
    const auto& frame_duration = animation_length / frames_count;
    for (const auto& frame: frames)
        ret.addFrame(&texman.get(frame), frame_duration);
    ret.setName(node.getName());
    return ret;
}
}

void AnimationManager::load(const vvv::CfgNode& cfg)
{
    if (!cfg.hasChild("animations"))
        return;

   for (const auto& c: cfg.getChild("animations").getChildren()) {
       auto animation = make_animation(c);
       add(animation.getName(), std::move(animation));
   }
}

void AnimationManager::load(const std::string& string)
{
    std::stringstream ss;
    ss << string;
    load(ss);
}

void AnimationManager::load(std::istream& stream)
{
    const auto& cfg = vvv::make_cfg(stream);
    load(cfg);
}

void AnimationManager::init()
{
    auto& textman = vvv3d::getTextureManager();
    auto& default_texture = textman.getDefault();

    Animation default_animation;
    default_animation.addFrame(&default_texture);

    add("default", std::move(default_animation));
}
}
