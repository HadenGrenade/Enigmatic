#pragma once
#include "definitions.h"

#include "cmatrix.h"
#include <cvector.h>

struct Model
{
	std::byte	pad0[0x4];		//0x0000
	char		name[260];	//0x0004
	int			loadFlags;		//0x0108
	int			serverCount;	//0x010C
	int			type;			//0x0110
	int			flags;			//0x0114
	CVector		mins;		//0x0118
	CVector		maxs;		//0x0124
	float		radius;		//0x0130
	std::byte	pad1[0x1C];		//0x0134
};

struct StudioBbox
{
	int bone;
	int group;
	CVector bbMin;
	CVector bbMax;
	int hitboxNameIndex;
	CVector offsetOrientation;
	float capsuleRadius;
	int	unused[4];

	inline const char* GetHitboxName() const
	{
		if (!hitboxNameIndex) return nullptr;
		return (const char*)this + hitboxNameIndex;
	}
};

struct StudioHitboxSet
{
	int nameIndex;
	int numHitboxes;
	int hitboxIndex;

	const char* GetName() noexcept
	{
		return nameIndex ? reinterpret_cast<const char*>(std::uintptr_t(this) + nameIndex) : nullptr;
	}

	StudioBbox* GetHitbox(int i) noexcept
	{
		return i >= 0 && i < numHitboxes ? reinterpret_cast<StudioBbox*>(std::uintptr_t(this) + hitboxIndex) + i : nullptr;
	}
};

struct StudioBone
{
	int nameIndex;
	int	parent;
	std::byte pad0[152];
	int flags;
	std::byte pad2[52];

	const char* GetName() const noexcept
	{
		return nameIndex ? reinterpret_cast<const char*>(std::uintptr_t(this) + nameIndex) : nullptr;
	}
};

struct StudioHdr
{
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	Vector eyePosition;
	Vector illumPosition;
	Vector hullMin;
	Vector hullMax;
	Vector bbMin;
	Vector bbMax;
	int flags;
	int numBones;
	int boneIndex;
	int numBoneControllers;
	int boneControllerIndex;
	int numHitboxSets;
	int hitboxSetIndex;
	int	localAnimations;
	int	localAnimationIndex;
	int localSequences;
	int localSequenceIndex;

	const StudioBone* GetBone(int i) const noexcept
	{
		return i >= 0 && i < numBones ? reinterpret_cast<StudioBone*>(uintptr_t(this) + boneIndex) + i : nullptr;
	}

	StudioHitboxSet* GetHitboxSet(int i) noexcept
	{
		return i >= 0 && i < numHitboxSets ? reinterpret_cast<StudioHitboxSet*>(uintptr_t(this) + hitboxSetIndex) + i : nullptr;
	}
};

enum RenderableTranslucencyType
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};

using MDLHandle = unsigned short;

struct Collide;
struct VirtualModel;
struct CombinerModelInput;

class UtlBuffer;

class ModelLoadCallback
{
public:
	virtual void OnModelLoadComplete(const Model* pModel) = 0;

protected:
	// Protected destructor so that nobody tries to delete via this interface.
	// Automatically unregisters if the callback is destroyed while still pending.
	//~ModelLoadCallback();
};

