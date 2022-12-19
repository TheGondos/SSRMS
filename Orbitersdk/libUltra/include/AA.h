/****************************************************************************
  This file is part of Space Shuttle Ultra

  STS Orbiter vessel class definition



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
#include "../discsignals/DiscOutPort.h"

namespace gnc {

	using class discsignals::DiscOutPort;

	class AccAssembly: AtlantisSubsystem
	{
		VECTOR3 ref;
		VECTOR3 vY; //lateral acceleration
		VECTOR3 vZ; //normal acceleration
		DiscOutPort outY;
		DiscOutPort outZ;
	public:
		/**
		 * @param direct Reference to the responsible SubsystemDirector
		 * @param ident Identification of the AA
		 * @param ref Reference position of the assembly in local 
		 * vessel coordinates
		 */
		AccAssembly(SubsystemDirector* direct, const string& ident, 
			const VECTOR3& ref);
		virtual ~AccAssembly();

	};

};