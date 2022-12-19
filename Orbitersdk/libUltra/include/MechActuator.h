/****************************************************************************
  This file is part of Space Shuttle Ultra

  Electomechanic actuator definition



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
#include "discsignals/DiscInPort.h"
#include "discsignals/DiscOutPort.h"
#include "eps/eps_defs.h"



class MechActuator: public AtlantisSubsystem
{
	double fOutPos; //Output position 0 ... 1
	double fSpeed;  // 1/s

	double fLimit0Max;
	double fLimit1Min;
	double fHardStopMin;
	double fHardStopMax;

	double fMaxSpeed;
	double fAccConstant;
	double fMechMI;
	
	UINT object_anim;
	bool fSingleMotorFlag;
	
public:
	MechActuator(SubsystemDirector* _pDirect, const string& _ident, double fStandardTime = 0.0);
	virtual ~MechActuator();
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	/*
	void SetMotorParams(double fNormTorque, double fNormAmps, double fNormSpeed, 
		double fNormVoltage);
	void SetObjectParams(double fMinTorque, double fFrictionCoeff, 
		double fMOICoeff);
	*/
	void SetSingleMotorTime(double fTimeSingle);
	/**
	 * @param fLimit0 maximum position to set Limit 0 discrete
	 * @param fLimit1 minimum position to set Limit 1 discrete
	 */
	void SetMicroSwitchPositions(double fLimit0, double fLimit1);
	void SetState(double fState0);
	void SetObjectAnim(UINT anim);

	discsignals::DiscOutPort PosLimit0;
	discsignals::DiscOutPort PosLimit1;
	
	discsignals::DiscInPort LogicPwrOn;

	eps::ACMotor system[2];
};