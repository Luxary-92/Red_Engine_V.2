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

private:



};