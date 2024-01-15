#include "Application.h"
#include "ComponentAnimation.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleRenderer3D.h"

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

void ComponentAnimation::PrintInspector()
{
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		/*
		if (meshes.empty()) return;
		for (int i = 0; i < meshes.size(); i++)
		{
			ImGui::LabelText("##%f", "Number of vertex:");
			ImGui::SameLine();
			ImGui::Text("%d", meshes[i]->vertexCount);
			ImGui::LabelText("##%f", "Number of index:");
			ImGui::Checkbox("Face Normals", &faceNormals);
		}
		*/

	}
}