/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic discrete I/O port definition



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
#if !defined(AFX_DISCRETEPORT_H__18D00C82_9652_493D_8A1D_D1CFF223A0B8__INCLUDED_)
#define AFX_DISCRETEPORT_H__18D00C82_9652_493D_8A1D_D1CFF223A0B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace discsignals
{

class DiscreteBundle;

class DiscretePort  
{
protected:
	DiscreteBundle* pBundle;
	unsigned short usLine;
public:
	virtual bool Connect(DiscreteBundle* pBundle, unsigned int iLine);
	bool Disconnect();
	DiscretePort();
	DiscretePort(DiscreteBundle* pBundle, unsigned int iLine);
	virtual ~DiscretePort();
};

class DiscMultiplex {
public:
	DiscMultiplex();
	virtual ~DiscMultiplex();
	virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
};

};

#endif // !defined(AFX_DISCRETEPORT_H__18D00C82_9652_493D_8A1D_D1CFF223A0B8__INCLUDED_)
