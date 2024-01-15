#include "Application.h"
#include "ComponentAnimation.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleRenderer3D.h"
#include "ModuleAnimation.h"

ComponentAnimation::ComponentAnimation() : Component(nullptr)
{
	type = ComponentType::ANIMATION;


}
ComponentAnimation::ComponentAnimation(GameObject* owner) : Component(owner)
{
	type = ComponentType::ANIMATION;
	mOwner = owner;

}

ComponentAnimation::~ComponentAnimation()
{

}

void ComponentAnimation::PushAnimation(Animation* pushedAnimation) {

	this->animationsList.push_back(pushedAnimation);

}

void ComponentAnimation::PrintInspector()
{
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		
		if (animationsList.empty()) return;
		for (int i = 0; i < animationsList.size(); i++)
		{
			ImGui::Text("Has animations");
		}
	}
}