#include "apex.h"

Apex::Apex() :
    mNbThreads(8),
    mPhysics(0),
    mFoundation(0),
    mCooking(0),
    mScene(0)

{
    return;
}

Apex::~Apex()
{
    gApexScene->setPhysXScene(0);

    // Now, it's safe to release the NxScene...
    gApexScene->fetchResults(true, NULL);                 // ensure scene is not busy
    gApexScene->release();
    mCpuDispatcher->release();


    // remember to release the connection by manual in the end
    if (pvdConnection)
            pvdConnection->release();
    mPhysics->release();
    mFoundation->release();

    return;
}

float mAccumulator = 0.0f;
float mStepSize = 1.0f / 60.0f;
float mCooldown = 0.0f;

bool Apex::advance(float dt)
{
    mAccumulator  += dt;
    if(mAccumulator < mStepSize)
        return false;

    mAccumulator -= mStepSize;

    if(mCooldown > 0.0f)
        mCooldown -= mStepSize;

    gApexScene->simulate(mStepSize);
    return true;
}

void Apex::fetch()
{
    gApexScene->fetchResults(true, NULL);
}

bool Apex::Init(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
    if(!InitPhysX())
        return false;

    // Init Apex
    static PxDefaultErrorCallback gDefaultErrorCallback;
    ZeusResourceCallback* rcallback = new ZeusResourceCallback();
    NxApexSDKDesc   apexDesc;
    apexDesc.outputStream = &gDefaultErrorCallback;
    apexDesc.resourceCallback = rcallback;
    apexDesc.physXSDK = mPhysics;
    apexDesc.cooking = mCooking;
    
    m_renderResourceManager = new ZeusRenderResourceManager(dev,devcon);
    apexDesc.renderResourceManager = m_renderResourceManager;

    if(apexDesc.isValid())
        gApexSDK = NxCreateApexSDK(apexDesc);
    else
        return false;
   
    if(!gApexSDK)
        return false;

    NxApexSceneDesc apexSceneDesc;
    // Create the APEX scene...
    
    apexSceneDesc.scene = mScene;
    if(apexSceneDesc.isValid())
        gApexScene = gApexSDK->createScene(apexSceneDesc);
    else
        return false;

    if(!gApexScene)
        return false;

    return true;
}

bool Apex::InitPhysX()
{
    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if(!mFoundation)
        return false;

    bool recordMemoryAllocations = true;

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
                PxTolerancesScale(), recordMemoryAllocations);
    if(!mPhysics)
        return false;

    mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams());
    if (!mCooking)
        return false;

    if (!PxInitExtensions(*mPhysics))
        return false;


    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

    if(!sceneDesc.cpuDispatcher)
    {
        mCpuDispatcher = PxDefaultCpuDispatcherCreate(mNbThreads);
        if(!mCpuDispatcher)
            return false;
        sceneDesc.cpuDispatcher    = mCpuDispatcher;
    }

    if(!sceneDesc.filterShader)
    {
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    }
    
    /*#ifdef PX_WINDOWS
    if(!sceneDesc.gpuDispatcher && mCudaContextManager)
    {
        sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
    }
    #*/

    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
        return false;

    defaultMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution
    if(!defaultMaterial)
        return false;

    // Create a plane
    PxRigidStatic* plane = PxCreatePlane(*mPhysics, PxPlane(PxVec3(0,1,0), 5), *defaultMaterial);
    if (!plane)
        return false;

    mScene->addActor(*plane);

    // Create a heightfield
    PhysXHeightfield* heightfield = new PhysXHeightfield();
    //heightfield->InitHeightfield(mPhysics, mScene, "terrain5.raw");


    // Create a box
    PxReal density = 10.0f;
    PxTransform transform(PxVec3(0.0, 8.0, 0.0) , PxQuat::createIdentity());
    PxVec3 dimensions(1.5,1.5,1.5);
    PxBoxGeometry geometry(dimensions);
    PxRigidDynamic* boxActor = PxCreateDynamic(*mPhysics, transform, geometry, *defaultMaterial, density);
    if (!boxActor)
        return false;
    
    boxActor->setLinearVelocity(PxVec3(0.0,0.0,0.0));
    boxActor->setAngularDamping((PxReal)0.95);
    //PxRigidBodyExt::updateMassAndInertia(*boxActor, density);

    mScene->addActor(*boxActor);
    
    gRenderer = new ZeusRenderer();

    // check if PvdConnection manager is available on this platform
    if(mPhysics->getPvdConnectionManager() == NULL)
        return false;

    // setup connection parameters
    const char*     pvd_host_ip = "127.0.0.1";  // IP of the PC which is running PVD
    int             port        = 5425;         // TCP port to connect to, where PVD is listening
    unsigned int    timeout     = 100;          // timeout in milliseconds to wait for PVD to respond,
                                                // consoles and remote PCs need a higher timeout.
    PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

    // and now try to connect
    pvdConnection = PxVisualDebuggerExt::createConnection(mPhysics->getPvdConnectionManager(),
        pvd_host_ip, port, timeout, connectionFlags);

    mPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlags::eTRANSMIT_CONTACTS, true);

    return true;
}

bool Apex::InitParticles()
{
    gApexParticles = new ApexParticles();
    gApexParticles->Init(gApexSDK);
    gApexParticles->CreateEmitter(gApexSDK, gApexScene);
    
    return true;
}

void Apex::Render()
{
    gApexParticles->RenderVolume(*gRenderer);
}