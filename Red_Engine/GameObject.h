#pragma once
#include "Module.h"
#include <vector>
#include <string>
#include "ModuleAnimation.h"


enum class ComponentType;
enum class GeometryType;
class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentAnimation;

//struct Animation;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	void AddComponent(Component* component);
	void Update();
	GameObject* GetParent();
	bool SetNewParent(GameObject* newParent);
	bool GameObject::SetAsChildOf(GameObject* gameObject);
	void AttachChild(GameObject* child);
	void PrintInspector();
	bool CheckChildOf(GameObject* parent);
	void DeleteChild(GameObject* child);


	ComponentMesh* GetMeshComponent();

	ComponentTransform* GetTransformComponent();

	ComponentMaterial* GetComponentTexture();

	ComponentCamera* GetComponentCamera();

	ComponentAnimation* GetComponentAnimation();

	void SetPendingToDelete(bool deleteGO) { isTimetoDelete = deleteGO; };

	bool isTimetoDelete;
	bool isActive;
	std::string name;

	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	std::vector<Component*> mComponents;

	ComponentTransform* transform;
	GeometryType Stype;


	// This has to go to componetAnimation

	void StartAnimation();
	void AddAnimation(Animation* animations);
	void UpdateAnimations(float dt, bool playing);


	void AddAnimations(std::vector<Animation*> animations);
	std::vector<Animation*> animation;

	//Animation prooeties
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


	//Bones stuff
	GameObject* rootBone = nullptr;
	std::vector <GameObject*> AllBones;
	std::vector<Animation*> animationBones;

	void UpdateChannels(const Animation* settings, const Animation* blend, float blendRatio);
	float3	GetCurrentChannelPosition(const Channel& ch, float currentKey, float3 default) const;
	Quat	GetCurrentChannelRotation(const Channel& ch, float currentKey, Quat default) const;
	float3	GetCurrentChannelScale(const Channel& ch, float currentKey, float3 default) const;



};