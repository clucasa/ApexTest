// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2012 NVIDIA Corporation. All rights reserved.

#ifndef NX_DESTRUCTIBLE_ASSET_H
#define NX_DESTRUCTIBLE_ASSET_H

#define NX_DESTRUCTIBLE_AUTHORING_TYPE_NAME "DestructibleAsset"

#include "foundation/Px.h"
#include "FractureTools.h"

#if NX_SDK_VERSION_MAJOR == 3
#include "PxFiltering.h"
#endif

namespace physx
{
namespace apex
{

class NxDestructibleActor;
class NxDestructiblePreview;

PX_PUSH_PACK_DEFAULT
#pragma warning(push)
#pragma warning(disable:4121)

/**
	Flags that may be set for all chunks at a particular depth in the fracture hierarchy
*/
struct NxDestructibleDepthParametersFlag
{
	enum Enum
	{
		/*
			If true, chunks at this hierarchy depth level will take impact damage iff OVERRIDE_IMPACT_DAMAGE = TRUE, no matter the setting of impactDamageDefaultDepth..
			Note, NxDestructibleParameters::forceToDamage must also be positive for this
			to take effect.
		*/
		OVERRIDE_IMPACT_DAMAGE			= (1 << 0),

		/**
			If OVERRIDE_IMPACT_DAMAGE = TRUE, chunks at this hierarchy depth level will take impact damage iff OVERRIDE_IMPACT_DAMAGE = TRUE, no matter the setting of impactDamageDefaultDepth..
		*/
		OVERRIDE_IMPACT_DAMAGE_VALUE	= (1 << 1),

		/**
			Chunks at this depth should have pose updates ignored.
		*/
		IGNORE_POSE_UPDATES				= (1 << 2),

		/**
			Chunks at this depth should be ignored in raycast callbacks.
		*/
		IGNORE_RAYCAST_CALLBACKS		= (1 << 3),

		/**
			Chunks at this depth should be ignored in contact callbacks.
		*/
		IGNORE_CONTACT_CALLBACKS		= (1 << 4),

		/**
			User defined flags.
		*/
		USER_FLAG_0						= (1 << 24),
		USER_FLAG_1						= (1 << 25),
		USER_FLAG_2						= (1 << 26),
		USER_FLAG_3						= (1 << 27)
	};
};


/**
	Parameters that may be set for all chunks at a particular depth in the fracture hierarchy
*/
struct NxDestructibleDepthParameters
{
	/**
	\brief constructor sets to default.
	*/
	PX_INLINE		NxDestructibleDepthParameters();

	/**
	\brief (re)sets the structure to the default.
	*/
	PX_INLINE void	setToDefault();

	/**
	\brief (re)sets the structure to parameters which are likely to be the most computationally expensive.
	*/
	PX_INLINE void	setToMostExpensive();

	/**
		A convenience function to determine if the OVERRIDE_IMPACT_DAMAGE is set.
	*/
	PX_INLINE bool	overrideImpactDamage() const;

	/**
		A convenience function to read OVERRIDE_IMPACT_DAMAGE_VALUE is set.
	*/
	PX_INLINE bool	overrideImpactDamageValue() const;

	/**
		A convenience function to determine if the IGNORE_POSE_UPDATES is set.
	*/
	PX_INLINE bool	ignoresPoseUpdates() const;

	/**
		A convenience function to determine if the IGNORE_RAYCAST_CALLBACKS is set.
	*/
	PX_INLINE bool	ignoresRaycastCallbacks() const;

	/**
		A convenience function to determine if the IGNORE_CONTACT_CALLBACKS is set.
	*/
	PX_INLINE bool	ignoresContactCallbacks() const;

	/**
		A convenience function to determine if the USER_FLAG_0, USER_FLAG_1, USER_FLAG_2, or USER_FLAG_3 flag is set.
	*/
	PX_INLINE bool	hasUserFlagSet(physx::PxU32 flagIndex) const;

	/**
		A collection of flags defined in NxDestructibleDepthParametersFlag.
	*/
	physx::PxU32	flags;
};

// NxDestructibleDepthParameters inline functions

PX_INLINE NxDestructibleDepthParameters::NxDestructibleDepthParameters()
{
	setToDefault();
}

PX_INLINE void NxDestructibleDepthParameters::setToDefault()
{
	flags = 0;
}

PX_INLINE bool NxDestructibleDepthParameters::overrideImpactDamage() const
{
	return (flags & NxDestructibleDepthParametersFlag::OVERRIDE_IMPACT_DAMAGE) != 0;
}

PX_INLINE bool NxDestructibleDepthParameters::overrideImpactDamageValue() const
{
	return (flags & NxDestructibleDepthParametersFlag::OVERRIDE_IMPACT_DAMAGE_VALUE) != 0;
}

PX_INLINE bool NxDestructibleDepthParameters::ignoresPoseUpdates() const
{
	return (flags & NxDestructibleDepthParametersFlag::IGNORE_POSE_UPDATES) != 0;
}

PX_INLINE bool NxDestructibleDepthParameters::ignoresRaycastCallbacks() const
{
	return (flags & NxDestructibleDepthParametersFlag::IGNORE_RAYCAST_CALLBACKS) != 0;
}

PX_INLINE bool NxDestructibleDepthParameters::ignoresContactCallbacks() const
{
	return (flags & NxDestructibleDepthParametersFlag::IGNORE_CONTACT_CALLBACKS) != 0;
}

PX_INLINE bool NxDestructibleDepthParameters::hasUserFlagSet(physx::PxU32 flagIndex) const
{
	switch (flagIndex)
	{
	case 0:
		return (flags & NxDestructibleDepthParametersFlag::USER_FLAG_0) != 0;
	case 1:
		return (flags & NxDestructibleDepthParametersFlag::USER_FLAG_1) != 0;
	case 2:
		return (flags & NxDestructibleDepthParametersFlag::USER_FLAG_2) != 0;
	case 3:
		return (flags & NxDestructibleDepthParametersFlag::USER_FLAG_3) != 0;
	default:
		return false;
	}
}


/**
	Flags that apply to a destructible actor, settable at runtime
*/
struct NxDestructibleParametersFlag
{
	enum Enum
	{
		/**
			If set, chunks will "remember" damage applied to them, so that many applications of a damage amount
			below damageThreshold will eventually fracture the chunk.  If not set, a single application of
			damage must exceed damageThreshold in order to fracture the chunk.
		*/
		ACCUMULATE_DAMAGE =	(1 << 0),

		/**
			Whether or not chunks at or deeper than the "debris" depth (see NxDestructibleParameters::debrisDepth)
			will time out.  The lifetime is a value between NxDestructibleParameters::debrisLifetimeMin and
			NxDestructibleParameters::debrisLifetimeMax, based upon the destructible module's LOD setting.
		*/
		DEBRIS_TIMEOUT =	(1 << 1),

		/**
			Whether or not chunks at or deeper than the "debris" depth (see NxDestructibleParameters::debrisDepth)
			will be removed if they separate too far from their origins.  The maxSeparation is a value between
			NxDestructibleParameters::debrisMaxSeparationMin and NxDestructibleParameters::debrisMaxSeparationMax,
			based upon the destructible module's LOD setting.
		*/
		DEBRIS_MAX_SEPARATION =	(1 << 2),

		/**
			If set, the smallest chunks may be further broken down, either by fluid crumbles (if a crumble particle
			system is specified in the NxDestructibleActorDesc), or by simply removing the chunk if no crumble
			particle system is specified.  Note: the "smallest chunks" are normally defined to be the deepest level
			of the fracture hierarchy.  However, they may be taken from higher levels of the hierarchy if
			NxModuleDestructible::setMaxChunkDepthOffset is called with a non-zero value.
		*/
		CRUMBLE_SMALLEST_CHUNKS =	(1 << 3),

		/**
			If set, the NxDestructibleActor::rayCast function will search within the nearest visible chunk hit
			for collisions with child chunks.  This is used to get a better raycast position and normal, in
			case the parent collision volume does not tightly fit the graphics mesh.  The returned chunk index
			will always be that of the visible parent that is intersected, however.
		*/
		ACCURATE_RAYCASTS =	(1 << 4),

		/**
			If set, the validBounds field of NxDestructibleParameters will be used.  These bounds are translated
			(but not scaled or rotated) to the origin of the destructible actor.  If a chunk or chunk island moves
			outside of those bounds, it is destroyed.
		*/
		USE_VALID_BOUNDS =	(1 << 5)
	};
};


/**
	Parameters that apply to a destructible actor
*/
struct NxDestructibleParameters
{
	/**
	\brief constructor sets to default.
	*/
	PX_INLINE		NxDestructibleParameters();

	/**
	\brief (re)sets the structure to the default.
	*/
	PX_INLINE void	setToDefault();

	/**
		The damage amount which will cause a chunk to fracture (break free) from the destructible.
		This is obtained from the damage value passed into the NxDestructibleActor::applyDamage,
		or NxDestructibleActor::applyRadiusDamage, or via impact (see 'forceToDamage', below).
	*/
	physx::PxF32		damageThreshold;

	/**
		Controls the distance into the destructible to propagate damage.  The damage applied to the chunk
		is multiplied by damageToRadius, to get the propagation distance.  All chunks within the radius
		will have damage applied to them.  The damage applied to each chunk varies with distance to the damage
		application position.  Full damage is taken at zero distance, and zero damage at the damage radius.
	*/
	physx::PxF32		damageToRadius;

	/**
		Limits the amount of damage applied to a chunk.  This is useful for preventing the entire destructible
		from getting pulverized by a very large application of damage.  This can easily happen when impact damage is
		used, and the damage amount is proportional to the impact force (see forceToDamage).
	*/
	physx::PxF32		damageCap;

	/**
		If a chunk is at a depth which takes impact damage (see NxDestructibleDepthParameters),
		then when a chunk has a collision in the NxScene, it will take damage equal to forceToDamage mulitplied by
		the impact force.
		The default value is zero, which effectively disables impact damage.
	*/
	physx::PxF32		forceToDamage;

	/**
		Large impact force may be reported if rigid bodies are spawned inside one another.  In this case the relative velocity of the two
		objects will be low.  This variable allows the user to set a minimum velocity threshold for impacts to ensure that the objects are
		moving at a min velocity in order for the impact force to be considered.
		Default value is zero.
	*/
	physx::PxF32		impactVelocityThreshold;

	/**
		When a chunk takes impact damage due to physical contact (see NxDestructibleDepthParameters), this parameter
		is the maximum impulse the contact can generate.  Weak materials such as glass may have this set to a low value, so that
		heavier objects will pass through them during fracture.
		N.B.: Setting this parameter to 0 disables the impulse cap; that is, zero is interpreted as infinite.
		Default value = 0.0f.
	*/
	physx::PxF32		materialStrength;

	/**
		The chunks will not be broken free below this depth.
	*/
	physx::PxU32		minimumFractureDepth;

	/**
		The default depth to which chunks will take impact damage.  This default may be overridden in the depth settings.
		Negative values imply no default impact damage.
		Default value = -1.
	*/
	physx::PxI32		impactDamageDefaultDepth;

	/**
		The chunk hierarchy depth at which chunks are considered to be "debris."  Chunks at this depth or
		below will be considered for various debris settings, such as debrisLifetime.
		Negative values indicate that no chunk depth is considered debris.
		Default value is -1.
	*/
	physx::PxI32		debrisDepth;

	/**
		The chunk hierarchy depth up to which chunks will not be eliminated due to LOD considerations.
		These chunks are considered to be essential either for gameplay or visually.
		The minimum value is 0, meaning the level 0 chunk is always considered essential.
		Default value is 0.
	*/
	physx::PxU32		essentialDepth;

	/**
		"Debris chunks" (see debrisDepth, above) will be destroyed after a time (in seconds)
		separated from non-debris chunks.  The actual lifetime is interpolated between these
		two values, based upon the module's LOD setting.  To disable lifetime, clear the
		NxDestructibleDepthParametersFlag::DEBRIS_TIMEOUT flag in the flags field.
		If debrisLifetimeMax < debrisLifetimeMin, the mean of the two is used for both.
		Default debrisLifetimeMin = 1.0, debrisLifetimeMax = 10.0f.
	*/
	physx::PxF32		debrisLifetimeMin;
	physx::PxF32		debrisLifetimeMax;

	/**
		"Debris chunks" (see debrisDepth, above) will be destroyed if they are separated from
		their origin by a distance greater than maxSeparation.  The actual maxSeparation is
		interpolated between these two values, based upon the module's LOD setting.  To disable
		maxSeparation, clear the NxDestructibleDepthParametersFlag::DEBRIS_MAX_SEPARATION flag in the flags field.
		If debrisMaxSeparationMax < debrisMaxSeparationMin, the mean of the two is used for both.
		Default debrisMaxSeparationMin = 1.0, debrisMaxSeparationMax = 10.0f.
	*/
	physx::PxF32		debrisMaxSeparationMin;
	physx::PxF32		debrisMaxSeparationMax;

	/**
		The probablity that a debris chunk, when fractured, will simply be destroyed instead of becoming
		dynamic or breaking down further into child chunks.  Valid range = [0.0,1.0].  Default value = 0.0.'
	*/

	physx::PxF32		debrisDestructionProbability;

	/**
		A bounding box around each NxDestructibleActor created, defining a range of validity for chunks that break free.
		These bounds are scaled and translated with the NxDestructibleActor's scale and position, but they are *not*
		rotated with the NxDestructibleActor.
	*/
	physx::PxBounds3	validBounds;

	/**
		If greater than 0, the chunks' speeds will not be allowed to exceed maxChunkSpeed.  Use 0
		to disable this feature (this is the default).
	*/
	physx::PxF32		maxChunkSpeed;

	/**
		A collection of flags defined in NxDestructibleParametersFlag.
	*/
	physx::PxU32		flags;

	/**
		Scale factor used to apply an impulse force along the normal of chunk when fractured.  This is used
		in order to "push" the pieces out as they fracture.
	*/
	physx::PxF32		fractureImpulseScale;

	/**
		Optional dominance group for dynamic chunks created when fractured. (ignored if > 31)
	*/
	physx::PxU8			dynamicChunksDominanceGroup;

	/**
		Whether or not to use dynamicChunksGroupsMask.  If false, NULL will be passed into the DestructibleActor upon
		instantiation, through the NxDestructibleActorDesc.
	*/
	bool				useDynamicChunksGroupsMask;

	/**
		Optional groups mask (2.x) or filter data (3.x) for dynamic chunks created when fractured. (Used if useDynamicChunksGroupsMask is true.)
	*/
#if (NX_SDK_VERSION_MAJOR == 2)
	NxGroupsMask		dynamicChunksGroupsMask;
#elif (NX_SDK_VERSION_MAJOR == 3)
	physx::PxFilterData	dynamicChunksFilterData;
#endif

	/**
		The maximum number of NxDestructibleDepthParameters (see depthParameters).
	*/
	enum { kDepthParametersCountMax = 16 };

	/**
		The number of NxDestructibleDepthParameters (see depthParameters).
		Must be in the range [0, kDepthParametersCountMax].
	*/
	physx::PxU32		depthParametersCount;

	/**
		Parameters that apply to every chunk at a given depth level (see NxDestructibleDepthParameters).
		The element [0] of the array applies to the depth 0 (unfractured) chunk, element [1] applies
		to the level 1 chunks, etc.
		The number of valid depth parameters must be given in depthParametersCount.
	*/
	NxDestructibleDepthParameters	depthParameters[kDepthParametersCountMax];
};

// NxDestructibleParameters inline functions

PX_INLINE NxDestructibleParameters::NxDestructibleParameters()
{
	setToDefault();
}

PX_INLINE void NxDestructibleParameters::setToDefault()
{
	damageThreshold = 1.0f;
	damageToRadius = 0.1f;
	damageCap = 0;
	forceToDamage = 0;
	impactVelocityThreshold = 0.0f;
	materialStrength = 0.0f;
	minimumFractureDepth = 0;
	impactDamageDefaultDepth = -1;
	debrisDepth = -1;
	essentialDepth = 0;
	debrisLifetimeMin = 1.0f;
	debrisLifetimeMax = 10.0f;
	debrisMaxSeparationMin = 1.0f;
	debrisMaxSeparationMax = 10.0f;
	debrisDestructionProbability = 0.0f;
	validBounds = physx::PxBounds3(physx::PxVec3(-10000.0f), physx::PxVec3(10000.0f));
	maxChunkSpeed = 0.0f;
	fractureImpulseScale = 0.0f;
	useDynamicChunksGroupsMask = false;
#if (NX_SDK_VERSION_MAJOR == 2)
	dynamicChunksGroupsMask.bits0 = dynamicChunksGroupsMask.bits1 = dynamicChunksGroupsMask.bits2 = dynamicChunksGroupsMask.bits3 = 0;
#elif  (NX_SDK_VERSION_MAJOR == 3)
	dynamicChunksFilterData.word0 = dynamicChunksFilterData.word1 = dynamicChunksFilterData.word2 = dynamicChunksFilterData.word3 = 0;
#endif
	dynamicChunksDominanceGroup = 0xFF;	// Out of range, so it won't be used.
	flags = NxDestructibleParametersFlag::ACCUMULATE_DAMAGE;
	depthParametersCount = 0;
}


struct NxDestructibleInitParametersFlag
{
	enum Enum
	{
		/**
			If set, then chunks which are tagged as "support" chunks (via NxDestructibleChunkDesc::isSupportChunk)
			will have environmental support in static destructibles.
			Note: if both ASSET_DEFINED_SUPPORT and WORLD_SUPPORT are set, then chunks must be tagged as
			"support" chunks AND overlap the NxScene's static geometry in order to be environmentally supported.
		*/
		ASSET_DEFINED_SUPPORT =	(1 << 0),

		/**
			If set, then chunks which overlap the NxScene's static geometry will have environmental support in
			static destructibles.
			Note: if both ASSET_DEFINED_SUPPORT and WORLD_SUPPORT are set, then chunks must be tagged as
			"support" chunks AND overlap the NxScene's static geometry in order to be environmentally supported.
		*/
		WORLD_SUPPORT =	(1 << 1),

		/**
			If this is set and the destructible is initially static, it will become part of an extended support
			structure if it is in contact with another static destructible that also has this flag set.
		*/
		FORM_EXTENDED_STRUCTURES =	(1 << 2)
	};
};

/**
	Parameters that apply to a destructible actor at initialization
*/
struct NxDestructibleInitParameters
{
	/**
	\brief constructor sets to default.
	*/
	PX_INLINE		NxDestructibleInitParameters();

	/**
	\brief (re)sets the structure to the default.
	*/
	PX_INLINE void	setToDefault();

	/**
		The chunk hierarchy depth at which to create a support graph.  Higher depth levels give more detailed support,
		but will give a higher computational load.  Chunks below the support depth will never be supported.
	*/
	physx::PxU32		supportDepth;

	/**
		A collection of flags defined in NxDestructibleInitParametersFlag.
	*/
	physx::PxU32		flags;
};


// NxDestructibleInitParameters inline functions

PX_INLINE NxDestructibleInitParameters::NxDestructibleInitParameters()
{
	setToDefault();
}

PX_INLINE void NxDestructibleInitParameters::setToDefault()
{
	supportDepth = 0;
	flags = 0;
}


/**
	Destructible authoring structure.

	Descriptor to build one chunk in a fracture hierarchy.
*/
class NxDestructibleChunkDesc : public NxApexDesc
{
public:
	/**
	\brief constructor sets to default.
	*/
	PX_INLINE		NxDestructibleChunkDesc();

	/**
	\brief (re)sets the structure to the default.
	*/
	PX_INLINE void	setToDefault();

	/**
		Returns true iff an object can be created using this descriptor.
	*/
	PX_INLINE bool	isValid() const;

	/**
		Defines the chunk to be environmentally supported, if the appropriate NxDestructibleParametersFlag flags
		are set in NxDestructibleParameters.
	*/
	bool			isSupportChunk;

	/**
		Defines the chunk to be unfractureable.  If this is true, then none of its children will be fractureable.
	*/
	bool			doNotFracture;

	/**
		Defines the chunk to be undamageable.  This means this chunk will not fracture, but its children might.
	*/
	bool			doNotDamage;

	/**
		Defines the chunk to be uncrumbleable.  This means this chunk will not be broken down into fluid mesh particles
		no matter how much damage it takes.  Note: this only applies to chunks with no children.  For a chunk with
		children, then:
		1) The chunk may be broken down into its children, and then its children may be crumbled, if the doNotCrumble flag
		is not set on them.
		2) If the Destructible module's chunk depth offset LOD may be set such that this chunk effectively has no children.
		In this case, the doNotCrumble flag will apply to it.
	*/
	bool			doNotCrumble;

	/**
		Whether or not to use instancing when rendering this chunk.  If useInstancedRendering = TRUE, this chunk will
		share a draw call with all others that instance the mesh indexed by meshIndex.  This may extend to other
		destructible actors created from this asset.  If useInstancedRendering = FALSE, this chunk may share a draw
		call only with chunks other chunks in this asset which have useInstancedRendering = FALSE.
		Default = FALSE.
	*/
	bool			useInstancedRendering;

	/**
		Translation for this chunk mesh within the asset. Normally a chunk needs no translation, but if a chunk is instanced within
		the asset, then this translation is needed.
		Default = (0,0,0).
	*/
	physx::PxVec3	instancePositionOffset;

	/**
		UV translation for this chunk mesh within the asset. Normally a chunk needs no UV translation, but if a chunk is instanced within
		the asset, then this translation is usually needed.
		Default = (0,0).
	*/
	physx::PxVec2	instanceUVOffset;

	/**
		If useInstancedRendering = TRUE, this index is the instanced mesh index.  If useInstancedRendering = FALSE,
		this index is the mesh part index for the skinned or statically rendered mesh.
		This must index a valid NxDestructibleGeometryDesc (see below).
		Default = 0xFFFF (invalid).
	*/
	physx::PxU16	meshIndex;

	/**
		The parent index of this chunk.  If the index is negative, this is a root chunk.
		Default = -1.
	*/
	physx::PxI32	parentIndex;

	/**
		A direction used to move the chunk out of the destructible, if an impact kick is applied.
	*/
	physx::PxVec3	surfaceNormal;
};

// NxDestructibleChunkDesc inline functions

PX_INLINE NxDestructibleChunkDesc::NxDestructibleChunkDesc()
{
	setToDefault();
}

PX_INLINE void NxDestructibleChunkDesc::setToDefault()
{
	NxApexDesc::setToDefault();
	isSupportChunk = false;
	doNotFracture = false;
	doNotDamage = false;
	doNotCrumble = false;
	useInstancedRendering = false;
	instancePositionOffset = physx::PxVec3(0.0f);
	instanceUVOffset = physx::PxVec2(0.0f);
	meshIndex = 0xFFFF;
	parentIndex = -1;
	surfaceNormal = physx::PxVec3(0.0f);
}

PX_INLINE bool NxDestructibleChunkDesc::isValid() const
{
	if (meshIndex == 0xFFFF)
	{
		return false;
	}

	return NxApexDesc::isValid();
}


/**
	Destructible authoring structure.

	Descriptor to build one chunk in a fracture hierarchy.
*/
class NxDestructibleGeometryDesc : public NxApexDesc
{
public:
	/**
	\brief constructor sets to default.
	*/
	PX_INLINE		NxDestructibleGeometryDesc();

	/**
	\brief (re)sets the structure to the default.
	*/
	PX_INLINE void	setToDefault();

	/**
		Returns true iff an object can be created using this descriptor.
	*/
	PX_INLINE bool	isValid() const;

	/**
		The convex hulls associated with this chunk.  These may be obtained from IExplicitHierarchicalMesh::convexHulls()
		if authoring using an IExplicitHierarchicalMesh.  The length of the array is given by convexHullCount.
	*/
	const physx::IExplicitHierarchicalMesh::IConvexHull**	convexHulls;

	/**
		The length of the convexHulls array.  If this is positive, then convexHulls must point to a valid array of this size.
		If this is zero, then collisionVolumeDesc must not be NULL, and convex hulls will be automatically created for this
		geometry using collisionVolumeDesc.
	*/
	physx::PxU32											convexHullCount;

	/**
		If convexHullCount = 0, then collisionVolumeDesc must not be NULL.  In this case convex hulls will automatically be
		created for this geometry.  See NxCollisionVolumeDesc.
	*/
	const physx::NxCollisionVolumeDesc*						collisionVolumeDesc;
};

// NxDestructibleGeometryDesc inline functions

PX_INLINE NxDestructibleGeometryDesc::NxDestructibleGeometryDesc()
{
	setToDefault();
}

PX_INLINE void NxDestructibleGeometryDesc::setToDefault()
{
	NxApexDesc::setToDefault();
	convexHulls = NULL;
	convexHullCount = 0;
	collisionVolumeDesc = NULL;
}

PX_INLINE bool NxDestructibleGeometryDesc::isValid() const
{
	if (convexHullCount == 0 && collisionVolumeDesc == NULL)
	{
		return false;
	}

	if (convexHullCount > 0 && convexHulls == NULL)
	{
		return false;
	}

	return NxApexDesc::isValid();
}


/**
	Destructible authoring structure.

	Descriptor for the cookChunk() method of NxDestructibleAssetAuthoring
*/
class NxDestructibleAssetCookingDesc : public NxApexDesc
{
public:
	/**
	\brief constructor sets to default.
	*/
	PX_INLINE		NxDestructibleAssetCookingDesc();

	/**
	\brief (re)sets the structure to the default.
	*/
	PX_INLINE void	setToDefault();

	/**
		Returns true iff an object can be created using this descriptor.
	*/
	PX_INLINE bool	isValid() const;

	/**
		Beginning of array of descriptors, one for each chunk.
	*/
	NxDestructibleChunkDesc*	chunkDescs;

	/**
		The size of the chunkDescs array.  This must be positive.
	*/
	physx::PxU32				chunkDescCount;

	/**
		Beginning of array of descriptors, one for each mesh part.
	*/
	NxDestructibleGeometryDesc*	geometryDescs;

	/**
		The size of the geometryDescs array.  This must be positive.
	*/
	physx::PxU32				geometryDescCount;
};

// NxDestructibleAssetCookingDesc inline functions

PX_INLINE NxDestructibleAssetCookingDesc::NxDestructibleAssetCookingDesc()
{
	setToDefault();
}

PX_INLINE void NxDestructibleAssetCookingDesc::setToDefault()
{
	NxApexDesc::setToDefault();
	chunkDescs = NULL;
	chunkDescCount = 0;
	geometryDescs = NULL;
	geometryDescCount = 0;
}

PX_INLINE bool NxDestructibleAssetCookingDesc::isValid() const
{
	if (chunkDescCount == 0 || chunkDescs == NULL)
	{
		return false;
	}

	for (physx::PxU32 i = 0; i < chunkDescCount; ++i )
	{
		if (!chunkDescs[i].isValid())
		{
			return false;
		}
	}

	if (chunkDescCount >= 65535)
	{
		return false;
	}

	if (geometryDescCount == 0 || geometryDescs == NULL)
	{
		return false;
	}

	for (physx::PxU32 i = 0; i < geometryDescCount; ++i )
	{
		if (!geometryDescs[i].isValid())
		{
			return false;
		}
	}

	return NxApexDesc::isValid();
}


/**
	Stats for an NxDestructibleAsset: memory usage, counts, etc.
*/

struct NxDestructibleAssetStats
{
	physx::PxU32			totalBytes;
	physx::PxU32			chunkCount;
	physx::PxU32			chunkBytes;
	physx::PxU32			chunkHullDataBytes;
	physx::PxU32			collisionCookedHullDataBytes;
	physx::PxU32			collisionMeshCount;
	NxRenderMeshAssetStats	renderMeshAssetStats;
};

/**
	Authoring API for a destructible asset.
*/
class NxDestructibleAssetAuthoring : public NxApexAssetAuthoring
{
public:

	/** Fracturing API */

	/**
		NxDestructibleAssetAuthoring contains an instantiation of IExplicitHierarchicalMesh.
		This function gives access to it.  See IExplicitHierarchicalMesh for details, it is
		the object used by the fracturing tool set for mesh fracturing operations and is used
		to generate the embedded NxApexRenderMesh as well as collision and hierarchy data
		for the destructible asset.
	*/
	virtual IExplicitHierarchicalMesh&		getExplicitHierarchicalMesh() = 0;

	/**
		NxDestructibleAssetAuthoring contains a second instantiation of IExplicitHierarchicalMesh
		used to describe the core mesh for slice fracturing (see FractureTools::FractureSliceDesc),
		done in createHierarchicallySplitMesh().  This function gives access to it.
	*/
	virtual IExplicitHierarchicalMesh&		getCoreExplicitHierarchicalMesh() = 0;

	/**
		NxDestructibleAssetAuthoring contains an instantiation of ICutoutSet used to describe the
		cutout fracturing shapes (see FractureTools::ICutoutSet), done in createChippedMesh().
		This function gives access to it.
	*/
	virtual FractureTools::ICutoutSet&		getCutoutSet() = 0;

	/**
		Builds a new IExplicitHierarchicalMesh from an array of triangles, used as the starting
		point for fracturing.  It will contain only one chunk, at depth 0.

		meshTriangles: pointer to array of NxExplicitRenderTriangles which make up the mesh
		meshTriangleCount the size of the meshTriangles array
		submeshData: pointer to array of NxExplicitSubmeshData, describing the submeshes
		submeshCount: the size of the submeshData array
		meshPartition: if not NULL, an array of size meshPartitionCount, giving the end elements of contiguous subsets of meshTriangles.
			If meshPartition is NULL, one partition is assumed.
			When there is one partition, these triangles become the level 0 part.
			When there is more than one partition, these triangles become level 1 parts, while the union of the parts will be the level 0 part.
		meshPartitionCount: if meshPartition is not NULL, this is the size of the meshPartition array.
	*/
	virtual bool							setRootMesh
	(
	    const NxExplicitRenderTriangle* meshTriangles,
	    physx::PxU32 meshTriangleCount,
	    const NxExplicitSubmeshData* submeshData,
	    physx::PxU32 submeshCount,
	    physx::PxU32* meshPartition = NULL,
	    physx::PxU32 meshPartitionCount = 0
	) = 0;

	/** 
		Builds the root IExplicitHierarchicalMesh from an NxRenderMeshAsset.
		Since an NxDestructibleAsset contains no hierarchy information, the input mesh must have only one part.
	*/
	virtual bool							importRenderMeshAssetToRootMesh(const physx::NxRenderMeshAsset& renderMeshAsset) = 0;

	/** 
		Builds the root IExplicitHierarchicalMesh from an NxDestructibleAsset.
		Since an NxDestructibleAsset contains hierarchy information, the explicit mesh formed
		will have this hierarchy structure.
	*/
	virtual bool							importDestructibleAssetToRootMesh(const physx::NxDestructibleAsset& destructibleAsset) = 0;

	/**
		Builds a new IExplicitHierarchicalMesh from an array of triangles, used as the core mesh
		for slice fracture operations (see FractureTools::FractureSliceDesc, passed into
		createHierarchicallySplitMesh).

		mesh: pointer to array of NxExplicitRenderTriangles which make up the mesh
		meshTriangleCount the size of the meshTriangles array
		submeshData: pointer to array of NxExplicitSubmeshData, describing the submeshes
		submeshCount: the size of the submeshData array
	*/
	virtual bool							setCoreMesh
	(
	    const NxExplicitRenderTriangle* mesh,
	    physx::PxU32 meshTriangleCount,
	    const NxExplicitSubmeshData* submeshData,
	    physx::PxU32 submeshCount,
	    physx::PxU32* meshPartition = NULL,
	    physx::PxU32 meshPartitionCount = 0
	) = 0;

	/**
		Splits the chunk in chunk[0], forming a hierarchy of fractured chunks in chunks[1...] using
		slice-mode fracturing.

		meshProcessingParams: describes generic mesh processing directives
		desc: describes the slicing surfaces (see FractureSliceDesc)
		volumeDesc: convex hulls will be generated for each chunk using the method.  See NxCollisionVolumeDesc.
		exportCoreMesh: if true, the core mesh will be included (at depth 1) in the hierarchically split mesh.  Otherwise, it will only be used to create a hollow space.
		coreMeshImprintSubmeshIndex: if this is < 0, use the core mesh materials (was applyCoreMeshMaterialToNeighborChunks).  Otherwise, use the given submesh
		randomSeed: seed for the random number generator, to ensure reproducibility.
		progressListener: The user must instantiate an IProgressListener, so that this function may report progress of this operation
		cancel: if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.  Meant to be set from another thread.

		returns true if successful.
	*/
	virtual bool							createHierarchicallySplitMesh
	(
	    const FractureTools::MeshProcessingParameters& meshProcessingParams,
	    const FractureTools::FractureSliceDesc& desc,
	    const physx::NxCollisionVolumeDesc& volumeDesc,
	    bool exportCoreMesh,
		physx::PxI32 coreMeshImprintSubmeshIndex,
	    physx::PxU32 randomSeed,
	    IProgressListener& progressListener,
	    volatile bool* cancel = NULL
	) = 0;

	/**
		Splits the mesh in chunk[0], forming a hierarchy of fractured meshes in chunks[1...] using
		cutout-mode (chippable) fracturing.

		meshProcessingParams: describes generic mesh processing directives
		desc: describes the slicing surfaces (see FractureCutoutDesc)
		cutoutSet: the cutout set to use for fracturing (see ICutoutSet)
		sliceDesc: used if desc.chunkFracturingMethod = SliceFractureCutoutChunks
		voronoiDesc: used if desc.chunkFracturingMethod = VoronoiFractureCutoutChunks
		volumeDesc: convex hulls will be generated for each chunk using the method.  See NxCollisionVolumeDesc.
		randomSeed: seed for the random number generator, to ensure reproducibility.
		progressListener: The user must instantiate an IProgressListener, so that this function may report progress of this operation
		cancel: if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.  Meant to be set from another thread.

		returns true if successful.
	*/
	virtual bool							createChippedMesh
	(
	    const FractureTools::MeshProcessingParameters& meshProcessingParams,
	    const FractureTools::FractureCutoutDesc& desc,
	    const FractureTools::ICutoutSet& cutoutSet,
	    const FractureTools::FractureSliceDesc& sliceDesc,
		const FractureTools::FractureVoronoiDesc& voronoiDesc,
	    const physx::NxCollisionVolumeDesc& volumeDesc,
	    physx::PxU32 randomSeed,
	    IProgressListener& progressListener,
	    volatile bool* cancel = NULL
	) = 0;

	/**
		Builds an internal cutout set.

		pixelBuffer: pointer to be beginning of the pixel buffer
		bufferWidth: the width of the buffer in pixels
		bufferHeight: the height of the buffer in pixels
		snapThreshold: the pixel distance at which neighboring cutout vertices and segments may be fudged into alignment.
		periodic: whether or not to use periodic boundary conditions when creating cutouts from the map
	*/
	virtual void							buildCutoutSet
	(
	    const physx::PxU8* pixelBuffer,
	    physx::PxU32 bufferWidth,
	    physx::PxU32 bufferHeight,
	    physx::PxF32 snapThreshold,
		bool periodic
	) = 0;

	/**
		Calculate the mapping between a cutout fracture map and a given triangle.
		The result is a 3 by 3 matrix M composed by an affine transformation and a rotation, we can get the 3-D projection for a texture coordinate pair (u,v) with such a formula:
		(x,y,z) = M*PxVec3(u,v,1)

		mapping: resulted mapping, composed by an affine transformation and a rotation
		targetDirection: the target face's normal
	**/
	virtual bool							calculateCutoutUVMapping
	(
		physx::PxMat33& mapping,
		const physx::NxExplicitRenderTriangle& triangle
	) = 0;

	/**
		Uses the passed-in target direction to find the best triangle in the root mesh with normal near the given targetDirection.  If triangles exist
		with normals within one degree of the given target direction, then one with the greatest area of such triangles is used.  Otherwise, the triangle
		with normal closest to the given target direction is used.  The resulting triangle is used to calculate a UV mapping as in the function
		calculateCutoutUVMapping (above).

		The assumption is that there exists a single mapping for all triangles on a specified face, for this feature to be useful. 

		mapping: resulted mapping, composed by an affine transformation and a rotation
		targetDirection: the target face's normal
	**/
	virtual bool							calculateCutoutUVMapping
	(
		physx::PxMat33& mapping,
		const physx::PxVec3& targetDirection
	) = 0;

	/**
		Splits the mesh in chunk[0], forming fractured pieces chunks[1...] using
		Voronoi decomposition fracturing.

		meshProcessingParams: describes generic mesh processing directives
		desc: describes the voronoi splitting parameters surfaces (see FractureVoronoiDesc)
		volumeDesc: convex hulls will be generated for each chunk using the method.  See NxCollisionVolumeDesc.
		exportCoreMesh: if true, the core mesh will be included (at depth 1) in the split mesh.  Otherwise, it will only be used to create a hollow space.
		coreMeshImprintSubmeshIndex: if this is < 0, use the core mesh materials (was applyCoreMeshMaterialToNeighborChunks).  Otherwise, use the given submesh
		randomSeed: seed for the random number generator, to ensure reproducibility.
		progressListener: The user must instantiate an IProgressListener, so that this function may report progress of this operation
		cancel: if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.  Meant to be set from another thread.

		returns true if successful.
	*/
	virtual bool							createVoronoiSplitMesh
	(
		const FractureTools::MeshProcessingParameters& meshProcessingParams,
		const FractureTools::FractureVoronoiDesc& desc,
		const physx::NxCollisionVolumeDesc& volumeDesc,
		bool exportCoreMesh,
		physx::PxI32 coreMeshImprintSubmeshIndex,
		physx::PxU32 randomSeed,
		IProgressListener& progressListener,
		volatile bool* cancel = NULL
	) = 0;

	/**
		Generates a set of uniformly distributed points in the interior of the root mesh.

		siteBuffer: an array of PxVec3, at least the size of siteCount
		siteCount: the number of points to write into siteBuffer
		randomSeed: pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		progressListener: The user must instantiate an IProgressListener, so that this function may report progress of this operation
		chunkIndex: If this is a valid index, the voronoi sites will only be created within the volume of the indexed chunk.  Otherwise,
			the sites will be created within each of the root-level chunks.  Default value is an invalid index.
	*/
	virtual void							createVoronoiSitesInsideMesh
	(
		physx::PxVec3* siteBuffer,
		physx::PxU32 siteCount,
		physx::PxU32* randomSeed,
		IProgressListener& progressListener,
		physx::PxU32 chunkIndex = 0xFFFFFFFF
	) = 0;

	/**
		Utility to visualize Voronoi cells for a given set of sites.

		debugRender: rendering object which will receive the drawing primitives associated with this cell visualization
		sites: an array of Voronoi cell sites, of length siteCount
		siteCount: the number of Voronoi cell sites (length of sites array)
		cellColors: an optional array of colors (see NxApexRenderDebug for format) for the cells.  If NULL, the white (0xFFFFFFFF) color will be used.
			If not NULL, this (of length cellColorCount) is used to color the cell graphics.  The number cellColorCount need not match siteCount.  If
			cellColorCount is less than siteCount, the cell colors will cycle.  That is, site N gets cellColor[N%cellColorCount].
		cellColorCount: the number of cell colors (the length of cellColors array)
		bounds: defines an axis-aligned bounding box which clips the visualization, since some cells extend to infinity
		cellIndex: if this is a valid index (cellIndex < siteCount), then only the cell corresponding to sites[cellIndex] will be drawn.  Otherwise, all cells will be drawn.
	*/
	virtual void							visualizeVoronoiCells
	(
		physx::NxApexRenderDebug& debugRender,
		const physx::PxVec3* sites,
		physx::PxU32 siteCount,
		const physx::PxU32* cellColors,
		physx::PxU32 cellColorCount,
		const physx::PxBounds3& bounds,
		physx::PxU32 cellIndex = 0xFFFFFFFF
	) = 0;

	/**
		Splits the chunk in chunk[chunkIndex], forming a hierarchy of fractured chunks using
		slice-mode fracturing.  The chunks will be rearranged so that they are in breadth-first order.

		chunkIndex: index of chunk to be split
		meshProcessingParams: describes generic mesh processing directives
		desc: describes the slicing surfaces (see FractureSliceDesc)
		volumeDesc: convex hulls will be generated for each chunk using the method.  See NxCollisionVolumeDesc.
		randomSeed: pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		progressListener: The user must instantiate an IProgressListener, so that this function may report progress of this operation
		cancel: if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.  Meant to be set from another thread.

		returns true if successful.
	*/
	virtual bool							hierarchicallySplitChunk
	(
		physx::PxU32 chunkIndex,
	    const FractureTools::MeshProcessingParameters& meshProcessingParams,
	    const FractureTools::FractureSliceDesc& desc,
	    const physx::NxCollisionVolumeDesc& volumeDesc,
	    physx::PxU32* randomSeed,
	    IProgressListener& progressListener,
	    volatile bool* cancel = NULL
	) = 0;

	/**
		Splits the chunk in chunk[chunkIndex], forming fractured chunks using
		Voronoi decomposition fracturing.  The chunks will be rearranged so that they are in breadth-first order.

		chunkIndex: index of chunk to be split
		meshProcessingParams: describes generic mesh processing directives
		desc: describes the voronoi splitting parameters surfaces (see FractureVoronoiDesc)
		volumeDesc: convex hulls will be generated for each chunk using the method.  See NxCollisionVolumeDesc.
		randomSeed: pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		progressListener: The user must instantiate an IProgressListener, so that this function may report progress of this operation
		cancel: if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.  Meant to be set from another thread.

		returns true if successful.
	*/
	virtual bool							voronoiSplitChunk
	(
		physx::PxU32 chunkIndex,
	    const FractureTools::MeshProcessingParameters& meshProcessingParams,
	    const FractureTools::FractureVoronoiDesc& desc,
	    const physx::NxCollisionVolumeDesc& volumeDesc,
	    physx::PxU32* randomSeed,
	    IProgressListener& progressListener,
	    volatile bool* cancel = NULL
	) = 0;

	/**
		Set the tolerances used in CSG calculations.

		linearTolerance: relative (to mesh size) tolerance used with angularTolerance to determine coplanarity.  Default = 1.0e-4.
		angularTolerance: used with linearTolerance to determine coplanarity.  Default = 1.0e-3
		baseTolerance: relative (to mesh size) tolerance used for spatial partitioning
		clipTolerance: relative (to mesh size) tolerance used when clipping triangles for CSG mesh building operations.  Default = 1.0e-4.
		cleaningTolerance: relative (to mesh size) tolerance used when cleaning the out put mesh generated from the toMesh() function.  Default = 1.0e-7.
	*/
	virtual void							setBSPTolerances
	(
		physx::PxF32 linearTolerance,
		physx::PxF32 angularTolerance,
		physx::PxF32 baseTolerance,
		physx::PxF32 clipTolerance,
		physx::PxF32 cleaningTolerance
	) = 0;

	/**
		Set the parameters used in BSP building operations.

		logAreaSigmaThreshold:	At each step in the tree building process, the surface with maximum triangle area is compared
			to the other surface triangle areas.  If the maximum area surface is far from the "typical" set of
			surface areas, then that surface is chosen as the next splitting plane.  Otherwise, a random
			test set is chosen and a winner determined based upon the weightings below.
			The value logAreaSigmaThreshold determines how "atypical" the maximum area surface must be to
			be chosen in this manner.
			Default value = 2.0.
		testSetSize: Larger values of testSetSize may find better BSP trees, but will take more time to create.
			testSetSize = 0 is treated as infinity (all surfaces will be tested for each branch).
			Default value = 10.
		splitWeight: How much to weigh the relative number of triangle splits when searching for a BSP surface.
			Default value = 0.5.
		imbalanceWeight: How much to weigh the relative triangle imbalance when searching for a BSP surface.
			Default value = 0.0.
	*/
	virtual void	setBSPBuildParameters
	(
		physx::PxF32 logAreaSigmaThreshold,
		physx::PxU32 testSetSize,
		physx::PxF32 splitWeight,
		physx::PxF32 imbalanceWeight
	) = 0;


	/**
		Instantiates an IExplicitHierarchicalMesh::IConvexHull

		See the IConvexHull API for its functionality.  Can be used to author chunk hulls in the
		cookChunks function.

		Use IConvexHull::release() to delete the object.
	*/
	virtual IExplicitHierarchicalMesh::IConvexHull*	createExplicitHierarchicalMeshConvexHull() = 0;

	/**
		Builds a mesh used for slice fracturing, given the noise parameters and random seed.  This function is mostly intended
		for visualization - to give the user a "typical" slice surface used for fracturing.

		Returns the head of an array of NxExplicitRenderTriangles, of length given by the return value.
	*/
	virtual physx::PxU32					buildSliceMesh(const NxExplicitRenderTriangle*& mesh, const FractureTools::NoiseParameters& noiseParameters, const physx::PxPlane& slicePlane, physx::PxU32 randomSeed) = 0;

	/**
		Serialization/deserialization of the data associated with the fracture API.  This includes
		the root mesh, core mesh, and cutout set.
	*/
	virtual void							serializeFractureToolState(physx::general_PxIOStream2::PxFileBuf& stream, physx::IExplicitHierarchicalMesh::IEmbedding& embedding) const = 0;
	virtual	void							deserializeFractureToolState(physx::general_PxIOStream2::PxFileBuf& stream, physx::IExplicitHierarchicalMesh::IEmbedding& embedding) = 0;

	/**
		If 'enabled' is true, then the depth value is passed directly to cacheChunkOverlapsUpToDepth.  (See
		NxDestructibleAsset::cacheChunkOverlapsUpToDepth).  If 'enabled' is false, cacheChunkOverlapsUpToDepth will
		not be called.
	*/
	virtual void							setChunkOverlapsCacheDepth(bool enabled, physx::PxI32 depth = -1) = 0;

	/**
		Gets the NxRenderMeshAsset associated with this asset.
	*/
	virtual const NxRenderMeshAsset*		getRenderMeshAsset() const = 0;

	/**
		Set the NxRenderMeshAsset associated with this asset.
		This is the asset used for non-instanced rendering.
		NULL is a valid argument, and can be used to clear the internal mesh data.
		Returns true if successful.
	*/
	virtual bool							setRenderMeshAsset(NxRenderMeshAsset*) = 0;

	/**
		Get the number of instanced chunk meshes in this asset.
	*/
	virtual physx::PxU32					getInstancedChunkCount() const = 0;

	/**
		Set the parameters used for runtime destruction behavior.  See NxDestructibleParameters.
	*/
	virtual void                            setDestructibleParameters(const NxDestructibleParameters&) = 0;

	/**
		The NxDestructibleParameters which describe the default fracturing behavior for instanced
		NxDestructibleActors.  These may be overridden by calling setDestructibleParameters().
	*/
	virtual NxDestructibleParameters		getDestructibleParameters() const = 0;

	/**
		Set the parameters used for default destructible initialization.  See NxDestructibleInitParameters.
	*/
	virtual void                            setDestructibleInitParameters(const NxDestructibleInitParameters&) = 0;

	/**
		The parameters used for default destructible initialization.  See NxDestructibleInitParameters.
	*/
	virtual NxDestructibleInitParameters	getDestructibleInitParameters() const = 0;

	/**
		Set the name of the emitter to use when generating crumble particles.
	*/
	virtual void                            setCrumbleEmitterName(const char*) = 0;

	/**
		Set the name of the emitter to use when generating fracture-line dust particles.
	*/
	virtual void                            setDustEmitterName(const char*) = 0;

	/**
		Set padding used for chunk neighbor tests.  This padding is relative to the largest diagonal
		of the asset's local bounding box.
		This value must be non-negative.
		Default value = 0.001f.
	*/
	virtual void							setNeighborPadding(physx::PxF32 neighborPadding) = 0;

	/**
		Get padding used for chunk neighbor tests.  Set setNeighborPadding().
	*/
	virtual physx::PxF32					getNeighborPadding() const = 0;

	/**
		Once the internal IExplicitHierarchicalMesh is built using the fracture tools functions
		and all emitter names and parameters set, this functions builds the destructible asset.
		Every chunk (corresponding to a part in the IExplicitHierarchicalMesh) must have
		destruction-specific data set through the descriptor passed into this function.  See
		NxDestructibleAssetCookingDesc.
	*/
	virtual void                            cookChunks(const NxDestructibleAssetCookingDesc&) = 0;

	/**
		The scale factor used to apply an impulse force along the normal of chunk when fractured.  This is used
		in order to "push" the pieces out as they fracture.
	*/
	virtual physx::PxF32					getFractureImpulseScale() const = 0;

	/**
		Large impact force may be reported if rigid bodies are spawned inside one another.  In this case the realative velocity of the two
		objects will be low.  This variable allows the user to set a minimum velocity threshold for impacts to ensure that the objects are
		moving at a min velocity in order for the impact force to be considered.
	*/
	virtual physx::PxF32					getImpactVelocityThreshold() const = 0;

	/**
		The total number of chunks in the cooked asset.
	*/
	virtual physx::PxU32					getChunkCount() const = 0;

	/**
		The total number of fracture hierarchy depth levels in the cooked asset.
	*/
	virtual physx::PxU32					getDepthCount() const = 0;

	/**
		Returns the number of children for the given chunk.
		chunkIndex must be less than getChunkCount().  If it is not, this function returns 0.
	*/
	virtual physx::PxU32					getChunkChildCount(physx::PxU32 chunkIndex) const = 0;

	/**
		Returns the index for the given child of the given chunk.
		chunkIndex must be less than getChunkCount() and childIndex must be less than getChunkChildCount(chunkIndex).
		If either of these conditions is not met, the function returns NxModuleDestructibleConst::INVALID_CHUNK_INDEX.
	*/
	virtual physx::PxI32					getChunkChild(physx::PxU32 chunkIndex, physx::PxU32 childIndex) const = 0;

	/**
		If this chunk is instanced within the same asset, then this provides the instancing position offset.
		Otherwise, this function returns (0,0,0).
	*/
	virtual physx::PxVec3					getChunkPositionOffset(physx::PxU32 chunkIndex) const = 0;

	/**
		If this chunk is instanced within the same asset, then this provides the instancing UV offset.
		Otherwise, this function returns (0,0).
	*/
	virtual physx::PxVec2					getChunkUVOffset(physx::PxU32 chunkIndex) const = 0;

	/**
		The render mesh asset part index associated with this chunk.
	*/
	virtual physx::PxU32					getPartIndex(physx::PxU32 chunkIndex) const = 0;

	/**
		Rebuild the collision volumes for the given chunk, using the geometryDesc (see NxDestructibleGeometryDesc).
		Returns true iff successful.
	*/
	virtual bool							rebuildCollisionGeometry(physx::PxU32 partIndex, const NxDestructibleGeometryDesc& geometryDesc) = 0;

	/**
		Trim collision geometry to prevent explosive behavior.  maxTrimFraction is the maximum (relative) distance to trim a hull in the direction
		of each trim plane.
		Returns true iff successful.
	*/
	virtual void							trimCollisionGeometry(const physx::PxU32* partIndices, physx::PxU32 partIndexCount, physx::PxF32 maxTrimFraction = 0.2f) = 0;

	/**
		Returns stats (sizes, counts) for the asset.  See NxDestructibleAssetStats.
	*/
	virtual void							getStats(NxDestructibleAssetStats& stats) const = 0;

	/**
	\brief Apply an arbitrary transformation to destructible asset

	This is a permanent transformation and it changes the object state. Should only be called immediately before serialization
	and without further modifying the object later on.

	\param transformation	This matrix is allowed to contain a translation and a rotation
	\param scale			Apply a uniform scaling as well
	*/
	virtual void							applyTransformation(const physx::PxMat44& transformation, physx::PxF32 scale) = 0;

	/**
	\brief Set a maximum fracture depth for a given platform string

	Returns true if the supplied maxDepth is lesser than the number of chunk depth levels for this asset
	*/
	virtual bool                            setPlatformMaxDepth(NxPlatformTag platform, physx::PxU32 maxDepth) = 0;

	/**
	\brief Removes the maximum fracture depth limit for a given platform string

	Returns true if the platform's maximum fracture depth was previously set and now removed
	*/
	virtual bool                            removePlatformMaxDepth(NxPlatformTag platform) = 0;

	/**
	\brief Returns the size of the actor transform array.  See getActorTransforms() for a description of this data.
	*/
	virtual physx::PxU32					getActorTransformCount() const = 0;

	/**
	\brief Returns the head of the actor transform array.  This list is a convenience for placing actors in a level from poses authored in a level editor.
	The transforms may contain scaling.
	*/
	virtual const physx::PxMat44*			getActorTransforms() const = 0;

	/**
	\brief Append transforms to the actor transform list.  See getActorTransforms() for a description of this data.

	\param transforms		Head of an array of transforms
	\param transformCount	Size of transforms
	*/
	virtual void							appendActorTransforms(const physx::PxMat44* transforms, physx::PxU32 transformCount) = 0;

	/**
	\brief Clear the actor transform array.  See getActorTransforms() for a description of this data.
	*/
	virtual void							clearActorTransforms() = 0;
};

/**
	Destructible asset API.  Destructible actors are instanced from destructible assets.
*/
class NxDestructibleAsset : public NxApexAsset
{
public:
	enum ChunkFlags
	{
		ChunkEnvironmentallySupported =		(1 << 0),
		ChunkAndDescendentsDoNotFracture =	(1 << 1),
		ChunkDoesNotFracture =				(1 << 2),
		ChunkDoesNotCrumble =				(1 << 3)
	};

	/** Instancing */

	/**
		Instances the NxDestructibleAsset, creating an NxDestructibleActor, using the NxDestructibleActorDesc.
		See NxDestructibleActor and NxDestructibleActorDesc.  This asset will own the NxDestructibleActor,
		so that any NxDestructibleActor created by it will be released when this asset is released.
		You may also call the NxDestructibleActor's release() method at any time.
	*/
	virtual void							releaseDestructibleActor(NxDestructibleActor& actor) = 0;


	/** General */

	/**
		Create a destructible actor representing the destructible asset in a scene. 
		Unlike a call to createApexActor, here the created actor takes explicit ownership of the provided actorParams.
		This can represent either the destructible descriptor or previously serialized destructible state.
		Note: The client should not attempt to use the provided actorParams after calling this method.
	*/
	virtual NxDestructibleActor*			createDestructibleActor(::NxParameterized::Interface* actorParams, NxApexScene& apexScene) = 0;

	/**
		The NxDestructibleParameters which describe the default fracturing behavior for instanced
		NxDestructibleActors.
	*/
	virtual NxDestructibleParameters		getDestructibleParameters() const = 0;

	/**
		The parameters used for default destructible initialization.  See NxDestructibleInitParameters.
	*/
	virtual NxDestructibleInitParameters	getDestructibleInitParameters() const = 0;

	/**
		The name of the emitter to use when generating crumble particles.
		Returns NULL if no emitter is configured.
	*/
	virtual const char*						getCrumbleEmitterName() const = 0;

	/**
		The name of the emitter to use when generating fracture-line dust particles.
		Returns NULL if no emitter is configured.
	*/
	virtual const char*						getDustEmitterName() const = 0;

	/**
		The total number of chunks in the asset.
	*/
	virtual physx::PxU32					getChunkCount() const = 0;

	/**
		The total number of fracture hierarchy depth levels in the asset.
	*/
	virtual physx::PxU32					getDepthCount() const = 0;

	/**
		Gets the NxRenderMeshAsset associated with this asset.
	*/
	virtual const NxRenderMeshAsset*		getRenderMeshAsset() const = 0;

	/**
		Set the NxRenderMeshAsset associated with this asset.
		This is the asset used for non-instanced rendering.
		NULL is a valid argument, and can be used to clear the internal mesh data.
		Returns true if successful.
	*/
	virtual bool							setRenderMeshAsset(NxRenderMeshAsset*) = 0;

	/**
		Get the number of instanced chunk meshes in this asset.
	*/
	virtual physx::PxU32					getInstancedChunkCount() const = 0;

	/**
		Returns stats (sizes, counts) for the asset.  See NxDestructibleAssetStats.
	*/
	virtual void							getStats(NxDestructibleAssetStats& stats) const = 0;

	/**
		Ensures that the asset has chunk overlap informationc cached up to the given depth.
		If depth < 0 (as it is by default), the depth will be taken to be the supportDepth
		given in the asset's destructibleParameters.
		It is ok to pass in a depth greater than any chunk depth in the asset.
	*/
	virtual void							cacheChunkOverlapsUpToDepth(physx::PxI32 depth = -1) = 0;

	/**
		The size of the array returned by getCachedOverlapsAtDepth(depth) (see below).
		Note: this function will not trigger overlap caching for the given depth.  If no overlaps
		have been calculated for the depth given, this function returns NULL.
	*/
	virtual physx::PxU32					getCachedOverlapCountAtDepth(physx::PxU32 depth) = 0;

	/**
		Array of integer pairs, indexing chunk pairs which touch at a given depth in the heirarcy.
		The size of the array is given by getCachedOverlapCountAtDepth(depth).
		Note: this function will not trigger overlap caching for the given depth.  If no overlaps
		have been calculated for the depth given, this function returns NULL.
	*/
	virtual const NxIntPair*				getCachedOverlapsAtDepth(physx::PxU32 depth) = 0;

	/**
		If this chunk is instanced within the same asset, then this provides the instancing position offset.
		Otherwise, this function returns (0,0,0).
	*/
	virtual physx::PxVec3					getChunkPositionOffset(physx::PxU32 chunkIndex) const = 0;

	/**
		If this chunk is instanced within the same asset, then this provides the instancing UV offset.
		Otherwise, this function returns (0,0).
	*/
	virtual physx::PxVec2					getChunkUVOffset(physx::PxU32 chunkIndex) const = 0;

	/**
		Retrieve flags (see ChunkFlags) for the given chunk.
	*/
	virtual physx::PxU32					getChunkFlags(physx::PxU32 chunkIndex) const = 0;

	/** 
		Returns the index of the given chunk's parent.  If the chunk has no parent (is the root of the fracture hierarchy),
		then -1 is returned.
	*/
	virtual physx::PxI32					getChunkParentIndex(physx::PxU32 chunkIndex) const = 0;

	/**
		The render mesh asset part index associated with this chunk.
	*/
	virtual physx::PxU32					getPartIndex(physx::PxU32 chunkIndex) const = 0;

	/**
		The number of convex hulls associated with a given part.
	*/
	virtual physx::PxU32					getPartConvexHullCount(const physx::PxU32 partIndex) const = 0;

	/**
		Returns the head of an array to convex hull NxParamterized::Interface* pointers for a given part.
	*/
	virtual NxParameterized::Interface**	getPartConvexHullArray(const physx::PxU32 partIndex) const = 0;

	/**
	\brief Returns the size of the actor transform array.  See getActorTransforms() for a description of this data.
	*/
	virtual physx::PxU32					getActorTransformCount() const = 0;

	/**
	\brief Returns the head of the actor transform array.  This list is a convenience for placing actors in a level from poses authored in a level editor.
	The transforms may contain scaling.
	*/
	virtual const physx::PxMat44*			getActorTransforms() const = 0;

protected:
	/** Hidden destructor.  Use release(). */
	virtual									~NxDestructibleAsset() {}
};


#pragma warning(pop)
PX_POP_PACK

}
} // end namespace physx::apex

#endif // NX_DESTRUCTIBLE_ASSET_H
