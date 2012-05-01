#pragma once

#include "NXU_helper.h"  // NxuStream helper functions.

class Simulation;

class MyUserNotify: public NXU_userNotify, public NXU_errorReport
{
private:
	Simulation* simulation;
public:
	virtual void	NXU_errorMessage(bool isError, const char *str);

	virtual void	NXU_notifyScene(NxU32 sno,	NxScene	*scene,	const	char *userProperties);

	virtual void	NXU_notifyJoint(NxJoint	*joint,	const	char *userProperties);

	virtual void	NXU_notifyActor(NxActor	*actor,	const	char *userProperties);

	virtual void	NXU_notifyCloth(NxCloth	*cloth,	const	char *userProperties);

	virtual void	NXU_notifyFluid(NxFluid	*fluid,	const	char *userProperties);

	virtual void 	NXU_notifyTriangleMesh(NxTriangleMesh *t,const char *userProperties);

	virtual void 	NXU_notifyConvexMesh(NxConvexMesh *c,const char *userProperties);

	virtual void 	NXU_notifyClothMesh(NxClothMesh *t,const char *userProperties);

	virtual void 	NXU_notifyCCDSkeleton(NxCCDSkeleton *t,const char *userProperties);

	virtual void 	NXU_notifyHeightField(NxHeightField *t,const char *userProperties);

	virtual NxScene *NXU_preNotifyScene(unsigned	int	sno, NxSceneDesc &scene, const char	*userProperties);

	virtual bool	NXU_preNotifyJoint(NxJointDesc &joint, const char	*userProperties);

	virtual bool	NXU_preNotifyActor(NxActorDesc &actor, const char	*userProperties);

	virtual bool 	NXU_preNotifyTriangleMesh(NxTriangleMeshDesc &t,const char *userProperties);

	virtual bool 	NXU_preNotifyConvexMesh(NxConvexMeshDesc &t,const char *userProperties);

	virtual bool 	NXU_preNotifyClothMesh(NxClothMeshDesc &t,const char *userProperties);

	virtual bool 	NXU_preNotifyCCDSkeleton(NxSimpleTriangleMesh &t,const char *userProperties);

	virtual bool 	NXU_preNotifyHeightField(NxHeightFieldDesc &t,const char *userProperties);

	virtual bool 	NXU_preNotifySceneInstance(const char *name,const char *sceneName,const char *userProperties,NxMat34 &rootNode,NxU32 depth);

	virtual void	NXU_notifySceneFailed(unsigned	int	sno, NxSceneDesc &scene, const char	*userProperties);

	virtual void	NXU_notifyJointFailed(NxJointDesc &joint, const char	*userProperties);

	virtual void	NXU_notifyActorFailed(NxActorDesc &actor, const char	*userProperties);

	virtual void 	NXU_notifyTriangleMeshFailed(NxTriangleMeshDesc &t,const char *userProperties);

	virtual void 	NXU_notifyConvexMeshFailed(NxConvexMeshDesc &t,const char *userProperties);

	virtual void 	NXU_notifyClothMeshFailed(NxClothMeshDesc &t,const char *userProperties);

	virtual void 	NXU_notifyCCDSkeletonFailed(NxSimpleTriangleMesh &t,const char *userProperties);

	virtual void 	NXU_notifyHeightFieldFailed(NxHeightFieldDesc &t,const char *userProperties);

	MyUserNotify(Simulation* simulation=0);
	~MyUserNotify(void);
};
