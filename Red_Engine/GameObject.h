#pragma once
#include "Module.h"
#include <vector>
#include <string>
#include "ModuleAnimation.h"
#include "MathGeoLib/include/Math/float3.h"
#include <math.h>
#include "glmath.h"

enum class ComponentType;
enum class GeometryType;
class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentAnimation;

struct Animation;


struct AnimationClip
{
	AnimationClip();
	char name[32];
	float startFrame, endFrame;
	bool loop;
	Animation* originalAnimation;
};

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

	void AddAnimations(std::vector<Animation*> animations);
	std::vector<Animation*> animation_go;

	//Animation prooeties
	bool isAnimationPlaying = true;
	bool hasAnimationStarted = false;

	float Time_2 = 0.f;

	bool animBonesLinked = false;
	bool ChannelsLinked = false;
	bool showBones = false;

	float blendingTime = 0.f;
	float blendingDuration = 0.f;

	uint previousAnimation = 0;
	uint currentAnimation = 0;

	Animation* previousAnimationA = nullptr;
	Animation* currentAnimationA = nullptr;

	//Bones stuff
	GameObject* rootBone = nullptr;
	std::map<std::string, GameObject*> AllBones;
	std::vector<Animation*> animationBones;

	uint boneID;

	Channel* bone;

	std::map <GameObject*, Channel*> BonesCurrentAnim;
	std::map <GameObject*, Channel*> BonesPrevAnim;
	void DrawBones(GameObject* p);
	void StoreBoneMapping(GameObject* go);
	bool FindRootBone();

	//Chanels

	void UpdateChannels(const Animation* settings, const Animation* blend, float blendRatio);
	float3	GetCurrentChannelPosition(const Channel& ch, float currentKey, float3 defaultPos) const;
	float3	GetCurrentChannelRotation(const Channel& ch, float currentKey, float3 defaultRot) const;
	float3	GetCurrentChannelScale(const Channel& ch, float currentKey, float3 defaultScale) const;

	//Load Animations

	void StartAnimation();
	void AddAnimation(Animation* animation);
	void UpdateAnimations(float dt);

	void DeleteAnimation(Animation* anim);

	void PlayAnim(Animation* anim, float blendDuration = 0.2f, float Speed = 1.0f);
	void PauseAnim();
	void ResumeAnim();

	void AddClip(Animation* anim);

	std::vector<AnimationClip> clips;
	AnimationClip* selectedClip = nullptr;

	Animation* ClipToAnim(AnimationClip clip);
	
};

