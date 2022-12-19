/****************************************************************************
  This file is part of Space Shuttle Ultra

  Discrete output port definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#if !defined(AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_)
#define AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DiscretePort.h"

namespace discsignals
{

class DiscOutPort : public DiscretePort  
{
private:
	float initval;
public:
	DiscOutPort();
	virtual ~DiscOutPort();
	void Propagate(double fSimT, double fDeltaT, double fMJD);
	virtual void ResetLine();
	void SetLine();
	void SetLine(float fVoltage);
	bool Connect( DiscreteBundle *pBundle, unsigned int iLine );
	
};



class DiscMultiplex4: public DiscMultiplex {
protected:
	unsigned char ucLatch;
public:
	DiscOutPort r[4];
public:
	DiscMultiplex4();
	virtual ~DiscMultiplex4();
	void Set(unsigned char ucNewLatch);
	void ResetAll();
	void SetSingle(unsigned int index);
	virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
};

class DiscMultiplex16: public DiscMultiplex {
protected:
	unsigned short usLatch;
public:
	DiscOutPort r[16];
public:
	DiscMultiplex16();
	virtual ~DiscMultiplex16();
	void Set(unsigned short usNewLatch);
	void ResetAll();
	void SetSingle(unsigned int index);
	virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
};

};

#endif // !defined(AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_)
