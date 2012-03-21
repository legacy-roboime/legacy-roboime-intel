#pragma once
#include "UDPServer.h"
#include <QObject>

class QUdpSocket;
class Simulation;

class UDPServerSimInt : public QObject
{
	Q_OBJECT

private:
	Simulation* simulation;
	QUdpSocket* udpSocket;

private slots:
	void parsing(); //Parse the received string and define the string to send

public:
	UDPServerSimInt(QObject* parent=0, Simulation* simulation=0, quint16 port=9876);
	~UDPServerSimInt(void);		
	void setSimulation(Simulation* simulation);
};
