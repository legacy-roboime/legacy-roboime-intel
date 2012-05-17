#include <QtCore/QCoreApplication>
#include <iostream>
#include "Intelligence.h"

using namespace std;

static Intelligence* intel;
static int winWidth, winHeight;
static QCoreApplication* app;

void reshape(int wid, int ht)
{
	winWidth = wid;
	winHeight = ht;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-soccer_env()->hfield_w, soccer_env()->hfield_w, 
		-soccer_env()->hfield_h, soccer_env()->hfield_h, -1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
}

void idleFunc(void)
{
	QCoreApplication::processEvents();
	intel->update();
}

int main(int argc, char *argv[])
{
	app = new QCoreApplication(argc, argv);

	cout << "Modulo Inteligencia" << endl;

	intel = new Intelligence(app);

#ifdef SOCCER_DEBUG
	glutInit(&argc, argv);
	glutInitWindowSize(1200, 400);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	(void)glutCreateWindow("Soccer");
	glutReshapeFunc(reshape);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(idleFunc);
	glutIdleFunc(idleFunc);
	glutMainLoop();
#else
	app->exec();
#endif

	//Intelligence intel(&app);

	return 0;
}
