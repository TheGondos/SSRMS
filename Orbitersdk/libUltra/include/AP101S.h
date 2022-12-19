/****************************************************************************
  This file is part of Space Shuttle Ultra

  AP-101S simulation definition



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
#if !defined(AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_)
#define AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\AtlantisSubsystem.h"
#include "GPC.h"
#include "RAM32.h"

namespace dps {

	class GPCMemoryConfiguration;
	class GPCSoftware;

	class AP101S : public GPC  
	{
	
	// ===================================================
	// Random access memory
	// Important variables are stored inside the lower 
	// 64 KB (SECTION 0) of the memory
	// ===================================================
		//long lMemory[262144];
		
	// ===================================================
	// General submodules: IOP
	// ===================================================
	
	// IOP software is storing data in the memory and
	// transmits data half words from the memory over 
	// the MIAs. 
	protected:
		RAM32S memory;
		/**
		 * Fill the first memory page (4096 byte) by PROM data with 
		 * information about how to find the IPL.
		 */
		virtual void GeneratePROMData();
		//void LoadMemoryConfiguration(const GPCMemoryConfiguration* pMemConfig);
		//void LoadSoftware(const GPCSoftware* pSoftware);
	public:
		AP101S(SubsystemDirector* _director, const string& _ident, unsigned short usGPCID);
		virtual ~AP101S();
	};

};

#endif // !defined(AFX_AP101S_H__01BE9789_9E06_4F6B_9FAD_F48EA99C5161__INCLUDED_)
