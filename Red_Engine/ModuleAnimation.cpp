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

//Save Chanels
void ModuleAnimation::SaveChannel(const Channel& channel, char** cursor) {

    // Name
    uint nameSize = channel.name.size();
    memcpy(*cursor, &nameSize, sizeof(uint));
    *cursor += sizeof(uint);

    memcpy(*cursor, channel.name.c_str(), channel.name.size());
    *cursor += channel.name.size();

	// Range
	uint range[3] = { channel.PosKeyFrames.size(), channel.RotKeyFrames.size(), channel.ScaleKeyFrames.size() };
	memcpy(*cursor, range, sizeof(uint) * 3);
	*cursor += sizeof(uint) * 3;

	// Save Channels
	SaveChannelKeys(channel.PosKeyFrames, cursor);
	SaveChannelKeysQuat(channel.RotKeyFrames, cursor);
	SaveChannelKeys(channel.ScaleKeyFrames, cursor);

}

void ModuleAnimation::SaveChannelKeys(const std::map<double, float3>& map, char** cursor) {
	std::map<double, float3>::const_iterator it = map.begin();

	for (it = map.begin(); it != map.end(); it++)
	{
		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);

		memcpy(*cursor, &it->second, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;
	}
}


void ModuleAnimation::SaveChannelKeysQuat(const std::map<double, Quat>& map, char** cursor) {
	std::map<double, Quat>::const_iterator it = map.begin();
	for (it = map.begin(); it != map.end(); it++)
	{
		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);

		memcpy(*cursor, &it->second, sizeof(float) * 4);
		*cursor += sizeof(float) * 4;
	}
}
//Load Chanels
void ModuleAnimation::LoadChannel(Channel& channel, const char** cursor)
{
	uint bytes = 0;

	uint nameSize = 0;
	memcpy(&nameSize, *cursor, sizeof(uint));
	*cursor += sizeof(uint);

	if (nameSize > 0)
	{
		char* string = new char[nameSize + 1];
		bytes = sizeof(char) * nameSize;

		memcpy(string, *cursor, bytes);
		*cursor += bytes;
		string[nameSize] = '\0';

		channel.name = string;

		RELEASE_ARRAY(string);
	}

	uint range[3];
	memcpy(&range, *cursor, sizeof(uint) * 3);
	*cursor += sizeof(uint) * 3;

	LoadChannelKeys(channel.PosKeyFrames, cursor, range[0]);
	LoadChannelKeysQuat(channel.RotKeyFrames, cursor, range[1]);
	LoadChannelKeys(channel.ScaleKeyFrames, cursor, range[2]);
}

void ModuleAnimation::LoadChannelKeys(std::map<double, float3>& map, const char** cursor, uint size) {
	for (uint i = 0; i < size; i++)
	{
		double time;
		memcpy(&time, *cursor, sizeof(double));
		*cursor += sizeof(double);
		float data[3];
		memcpy(&data, *cursor, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;

		map[time] = float3(data);
	}
}

void ModuleAnimation::LoadChannelKeysQuat(std::map<double, Quat>& map, const char** cursor, uint size) {
	for (uint i = 0; i < size; i++)
	{
		double time;
		memcpy(&time, *cursor, sizeof(double));
		*cursor += sizeof(double);
		float data[4];
		memcpy(&data, *cursor, sizeof(float) * 4);
		*cursor += sizeof(float) * 4;

		map[time] = Quat(data);
	}
}




Animation* ModuleAnimation::LoadAnimation(aiAnimation* anim) {

	std::string animationName = anim->mName.C_Str();

	if (animationName.size() > 32) {
		animationName.resize(32);
	}

	Animation* animation = new Animation(animationName, anim->mDuration, anim->mTicksPerSecond);

	for (int i = 0; i < anim->mNumChannels; i++) {

		Channel channel;

		channel.name = anim->mChannels[i]->mNodeName.C_Str();

		//uint pos = channel.name.find("_$AssimpFbx$_");

		uint pos = channel.name.find("FBX");
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

