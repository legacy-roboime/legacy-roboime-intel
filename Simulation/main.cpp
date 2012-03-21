#include "SimulationView.h"
#include <QtGui/QApplication>
#include "UDPMulticastSenderSSLVision.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SimulationView w;
	w.mainLoop(argc,argv);
	//w.show();
	return a.exec();
}
