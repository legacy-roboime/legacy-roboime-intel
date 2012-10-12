#include "config.h"

#include "CommanderGrSim.h"
#include "Command.h"
#include "Robot.h"
#include "Team.h"

#include "grSim_Commands.pb.h"
#include "grSim_Packet.pb.h"

using namespace std;
using namespace LibIntelligence;

CommanderGrSim::CommanderGrSim(QObject* parent, const char* address, quint16 port)
	: UdpServer(parent, address, port),
	Commander()
{
}

#define KFACTOR 7

void CommanderGrSim::step()
{
    grSim_Packet* packet = new grSim_Packet();
    //grSim_Commands commands =  packet->commands();
    grSim_Commands* commands =  packet->mutable_commands();

    //XXX: only the color of the first robot is considered
    commands->set_isteamyellow(robots[0]->team()->color() == YELLOW);
    //FIXME: set proper timestamp
    commands->set_timestamp(0.0);

    for(uint n = 0; n < robots.size(); n++) {
        Command& c = robots[n]->command();
        grSim_Robot_Command* command = commands->add_robot_commands();
        command->set_id(c.id());
        command->set_kickspeedx(KFACTOR * c.kickSpeed());
        command->set_spinner(c.dribbleSpeed() > 0.0);
        //XXX: we don't use chip kick yet.
        command->set_kickspeedz(0.0);
        //XXX: we don't use tanget/normal/angular speed, we use wheel speeds.
        command->set_veltangent(0.0);
        command->set_velnormal(0.0);
        command->set_velangular(0.0);
        command->set_wheelsspeed(true);
        command->set_wheel1(c.wheelSpeedAt(1));
        command->set_wheel2(c.wheelSpeedAt(2));
        command->set_wheel3(c.wheelSpeedAt(3));
        command->set_wheel4(c.wheelSpeedAt(0));
    }

    QByteArray datagram;
    datagram.resize(packet->ByteSize());
    packet->SerializeToArray(datagram.data(), datagram.size());
    append(datagram);
	delete packet;
}

