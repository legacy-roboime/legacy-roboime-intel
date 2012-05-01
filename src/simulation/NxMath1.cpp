#include "NxMath1.h"

NxMath1::NxMath1(void)
{
}

NxMath1::~NxMath1(void)
{
}

NxF32 NxMath1::calcDistanceVec2D( NxF32 x1, NxF32 y1, NxF32 x2, NxF32 y2 )
{
	return NxMath::pow( NxMath::pow( x2 - x1, (NxF32)2 ) + NxMath::pow( y2 - y1, (NxF32)2 ), (NxF32)0.5 );
}

NxReal NxMath1::getBiggestAbsoluteValue(NxReal* values, int size)
{
	NxReal biggest = 0;
	int indexBiggest = 0;
	for( int i = 0; i < size; i++ )
	{
		if( NxMath::abs( values[i] ) > biggest ) 
		{
			biggest = NxMath::abs( values[i] );
			indexBiggest = i;
		}
	}
	return NxMath::abs( values[indexBiggest] );
}

/**
* Retorna o angulo em radianos no plano x-y de uma determinada matriz de orientacao 3x3. Anti-horario positivo. Zero no eixo x+. Angulo entre 0 e 2*PI.
*/
NxReal NxMath1::getAngle2DFromMat33( NxMat33 matrixOrientation )
{
	NxMat33 rotMatrix = matrixOrientation;
	NxMat33 rotMatrixInv;
	rotMatrix.getInverse(rotMatrixInv);
	NxVec3 vecY = rotMatrixInv.getColumn(1);
	NxReal value = vecY.dot(NxVec3(0,1,0));
	value /= vecY.magnitude();

	NxReal teta = NxMath::acos(value);
	if( teta < NxPi*0.5 )
	{
		if( vecY.x * vecY.y > 0 )
		{
			teta = NxPi*2 - teta;
		}
	}
	else
	{
		if( vecY.x * vecY.y < 0 )
		{
			teta = NxPi*2 - teta;
		}
	}
	teta = NxPi*2 - teta;
	return teta;
}
