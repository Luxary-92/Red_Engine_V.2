#include "ModuleAnimation.h"
#include "Globals.h"
#include "Application.h"

Animation::Animation(const std::string& animName, float animDuration, float ticksPerSec)
    : name(animName), duration(animDuration), ticksPerSec(ticksPerSec)
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

Animation* ModuleAnimation::LoadAnimation(aiAnimation* anim) {

	std::string animationName = anim->mName.C_Str();

	if (animationName.size() > 32) {
		animationName.resize(32);
	}

	Animation* animation = new Animation(animationName, anim->mDuration, anim->mTicksPerSecond);

	for (int i = 0; i < anim->mNumChannels; i++) {
		Channel channel;

		channel.name = anim->mChannels[i]->mNodeName.C_Str();

		uint pos = channel.name.find("_$AssimpFbx$_");
		if (pos != std::string::npos)
		{
			channel.name = channel.name.substr(0, pos);
		}
		for (int j = 0; j < anim->mChannels[i]->mNumPositionKeys; j++)
		{
			aiVector3D aiValue = anim->mChannels[i]->mPositionKeys[j].mValue;
			float3 positionKey = float3(aiValue.x, aiValue.y, aiValue.z);

			channel.PosKeyFrames[anim->mChannels[i]->mPositionKeys[j].mTime] = positionKey;
		}

		for (int j = 0; j < anim->mChannels[i]->mNumRotationKeys; j++)
		{
			aiQuaternion aiValue = anim->mChannels[i]->mRotationKeys[j].mValue;
			Quat rotationKey = Quat(aiValue.x, aiValue.y, aiValue.z, aiValue.w);
			channel.RotKeyFrames[anim->mChannels[i]->mRotationKeys[j].mTime] = rotationKey;
		}

		for (int j = 0; j < anim->mChannels[i]->mNumScalingKeys; j++)
		{
			aiVector3D aiValue = anim->mChannels[i]->mScalingKeys[j].mValue;
			float3 scaleKey = float3(aiValue.x, aiValue.y, aiValue.z);

			channel.ScaleKeyFrames[anim->mChannels[i]->mScalingKeys[j].mTime] = scaleKey;
		}

		//animation->channels[channel.name] = channel;
		animation->channels.push_back(channel);
	}

	LOG("MESH 3D: Loaded %s Animation with %.2f duration.", animation->name.c_str(), animation->duration);

	return animation;
}

