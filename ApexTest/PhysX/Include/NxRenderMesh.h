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

#ifndef NX_RENDER_MESH_H
#define NX_RENDER_MESH_H

/*!
\file
\brief classes NxRenderSubmesh, NxVertexBuffer, and NxMaterialNamingConvention enums
*/

#include "NxApexUsingNamespace.h"
#include "NxVertexFormat.h"

namespace physx
{
namespace apex
{

PX_PUSH_PACK_DEFAULT

class NxRenderMeshActor;
class NxApexContext;
struct NxVertexUV;


/**
\brief a vertex buffer that supports multiple formats
 */
class NxVertexBuffer
{
public:
	/**
	\brief Returns the number of vertices in the buffer
	*/
	virtual physx::PxU32			getVertexCount() const = 0;

	/**
	\brief Returns the data format.  See NxVertexFormat.
	*/
	virtual const NxVertexFormat&	getFormat() const = 0;

	/**
	\brief Returns the data format.  See NxVertexFormat. Can be changed.
	*/
	virtual NxVertexFormat&			getFormatWritable() = 0;

	/**
	\brief Accessor for the data buffer indexed by bufferIndex. To get the buffer format, use getFormat().getBufferFormat( index ).
	If the data channel doesn't exist then this function returns NULL.
	*/
	virtual const void*				getBuffer(physx::PxU32 bufferIndex) const = 0;

	/**
	\brief Like getBuffer(), but also returns the buffer's format.
	*/
	virtual const void*				getBufferAndFormat(NxRenderDataFormat::Enum& format, physx::PxU32 bufferIndex) const = 0;

	/**
	\brief Like getBuffer(), but also returns the buffer's format. Can be changed.
	*/
	virtual void*					getBufferAndFormatWritable(NxRenderDataFormat::Enum& format, physx::PxU32 bufferIndex) = 0;

	/**
	\brief Accessor for data in a desired format from the buffer indexed by bufferIndex. If the channel does not exist, or if it is in
	a format for which there is not presently a converter to the the desired format dstBufferFormat, this function returns false.
	The dstBufferStride field must be at least the size of the dstBufferFormat data, or zero (in which case the stride is assumed to be
	the size of the dstBufferFormat data).  If neither of these conditions hold, this function returns false.
	Otherwise, dstBuffer is filled in with elementCount elements of the converted data, starting from startVertexIndex, withe the given stride.
	*/
	virtual bool					getBufferData(void* dstBuffer, physx::NxRenderDataFormat::Enum dstBufferFormat, physx::PxU32 dstBufferStride, physx::PxU32 bufferIndex,
	        physx::PxU32 startVertexIndex, physx::PxU32 elementCount) const = 0;

protected:
	/* Do not allow class to be created directly */
	NxVertexBuffer() {}
};


/**
\brief a mesh that has only one material (or render state, in general)
 */
class NxRenderSubmesh
{
public:
	virtual							~NxRenderSubmesh() {}

	/**
		Returns the number of vertices associated with the indexed part.
	*/
	virtual physx::PxU32			getVertexCount(physx::PxU32 partIndex) const = 0;

	/**
		Returns the submesh's index buffer (contains all parts' vertices)
	*/
	virtual const NxVertexBuffer&	getVertexBuffer() const = 0;

	/**
		Returns the submesh's index buffer (contains all parts' vertices). Can be changed.
	*/
	virtual NxVertexBuffer&			getVertexBufferWritable() = 0;

	/**
		Vertices for a given part are contiguous within the vertex buffer.  This function
		returns the first vertex index for the indexed part.
	*/
	virtual physx::PxU32			getFirstVertexIndex(physx::PxU32 partIndex) const = 0;

	/**
		Returns the number of indices in the part's index buffer.
	*/
	virtual physx::PxU32			getIndexCount(physx::PxU32 partIndex) const = 0;

	/**
		Returns the index buffer associated with the indexed part.
	*/
	virtual const physx::PxU32*		getIndexBuffer(physx::PxU32 partIndex) const = 0;

	/**
		Creates a smoothing groups list in the groups array given.
		Every triangle defined by this submesh's index buffer will be assigned a smoothing group index,
		from a contiguous set of indices starting with 0.  Any two triangles which share an edge with
		common vertex buffer indices, with the same winding, will have the same smoothing group index.
		It is assumed that the memory buffer pointed to by groupsArray holds at least
		(groupsArraySize-1)*groupsArrayByteStride + 4 bytes.
		Returns the number of elements written into the groupsArray buffer, which will be no more than
		groupsArraySize.  If the number of triangle in this submesh is less than groupsArraySize, then
		the number of triangles will be returned.
	*/
	virtual physx::PxU32			createTriangleSmoothingGroups(physx::PxU32* groupsArray, physx::PxU32 groupsArraySize, physx::PxU32 groupsArrayByteStride) const = 0;

protected:
	NxRenderSubmesh() {}
};

PX_POP_PACK

}
} // end namespace physx::apex

#endif // NX_RENDER_MESH_H
