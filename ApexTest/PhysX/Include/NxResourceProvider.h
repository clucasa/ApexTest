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

#ifndef NX_RESOURCE_PROVIDER_H
#define NX_RESOURCE_PROVIDER_H

/*!
\file
\brief class NxResourceProvider
*/

#include "NxApexUsingNamespace.h"

namespace physx
{
namespace apex
{

class NxResourceCallback;

PX_PUSH_PACK_DEFAULT

/*!
\brief A user provided class for mapping names to pointers or integers

Named resource provider - a name-to-pointer utility.  User must provide the pointed-to data.
				        - also supports name-to-integer
*/

class NxResourceProvider
{
public:
	/**
	\brief Register a callback

	Register a callback function for unresolved named resources.  This function will be called by APEX when
	an unresolved named resource is requested.  The function will be called at most once for each named
	resource.  The function must directly return the pointer to the resource or NULL.
	*/
	virtual void registerCallback(NxResourceCallback* impl) = 0;

	/**
	\brief Provide the pointer for the specified named resource
	*/
	virtual void setResource(const char* nameSpace, const char* name, void* resource, bool incRefCount = false) = 0;


	/**
	\brief Provide the unsigned integer for the specified named resource
	*/
	virtual void   	setResourceU32(const char* nameSpace, const char* name, physx::PxU32 id, bool incRefCount = false) = 0;

	/**
	\brief Retrieve the pointer to the specified named resource.

	The game application (level loader) may use this to find assets by
	name, as a convenience.  If the named resource has not yet been
	loaded it will trigger a call to NxResourceCallback::requestResource(),
	assuming an NxResourceCallback instance was registered with the APEX SDK.
	*/
	virtual void* getResource(const char* nameSpace, const char* name) = 0;

	/**
	\brief Releases all resources in this namespace.
	\return the total number of resources released.
	*/
	virtual physx::PxU32  	releaseAllResourcesInNamespace(const char* nameSpace) = 0;

	/**
	\brief Releases a single resource.
	\return the outstanding referernce count after the release is performed.
	*/
	virtual physx::PxU32  	releaseResource(const char* nameSpace, const char* name) = 0;

	/**
	\brief Reports if a current resource exists and, if so, the reference count.
	*/
	virtual bool    findRefCount(const char* nameSpace, const char* name, physx::PxU32& refCount) = 0;

	/**
	\brief Locates an existing resource

	This function will *not* call back to the application if the resource does not exist.
	Only reports currently set resources.
	*/
	virtual void* 	findResource(const char* nameSpace, const char* name) = 0;

	/**
	\brief Locates an existing integer resource.
	*/
	virtual physx::PxU32 	findResourceU32(const char* nameSpace, const char* name) = 0;

	/**
	\brief Returns a list of all resources in a particular namespace.
	*/
	virtual void** findAllResources(const char* nameSpace, physx::PxU32& count) = 0;

	/**
	\brief Returns a list of the names of all resources within a particular namespace
	*/
	virtual const char** findAllResourceNames(const char* nameSpace, physx::PxU32& count) = 0;

	/**
	\brief Returns a list of all registered namespaces.
	*/
	virtual const char** findNameSpaces(physx::PxU32& count) = 0;

	/**
	\brief Write contents of resource table to error stream.
	*/
	virtual void dumpResourceTable() = 0;
};

PX_POP_PACK

}
} // end namespace physx::apex

#endif // NX_RESOURCE_PROVIDER_H
