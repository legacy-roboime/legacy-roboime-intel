#pragma once
#ifndef FILTER_H
#define FILTER_H

#include "LibIntelligence.h"
#include "Updates.h"
#include <QObject>
#include <QQueue>

namespace LibIntelligence
{
	class Update;
	class Command;

	namespace Filters
	{
		class Filter : public QObject
		{
			Q_OBJECT

		public:
			Filter(QObject *parent=0, Updates* updates_from=0);
			
			void setUpdatesFrom(Updates*);

			Updates* updates();

		public slots:
			virtual void step() = 0;

		signals:
			void filteredUpdate();

		protected:
			Updates updates_;
			Updates* updates_from;
		};
	}
}

#endif // FILTER_H
