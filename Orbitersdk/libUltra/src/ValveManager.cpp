#include "ValveManager.h"
#include "assert.h"


ValveManager::ValveManager( void )
{
}

ValveManager::~ValveManager( void )
{
	BasicValve* pValve;

	for (unsigned int i = 0; i < valves.size(); i++)
	{
		pValve = valves[i];
		delete pValve;
	}

	valves.clear();
}


void ValveManager::AddValve( BasicValve *pValve )
{
	assert( (pValve != NULL) && "ValveManager::AddValve.pValve" );
	
	valves.push_back( pValve );
	return;
}

bool ValveManager::OnParseLine( const char* line )
{
	BasicValve* pValve;

	for (unsigned int i = 0; i < valves.size(); i++)
	{
		pValve = valves[i];
		if (pValve->OnParseLine( line )) return true;
	}
	return false;
}

void ValveManager::OnSaveState( FILEHANDLE scn ) const
{
	BasicValve* pValve;

	for (unsigned int i = 0; i < valves.size(); i++)
	{
		pValve = valves[i];
		pValve->OnSaveState( scn );
	}
	return;
}

void ValveManager::OnPreStep( double fSimT, double fDeltaT, double fMJD )
{
	BasicValve* pValve;

	for (unsigned int i = 0; i < valves.size(); i++)
	{
		pValve = valves[i];
		pValve->OnPostStep( fSimT, fDeltaT, fMJD );
	}
	return;
}

void ValveManager::OnPostStep( double fSimT, double fDeltaT, double fMJD )
{
	BasicValve* pValve;

	for (unsigned int i = 0; i < valves.size(); i++)
	{
		pValve = valves[i];
		pValve->OnPostStep( fSimT, fDeltaT, fMJD );
	}
	return;
}
