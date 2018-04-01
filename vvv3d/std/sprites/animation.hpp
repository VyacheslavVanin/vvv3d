#pragma once
#include "vvv3d/core/graphics/textures/texture.hpp"
#include <unordered_map>
#include <vector>
#include <vvvcfg/vvvcfg.hpp>

namespace vvv3d {

class Animation {
public:
    Animation();
    void addFrame(vvv3d::Texture* frame, double frameDuraton = 1.0 / 24);
    void setName(const std::string& name);
    const std::string& getName() const;

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
    std::string name;
    double m_duration;
};

class AnimationManager {
public:
    AnimationManager();
    void add(const std::string& name, const Animation& animation);
    void add(const std::string& name, Animation&& animation);

    const Animation& get(const std::string& name) const;

    // Config have to contain something like this:
    //animations
    //    animation1 frames=[texture_name, texture_name2, ...] length=1
    //    animation2 frames=[texture_name, texture_name2, ...] length=1
    void load(const vvv::CfgNode& cfg);
    void load(const std::string& cfg);
    void load(std::istream& cfg);

    void init();

private:
    std::unordered_map<std::string, Animation> animations;
};
}
