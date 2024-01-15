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




