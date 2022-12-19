/****************************************************************************
  This file is part of Space Shuttle Ultra

  Inertial Measurement Unit definition



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
#pragma once

#include "..\AtlantisSubsystem.h"
#include "..\dps\dps_defs.h"
#include "..\dps\BIU.h"
#include "..\discsignals\discsignals.h"

namespace gnc {

	class IMU: public AtlantisSubsystem, public dps::IConnectedToBus {
		double fOuterGimbalAngle;
		double fMiddleGimbalAngle;
		double fInnerGimbalAngle;
		double fInnerRollGimbalAngle;

		VECTOR3 navbase;
		VECTOR3 old_vel;
		VECTOR3 stable_member_gpos_old;
		VECTOR3 sm_xref_gpos_old;
		VECTOR3 sm_yref_gpos_old;
		VECTOR3 sm_zref_gpos_old;

		double fTemperatureState; //????
	public:
		IMU(SubsystemDirector* _director, const std::string& name, unsigned short usTerminalID=5);
		virtual ~IMU();
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnSaveState(FILEHANDLE scn) const;
		virtual bool OnParseLine(const char* line);

		virtual bool Cage();
		virtual bool Uncage();

		virtual void busCommandPhase(dps::BusController* biu);
		virtual void busReadPhase(dps::BusController* biu);
		virtual dps::BUS_COMMAND_WORD busCommand(dps::BusTerminal* biu, dps::BUS_COMMAND_WORD cw, 
			unsigned long num_data, dps::word16 *cdw);


		virtual const VECTOR3& GetM50Position() const;
		virtual const VECTOR3& GetM50Velocity() const;
		virtual const VECTOR3& GetM50Acceleration() const;
		virtual const VECTOR3& GetM50Attitude() const;

		virtual void TestInput(unsigned short word1, unsigned short word2);

		//replace by subsystem serial I/O output
		dps::BusTerminal fc_connect;		
		//subsystem serial I/O is two-way.
		
		discsignals::DiscInPort power_on;
		discsignals::DiscInPort operate;

		discsignals::DiscOutPort InOperateMode;
	};
};