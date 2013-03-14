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

#ifndef NX_APEX_ASSET_H
#define NX_APEX_ASSET_H

/*!
\file
\brief class NxApexAsset
*/

#include "NxApexInterface.h"
#include "NxApexSDK.h"

namespace physx
{
namespace apex
{

PX_PUSH_PACK_DEFAULT

/// Usual typedef
typedef const char* NxPlatformTag;

class NxApexActor;
class NxApexAssetPreview;

/**
\brief Base class of all APEX assets
*/
class NxApexAsset : public NxApexInterface
{
public:
	/**
	 * \brief Returns the name of this deserialized asset
	 */
	virtual const char* getName() const = 0;

	/**
	 * \brief Returns the ID of the asset's authorable object type.
	 *
	 * Every asset will correspond to an APEX authorable object type.
	 * The module must register those types with the SDK at startup.
	 */
	virtual NxAuthObjTypeID getObjTypeID() const = 0;

	/**
	 * \brief Returns the name of this asset's authorable object type
	 */
	virtual const char* getObjTypeName() const = 0;

	/**
	 * \brief Returns the number of assets force loaded by all of
	 * this asset's named asset references
	 */
	virtual physx::PxU32 forceLoadAssets() = 0;

	/**
	 * \brief Returns the default actor descriptor NxParamaterized interface
	 * Memory ownership stays with this asset.  The user may modify the interface values, but they will not persist past another acll to 'getDefaultActorDesc'
	 * Typically used to create an actor after making small local editing changes.
	 */
	virtual ::NxParameterized::Interface* getDefaultActorDesc() = 0;

	/**
	 * \brief Returns the default AssetPreview descriptor NxParamaterized interface
	 */
	virtual ::NxParameterized::Interface* getDefaultAssetPreviewDesc() = 0;

	/**
	 * \brief Returns the asset's NxParamaterized interface
	 * This cannot be directly modified!  It is read only to the user.
	 */
	virtual const ::NxParameterized::Interface* getAssetNxParameterized() const = 0;

	/**
	 * \brief Creates an Actor representing the Asset in a Scene
	 */
	virtual NxApexActor* createApexActor(const ::NxParameterized::Interface& actorParams, NxApexScene& apexScene) = 0;

	/**
	 * \brief Creates an Asset Preview for the asset.
	 */
	virtual NxApexAssetPreview* createApexAssetPreview(const ::NxParameterized::Interface& params) = 0;

	/**
	 * \brief Releases the ApexAsset but returns the NxParameterized::Interface and *ownership* to the caller.
	 */
	virtual NxParameterized::Interface* releaseAndReturnNxParameterizedInterface(void) = 0;

	/**
	 * \brief Returns true if the asset is in a state that is valid for creating an actor.
	 * \param actorParams parameters of actor to create
	 * \returns true if call to createApexActor will return true on inputs, false otherwise
	 */
	virtual bool isValidForActorCreation(const ::NxParameterized::Interface& actorParams, NxApexScene& /*apexScene*/) const = 0;

	/**
	 * \brief Returns true if the parameterized object of the asset has been modified.
	 *
	 * This flag will be reset once the parameterized object has been serialized again.
	 */
	virtual bool isDirty() const = 0;

protected:
	virtual ~NxApexAsset() {}; // illegal, do not call
};

/**
\brief base class of all APEX asset authoring classes
*/
class NxApexAssetAuthoring : public NxApexInterface
{
public:
	/**
	 * \brief Returns the name of asset author
	 */
	virtual const char* getName() const = 0;

	/**
	 * \brief Returns the name of this APEX authorable object type
	 */
	virtual const char* getObjTypeName() const = 0;

	/**
	 * \brief Prepares a fully authored Asset Authoring object for a specified platform
	 */
	virtual bool prepareForPlatform(physx::apex::NxPlatformTag) = 0;

	/**
	 * \brief Returns the asset's NxParameterized interface, may return NULL
	 */
	virtual ::NxParameterized::Interface* getNxParameterized() const = 0;

	/**
	 * \brief Releases the ApexAsset but returns the NxParameterized::Interface and *ownership* to the caller.
	 */
	virtual NxParameterized::Interface* releaseAndReturnNxParameterizedInterface(void) = 0;

	/**
	 * \brief Generates a string that is stored in the asset with all the relevant information about the build
	 *
	 * \param toolName			The name of the tool with proper casing, i.e. "Clothing Tool".
	 * \param toolVersion		The version of the tool as a string, can be NULL.
	 * \param toolChangelist	The CL# of the tool, will use internal tools directory CL if 0
	 */
	virtual void setToolString(const char* toolName, const char* toolVersion, PxU32 toolChangelist) = 0;
};

PX_POP_PACK

}
} // end namespace physx::apex

#endif // NX_APEX_ASSET_H
