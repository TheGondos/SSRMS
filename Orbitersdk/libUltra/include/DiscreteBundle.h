/****************************************************************************
  This file is part of Space Shuttle Ultra

  Discrete line bundle definition



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
#if !defined(AFX_DISCRETEBUNDLE_H__A928D10B_004B_495D_87CD_3A0309B3EFDA__INCLUDED_)
#define AFX_DISCRETEBUNDLE_H__A928D10B_004B_495D_87CD_3A0309B3EFDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

namespace discsignals
{
	using namespace std;

	class DiscretePort;

/**
 * class for handling multiple discrete data lines or analog inputs
 * State for each discrete can be -28V to +28V
 * One bundle can have up to 16 lines. 
 * one line has a beginning and a end point.
 * 0 = ground (GND)
 */
class DiscreteBundle  
{
	string ident;
	unsigned short snLines;
	float fState;
	float fLines[16];
	/**
	 * Indicate that a line is broken and does always return 0V. 
	 */
	bool bBrokenLine[16];
public:
	DiscreteBundle(const string& _ident, unsigned int nLines = 16);
	virtual ~DiscreteBundle();

	unsigned int CountLines() const throw();
	/**
	 * Set the mode how the Discrete bundle class handles logging information
	 */
	void SetLogMode(int iMode);
	float GetVoltage(unsigned int iIndex);
	const string& GetIdentity() const;
	void SetDiscrete(unsigned int iIndex, float fVoltage);
};

};

#endif // !defined(AFX_DISCRETEBUNDLE_H__A928D10B_004B_495D_87CD_3A0309B3EFDA__INCLUDED_)
