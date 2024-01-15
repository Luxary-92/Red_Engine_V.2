#pragma once
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Application.h"
#include "ImGui/imgui.h"

struct Animation;

class ComponentAnimation : public Component
{
public:
	ComponentAnimation();
	ComponentAnimation(GameObject* owner);
	~ComponentAnimation();

	void PrintInspector();

	void PushAnimation(Animation* pushedAnimation);

	std::vector<Animation*> animationsList;

	bool animBonesLink = false;
	bool linkChannels = false;
	bool showAnimBones = false;
	float currentTime = 0.f;
	bool isAnimationPlaying = false;
	bool hasAnimationStarted = false;
	float blendingCurrentTime = 0.f;
	float blendingDuration = 0.f;
	uint previousAnimation = 0;
	uint currentAnimation = 0;
	Animation* previousAnimationA = nullptr;
	Animation* currentAnimationA = nullptr;


private:



};