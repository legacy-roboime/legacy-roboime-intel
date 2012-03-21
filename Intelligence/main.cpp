#include <QtCore/QCoreApplication>
#include <iostream>
#include "Intelligence.h"

using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	cout << "Modulo Inteligencia" << endl;

	Intelligence* intel = new Intelligence(&app);
	//Intelligence intel(&app);

	return app.exec();//it MUST get to this part!
}
