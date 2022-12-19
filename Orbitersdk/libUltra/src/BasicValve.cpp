#include "BasicValve.h"
#include "assert.h"
#include <cstring>


BasicValve::BasicValve( const string& name, double initpos, double rate )
{
	assert( (initpos >= 0.0) && (initpos <= 1.0) && "BasicValve::BasicValve.initpos" );
	assert( (rate > 0.0) && "BasicValve::BasicValve.rate" );

	this->name = name;
	pos = initpos;
	this->rate = rate / 100;
}

BasicValve::~BasicValve( void )
{
}

double BasicValve::GetPos( void ) const
{
	return pos;
}

bool BasicValve::OnParseLine( const char* line )
{
	if (!_strnicmp( line, name.c_str(), name.length() ))
	{
		double tmp = 0.0;
		sscanf( line + name.length(), "%lf", &tmp );
		assert( (tmp >= 0.0) && (tmp <= 1.0) && "BasicValve::OnParseLine.tmp" );
		pos = tmp;
		return true;
	}
	else return false;
}

void BasicValve::OnSaveState( FILEHANDLE scn ) const
{
	oapiWriteScenario_float( scn, (char*)name.c_str(), pos );
	return;
}

void BasicValve::OnPreStep( double fSimT, double fDeltaT, double fMJD )
{
	return;
}

void BasicValve::OnPostStep( double fSimT, double fDeltaT, double fMJD )
{
	return;
}
