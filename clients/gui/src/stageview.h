#ifndef STAGEVIEW_H
#define STAGEVIEW_H

#include <QGraphicsView>
#include "Stage.h"
#include "Skills.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

class StageView : public QGraphicsView
{
	Q_OBJECT

public:
	StageView(QWidget *parent);
	~StageView();
	void setStage(Stage *sta);
	void redraw();

private:
	Stage *stage;
	
};

#endif // STAGEVIEW_H
