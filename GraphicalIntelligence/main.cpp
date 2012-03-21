#include "GraphicalIntelligence.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GraphicalIntelligence w;
	w.show();
	return a.exec();
}
