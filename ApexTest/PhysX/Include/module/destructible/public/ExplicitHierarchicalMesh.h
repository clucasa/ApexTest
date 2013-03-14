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

#ifndef EXPLICIT_HIERARCHICAL_MESH_H
#define EXPLICIT_HIERARCHICAL_MESH_H

#include "foundation/Px.h"
#include "NxApexUserProgress.h"
#include "NxRenderMeshAsset.h"
#include "NxApexUtils.h"
#include "foundation/PxPlane.h"

namespace physx
{
namespace apex
{

PX_PUSH_PACK_DEFAULT

/**
	NxExplicitVertexFormat

	This is used when authoring an NxVertexBuffer, to define which data channels exist.
 */
struct NxExplicitVertexFormat
{
	/** This value defines which vertex winding orders will be rendered.  See NxRenderCullMode. */
	physx::PxU32		mWinding;

	/** Whether or not the accompanying vertex data has defined static vertex positions. */
	bool				mHasStaticPositions;

	/** Whether or not the accompanying vertex data has defined static vertex normals. */
	bool				mHasStaticNormals;

	/** Whether or not the accompanying vertex data has defined static vertex tangents. */
	bool				mHasStaticTangents;

	/** Whether or not the accompanying vertex data has defined static vertex binormals. */
	bool				mHasStaticBinormals;

	/** Whether or not the accompanying vertex data has defined static vertex colors. */
	bool				mHasStaticColors;

	/** Whether or not to create separate render resource for a static bone index buffer. */
	bool				mHasStaticSeparateBoneBuffer;

	/** Whether or not the accompanying vertex data has defined dynamic displacement coordinates */
	bool				mHasStaticDisplacements;

	/** Whether or not the accompanying vertex data has defined dynamic vertex positions. */
	bool				mHasDynamicPositions;

	/** Whether or not the accompanying vertex data has defined dynamic vertex normals. */
	bool				mHasDynamicNormals;

	/** Whether or not the accompanying vertex data has defined dynamic vertex tangents. */
	bool				mHasDynamicTangents;

	/** Whether or not the accompanying vertex data has defined dynamic vertex binormals. */
	bool				mHasDynamicBinormals;

	/** Whether or not the accompanying vertex data has defined dynamic vertex colors. */
	bool				mHasDynamicColors;

	/** Whether or not to create separate render resource for a dynamic bone index buffer. */
	bool				mHasDynamicSeparateBoneBuffer;

	/** Whether or not the accompanying vertex data has defined dynamic displacement coordinates */
	bool				mHasDynamicDisplacements;

	/** How many UV coordinate channels there are (per vertex) */
	physx::PxU32		mUVCount;

	/** How many bones may influence a vertex */
	physx::PxU32		mBonesPerVertex;

	/** Constructor, calls clear() to set formats to default settings */
	NxExplicitVertexFormat()
	{
		clear();
	}

	/**
		Set formats to default settings:

		mWinding = NxRenderCullMode::CLOCKWISE;
		mHasStaticPositions = false;
		mHasStaticNormals = false;
		mHasStaticTangents = false;
		mHasStaticBinormals = false;
		mHasStaticColors = false;
		mHasStaticSeparateBoneBuffer = false;
		mHasStaticDisplacements = false;
		mHasDynamicPositions = false;
		mHasDynamicNormals = false;
		mHasDynamicTangents = false;
		mHasDynamicBinormals = false;
		mHasDynamicColors = false;
		mHasDynamicSeparateBoneBuffer = false;
		mHasDynamicDisplacements = false;
		mUVCount = 0;
		mBonesPerVertex = 0;
	*/
	void	clear()
	{
		mWinding = NxRenderCullMode::CLOCKWISE;
		mHasStaticPositions = false;
		mHasStaticNormals = false;
		mHasStaticTangents = false;
		mHasStaticBinormals = false;
		mHasStaticColors = false;
		mHasStaticSeparateBoneBuffer = false;
		mHasStaticDisplacements = false;
		mHasDynamicPositions = false;
		mHasDynamicNormals = false;
		mHasDynamicTangents = false;
		mHasDynamicBinormals = false;
		mHasDynamicColors = false;
		mHasDynamicSeparateBoneBuffer = false;
		mHasDynamicDisplacements = false;
		mUVCount = 0;
		mBonesPerVertex = 0;
	}

	/**
		Equality operator.  All values are tested for equality except mBonesPerVertex.
	*/
	bool	operator == (const NxExplicitVertexFormat& data) const
	{
		if (mWinding != data.mWinding)
		{
			return false;
		}
		if (mHasStaticPositions != data.mHasStaticPositions ||
		        mHasStaticNormals != data.mHasStaticNormals ||
		        mHasStaticTangents != data.mHasStaticTangents ||
		        mHasStaticBinormals != data.mHasStaticBinormals ||
		        mHasStaticColors != data.mHasStaticColors ||
		        mHasStaticSeparateBoneBuffer != data.mHasStaticSeparateBoneBuffer ||
		        mHasStaticDisplacements != data.mHasStaticDisplacements)
		{
			return false;
		}
		if (mHasDynamicPositions != data.mHasDynamicPositions ||
		        mHasDynamicNormals != data.mHasDynamicNormals ||
		        mHasDynamicTangents != data.mHasDynamicTangents ||
		        mHasDynamicBinormals != data.mHasDynamicBinormals ||
		        mHasDynamicColors != data.mHasDynamicColors ||
		        mHasDynamicSeparateBoneBuffer != data.mHasDynamicSeparateBoneBuffer ||
		        mHasDynamicDisplacements != data.mHasDynamicDisplacements)
		{
			return false;
		}
		if (mUVCount != data.mUVCount)
		{
			return false;
		}
		return true;
	}

	/**
		Returns the logical complement of the == operator.
	*/
	bool	operator != (const NxExplicitVertexFormat& data) const
	{
		return !(*this == data);
	}

	/**
		Creates a render-ready NxVertexFormat corresponding to this structure's member values.
	*/
	void	copyToNxVertexFormat(NxVertexFormat* format) const
	{
		format->reset();
		physx::PxU32 bi;
		if (mHasStaticPositions)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::POSITION));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT3);
			format->setBufferAccess(bi, mHasDynamicPositions ? NxRenderDataAccess::DYNAMIC :  NxRenderDataAccess::STATIC);
		}
		if (mHasStaticNormals)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::NORMAL));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT3);
			format->setBufferAccess(bi, mHasDynamicNormals ? NxRenderDataAccess::DYNAMIC :  NxRenderDataAccess::STATIC);
		}
		if (mHasStaticTangents)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::TANGENT));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT3);
			format->setBufferAccess(bi, mHasDynamicTangents ? NxRenderDataAccess::DYNAMIC :  NxRenderDataAccess::STATIC);
		}
		if (mHasStaticBinormals)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BINORMAL));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT3);
			format->setBufferAccess(bi, mHasDynamicBinormals ? NxRenderDataAccess::DYNAMIC :  NxRenderDataAccess::STATIC);
		}
		if (mHasStaticDisplacements)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::DISPLACEMENT_TEXCOORD));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT3);
			format->setBufferAccess(bi, mHasDynamicDisplacements ? NxRenderDataAccess::DYNAMIC : NxRenderDataAccess::STATIC);
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::DISPLACEMENT_FLAGS));
			format->setBufferFormat(bi, NxRenderDataFormat::UINT1);
			format->setBufferAccess(bi, mHasDynamicDisplacements ? NxRenderDataAccess::DYNAMIC : NxRenderDataAccess::STATIC);
		}
		if (mUVCount > 0)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::TEXCOORD0));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT2);
		}
		if (mUVCount > 1)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::TEXCOORD1));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT2);
		}
		if (mUVCount > 2)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::TEXCOORD2));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT2);
		}
		if (mUVCount > 3)
		{
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::TEXCOORD3));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT2);
		}
		switch (mBonesPerVertex)
		{
		case 1:
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_INDEX));
			format->setBufferFormat(bi, NxRenderDataFormat::USHORT1);
			break;
		case 2:
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_INDEX));
			format->setBufferFormat(bi, NxRenderDataFormat::USHORT2);
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_WEIGHT));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT2);
			break;
		case 3:
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_INDEX));
			format->setBufferFormat(bi, NxRenderDataFormat::USHORT3);
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_WEIGHT));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT3);
			break;
		case 4:
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_INDEX));
			format->setBufferFormat(bi, NxRenderDataFormat::USHORT4);
			bi = format->addBuffer(format->getSemanticName(NxRenderVertexSemantic::BONE_WEIGHT));
			format->setBufferFormat(bi, NxRenderDataFormat::FLOAT4);
			break;
		}

		format->setHasSeparateBoneBuffer(mHasStaticSeparateBoneBuffer);
		format->setWinding((NxRenderCullMode::Enum)mWinding);
	}
};


/**
	NxExplicitSubmeshData

	This is used when authoring an NxRenderMeshAsset.

	This is the per-submesh data: the material name, and vertex format.
 */
struct NxExplicitSubmeshData
{
	enum
	{
		MaterialNameBufferSize = 1024
	};

	char					mMaterialName[MaterialNameBufferSize];
	NxExplicitVertexFormat	mVertexFormat;

	bool	operator == (const NxExplicitSubmeshData& data) const
	{
		return !strcmp(mMaterialName, data.mMaterialName) && mVertexFormat == data.mVertexFormat;
	}

	bool	operator != (const NxExplicitSubmeshData& data) const
	{
		return !(*this == data);
	}
};


/**
	NxChunkAuthoringFlag

	Used to describe attributes of chunks (see IExplicitHierarchicalMesh::chunkFlags()).
*/
struct NxChunkAuthoringFlag
{
	enum Enum
	{
		IsCutoutFaceSplit =	(1U << 0),
		IsCutoutLeftover =	(1U << 1),

		Instance =			(1U << 31)
	};
};


/**
	Collision volume descriptor for a chunk
*/
struct NxCollisionVolumeDesc
{
	NxCollisionVolumeDesc()
	{
		setToDefault();
	}

	void setToDefault()
	{
		mHullMethod = NxConvexHullMethod::CONVEX_DECOMPOSITION;
		mConcavityPercent = 4.0f;
		mMergeThreshold = 4.0f;
		mRecursionDepth = 0;
	}

	/**
		How to generate convex hulls for a chunk.  See NxConvexHullMethod::Enum.
	*/
	NxConvexHullMethod::Enum	mHullMethod;

	/**
		Allowed concavity if mHullMethod = NxConvexHullMethod::CONVEX_DECOMPOSITION.
		Default = 4.0.
	*/
	physx::PxF32				mConcavityPercent;

	/**
		Merge threshold if mHullMethod = NxConvexHullMethod::CONVEX_DECOMPOSITION.
		Default = 4.0.
	*/
	physx::PxF32				mMergeThreshold;

	/**
		Recursion depth if mHullMethod = NxConvexHullMethod::CONVEX_DECOMPOSITION.
		Depth = 0 generates a single convex hull.  Higher recursion depths may generate
		more convex hulls to fit the mesh.
		Default = 0.
	*/
	physx::PxU32				mRecursionDepth;
};


/**
	Enumeration of current fracture methods.  Used when an authored mesh needs to know how it was created, for
	example if we need to re-apply UV mapping information.
*/
struct NxFractureMethod
{
	enum Enum
	{
		Unknown,
		Slice,
		Cutout,
		Voronoi,

		FractureMethodCount
	};
};


/**
	NxFractureMaterialDesc

	Descriptor for materials applied to interior faces.
*/
struct NxFractureMaterialDesc
{
	/** The UV scale (geometric distance/unit texture distance) for interior materials.
	Default = (1.0f,1.0f).
	*/
	physx::PxVec2	uvScale;

	/** A UV origin offset for interior materials.
	Default = (0.0f,0.0f).
	*/
	physx::PxVec2	uvOffset;

	/** World space vector specifying surface tangent direction.  If this vector
	is (0.0f,0.0f,0.0f), then an arbitrary direction will be chosen.
	Default = (0.0f,0.0f,0.0f).
	*/
	physx::PxVec3	tangent;

	/** Angle from tangent direction for the u coordinate axis.
	Default = 0.0f.
	*/
	physx::PxF32	uAngle;

	/**
		The submesh index to use for the newly-created triangles.
		If an invalid index is given, 0 will be used.
	*/
	physx::PxU32 interiorSubmeshIndex;

	/** Constructor sets defaults */
	NxFractureMaterialDesc()
	{
		setToDefault();
	}

	/**
		Set default values:
		uvScale = physx::PxVec2(1.0f);
		uvOffset = physx::PxVec2(0.0f);
		tangent = physx::PxVec3(0.0f);
		uAngle = 0.0f;
		interiorSubmeshIndex = 0;
	*/
	void	setToDefault()
	{
		uvScale = physx::PxVec2(1.0f);
		uvOffset = physx::PxVec2(0.0f);
		tangent = physx::PxVec3(0.0f);
		uAngle = 0.0f;
		interiorSubmeshIndex = 0;
	}
};


/**
	A reference frame for applying UV mapping to triangles.  Also includes the fracturing method and an index
	which is used internally for such operations as re-applying UV mapping information.
*/
struct NxMaterialFrame
{
	NxMaterialFrame() :
		mCoordinateSystem(physx::PxVec4(1.0f)),
		mUVPlane(physx::PxVec3(0.0f, 0.0f, 1.0f), 0.0f),
		mUVScale(1.0f),
		mUVOffset(0.0f),
		mFractureMethod(NxFractureMethod::Unknown),
		mFractureIndex(-1),
		mSliceDepth(0)
	{		
	}

	void	buildCoordinateSystemFromMaterialDesc(const physx::NxFractureMaterialDesc& materialDesc, const physx::PxPlane& plane)
			{
				physx::PxVec3 zAxis = plane.n;
				zAxis.normalize();
				physx::PxVec3 xAxis = materialDesc.tangent;
				physx::PxVec3 yAxis = zAxis.cross(xAxis);
				const physx::PxF32 l2 = yAxis.magnitudeSquared();
				if (l2 > PX_EPS_F32*PX_EPS_F32)
				{
					yAxis *= physx::PxRecipSqrt(l2);
				}
				else
				{
					physx::PxU32 maxDir =  physx::PxAbs(plane.n.x) > physx::PxAbs(plane.n.y) ?
						(physx::PxAbs(plane.n.x) > physx::PxAbs(plane.n.z) ? 0 : 2) :
						(physx::PxAbs(plane.n.y) > physx::PxAbs(plane.n.z) ? 1 : 2);
					xAxis = physx::PxMat33::createIdentity()[(maxDir + 1) % 3];
					yAxis = zAxis.cross(xAxis);
					yAxis.normalize();
				}
				xAxis = yAxis.cross(zAxis);

				const physx::PxF32 c = physx::PxCos(materialDesc.uAngle);
				const physx::PxF32 s = physx::PxSin(materialDesc.uAngle);

				mCoordinateSystem.column0 = physx::PxVec4(c*xAxis + s*yAxis, 0.0f);
				mCoordinateSystem.column1 = physx::PxVec4(c*yAxis - s*xAxis, 0.0f);
				mCoordinateSystem.column2 = physx::PxVec4(zAxis, 0.0f);
				mCoordinateSystem.setPosition(plane.project(physx::PxVec3(0.0f)));

				mUVPlane = plane;
				mUVScale = materialDesc.uvScale;
				mUVOffset = materialDesc.uvOffset;
			}

	physx::PxMat44	mCoordinateSystem;
	physx::PxPlane	mUVPlane;
	physx::PxVec2	mUVScale;
	physx::PxVec2	mUVOffset;
	physx::PxU32	mFractureMethod;
	physx::PxI32	mFractureIndex;
	physx::PxU32	mSliceDepth;	// The depth being created when this split is done.  mSliceDepth = 0 means "unknown"
};


/**
	Interface to a "displacement map volume," used with tessellated fracturing.
	A displacement map volume captures how to displace a particular point in 3D space
	along the x, y and z axes.  The data is stored as a 3D texture volume, with
	corresponding displacement coordinates acting as a look-up into this volume.
	X, Y and Z offsets correspond to R, G, and B color channels

	Various approaches can be used to generate the 3D noise field, in this case
	Perlin noise is used, with appropriate settings specified by the FractureSliceDesc.
*/
class IDisplacementMapVolume
{
public:
	/** 
		Fills the specified array and parameters with texture-compatible information. 

		The corresponding texture aligns with the displacement UVs generated as fracturing occurs
		when displacement maps are enabled, with RGB data corresponding to XYZ offsets, repectively.
	*/
	virtual void getData(physx::PxU32& width, physx::PxU32& height, physx::PxU32& depth, physx::PxU32& size, unsigned char const** ppData) const = 0;

	virtual ~IDisplacementMapVolume() { }
};


/**
	IExplicitHierarchicalMesh

	An interface to a representation of a render mesh that is used for authoring.

	The "hierarchical" nature of this mesh is represtented by extra parent/child relations
	among the parts that will become the parts of an NxRenderMeshAsset.
 */
class IExplicitHierarchicalMesh
{
public:
	/** Used in the visualize method to determine what to draw. */
	enum Enum
	{
		/**
			Draws the convex hulls associated with the BSP regions marked "outside," if a BSP has
			been created for this object (see calculateMeshBSP()).
		*/
		VisualizeMeshBSPOutsideRegions	= (1 << 0),

		/**
			Draws the convex hulls associated with the BSP regions marked "inside," if a BSP has
			been created for this object (see calculateMeshBSP()).
		*/
		VisualizeMeshBSPInsideRegions	= (1 << 1),

		/**
			Draws the convex hull associated with a single BSP regions, if a BSP has
			been created for this object (see calculateMeshBSP()).  The region index must
			be passed into the visualize function if this flag is set.
		*/
		VisualizeMeshBSPSingleRegion	= (1 << 8),

		/** Currently unused */
		VisualizeSliceBSPOutsideRegions	= (1 << 16),

		/** Currently unused */
		VisualizeSliceBSPInsideRegions	= (1 << 17),

		/** Currently unused */
		VisualizeSliceBSPSingleRegion	= (1 << 24),

		VisualizeMeshBSPAllRegions		= VisualizeMeshBSPOutsideRegions | VisualizeMeshBSPInsideRegions
	};

	/**
		Used in the serialize and deserialize methods, to embed extra data.
		The user must implement this class to serialize and deserialize
		the enumerated data type given.
	*/
	class IEmbedding
	{
	public:
		enum DataType
		{
			MaterialLibrary
		};
		virtual void	serialize(physx::general_PxIOStream2::PxFileBuf& stream, IEmbedding::DataType type) const = 0;
		virtual void	deserialize(physx::general_PxIOStream2::PxFileBuf& stream, IEmbedding::DataType type, physx::PxU32 version) = 0;
	};

	/**
		Used to access the collision data for each mesh part
	*/
	class IConvexHull
	{
	protected:
		IConvexHull()
		{
		}

		virtual ~IConvexHull()
		{
		}

	public:
		/**
			Builds the convex hull of the points given.
		*/
		virtual void					buildFromPoints(const void* points, physx::PxU32 numPoints, physx::PxU32 pointStrideBytes) = 0;

		/**
			The hull's axis aligned bounding box.
		*/
		virtual const physx::PxBounds3&	getBounds() const = 0;

		/**
			The hull's volume.
		*/
		virtual physx::PxF32			getVolume() const = 0;

		/**
			This is the number of vertices in the convex hull.
		*/
		virtual physx::PxU32			getVertexCount() const = 0;

		/**
			This is the vertex indexed by vertexIndex, which must in
			the range [0, getVertexCount()-1].
		*/
		virtual physx::PxVec3			getVertex(physx::PxU32 vertexIndex) const = 0;

		/**
			This is the number of edges in the convex hull.
		*/
		virtual physx::PxU32			getEdgeCount() const = 0;

		/**
			This is an edge endpoint indexed by edgeIndex, which must in
			the range [0, getEdgeCount()-1], and
			whichEndpoint, which must be 0 or 1.
		*/
		virtual physx::PxVec3			getEdgeEndpoint(physx::PxU32 edgeIndex, physx::PxU32 whichEndpoint) const = 0;

		/**
			This is the number of planes which bound the convex hull.
		*/
		virtual physx::PxU32			getPlaneCount() const = 0;

		/**
			This is the plane indexed by planeIndex, which must in
			the range [0, getPlaneCount()-1].
		*/
		virtual physx::PxPlane			getPlane(physx::PxU32 planeIndex) const = 0;

		/**
			Perform a ray cast against the convex hull.

			in: this MUST be set to the minimum 'time' that you wish to have reported for intersection.
				you may consider this an origin offset for the ray.
				On exit, if the hull is intersected, this value will contain the time of intersection,
				or its original value, which ever is larger.

			out: this MUST be set to the maximum 'time' that you wish to have reported for intersection.
				you may consider this the endpoint of a line segment intersection.
				On exit, if the hull is intersected, this value will contain the time that the ray
				exits the hull, or its original value, which ever is smaller.

			orig, dir: describe the ray to intersect with the convex hull.

			localToWorldRT: the rotation applied to the convex hull.

			scale: the scale applied to the convex hull.

			normal: if not NULL, *normal will contain the surface normal of the convex hull at the
				point of intersection (at the 'in' time).  If the point on the ray at the 'in' time lies
				within the volume of the convex hull, then *normal will be set to (0,0,0).

			Return value: returns true if the line segment described by the user's supplied 'in' and 'out'
			parameters along the ray intersects the convex hull, false otherwise.
		*/
		virtual bool					rayCast(physx::PxF32& in, physx::PxF32& out, const physx::PxVec3& orig, const physx::PxVec3& dir,
		                                        const physx::PxMat44& localToWorldRT, const physx::PxVec3& scale, physx::PxVec3* normal = NULL) const = 0;

		/**
			Releases all memory associated with this object and deletes itself.
		*/
		virtual void					release() = 0;
	};

	/**
		"Resets" this object to its initial state, freeing all internal data.
		If keepRoot is true, then parts up to the root depth will not be cleared.
		(In this case, not all of the submesh data, etc. will be deleted.)
		The root depth is set when the IExplicitHierarchicalMesh is first created.
		Fracturing methods create pieces beyond the root depth.
	*/
	virtual void						clear(bool keepRoot = false) = 0;

	/**
		The maximum child depth in the hierarchy.  Depth 0 is the base, depth 1 parts are children of depth 0, etc.
		If there are no parts, this function returns -1.
	*/
	virtual physx::PxI32				maxDepth() const = 0;

	/**
		The number of parts in this mesh.
	*/
	virtual physx::PxU32				partCount() const = 0;

	/**
		The number of chunks in this mesh.
	*/
	virtual physx::PxU32				chunkCount() const = 0;

	/**
		The parent index of the chunk indexed by chunkIndex.
		Depth 0 parts have no parents, snd for those parts this function returns -1.
	*/
	virtual physx::PxI32*				parentIndex(physx::PxU32 chunkIndex) = 0;

	/**
		A runtime unique identifier for a chunk.  During one execution of an application which
		contains the fracture tools, this chunk ID will be unique for the chunk.
	*/
	virtual physx::PxU64				chunkUniqueID(physx::PxU32 chunkIndex) = 0;

	/**
		The geometric part index this chunk references
	*/
	virtual physx::PxI32*				partIndex(physx::PxU32 chunkIndex) = 0;

	/**
		If instanced, the part instance offset (translation).
	*/
	virtual physx::PxVec3*				instancedPositionOffset(physx::PxU32 chunkIndex) = 0;

	/**
		If instanced, the part instance offset (UV).
	*/
	virtual physx::PxVec2*				instancedUVOffset(physx::PxU32 chunkIndex) = 0;

	/**
		The number of triangles in the part indexed by partIndex.
		This includes all submeshes.
	*/
	virtual physx::PxU32				meshTriangleCount(physx::PxU32 partIndex) const = 0;

	/**
		A pointer into the array of NxExplicitRenderTriangles which form the mesh
		of the part indexed by partIndex.
	*/
	virtual NxExplicitRenderTriangle*	meshTriangles(physx::PxU32 partIndex) = 0;

	/**
		The axis aligned bounding box of the triangles for the part index by partIndex.
	*/
	virtual physx::PxBounds3			meshBounds(physx::PxU32 partIndex) const = 0;

	/**
		The axis aligned bounding box of the triangles for the chunk index by chunkIndex.
	*/
	virtual physx::PxBounds3			chunkBounds(physx::PxU32 chunkIndex) const = 0;

	/**
		Flags describing attributes of the part indexed by partIndex.
		See NxChunkAuthoringFlag.
	*/
	virtual physx::PxU32				chunkFlags(physx::PxU32 chunkIndex) const = 0;

	/**
		Build collision volumes for the part indexed by partIndex, using (See NxCollisionVolumeDesc.)
	*/
	virtual void						buildCollisionGeometryForPart(physx::PxU32 partIndex, const NxCollisionVolumeDesc& desc) = 0;

	/**
		Build collision volumes for all parts referenced by chunks at the root depth.
	*/
	virtual void						buildCollisionGeometryForRootChunkParts(const NxCollisionVolumeDesc& desc) = 0;

	/**
		The number of convex hulls for the given part.
	*/
	virtual physx::PxU32				convexHullCount(physx::PxU32 partIndex) const = 0;

	/**
		The convex hulls for the given part.
	*/
	virtual const IConvexHull**			convexHulls(physx::PxU32 partIndex) const = 0;

	/**
		The outward surface normal associated with the chunk mesh geometry.
	*/
	virtual physx::PxVec3*				surfaceNormal(physx::PxU32 partIndex) = 0;

	/**
		The displacement map volume for the mesh.
	*/
	virtual const IDisplacementMapVolume&	displacementMapVolume() const = 0;

	/**
		The number of submeshes.  The explicit mesh representation is just a list
		of NxExplicitRenderTriangles for each part, and each NxExplicitRenderTriangle has
		a submesh index.  These indices will lie in a contiguous range from 0 to submeshCount()-1.
	*/
	virtual physx::PxU32				submeshCount() const = 0;

	/**
		The submeshData indexded by submeshIndex.  See NxExplicitSubmeshData.
	*/
	virtual NxExplicitSubmeshData*		submeshData(physx::PxU32 submeshIndex) = 0;

	/**
		Creates a submesh and adds it to the list of submeshes, and returns the index of
		the newly created submesh.
	*/
	virtual physx::PxU32				addSubmesh(const NxExplicitSubmeshData& submeshData) = 0;

	/**
		If there are interior submeshes, then triangles belonging
		to those submesh will have materials applied to them with a using a coordinate frame.
		In the event that materials need to be re-applied (possibly at a different scale), it
		is convenient to store the material frames used.  This function returns the array of
		material frames.  The index extraDataIndex stored in each NxExplicitRenderTriangle
		references this array.
	*/
	virtual physx::PxU32				getMaterialFrameCount() const = 0;
	virtual physx::NxMaterialFrame		getMaterialFrame(physx::PxU32 index) const = 0;
	virtual void						setMaterialFrame(physx::PxU32 index, const physx::NxMaterialFrame& materialFrame) = 0;
	virtual physx::PxU32				addMaterialFrame() = 0;

	/**
		Serialization.  The user must instantiate IEmbedding in order to successfully
		serialize any embedded data.
	*/
	virtual void						serialize(physx::PxFileBuf& stream, IEmbedding& embedding) const = 0;
	virtual void						deserialize(physx::PxFileBuf& stream, IEmbedding& embedding) = 0;

	/**
		Copies the input mesh in to this object.
	*/
	virtual void						set(const IExplicitHierarchicalMesh& mesh) = 0;

	/**
		Creates an internal BSP representation of the mesh parts up to the root depth.
		This is used by authoring tools to perform CSG operations.  If the user instantiates
		IProgressListener, they may pass it in to report progress of this operation.
	*/
	virtual void						calculateMeshBSP(physx::PxU32 randomSeed, IProgressListener* progressListener = NULL) = 0;

	/**
		Draws various components of this object to the debugRenderer, as
		defined by the flags (see the visualization Enum above).  Some
		of the flags require an index be passed in as well.
	*/
	virtual void						visualize(NxApexRenderDebug& debugRender, physx::PxU32 flags, physx::PxU32 index = 0) const = 0;

	/**
		Releases all memory associated with this object and deletes itself.
	*/
	virtual void						release() = 0;

protected:
	/**
		Constructor and destructor are not public
		Use createExplicitHierarchicalMesh() to instantiate an IExplicitHierarchicalMesh and
		IExplicitHierarchicalMesh::release() to destroy it.
	*/
	IExplicitHierarchicalMesh() {}
	virtual								~IExplicitHierarchicalMesh() {}

private:
	/** The assignment operator is disabled, use set() instead. */
	IExplicitHierarchicalMesh&			operator = (const IExplicitHierarchicalMesh&)
	{
		return *this;
	}
};


PX_POP_PACK

}
} // end namespace physx::apex

namespace FractureTools
{
/** Instantiates an IExplicitHierarchicalMesh */
physx::IExplicitHierarchicalMesh*	createExplicitHierarchicalMesh();

/** Instantiates an IExplicitHierarchicalMesh::IConvexHull */
physx::IExplicitHierarchicalMesh::IConvexHull*	createExplicitHierarchicalMeshConvexHull();
} // namespace FractureTools


#endif // EXPLICIT_HIERARCHICAL_MESH_H
