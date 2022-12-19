/****************************************************************************
  This file is part of Space Shuttle Ultra

  16 bit RAM definition



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

#include <OrbiterAPI.h>
#include "dps_defs.h"
#include <string>


namespace dps {

	const unsigned long AP101S_MEMORY_SIZE = 262144;

	
	class RAM32
	{
	public:
		RAM32();
		virtual ~RAM32();
		virtual unsigned long GetNumberOfWords() const = 0;
		virtual word16 GetWord(unsigned long ulAddress) const = 0;
		virtual word32 GetDWord(unsigned long ulAddress) const = 0;
		virtual float32 GetFloat(unsigned long ulAddress) const = 0;
		virtual void SetWord(unsigned long ulAddress, word16 sword) = 0;
		virtual void SetDWord(unsigned long ulAddress, word32 dword) = 0;
		virtual void SetFloat(unsigned long ulAddress, float32 fword) = 0;
		virtual void OnParseLine(char* line);
		virtual void OnSaveState(FILEHANDLE scn);
		virtual void ReadImage(const std::string& strFileName);
		virtual void WriteImage(const std::string& strFileName) const;
	};

	//Implementation for RAM32 RAM for the AP-101S class GPCs.

	class RAM32S:public RAM32 {
		MEMORY_WORD m_memory[AP101S_MEMORY_SIZE];
	public:
		RAM32S();
		virtual ~RAM32S();
		virtual unsigned long GetNumberOfWords() const;
		virtual word16 GetWord(unsigned long ulAddress) const;
		virtual word32 GetDWord(unsigned long ulAddress) const;
		virtual float32 GetFloat(unsigned long ulAddress) const;
		virtual void SetWord(unsigned long ulAddress, word16 sword);
		virtual void SetDWord(unsigned long ulAddress, word32 dword);
		virtual void SetFloat(unsigned long ulAddress, float32 fword);
	};
};