// DiscOutPort.cpp: Implementierung der Klasse DiscOutPort.
//
//////////////////////////////////////////////////////////////////////

#include "DiscOutPort.h"
#include "DiscreteBundle.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals {

DiscOutPort::DiscOutPort()
{
	initval = 0.0f;
}

DiscOutPort::~DiscOutPort()
{

}

void DiscOutPort::SetLine()
{
	if(pBundle != NULL) {
		pBundle->SetDiscrete(usLine, 5.0f);
	}
	else initval = 5.0f;
}

void DiscOutPort::SetLine( float fVoltage )
{
	if(pBundle != NULL) {
		pBundle->SetDiscrete( usLine, fVoltage );
	}
	else initval = fVoltage;
}

void DiscOutPort::ResetLine()
{
	if(pBundle != NULL) {
		pBundle->SetDiscrete(usLine, 0.0f);
	}
	else initval = 0.0f;
}

bool DiscOutPort::Connect( DiscreteBundle *pBundle, unsigned int iLine )
{
	DiscretePort::Connect( pBundle, iLine );// connect

	pBundle->SetDiscrete( iLine, initval );// set inital value (or whatever value was set before the connection was made)
	return true;
}


//

	DiscMultiplex16::DiscMultiplex16() {

	}

	DiscMultiplex16::~DiscMultiplex16() {
	}

	void DiscMultiplex16::ResetAll()
	{
		for(unsigned int i = 0; i<16; i++)
		{
			r[i].ResetLine();
		}
	}

	void DiscMultiplex16::SetSingle(unsigned int index)
	{
		for(unsigned int i = 0; i<16; i++)
		{
			if(i != index)
			
				r[i].ResetLine();
			else
				r[i].SetLine();
			
		}
	}
	

	void DiscMultiplex16::Propagate(double fSimT, double fDeltaT, double fMJD) {
	}

	void DiscMultiplex16::Set(unsigned short usNewLatch)
	{
		for(unsigned int i = 0; i<16; i++)
		{
			if((usNewLatch & 0x0001) != 0)
				r[i].SetLine();
			else
				r[i].ResetLine();
			usNewLatch >>= 1;
		}
	}

};

