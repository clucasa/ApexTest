#ifndef APEX_H
#define APEX_H
#define WIN32
#include "PxPhysicsAPI.h"
#include "PxVisualDebugger.h"
#include "PxVisualDebuggerExt.h"
#include "PvdNetworkStreams.h"

#include "PhysXHeightField.h"

#include "NxApex.h"
//#include "NxApexSDK.h"
#include "ZeusRenderResourceManager.h"
#include "ApexParticles.h"

#include <vector>

//using namespace std;
using namespace physx;
using namespace debugger;

#pragma comment(lib, "PhysX3CHECKED_x86.lib")
#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib") 
#pragma comment(lib, "PhysX3CookingCHECKED_x86.lib") 
#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDKCHECKED.lib")

#pragma comment(lib ,"ApexFrameworkCHECKED_x86")

class Apex
{
public:
	Apex();
	~Apex();

    bool Init();
	bool InitParticles();

    bool advance(float dt);
    void fetch();
private:
    NxApexSDK*                  gApexSDK;
    NxApexScene*                gApexScene;
	physx::apex::NxUserRenderResourceManager*	m_renderResourceManager;

	ApexParticles*				gApexParticles;

private:
    bool InitPhysX();

    PxFoundation*               mFoundation;
    PxPhysics*                  mPhysics;
    PxProfileZoneManager*       mProfileZoneManager;
    PxCooking*                  mCooking;
    PxScene*                    mScene;
    PxDefaultCpuDispatcher*     mCpuDispatcher;
    PxU32                       mNbThreads;
    PxMaterial*					defaultMaterial;
	PVD::PvdConnection*	        pvdConnection;
};

#endif