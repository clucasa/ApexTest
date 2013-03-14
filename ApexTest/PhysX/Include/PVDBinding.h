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

#ifndef PVDBINDING_H
#define PVDBINDING_H

#include "foundation/Px.h"
#include "physxvisualdebuggersdk/PvdConnectionFlags.h"

#define DEFAULT_PVD_BINDING_PORT 5425
#define DEFAULT_PVD_BINDING_TIMEOUT_MS 100

namespace physx 
{
	class PxProfileZoneManager;

	namespace debugger 
	{
		namespace comm 
		{
			class PvdConnectionManager;
			class PvdConnection;
		}
	}
}

namespace PVD
{
	/**
	\brief The PvdBinding class allows APEX and PhysX to both connect to the PhysX Visual Debugger (PVD)
	*/
	class PvdBinding
	{
	protected:
		virtual ~PvdBinding(){}
	public:
		/**
		\brief Retrieve the PvdConnectionManager
		*/
		virtual physx::debugger::comm::PvdConnectionManager& getConnectionManager() = 0;

		/**
		\brief Retrieve the PxProfileZoneManager
		*/
		virtual physx::PxProfileZoneManager& getProfileManager() = 0;

		/**
		\brief Check if the PVD connection is active
		*/
		virtual bool isConnected() = 0;

		/**
		\brief Terminate the PVD connection
		*/
		virtual void disconnect() = 0;

		/**
		\brief Start the profiling frame
		\note inInstanceId must *not* be used already by pvd
		*/
		virtual void beginFrame( void* inInstanceId ) = 0;

		/**
		\brief End the profiling frame
		*/
		virtual void endFrame( void* inInstanceId ) = 0;
		
		/**
		\brief Destroy this instance
		*/
		virtual void release() = 0;
		/**
		 *	Connect call either connects through the legacy debugger interface and tunnels through
		 *	it if you want debug information *or* connects purely through the newer SDK connection
		 *	if you just want to send profile and memory information across.
		 */
		virtual void connect( const char* inHost
							, int inPort = DEFAULT_PVD_BINDING_PORT
							, unsigned int inTimeoutInMilliseconds = DEFAULT_PVD_BINDING_TIMEOUT_MS
							//If we are using the debug connection type, we tunnel through the
							//older debugger.  Else we don't and connect directly to pvd without
							//using the older debugger.
							, physx::debugger::TConnectionFlagsType inConnectionType = physx::debugger::defaultConnectionFlags() ) = 0;

		/**
		 *	Assumes foundation is already booted up.
		 */
		static PvdBinding& create( physx::debugger::comm::PvdConnectionManager& inConnectionManager, physx::PxProfileZoneManager& inProfileManager );

		/**
		 *	Assumes foundation is already booted up, boots up the connection and profile manager and releases
		 *	them itself.
		 *	\param inRecordMemoryEvents true if you would like the system to record outstanding allocations.
		 *			This is necessary for PVD to accurately reflect what is going on w/r/t to memory allocations
		 *			but doesn't help or hurt per-frame allocation statistics.  It only serves to ensure that when
		 *			a new PVD connection is made, it is booted up with accurate oustanding memory information.
		 */
		static PvdBinding& create( bool inRecordMemoryEvents );
	};

}



#endif // PVDBINDING_H
