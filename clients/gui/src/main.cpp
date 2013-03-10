#include "GraphicalIntelligence.h"
#include <QApplication>
#include <QCoreApplication>
#include <iostream>
#include "config.h"

using namespace std;

static GraphicalIntelligence* w;
//static int winWidth, winHeight;//SA: Unused variables

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	cout << "Modulo Inteligencia" << endl;

	w = new GraphicalIntelligence();
	w->show();

	return app.exec();

	return 0;
}
