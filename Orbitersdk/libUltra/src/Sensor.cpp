#include "Sensor.h"
#include <stdlib.h>
#include "assert.h"


Sensor::Sensor( double MinValue, double MaxValue, double FSerror )
{
	assert( (MinValue < MaxValue) && "Sensor::Sensor.(MinValue < MaxValue)" );
	
	this->MaxValue = MaxValue;
	this->MinValue = MinValue;

	conversor = 5 / (MaxValue - MinValue);

	error = (MaxValue - MinValue) * FSerror * (((double)rand() / (RAND_MAX + 1)) - 0.5);
	return;
}

Sensor::~Sensor( void )
{
	return;
}

void Sensor::Connect( DiscreteBundle* pBundle, int iLine )
{
	dipOutput.Connect( pBundle, iLine );
	return;
}

void Sensor::Disconnect( void )
{
	dipOutput.ResetLine();// set to 0 before disconnecting
	dipOutput.Disconnect();
	return;
}

void Sensor::SetValue( double value )
{
	value += error;

	if (value > MaxValue) value = MaxValue;
	else if (value < MinValue) value = MinValue;

	dipOutput.SetLine( static_cast<float>((value - MinValue) * conversor) );
	return;
}