/****************************************************************************
  This file is part of Space Shuttle Ultra

  Bus Interface Unit definition



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

#include "dps_defs.h"
#include <string>

class AtlantisSubsystem;

namespace dps {

	using namespace std;


	class FastQueue {
		word16 sBuffer[4096];
		unsigned short usTailPtr;
		unsigned short usHeadPtr;
	public:
		FastQueue() {
			usTailPtr = 0;
			usHeadPtr = 0;
		};


		inline word16 front() const {
			return sBuffer[usHeadPtr];
		};

		inline void Pop() {
			if(usHeadPtr != usTailPtr) {
				usHeadPtr = (usHeadPtr + 1);
			}
		};

		inline void Push(word16 sVal) {
			if( usTailPtr < 4096) {
				sBuffer[usTailPtr] = sVal;
				usTailPtr++;
			}
		};

		inline bool EndOfData() const {
			return (usTailPtr < usHeadPtr) || IsEmpty();
		}

		inline void Purge() {
			usTailPtr = 0;
			usHeadPtr = 0;
		};

		inline void Rewind() {
			usHeadPtr = 0;
		}

		inline bool IsEmpty() const {
			return (usTailPtr == 0);
		}
	};


	class BIU {
	protected:
		AtlantisSubsystem* pParent;
		string label;
		IConnectedToBus* m_owner;

		ShuttleBus* pBus;
		unsigned short usTerminalID;
	public:
		BIU();
		BIU(unsigned short usTerminalID);
		virtual ~BIU();

		virtual BUS_COMMAND_WORD busCommand(BUS_COMMAND_WORD cw, unsigned long word_count, short* cdw) = 0;
		virtual void busCommandPhase() = 0;
		virtual void busReadPhase() = 0;

		void ConnectTo(ShuttleBus* pBus);

		/**
		 * Clear the communication queue.
		 */
		

		const string& GetLabel() const;
		AtlantisSubsystem* GetParent() const;
		inline short GetTerminalID() const {return usTerminalID;};

		void Init(AtlantisSubsystem* pSubsys,
			IConnectedToBus* owner,
			const string& _label, 
			unsigned short usTermID);
		virtual bool IsController() const;
		virtual bool IsTerminal() const;

		void Label(const string& _label);

		void SetParent(AtlantisSubsystem* pSubsys);		
		void SetTerminalID(unsigned short TermID);
	};



	class BusTerminal: public BIU
	{
	protected:
		FastQueue output_data;
	public:
		BusTerminal();
		BusTerminal(unsigned short usTerminalID);
		virtual ~BusTerminal();
		virtual void busCommandPhase();
		virtual void busReadPhase();
		bool Flush();
		virtual bool IsTerminal() const;
		virtual unsigned long ReadData(unsigned long num_data, 
			word16* data);
		bool Rewind();
		void WriteData(unsigned long num_data, word16* data);
		virtual BUS_COMMAND_WORD busCommand(BUS_COMMAND_WORD cw, 
			unsigned long word_count, word16* cdw);
	};

	class BusController: public BIU
	{
	public:
		BusController();
		BusController(unsigned short usTerminalID);
		virtual ~BusController();
		virtual BUS_COMMAND_WORD busCommand(BUS_COMMAND_WORD cw, unsigned long word_count, short* cdw);
		virtual void busCommandPhase();
		virtual void busReadPhase();
		virtual bool IsController() const;
		void Listen(bool fEnable = true);
		unsigned long ReadData(unsigned int mdm, unsigned long word_count,
			short* rdw);
	};

	
};