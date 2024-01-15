#include "ModuleAnimation.h"
#include "Globals.h"
#include "Application.h"

Animation::Animation(const std::string& animName, float animDuration)
    : name(animName), duration(animDuration)
{
   
}

ModuleAnimation::ModuleAnimation(Application* app, bool start_enabled)
    : Module(app, start_enabled), currentAnimation(nullptr)
{
    
}

ModuleAnimation::~ModuleAnimation()
{
    for (Animation* anim : animations)
    {
        delete anim;
    }
    animations.clear();
}

bool ModuleAnimation::Init()
{
    return true;
}

update_status ModuleAnimation::Update(float dt)
{

    if (currentAnimation)
    {

    }

    return update_status::UPDATE_CONTINUE;
}

/*
void ModuleAnimation::SaveChannel1(const Channel& channel, char** cursor) {

    // Name
    uint nameSize = channel.name.size();
    memcpy(*cursor, &nameSize, sizeof(uint));
    *cursor += sizeof(uint);

    memcpy(*cursor, channel.name.c_str(), channel.name.size());
    *cursor += channel.name.size();


}
*/




