/****************************************************************************
  This file is part of Space Shuttle Ultra

  ATVC subsystem definition



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

#ifndef _gnc_ATVC_H_
#define _gnc_ATVC_H_


#include "..\AtlantisSubsystem.h"
#include "..\discsignals\DiscInPort.h"
#include "..\discsignals\DiscOutPort.h"


namespace gnc
{
	using class discsignals::DiscInPort;
	using class discsignals::DiscOutPort;

	class ATVC:public AtlantisSubsystem
	{
	private:
		short ID;
	public:
		/**
		 * in_MDMFA_pos_XXXXX
		 * +5v -> 0%
		 * +15v -> 100%
		 */
		DiscInPort in_MDMFA_pos_1P;
		DiscInPort in_MDMFA_pos_1Y;
		DiscInPort in_MDMFA_pos_2P;
		DiscInPort in_MDMFA_pos_2Y;
		DiscInPort in_MDMFA_pos_3P;
		DiscInPort in_MDMFA_pos_3Y;
		DiscInPort in_MDMFA_pos_LSRBT;
		DiscInPort in_MDMFA_pos_LSRBR;
		DiscInPort in_MDMFA_pos_RSRBT;
		DiscInPort in_MDMFA_pos_RSRBR;

		/**
		 * in_MDMFA_FCSCHSW
		 * +5v < OVERRIDE < 10v
		 * +10v < AUTO < 15v
		 * +15v < OFF < 20v
		 */
		DiscInPort in_MDMFA_FCSCHSW;

		DiscInPort in_1P_dp;
		DiscInPort in_1Y_dp;
		DiscInPort in_2P_dp;
		DiscInPort in_2Y_dp;
		DiscInPort in_3P_dp;
		DiscInPort in_3Y_dp;
		DiscInPort in_LSRBT_dp;
		DiscInPort in_LSRBR_dp;
		DiscInPort in_RSRBT_dp;
		DiscInPort in_RSRBR_dp;

		/**
		 * in_XXXXX_pos
		 * +5v -> 0%
		 * +15v -> 100%
		 */
		DiscInPort in_1P_pos;
		DiscInPort in_1Y_pos;
		DiscInPort in_2P_pos;
		DiscInPort in_2Y_pos;
		DiscInPort in_3P_pos;
		DiscInPort in_3Y_pos;
		DiscInPort in_LSRBT_pos;
		DiscInPort in_LSRBR_pos;
		DiscInPort in_RSRBT_pos;
		DiscInPort in_RSRBR_pos;

		/**
		 * out_MDMFA_pos_XXXXX
		 * +5v -> 0%
		 * +15v -> 100%
		 */
		DiscOutPort out_MDMFA_pos_1P;
		DiscOutPort out_MDMFA_pos_1Y;
		DiscOutPort out_MDMFA_pos_2P;
		DiscOutPort out_MDMFA_pos_2Y;
		DiscOutPort out_MDMFA_pos_3P;
		DiscOutPort out_MDMFA_pos_3Y;
		DiscOutPort out_MDMFA_pos_LSRBT;
		DiscOutPort out_MDMFA_pos_LSRBR;
		DiscOutPort out_MDMFA_pos_RSRBT;
		DiscOutPort out_MDMFA_pos_RSRBR;

		/**
		 * out_XXXXX_srvlv
		 * +10v -> extend 100% (full open from full close)
		 * +5v -> extend 50%
		 * yada yada yada...
		 * -10v -> retract 100% (full close from full open)
		 */
		DiscOutPort out_1P_srvlv;
		DiscOutPort out_1Y_srvlv;
		DiscOutPort out_2P_srvlv;
		DiscOutPort out_2Y_srvlv;
		DiscOutPort out_3P_srvlv;
		DiscOutPort out_3Y_srvlv;
		DiscOutPort out_LSRBT_srvlv;
		DiscOutPort out_LSRBR_srvlv;
		DiscOutPort out_RSRBT_srvlv;
		DiscOutPort out_RSRBR_srvlv;

		/**
		 * out_XXXXX_isolvlv
		 * 0v -> OFF
		 * +5v -> ON
		 */
		DiscOutPort out_1P_isolvlv;
		DiscOutPort out_1Y_isolvlv;
		DiscOutPort out_2P_isolvlv;
		DiscOutPort out_2Y_isolvlv;
		DiscOutPort out_3P_isolvlv;
		DiscOutPort out_3Y_isolvlv;
		DiscOutPort out_LSRBT_isolvlv;
		DiscOutPort out_LSRBR_isolvlv;
		DiscOutPort out_RSRBT_isolvlv;
		DiscOutPort out_RSRBR_isolvlv;

		ATVC( SubsystemDirector* _director, const string& _ident, short nID );
		~ATVC( void );

		void Realize( void );

		void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};
}


#endif// _gnc_ATVC_H_

/*
EA P/S
------
1P 1/3
1Y 3/1

2P 2/1
2Y 1/2

3P 3/2
3Y 2/3


P +/- 10.5deg
Y +/- 8.5deg

10 <= RATE(deg/sec) <= 20

1P 16
1Y 0

2P 10
2Y 3.5

3P 10
3Y 3.5
*/
