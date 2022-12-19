/****************************************************************************
  This file is part of Space Shuttle Ultra

  General Purpose Computer definition



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

#include "../AtlantisSubsystem.h"
#include "../discsignals/DiscInPort.h"
#include "../discsignals/DiscOutPort.h"
#include "BIU.h"

namespace dps {

	using class discsignals::DiscDemultiplex16;
	using class discsignals::DiscMultiplex16;
	using class discsignals::DiscInPort;

	class GPCSoftware;

#pragma pack()

	typedef struct __psw {
		unsigned int uIA:24;
		unsigned int uPM:4;
		unsigned int uCC:2;
		unsigned int uILC:2;
		unsigned int uIntCode:16;
		unsigned int bProblemState:1;
		unsigned int bWaitState:1;
		unsigned int bMachineCheckMask:1;
		unsigned int bASCIIMode:1;
		unsigned int bProtKey:4;
		unsigned int SystemMask:8;
	} AP101PSW;

	

	typedef struct {
		//Reserved area
		word16 tpsares1[4];
		//power on PSW
		AP101PSW tpsapwr;
		word16 tpsars2[4];
		word16 tpsars3[4];
		//Power off PSW
		AP101PSW tpsapwrf;
		//System reset PSW
		AP101PSW tpsasrp;
		word16 tpsars4[2];
		word24 tpsacvta;
		word16 tpsaspr1;
		word16 tpsars5[4];
		word16 tpsars6[4];
		word16 tpsars7[4];
		word16 tpsars8[4];
		word16 tpsars9[4];
		word16 tpsars10[4];
		word16 tpsars11[4];
		word16 tpsars12[4];
		word16 tpsars13[4];
		//Machine check old PSW
		AP101PSW tpsamcop;
		//Machine check new PSW
		AP101PSW tpsamcnp;
		//program check old PSW
		AP101PSW tpsapiop;
		//Program check new PSW
		AP101PSW tpsapinp;
		word16 tpsars14[4];
		word16 tpsars15[4];
		//SVC old PSW
		AP101PSW tpsasop;
		//Program check new PSW
		AP101PSW tpsasnp;
		//Program counter 1 old PSW
		AP101PSW tpsac1op;
		//Program counter 1 new PSW
		AP101PSW tpsac1np;
		//Program counter 2 old PSW
		AP101PSW tpsac2op;
		//Program counter 2 new PSW
		AP101PSW tpsac2np;
		//Instruction monitor old PSW
		AP101PSW tpsaimop;
		//Instruction monitor new PSW
		AP101PSW tpsaimnp;
		//External interrupt old PSW
		AP101PSW tpsaeop;
		//External interrupt new PSW
		AP101PSW tpsaenp;
		//External interrupt 1 old PSW
		AP101PSW tpsae1op;
		//External interrupt 1 new PSW
		AP101PSW tpsae1np;
		//External interrupt 2 old PSW
		AP101PSW tpsae2op;
		//External interrupt 2 new PSW
		AP101PSW tpsae2np;
		//External interrupt 3 old PSW
		AP101PSW tpsae3op;
		//External interrupt 3 new PSW
		AP101PSW tpsae3np;
		//Special interrupt old PSW
		AP101PSW tpsasiop;
		//Special interrupt new PSW
		AP101PSW tpsasinp;
		word16 tpsars16[16];
		word16 tpsapcs1;
		word16 tpsapcs2;
		word16 tpsawork[14];
		word16 tpsapar1[16];
		word16 tpsapar2[16];
		word16 tpsapafp[16];
		word16 tpsapamr[16];
		word16 tpsapars;
		word16 tpsapac1;
		word16 tpsapac2;
		word16 tpsapaor;
		word16 tpsars17[60];
	} PREFERRED_STORAGE;



	/**
	 * Quick and dirty IOP implementation, should be 
	 * improved if possible.
	 */
	class IOP {
	public:
		IOP();
		virtual ~IOP();
		
	};

	/**
	 * base class for all GPC subsystem units. Must implement IOP and CPU 
	 * behavior. 
	 */
	class GPC:public AtlantisSubsystem {
	protected:
		unsigned short usGPCID;

		long lGPR[16];
		float fFPR[8];

		AP101PSW psw;
	public:
		GPC(SubsystemDirector* _direct, const string& _ident, unsigned short usGPCID);
		virtual ~GPC();
	
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	public:
		//@todo: Number is not unknown, needs research!
		DiscDemultiplex16 disc_in[4];
		DiscMultiplex16 disc_out;
		BusController channel[24];
		BusTerminal ICC[4];
		DiscInPort power_on;
		DiscInPort ipl;
		DiscInPort ipl_source1;
		DiscInPort ipl_source2;
		DiscInPort run;
		DiscInPort halt;
	};

};