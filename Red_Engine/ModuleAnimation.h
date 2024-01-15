#pragma once
#include "Module.h"
#include <vector>
#include <string>

class Animation
{
public:
    std::string name;
    float duration;

    Animation(const std::string& animName, float animDuration);
};

class ModuleAnimation : public Module
{
public:
    ModuleAnimation(Application* app, bool start_enabled = true);
    ~ModuleAnimation();

    bool Init() override;
    update_status Update(float dt) override;


private:
    std::vector<Animation*> animations;
    Animation* currentAnimation;
};


