#include "GraphicalIntelligence.h"
#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include <iostream>
#include "config.h"

using namespace std;

static GraphicalIntelligence* w;
static int winWidth, winHeight;

#ifdef SOCCER_DEBUG
#include "minimax.h"
#include "Minmax2.h"

using namespace LibIntelligence::Plays;

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
	((Minmax2 *)w->play["minimax2"])->drawOpenGL();
}
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	cout << "Modulo Inteligencia" << endl;

	w = new GraphicalIntelligence();
	w->show();

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
	return app.exec();
#endif

	return 0;
}