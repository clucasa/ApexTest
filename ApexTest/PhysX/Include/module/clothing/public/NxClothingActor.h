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

#ifndef NX_CLOTHING_ACTOR_H
#define NX_CLOTHING_ACTOR_H

#include "NxApexActor.h"
#include "NxApexRenderable.h"
#include "NxClothingAsset.h"

namespace NxParameterized
{
class Interface;
}

namespace physx
{
namespace apex
{

PX_PUSH_PACK_DEFAULT

class NxClothingUserRecompute;
class NxClothingVelocityCallback;
class NxClothingPlane;
class NxClothingConvex;
class NxClothingSphere;
class NxClothingCapsule;
class NxClothingTriangleMesh;


/**
\brief selects the mode the clothing actor will be in the simulation frame
*/
struct ClothingTeleportMode
{
	/** \brief the enum type */
	enum Enum
	{
		/**
		\brief Simulation continues smoothly. This is the most commonly used mode
		*/
		Continuous,

		/**
		\brief Transforms the current simulation state from the old global pose to the new global pose.

		This will transform positions and velocities and thus keep the simulation state, just translate it to a new pose.
		*/
		Teleport,

		/**
		\brief Forces the cloth to the animated position in the next frame.

		This can be used to reset it from a bad state or by a teleport where the old state is not important anymore.
		*/
		TeleportAndReset,
	};
};



/**
\brief Instance of NxClothingAsset. Can be positioned, animated, updated and rendered.
*/
class NxClothingActor : public NxApexActor, public NxApexRenderable
{
protected:
	virtual ~NxClothingActor() {}
public:

	/**
	\brief Returns a reference to the actor descriptor as it is held by the ClothingActor.

	This descriptor can be modified at any time, changes will only be read during the first part of the simulation.
	*/
	virtual ::NxParameterized::Interface* getActorDesc() = 0;

	/**
	\brief Updates all internal bone matrices. This should be called with updated information before apex scenes start simulating.

	\param[in] globalPose				The new location of the actor
	\param[in] newBoneMatrices			Pointer to the array of transformations that contain the composite bone transformations for the
										current frame
	\param[in] boneMatricesByteStride	stride of the bone matrices, must be bigger than sizeof(physx::PxMat44)
	\param[in] numBoneMatrices			number of bone matrices available. This should correspond with the number of bones present in the asset
	\param[in] teleportMode				Setting this to anything but TM_Continuous will force apply a teleport and optionally a reset.

	\note This must be called before NxApexScene::simulate is called
	*/
	virtual void updateState(const physx::PxMat44& globalPose, const physx::PxMat44* newBoneMatrices, physx::PxU32 boneMatricesByteStride, physx::PxU32 numBoneMatrices, ClothingTeleportMode::Enum teleportMode) = 0;

	/**
	\brief Change the max distance of all active vertices with a scalar parameter

	\param[in] scale         Must be in [0,1] range
	\param[in] multipliable  Setting to define how the scale is applied. True will mutliply the scale on top of the max distance, False will subtract the maximum max distance.
	*/
	virtual void updateMaxDistanceScale(physx::PxF32 scale, bool multipliable) = 0;

	/**
	\brief returns the globalPose that was set with NxClothingActor::updateState()
	*/
	virtual const physx::PxMat44& getGlobalPose() const = 0;

	/**
	\brief Sets the wind strength and direction, can be called any time

	\deprecated Use NxClothingActor::getActorDesc() and modify the wind part of it

	\param[in] windAdaption The rate of adaption. The higher this value, the faster the cloth reaches the wind velocity. Set to 0 to turn off wind
	\param[in] windVelocity The target velocity each vertex tries to achieve.

	\note It is safe to call this even during simulation, but it will only have an effect after the next call to NxApexScene::simulate()
	*/
	virtual void setWind(physx::PxF32 windAdaption, const physx::PxVec3& windVelocity) = 0;

	/**
	\brief \b DEPRECATED Time in seconds how long it takes to go from 0 maxDistance to full maxDistance

	\deprecated Use NxClothingActor::getActorDesc() and modify the maxDistanceBlendTime part of it
	<b>Default:</b> 1.0
	\note This also influences how quickly different physical LoDs can be switched
	\note It is safe to call this even during simulation.
	*/
	virtual void setMaxDistanceBlendTime(physx::PxF32 blendTime) = 0;

	/**
	\brief \b DEPRECATED Time in seconds how long it takes to go from 0 maxDistance to full maxDistance

	\deprecated Use NxClothingActor::getActorDesc() and read the maxDistanceBlendTime part of it
	*/
	virtual physx::PxF32 getMaxDistanceBlendTime() const = 0;

	/**
	\brief Tells the actor if it will be rendered or not.
	If an actor is simulated, but not rendered, some computations (skinning, normal and tangent calculation)
	doesn't need to be done.

	disabling is set immediately, disabling is buffered for the next frame.
	*/
	virtual void setVisible(bool enable) = 0;

	/**
	\brief Returns the current visibility setting.
	The most recently set value is returned (i.e. the buffered value, not the actual value).
	*/
	virtual bool isVisible() const = 0;

	/**
	\brief Stops simulating the actor.
	*/
	virtual void setFrozen(bool enable) = 0;

	/**
	\brief Returns if the simulation is currently stopped for this actor.
	*/
	virtual bool isFrozen() const = 0;

	/**
	\brief Returns whether the actor is simulated using the 2.8.x or the 3.x cloth solver.
	*/
	virtual ClothSolverMode::Enum getClothSolverMode() const = 0;

	/**
	\brief \b DEPRECATED Sets LOD parameters that control how the distance from the eye position should influence the benefit of an object.

	The LOD benefit function for cloth is:

	benefit = (distanceWeight * max(0, maxDistance - distanceFromEye) + bias) * simulationCost + benefitBias

	\param [in] maxDistance		the benefit of an actor should fall off linearly with eye-distance from zero to this maxDistance.
	\param [in] distanceWeight	the weight of the distance-benefit in the benefit function
	\param [in] bias			bias of the distance formula
	\param [in] benefitBias		bias of the overall benefit

	\deprecated Use NxClothingActor::getActorDesc() and modify the lodWeights part of it
	\note It is safe to call this even during simulation
	*/
	virtual void setLODWeights(physx::PxF32 maxDistance, physx::PxF32 distanceWeight, physx::PxF32 bias, physx::PxF32 benefitBias) = 0;

	/**
	\brief sets the graphical Lod
	This chooses the graphical mesh of all the meshes stored in the asset to be rendered. It has to be set
	before the simulate call to take effect for the next time the actor is rendered. Otherwise, the
	given value will be buffered and used as soon as possible.

	\param [in] lod		lod used to render the mesh

	\note It is safe to call this even during simulation
	*/
	virtual void setGraphicalLOD(physx::PxU32 lod) = 0;

	/**
	\brief returns the graphical Lod
	This returns the buffered most recently set graphical lod, even if it's not active yet.
	*/
	virtual PxU32 getGraphicalLod() = 0;

	/**
	\brief Raycasts against the NxClothingActor

	\param [in] worldOrigin		The world ray's origin
	\param [in] worldDirection	The world ray's direction, needs not to be normalized
	\param [out] time			Impact time
	\param [out] normal			Impact normal in world space
	\param [out] vertexIndex	Vertex index that was hit

	\return true if this actor is hit
	*/
	virtual bool rayCast(const PxVec3& worldOrigin, const PxVec3& worldDirection, physx::PxF32& time, physx::PxVec3& normal, physx::PxU32& vertexIndex) = 0;

	/**
	\brief Attach a vertex to a global position
	*/
	virtual void attachVertexToGlobalPosition(PxU32 vertexIndex, const PxVec3& globalPosition) = 0;

	/**
	\brief Free a previously attached vertex
	*/
	virtual void freeVertex(PxU32 vertexIndex) = 0;

	/**
	\brief Returns the actively selected material.
	*/
	virtual physx::PxU32 getClothingMaterial() const = 0;

	/**
	\brief Sets which clothing material is used from the assets library
	*/
	virtual void setClothingMaterial(physx::PxU32 index) = 0;

	/**
	\brief Sets the override material for the submesh with the given index.
	*/
	virtual void setOverrideMaterial(PxU32 submeshIndex, const char* overrideMaterialName) = 0;

	/**
	\brief sets the recompute callback for an individual actor, can be NULL to use default.
	*/
	virtual void setUserRecompute(NxClothingUserRecompute* recompute) = 0;

	/**
	\brief sets the velocity callback for an individual actor, turned off when NULL
	*/
	virtual void setVelocityCallback(NxClothingVelocityCallback* callback) = 0;

	/**
	\brief Returns the current position of all physics vertices.

	This method provides the positions of the currently simulated part of the physics mesh
	and skins the non-simulated part with the current bone positions.

	\note This should usually not be needed other than for rendering the physics mesh yourself
	\note Must be called in between NxApexScene::fetchResults and NxApexScene::simulate.
	*/
	virtual void getPhysicalMeshPositions(void* buffer, physx::PxU32 byteStride) = 0;

	/**
	\brief Returns the current normals of all physics vertices.

	This method provides the normals of the currently simulated part of the physics mesh
	and skins the non-simulated part with the current bone positions.

	\note This should usually not be needed other than for rendering the physics mesh yourself
	\note Must be called in between NxApexScene::fetchResults and NxApexScene::simulate.
	*/
	virtual void getPhysicalMeshNormals(void* buffer, physx::PxU32 byteStride) = 0;

	/**
	\brief Returns how much an Actor will cost at maximum
	*/
	virtual physx::PxF32 getMaximumSimulationBudget() const = 0;

	/**
	\brief Returns the number of currently simulation vertices.

	\note	This is not to be used for getSimulationVertices.
			getNumSimulatedVertices returns the number
			of actually simulated verts, while getSimulationVertices returns the complete
			physical mesh, regardless of the current physical lod
	*/
	virtual physx::PxU32 getNumSimulationVertices() const = 0;

	
	/**
	\brief Returns a pointer to the internal positions array of the simulated physics mesh

	\return											Pointer the simulation mesh positions
	*/
	virtual const physx::PxVec3* getSimulationPositions() const = 0;

	/**
	\brief Returns a pointer to the internal normals array of the simulated physics mesh

	\return											Pointer the simulation mesh normals
	*/
	virtual const physx::PxVec3* getSimulationNormals() const = 0;

	/**
	\brief Returns the number of the graphical vertices that need to be skinned to the simulation mesh [0, numGraphicalVerticesActive)
	
	The rest must be regularly skinned to bones [numGraphicalVerticesActive, numVertices)

	\note Only valid after simulate.
	*/
	virtual PxU32 getNumGraphicalVerticesActive(PxU32 submeshIndex) const = 0;

	/**
	\brief Returns the transform that needs to be applied to the rendered mesh.

	\note Only valid after simulate.
	*/
	virtual PxMat44 getRenderGlobalPose() const = 0;

	/**
	\brief Returns the current skinning matrices.
	The skinning matrices already contain the global pose of the actor.

	\note Only valid after simulate.
	*/
	virtual const PxMat44* getCurrentBoneSkinningMatrices() const = 0;


	/**
	\brief Create a collision plane for the actor.
	*/
	virtual NxClothingPlane* createCollisionPlane(const PxPlane& plane) = 0;

	/**
	\brief Create a collision convex for the actor, defined by planes.
	*/
	virtual NxClothingConvex* createCollisionConvex(NxClothingPlane** planes, PxU32 numPlanes) = 0;

	/**
	\brief Create a collision sphere for the actor.
	*/
	virtual NxClothingSphere* createCollisionSphere(const PxVec3& position, PxF32 radius) = 0;

	/**
	\brief Create a tapered collision capsule for the actor, defined by two spheres.
	*/
	virtual NxClothingCapsule* createCollisionCapsule(NxClothingSphere& sphere1, NxClothingSphere& sphere2) = 0;

	/**
	\brief Create a collision triangle mesh for the actor, defined by two spheres.
	*/
	virtual NxClothingTriangleMesh* createCollisionTriangleMesh() = 0;
};

PX_POP_PACK

}
} // namespace physx::apex

#endif // NX_CLOTHING_ACTOR_H
