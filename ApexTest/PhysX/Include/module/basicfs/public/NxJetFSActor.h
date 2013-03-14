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

#ifndef NX_JET_FSACTOR_H
#define NX_JET_FSACTOR_H

#include "NxApex.h"

namespace physx
{
namespace apex
{

PX_PUSH_PACK_DEFAULT

class NxBasicFSAsset;


/**
 \brief JetFS Actor class
 */
class NxJetFSActor : public NxApexActor, public NxApexRenderable
{
protected:
	virtual ~NxJetFSActor() {}

public:
	/**
	\brief Returns the asset the instance has been created from.
	*/
	virtual NxBasicFSAsset* 	getJetFSAsset() const = 0;


	/**
	\brief Gets the curent pose of the actor
	*/
	virtual physx::PxMat44		getCurrentPose() const = 0;
	/**
	\brief Sets the curent pose of the actor
	*/
	virtual void				setCurrentPose(const physx::PxMat44& pose) = 0;
	/**
	\brief Gets the curent position of the actor
	*/
	virtual physx::PxVec3		getCurrentPosition() const = 0;
	/**
	\brief Sets the curent position of the actor
	*/
	virtual void				setCurrentPosition(const physx::PxVec3& pos) = 0;

	/**
	\brief Gets the curent scale of the actor
	*/
	virtual physx::PxF32		getCurrentScale() const = 0;
	/**
	\brief Sets the curent scale of the actor
	*/
	virtual void				setCurrentScale(const physx::PxF32& scale) = 0;

	/**
	\brief Sets the field strength
	*/
	virtual void				setFieldStrength(physx::PxF32) = 0;
	/**
	\brief Sets the field direction
	*/
	virtual void				setFieldDirection(const physx::PxVec3&) = 0;

	/**
	\brief Enable/Disable the field simulation
	*/
	virtual void				setEnabled(bool isEnabled) = 0;

};

PX_POP_PACK

}
} // end namespace physx::apex

#endif // NX_Jet_FSACTOR_H
