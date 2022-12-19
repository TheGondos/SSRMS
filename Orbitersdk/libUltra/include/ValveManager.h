/****************************************************************************
  This file is part of Space Shuttle Ultra

  Valve manager definition



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
#ifndef _VALVEMANAGER_H_
#define _VALVEMANAGER_H_


#include <vector>
#include <string>
#include "BasicValve.h"


class ValveManager
{
		vector <BasicValve*> valves;
	public:
		ValveManager( void );
		virtual ~ValveManager( void );

		void AddValve( BasicValve *pValve );

		bool OnParseLine( const char* line );
		void OnSaveState( FILEHANDLE scn ) const;
		
		void OnPreStep( double fSimT, double fDeltaT, double fMJD );
		void OnPostStep( double fSimT, double fDeltaT, double fMJD );
};


#endif// _VALVEMANAGER_H_
