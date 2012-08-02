#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include <QtGui/QMainWindow>
#include "ui_SimulationView.h"
#include "Simulation.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef __PPCGEKKO__
#include "GLFontRenderer.h"
printf("__PPCGEKKO__");
#endif
#include "GLFontRenderer.h"
#include "DebugRenderer.h"
#include "ActorPicking.h"
#include "PerfRenderer.h"
//#include "AVIGenerator.h"

class UDPMulticastSenderSSLVision;
class UDPServerSimInt;
class QTimer;

class SimulationView : public QMainWindow
{
	Q_OBJECT

public:
	SimulationView(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SimulationView();

	static void mainLoop(int argc, char **argv);
	
private:
	//QTimer* time;
	QTimer* timerSim;
	static Simulation* simulation;
	Ui::SimulationViewClass ui;
	//static 
	UDPMulticastSenderSSLVision* visionServer;
	//static 
	UDPServerSimInt* intServer;

	static bool bRightMouseButtonPressed;

	static ActorPicking* actorPicking;
	static QMapIterator<int, NxScene1*> indexRenderScene;
	//static FILE * outputfile;
	//static bool gravar;
	static int count;
	static bool keyDown[256];
	static int gMainHandle;
	static NxVec3 gEye;
	static NxVec3 Dir;
	static NxVec3 Up;
	static NxVec3 N;
	static GLdouble zNear;
	static GLdouble zFar;
	static int mx;
	static int my;
	static DebugRenderer gDebugRenderer;
	static bool gDebugVisualization;
	static PerfRenderer gPerfRenderer;
	static bool gTextEnabled;
	static GLuint mTexId;
	//AVI GENERATOR OPENGL
	//CAVIGenerator AviGen;	// generator
	//BYTE* bmBits;	// image buffer
	//HRESULT hr;
	//LPBITMAPINFOHEADER lpbih; // getting bitmap info

	static void appKey(unsigned char key, bool down);
	static void callback_keyUp(unsigned char c, int x, int y);
	static void callback_key(unsigned char c, int x, int y);
	static void ArrowKeyCallback(int key, int x, int y);
	static void MouseCallback(int button, int state, int x, int y);
	static void MotionCallback(int x, int y);
	static void RenderCallback();
	static void setupCamera();
	static void changeCamera();
	static void ReshapeCallback(int width, int height);
	static void IdleCallback();
	static NX_BOOL LoadScene(const char *pFilename,NXU::NXU_FileType type);
	static void SaveScene(const char *pFilename);
	static bool FileExistTestSimple(const char *fname);
	static void CSL_Scene();
	static void DrawForce(NxActor* actor, NxVec3& forceVec, const NxVec3& color);
	static bool createTexture(const char *filename);
	static void drawTexture(bool shadows);

	friend class NxRobot;
	friend class Simulation;
	friend class Joints;
	friend class ActorPicking;
	friend class Actors;
private slots:
	void simulate();
};

#endif // SIMULATIONVIEW_H
