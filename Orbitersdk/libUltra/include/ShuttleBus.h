/****************************************************************************
  This file is part of Space Shuttle Ultra

  Shuttle Bus class definition



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
#if !defined(AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_)
#define AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include <set>
#include "dps_defs.h"

namespace dps {

	using namespace std;

	

	class IMDMProtocol {
	public:
		virtual bool MDMGetModuleType(unsigned int uiModule, unsigned short& usModuleType) = 0;
		virtual bool MDMWrite(unsigned int uiModuleAddr, short sWord) = 0;
		virtual bool MDMRead(unsigned int uiModuleAddr, short& sWord) = 0;
		virtual bool MDMExecute(unsigned uiProgram, short* writeBuffer, short* readBuffer) = 0;
	};

	class ShuttleBus
	{
		unsigned long ulCycle;
		string ident;
		BusTerminal* m_lookup_table[31];
		set<BIU*> connections;
		set<BusTerminal*> terminals;
		set<BusController*> controllers;
	public:
		ShuttleBus(const string& _ident);
		virtual ~ShuttleBus();
		virtual const string& GetIdent() const;
		virtual void ConnectTo(BIU* pTarget);
		//virtual long Transmit(unsigned short usTarget, unsigned short usNumData, unsigned short usData[]);
		//virtual unsigned short Transmit(bool toMDM, short sDataCount, word24 dataw[]);
		virtual void busPrePhase();
		virtual void busCommandPhase();
		virtual void busReadPhase();

		BUS_COMMAND_WORD busCommand(BUS_COMMAND_WORD cw, 
			unsigned long word_count, short* cdw);
		unsigned long ReadData(unsigned int mdm, 
			unsigned long word_count, short* rdw);
	};

	class ShuttleBusManager {
		vector<ShuttleBus*> busses;
	public:
		ShuttleBusManager();
		virtual ~ShuttleBusManager();
		ShuttleBus* CreateBus(const string& _ident);
		ShuttleBus* FindBus(const string& _ident) const;
		ShuttleBus* GetBus(const string& _ident);
		void clbkPropagate(double fSimT, double fDeltaT);
	};

};

#endif // !defined(AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_)
