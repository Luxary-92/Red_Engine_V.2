#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>
#include <map>
#include "MathGeoLib/include/Math/float3.h"

struct aiAnimation;

struct Channel
{
    std::string name;
    std::map<double, float3> posKeys;
    std::map<double, float3> rotKeys;
    std::map<double, float3> scaleKeys;
};


class Animation
{
public:

    std::string name;
    float duration;
    float ticksPerSec;
    float currentFrame = 0;
    bool loop = false;


    Animation(const std::string& animName, float animDuration);
};

class ModuleAnimation : public Module
{
public:
    ModuleAnimation(Application* app, bool start_enabled = true);
    ~ModuleAnimation();

    Animation* LoadAnimation(aiAnimation* anim);

    bool Init() override;
    update_status Update(float dt) override;


private:
    std::vector<Animation*> animations;
    Animation* currentAnimation;
};


