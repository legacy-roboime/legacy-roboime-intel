#ifndef KALMANFILTERS_H
#define KALMANFILTERS_H

#include "LibIntelligence.h" 
#include <QObject>
#include <QQueue>
#include "Filter.h"

namespace LibIntelligence
{
	class Update;
	class KalmanFilter;

	namespace Filters
	{
		class KalmanFilters : public Filter
		{
			Q_OBJECT

		public:
			KalmanFilters(QObject *p, Updates* u);
			~KalmanFilters();

		public slots:
			virtual void step();

		protected:
			QList<KalmanFilter*> filters;
		};
	}
}
#endif // KALMANFILTERS_H
