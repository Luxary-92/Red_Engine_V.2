#include "ModuleAnimation.h"
#include "Globals.h"
#include "Application.h"

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
	LoadChannelKeys(channel.RotKeyFrames, cursor, range[1]);
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
			Quat q = Quat(aiValue.x, aiValue.y, aiValue.z, aiValue.w);
			float3 rotationKey;

			double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			rotationKey.x = std::atan2(sinr_cosp, cosr_cosp);

			double sinp = std::sqrt(1 + 2 * (q.w * q.x - q.y * q.z));
			double cosp = std::sqrt(1 - 2 * (q.w * q.x - q.y * q.z));
			rotationKey.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

			double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			rotationKey.z = std::atan2(siny_cosp, cosy_cosp);

			channel.RotKeyFrames[anim->mChannels[i]->mRotationKeys[j].mTime] = rotationKey;
		}

		for (int j = 0; j < anim->mChannels[i]->mNumScalingKeys; j++)
		{
			aiVector3D aiValue = anim->mChannels[i]->mScalingKeys[j].mValue;
			float3 scaleKey = float3(aiValue.x, aiValue.y, aiValue.z);

			channel.ScaleKeyFrames[anim->mChannels[i]->mScalingKeys[j].mTime] = scaleKey;
		}

		//animation->channels[channel.name] = channel;
		animation->channels[channel.name] = channel;
	}

	LOG("MESH 3D: Loaded %s Animation with %.2f duration.", animation->name.c_str(), animation->duration);

	return animation;
}

// Position
std::map<double, float3>::const_iterator Channel::PositionNextKey(double currentKey) const
{
	return PosKeyFrames.upper_bound(currentKey);
}

std::map<double, float3>::const_iterator Channel::PositionPrevousKey(double currentKey) const {
	std::map<double, float3>::const_iterator ret = PosKeyFrames.lower_bound(currentKey);
	if (ret != PosKeyFrames.begin())
		ret--;

	return ret;
}
// Rotation
std::map<double, float3>::const_iterator Channel::RotationNextKey(double currentKey) const
{
	return RotKeyFrames.upper_bound(currentKey);
}

std::map<double, float3>::const_iterator Channel::RotationPrevousKey(double currentKey) const
{
	std::map<double, float3>::const_iterator ret = RotKeyFrames.lower_bound(currentKey);
	if (ret != RotKeyFrames.begin())
		ret--;
	return ret;
}

// Scale
std::map<double, float3>::const_iterator Channel::ScaleNextKey(double currentKey) const
{
	return ScaleKeyFrames.upper_bound(currentKey);
}

std::map<double, float3>::const_iterator Channel::ScalePrevousKey(double currentKey) const
{
	std::map<double, float3>::const_iterator ret = ScaleKeyFrames.lower_bound(currentKey);
	if (ret != ScaleKeyFrames.begin())
		ret--;
	return ret;
}

