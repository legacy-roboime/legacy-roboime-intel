#pragma once
#ifndef KALMANFILTER_H
#define KALMANFILTER_H
#include "LibIntelligence.h"
#include "kalman/ekfilter.hpp"

#include <QObject>

namespace LibIntelligence
{
	class Update;

	namespace Filters
	{
		typedef enum colour {BLUE, YELLOW, BALL} filterobject;

		//DON'T USE THIS, THIS IS FOR INTERNAL USE OF KalmanFilters
		class KalmanFilter : public Kalman::EKFilter<double,1,false,true,false> 
		{	
		public:
			KalmanFilter(filterobject type, qint8 id);

			void next_step();

			Update* estimate();

			Update* current();
			void set_current(Update* p);

			qint8 id();
			filterobject type();

		protected:
			//static KMatrix (K_UINT_32 m, K_UINT_32 n, const T *v)
			void makeA();
			void makeH();
			void makeV();
			void makeR();
			void makeW();
			void makeQ();

			void makeProcess();
			void makeMeasure();

			Matrix P0;
			qreal dt;
			qreal cov_q;
			qreal cov_r;
			
			filterobject type_;

			qint8 id_;
			
			bool virgin;

			Update* current_;
			Update* estimate_;
		};

		typedef KalmanFilter::Vector Vector;
		typedef KalmanFilter::Matrix Matrix;
	}
}
#endif