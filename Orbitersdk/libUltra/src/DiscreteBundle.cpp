// DiscreteBundle.cpp: Implementierung der Klasse DiscreteBundle.
//
//////////////////////////////////////////////////////////////////////

#include "DiscreteBundle.h"
#include <cassert>
#include <OrbiterAPI.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals
{

DiscreteBundle::DiscreteBundle(const string& _ident, unsigned int nLines)
{
	assert( (nLines > 0) && (nLines <= 16) && "DiscreteBundle::DiscreteBundle.nLines" );
	
	snLines = nLines;
	ident = _ident;
	for(unsigned int i=0;i<nLines;i++) fLines[i] = 0.0; // by default, all lines are not set
}

DiscreteBundle::~DiscreteBundle()
{
	char pszBuffer[400];
	sprintf(pszBuffer, "(DiscreteBundle::~DiscreteBundle) destructor called for %s:%d",
		ident.c_str(),
		snLines);
	oapiWriteLog(pszBuffer);
}

const string& DiscreteBundle::GetIdentity() const
{
	return ident;
}

void DiscreteBundle::SetDiscrete(unsigned int iIndex, float fVoltage)
{
	assert( (iIndex < snLines) && "DiscreteBundle::SetDiscrete.iIndex" );
	fLines[iIndex] = fVoltage;
}

float DiscreteBundle::GetVoltage(unsigned int iIndex)
{
	assert( (iIndex < snLines) && "DiscreteBundle::GetVoltage.iIndex" );
	return fLines[iIndex];
}

void DiscreteBundle::SetLogMode(int iMode)
{

}

unsigned int DiscreteBundle::CountLines() const throw() 
{
	return snLines;
}

};
