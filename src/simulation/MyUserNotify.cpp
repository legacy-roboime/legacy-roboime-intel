#include "MyUserNotify.h"
#include "Simulation.h"

MyUserNotify::MyUserNotify(Simulation* simulation)
{
	this->simulation = simulation;
}

MyUserNotify::~MyUserNotify(void)
{
	delete simulation;
}

void	MyUserNotify::NXU_errorMessage(bool isError, const char *str)
{
	if (isError)
	{
		printf("NxuStream ERROR: %s\r\n", str);
	}
	else
	{
		printf("NxuStream WARNING: %s\r\n", str);
	}
}

void	MyUserNotify::NXU_notifyScene(NxU32 sno,	NxScene	*scene,	const	char *userProperties)
{
		simulation->gScenes[simulation->gBaseScene]->scene = scene;
		simulation->gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
		simulation->gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
		simulation->gPhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_MESH, 1.0f );
}

void	MyUserNotify::NXU_notifyJoint(NxJoint	*joint,	const	char *userProperties){}

void	MyUserNotify::NXU_notifyActor(NxActor	*actor,	const	char *userProperties){}

void	MyUserNotify::NXU_notifyCloth(NxCloth	*cloth,	const	char *userProperties){}

void	MyUserNotify::NXU_notifyFluid(NxFluid	*fluid,	const	char *userProperties){}

void 	MyUserNotify::NXU_notifyTriangleMesh(NxTriangleMesh *t,const char *userProperties){}

void 	MyUserNotify::NXU_notifyConvexMesh(NxConvexMesh *c,const char *userProperties){}

void 	MyUserNotify::NXU_notifyClothMesh(NxClothMesh *t,const char *userProperties){}

void 	MyUserNotify::NXU_notifyCCDSkeleton(NxCCDSkeleton *t,const char *userProperties){}

void 	MyUserNotify::NXU_notifyHeightField(NxHeightField *t,const char *userProperties){}

NxScene *MyUserNotify::NXU_preNotifyScene(unsigned	int	sno, NxSceneDesc &scene, const char	*userProperties)
{
		assert( simulation->gScenes[simulation->gBaseScene]->scene == 0 );
		if ( simulation->gScenes[simulation->gBaseScene]->scene )
		{
			simulation->releaseScene(*(simulation->gScenes[simulation->gBaseScene]->scene));
			simulation->gScenes[simulation->gBaseScene]->scene = 0;
		}
		return 0;
}

bool	MyUserNotify::NXU_preNotifyJoint(NxJointDesc &joint, const char	*userProperties) { return	true; }

bool	MyUserNotify::NXU_preNotifyActor(NxActorDesc &actor, const char	*userProperties)
{
	for (NxU32 i=0; i<actor.shapes.size(); i++)
	{
		NxShapeDesc *s = actor.shapes[i];
		s->shapeFlags|=NX_SF_VISUALIZATION; // make sure the shape visualization flags are on so we can see things!
	}
	return true;
}

bool 	MyUserNotify::NXU_preNotifyTriangleMesh(NxTriangleMeshDesc &t,const char *userProperties) { return true;	}

bool 	MyUserNotify::NXU_preNotifyConvexMesh(NxConvexMeshDesc &t,const char *userProperties)	{	return true; }

bool 	MyUserNotify::NXU_preNotifyClothMesh(NxClothMeshDesc &t,const char *userProperties) { return true; }

bool 	MyUserNotify::NXU_preNotifyCCDSkeleton(NxSimpleTriangleMesh &t,const char *userProperties)	{	return true; }

bool 	MyUserNotify::NXU_preNotifyHeightField(NxHeightFieldDesc &t,const char *userProperties) {	return true; }

bool 	MyUserNotify::NXU_preNotifySceneInstance(const char *name,const char *sceneName,const char *userProperties,NxMat34 &rootNode,NxU32 depth) { return true; }


void	MyUserNotify::NXU_notifySceneFailed(unsigned	int	sno, NxSceneDesc &scene, const char	*userProperties) { }

void	MyUserNotify::NXU_notifyJointFailed(NxJointDesc &joint, const char	*userProperties) {  }

void	MyUserNotify::NXU_notifyActorFailed(NxActorDesc &actor, const char	*userProperties) { }

void 	MyUserNotify::NXU_notifyTriangleMeshFailed(NxTriangleMeshDesc &t,const char *userProperties) {	}

void 	MyUserNotify::NXU_notifyConvexMeshFailed(NxConvexMeshDesc &t,const char *userProperties)	{	 }

void 	MyUserNotify::NXU_notifyClothMeshFailed(NxClothMeshDesc &t,const char *userProperties) { }

void 	MyUserNotify::NXU_notifyCCDSkeletonFailed(NxSimpleTriangleMesh &t,const char *userProperties)	{	 }

void 	MyUserNotify::NXU_notifyHeightFieldFailed(NxHeightFieldDesc &t,const char *userProperties) {	}

