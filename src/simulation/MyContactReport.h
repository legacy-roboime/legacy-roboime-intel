#pragma once
#include "NxUserContactReport.h"

class Simulation;

class MyContactReport : public NxUserContactReport
{
private:
	Simulation* simulation;
public:
	MyContactReport(Simulation* simulation);
	~MyContactReport(void);
	/*virtual*/ void  onContactNotify(NxContactPair& pair, NxU32 events);
}/*static robotContactReportObj*/;

//static NxUserContactReport * robotContactReport = &robotContactReportObj;