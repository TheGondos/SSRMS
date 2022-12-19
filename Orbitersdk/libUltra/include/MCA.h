/****************************************************************************
  This file is part of Space Shuttle Ultra

  Motor Control Assembly definition



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

#include "AtlantisSubsystem.h"
#include "discsignals\discsignals.h"
#include "eps\eps_defs.h"



class MCAModule {
public:
	MCAModule();
	~MCAModule();

	void OnPropagate(double fSimT, double fDeltaT);

	discsignals::DiscInPort CmdLogic;
	discsignals::DiscOutPort OutPos0;
	discsignals::DiscOutPort OutPos1;

	//Status signals from mechanics.
	discsignals::DiscInPort MotorInPos0;
	discsignals::DiscInPort MotorInPos1;

	eps::ACMotorController mc;
};

class MCA: public AtlantisSubsystem 
{
public:
	MCA(SubsystemDirector* _direct, const std::string& name);
	virtual ~MCA();
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	//@todo Add AC power interface for all modules

	MCAModule module[8];
	
	discsignals::DiscInPort logic;
};