#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentAnimation.h"
#include "OurPrimitive.h"


GameObject::GameObject()
{
	name = "GameObject";
	mParent = nullptr;
	transform = new ComponentTransform(this);
	isTimetoDelete = false;
	Stype = GeometryType::NONE;
	mComponents.push_back(transform);
}

GameObject::~GameObject()
{
	name = "";
	transform = nullptr;

	if (isTimetoDelete && mParent != nullptr) {
		mParent->DeleteChild(this);
	}


	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		delete mComponents[i];
		mComponents[i] = nullptr;
	}

	for (size_t i = 0; i < mChildren.size(); ++i)
	{
		delete mChildren[i];
		mChildren[i] = nullptr;
	}

	mComponents.clear();

	if (!animation_go.empty()) {
		animation_go.clear();
	}

}

GameObject::GameObject(GameObject* parent)
{
	name = "GameObject";
	this->mParent = parent;
	isTimetoDelete = false;
	Stype = GeometryType::NONE;

	if (parent != nullptr)
	{
		parent->mChildren.push_back(this);
	}

	transform = new ComponentTransform(this);
	

	mComponents.push_back(transform);
}

void GameObject::AddComponent(Component* component)
{
	mComponents.push_back(component);
	component->mOwner = this;
}


void GameObject::Update()
{
	// No tots els children o components tenen un update
	for (size_t i = 0; i < mChildren.size(); ++i)
	{
		mChildren[i]->Update();
	}

	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		mComponents[i]->Update();
	}
}

GameObject* GameObject::GetParent()
{
	return mParent;
}

ComponentMesh* GameObject::GetMeshComponent()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::MESH)
		{
			return (ComponentMesh*)mComponents[i];
		}
	}
	return nullptr;
}

ComponentTransform* GameObject::GetTransformComponent()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::TRANSFORM)
		{
			return (ComponentTransform*)mComponents[i];
		}
	}
	return nullptr;
}

ComponentMaterial* GameObject::GetComponentTexture()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::MATERIAL) {

			return (ComponentMaterial*)mComponents[i];
		}
	}

	return nullptr;
}

ComponentCamera* GameObject::GetComponentCamera()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::CAMERA) {

			return (ComponentCamera*)mComponents[i];
		}
	}

	return nullptr;
}

ComponentAnimation* GameObject::GetComponentAnimation()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->type == ComponentType::ANIMATION) {

			return (ComponentAnimation*)mComponents[i];
		}
	}

	return nullptr;
}


bool GameObject::CheckChildOf(GameObject* parent)
{
	if (parent == this)
	{
		return true;
	}
	if (parent->mChildren.empty())
	{
		return false;
	}
	for (size_t i = 0; i < parent->mChildren.size(); i++)
	{
		if (CheckChildOf(parent->mChildren[i])) return true;
	}
	return false;
}

void GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < mChildren.size(); i++) {
		if (mChildren[i] == child) {
			mChildren.erase(mChildren.begin() + i);
			child->mParent = nullptr;
		}
	}
}

bool GameObject::SetNewParent(GameObject* newParent)
{
	if (mParent != nullptr) {
		if (newParent->CheckChildOf(this)) return false;

		mParent->DeleteChild(this);
	}

	mParent = newParent;
	newParent->mChildren.push_back(this);

	return true;
}

bool GameObject::SetAsChildOf(GameObject* gameObject)
{
	if (CheckChildOf(gameObject))
	{
		return false;
	}
	gameObject->mParent = this;
	mChildren.push_back(gameObject);
	return true;
}

void GameObject::AttachChild(GameObject* child)
{
	child->mParent = this;
	mChildren.push_back(child);
	//child->transform
	//child->transform
}


void GameObject::PrintInspector()
{


	char* listComponenets[]{ "Add Component", "Mesh Component", "Texture Component" };
	char aux[255] = { ' ' }; 


	if (mParent != nullptr)
	{
		strcpy(aux, this->name.c_str());


		ImGui::BulletText("Name:");
		ImGui::SameLine();

		
		ImGui::InputText("##Name", aux, 255, ImGuiInputTextFlags_EnterReturnsTrue);

		if (ImGui::IsKeyDown(ImGuiKey_Enter))
			name = aux;


		if (ImGui::Button("Delete") || isTimetoDelete) {
			
			isTimetoDelete = true;
			delete App->hierarchy->objSelected;
			App->hierarchy->objSelected = nullptr;
		}

		for (size_t i = 0; i < mComponents.size(); i++)
		{
			ImGui::Separator();

			mComponents[i]->PrintInspector();
		}

		ImGui::Separator();
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");

		ImGui::Text("");
		ImGui::SameLine(ImGui::GetWindowWidth() / 6);

	}
}


// This has to go to componetAnimation

void GameObject::AddAnimation(Animation* animation) {
	this->animation_go.push_back(animation);
}

void GameObject::AddAnimations(std::vector<Animation*> animations)
{
	this->animation_go = animations;
	for (int i = 0; i < animations.size(); i++) {
		this->animation_go.push_back(animations[i]);
	}
}

void GameObject::AddClip(Animation* animation) {
	if (animation != nullptr) {
		AnimationClip clip;

		strcpy(clip.name, animation->name.c_str());
		clip.startFrame = animation->initTimeAnim;
		clip.endFrame = animation->initTimeAnim + animation->duration;
		clip.originalAnimation = animation;

		clips.push_back(clip);
	}
}

void GameObject::StartAnimation() {
	if (rootBone == nullptr) {
		if (!AllBones.empty()) {
			rootBone == AllBones[0];
		}
		else {
			return;
		}
	}

}

void GameObject::UpdateAnimations(float dt) {

	// Update Current Animation
	if (this->isAnimationPlaying) {
		if (!hasAnimationStarted) { StartAnimation(); }
		else {

			if (currentAnimationA != nullptr) {

				//Updating animation blend
				float blendRatio = 0.0f;
				if (blendingDuration > 0.0f)
				{
					previousAnimation += dt;
					Time_2 += dt;
					//Blendig
					if (Time_2 >= blendingDuration)
					{
						blendingDuration = 0.0f;
					}
					else if (previousAnimationA && previousAnimation >= previousAnimationA->duration)
					{
						if (previousAnimationA->loop == true)
						{
							previousAnimation = 0.0f;
						}
					}

					if (blendingDuration > 0.0f)
						blendRatio = Time_2 / blendingDuration;
				}
				Time_2 += dt;



				currentAnimation = dt * currentAnimationA->ticksPerSec;
				currentAnimationA += currentAnimationA->initTimeAnim;
				if (currentAnimationA->loop == true) {
					Time_2 = 0.0f;
				}


				UpdateChannels(currentAnimationA, blendRatio > 0.0f ? previousAnimationA : nullptr, blendRatio);
			}
		}
	}

	// Draw funcion
	if (showBones) {
		DrawBones(rootBone);
	}
}

void GameObject::DrawBones(GameObject* p)
{
	if (!p->mChildren.empty()) {
		for (int i = 0; i < p->mChildren.size(); i++) {
			p->mChildren[i]->DrawBones(p->mChildren[i]);

			std::vector<float3> line;

			line.push_back(float3(p->transform->getPosition().x, p->transform->getPosition().y, p->transform->getPosition().z));
			line.push_back(float3(p->mChildren[i]->transform->getPosition().x, p->mChildren[i]->transform->getPosition().y, p->mChildren[i]->transform->getPosition().z));

			for (int j = 0; j < line.size(); j++) {
				App->scene->AddLines(line[j], Red);
			}
		}
	}
}

void GameObject::UpdateChannels(const Animation* settings, const Animation* blend, float blendRatio) {

	uint currentFrame = currentAnimation;
	uint prevBlendFrame = 0;

	if (blend != nullptr) {
		prevBlendFrame = blend->ticksPerSec * previousAnimation;
	}

	std::map<GameObject*, Channel*>::iterator createBones;
	for (createBones = BonesCurrentAnim.begin(); createBones != BonesCurrentAnim.end(); ++createBones)
	{
		Channel& channel = *createBones->second;

		float3 position = GetCurrentChannelPosition(channel, currentFrame, float3(createBones->first->transform->getPosition().x, createBones->first->transform->getPosition().y, createBones->first->transform->getPosition().z));
		float3 rotation = GetCurrentChannelRotation(channel, currentFrame, float3(createBones->first->transform->getRotation().x, createBones->first->transform->getRotation().y, createBones->first->transform->getRotation().z));
		float3 scale = GetCurrentChannelScale(channel, currentFrame, float3(createBones->first->transform->getScale().x, createBones->first->transform->getScale().y, createBones->first->transform->getScale().z));

		if (blend != nullptr)
		{
			std::map<GameObject*, Channel*>::iterator foundChannel = BonesPrevAnim.find(createBones->first);
			if (foundChannel != BonesPrevAnim.end()) {
				const Channel& blendChannel = *foundChannel->second;

				position = float3::Lerp(GetCurrentChannelPosition(blendChannel, prevBlendFrame, float3(createBones->first->transform->getPosition().x, createBones->first->transform->getPosition().y, createBones->first->transform->getPosition().z)), position, blendRatio);
				rotation = float3::Lerp(GetCurrentChannelRotation(blendChannel, prevBlendFrame, float3(createBones->first->transform->getRotation().x, createBones->first->transform->getRotation().y, createBones->first->transform->getRotation().z)), rotation, blendRatio);
				scale = float3::Lerp(GetCurrentChannelScale(blendChannel, prevBlendFrame, float3(createBones->first->transform->getScale().x, createBones->first->transform->getScale().y, createBones->first->transform->getScale().z)), scale, blendRatio);
			}
		}

		createBones->first->transform->SetTransform(vec3(position.x, position.y, position.z), vec3(rotation.x, rotation.y, rotation.z), vec3(scale.x, scale.y, scale.z));
	}
}

//Current Stadistics
float3	GameObject::GetCurrentChannelPosition(const Channel& ch, float currentKey, float3 defaultPos) const {
	if (ch.PosKeyFrames.size() > 0)
	{
		std::map<double, float3>::const_iterator previous = ch.PositionPrevousKey(currentKey);
		std::map<double, float3>::const_iterator next = ch.PositionNextKey(currentKey);

		if (ch.PosKeyFrames.begin()->first == -1) {
			return defaultPos;
		}

		if (previous == next) {
			defaultPos = previous->second;
		}
		else
		{
			float ratio = (currentKey - previous->first) / (next->first - previous->first);
			defaultPos = previous->second.Lerp(next->second, ratio);
		}
	}

	return defaultPos;
}

float3	GameObject::GetCurrentChannelRotation(const Channel& ch, float currentKey, float3 defaultRot) const {
	if (ch.RotKeyFrames.size() > 0)
	{
		std::map<double, float3>::const_iterator previous = ch.RotationPrevousKey(currentKey);
		std::map<double, float3>::const_iterator next = ch.RotationNextKey(currentKey);

		if (ch.RotKeyFrames.begin()->first == -1) {
			return defaultRot;
		}

		if (previous == next) {
			defaultRot = previous->second;
		}
		else
		{
			float ratio = (currentKey - previous->first) / (next->first - previous->first);
			defaultRot = previous->second.Lerp(next->second, ratio);
		}
	}

	return defaultRot;
}

float3	GameObject::GetCurrentChannelScale(const Channel& ch, float currentKey, float3 defaultScale) const {
	if (ch.ScaleKeyFrames.size() > 0)
	{
		std::map<double, float3>::const_iterator previous = ch.ScalePrevousKey(currentKey);
		std::map<double, float3>::const_iterator next = ch.ScaleNextKey(currentKey);

		if (ch.ScaleKeyFrames.begin()->first == -1) {
			return defaultScale;
		}

		// Check Blending Ratio between Keys
		if (previous == next)
		{
			defaultScale = previous->second;
		}
		else
		{
			float ratio = (currentKey - previous->first) / (next->first - previous->first);
			defaultScale = previous->second.Lerp(next->second, ratio);
		}
	}
	return defaultScale;
}


void GameObject::DeleteAnimation(Animation* anim) {
	for (int i = 0; i < animation_go.size(); i++) {
		if (animation_go[i] == anim) {
			animation_go.erase(animation_go.begin() + i);
		}
	}
}

void GameObject::PlayAnim(Animation* anim, float blendDuration, float Speed) {

}

void GameObject::PauseAnim() {

}

void GameObject::ResumeAnim() {

}

AnimationClip::AnimationClip() : name("Namen't"), startFrame(0), endFrame(0), originalAnimation(nullptr), loop(false) {

}

Animation* GameObject::ClipToAnim(AnimationClip clip)
{
	Animation* animation = new Animation(clip.name, clip.endFrame - clip.startFrame, clip.originalAnimation->ticksPerSec);

	animation->initTimeAnim = clip.startFrame;
	animation->loop = clip.loop;

	return animation;
}

