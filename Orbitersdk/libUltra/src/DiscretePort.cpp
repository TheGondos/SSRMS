// DiscretePort.cpp: Implementierung der Klasse DiscretePort.
//
//////////////////////////////////////////////////////////////////////

#include "DiscretePort.h"
#include <cstdio>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals
{

DiscretePort::DiscretePort()
{
	pBundle = NULL;
	usLine = 0;
}
DiscretePort::DiscretePort(DiscreteBundle* pBundle, unsigned int iLine)
{
	Connect(pBundle, iLine);
}

DiscretePort::~DiscretePort()
{

}

bool DiscretePort::Connect(DiscreteBundle *pBundle, unsigned int iLine)
{
	assert( (pBundle != NULL) && "DiscretePort::Connect.pBundle" );
	assert( (iLine <= 15) && "DiscretePort::Connect.iLine" );
	this->pBundle = pBundle;
	this->usLine = iLine;
	return true;
}

bool DiscretePort::Disconnect()
{
	pBundle = NULL;
	usLine = 0;
	return true;
}

	DiscMultiplex::DiscMultiplex() {	
	}

	DiscMultiplex::~DiscMultiplex() {
	}

	void DiscMultiplex::Propagate(double fSimT, double fDeltaT, double fMJD) {
	}

};