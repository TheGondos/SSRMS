/****************************************************************************
  This file is part of Space Shuttle Ultra

  Mass Memory Unit definition



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

namespace dps {

	class MMU: public AtlantisSubsystem {
	public:
		MMU(SubsystemDirector* _direct, const string& _ident);
		virtual ~MMU();
		BIU channel;
		DiscInPort power_on;
	};
};