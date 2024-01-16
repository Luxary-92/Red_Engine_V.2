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
    std::map<double, float3> PosKeyFrames;
    std::map<double, Quat> RotKeyFrames;
    std::map<double, float3> ScaleKeyFrames;

    std::map<double, float3>::const_iterator GetNextPosKey(double currentKey) const;
    std::map<double, Quat>::const_iterator GetNextRotKey(double currentKey) const;
    std::map<double, float3>::const_iterator GetNextScaleKey(double currentKey) const;
    std::map<double, float3>::const_iterator GetPreviousPosKey(double currentKey) const;
    std::map<double, Quat>::const_iterator GetPreviousRotKey(double currentKey) const;
    std::map<double, float3>::const_iterator GetPreviousScaleKey(double currentKey) const;
};


class Animation
{
public:

    std::string name;
    float duration;
    float ticksPerSec;
    float currentFrame = 0;
    bool loop = false;

    std::vector<Channel> channels, bakedChannels;

    Animation(const std::string& animName, float animDuration, float ticksPerSec);
};

class ModuleAnimation : public Module
{
public:
    ModuleAnimation(Application* app, bool start_enabled = true);
    ~ModuleAnimation();

    Animation* LoadAnimation(aiAnimation* anim);

    bool Init() override;
    update_status Update(float dt) override;
    void SaveChannel(const Channel& channel, char** cursor);
    void SaveChannelKeys(const std::map<double, float3>& map, char** cursor);
    void SaveChannelKeysQuat(const std::map<double, Quat>& map, char** cursor);

private:
    std::vector<Animation*> animations;
    Animation* currentAnimation;
};


