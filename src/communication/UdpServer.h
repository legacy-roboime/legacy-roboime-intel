#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QByteArray>
#include <QQueue>

class QHostAddress;
class QUdpSocket;

class UdpServer : public QObject
{
	Q_OBJECT

public:
	UdpServer(QObject* parent=0, char* address="127.0.0.1", quint16 port=9050);
	~UdpServer();

	void append(const QByteArray& data);
	void send();

protected:
	QUdpSocket* udpSocket;

private:
	QHostAddress* address;
	QQueue<QByteArray> queue;
	quint16 port;
};

#endif // UDPSERVER_H
