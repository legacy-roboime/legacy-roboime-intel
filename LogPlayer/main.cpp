#include "LogPlayer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LogPlayer w;
	w.mainLoop(argc,argv);
	//w.show();
	return a.exec();
}
