/****************************************************************************
  This file is part of Space Shuttle Ultra

  GPC Software definition



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
#ifndef _GPCSOFTWARE_H
#define _GPCSOFTWARE_H

#include "dps_defs.h"
#include "../Atlantis.h"



namespace dps
{	
	
	typedef void (*GPCSCHEDULEDFUNC) (GPC*);

	class ScheduledFunction
	{
	public:
		ScheduledFunction(GPC* pGPC, double _period, GPCSCHEDULEDFUNC _function);
		void Step(double simt);
	private:
		GPCSCHEDULEDFUNC function;
		//Atlantis* sts;
		GPC* m_gpc;

		/**
		 * Sim time at which to call function
		 */
		double CallTime;
		/**
		 * Period between function calls
		 */
		double period;
	};

	class GPCSoftware
	{
	public:
		GPCSoftware(GPC* pGPC);
		~GPCSoftware();

		/**
		 * Called when software is loaded into GPC Memory. 
		 * Happens during memory config changes and 
		 * loading of Orbiter simulation state.
		 * Use this to define scheduled functions in GPC scheduler
		 */
		virtual void OnLoad();
		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual void OnPostStep(double SimT, double DeltaT, double MJD);

		static GPCSoftware* CreateSoftware(const string& _softname);
	protected:
		GPC* m_gpc;

		//scheduled GPC functions
		vector<ScheduledFunction*> ScheduledFunctions;

		void CreateScheduledFunction(double fPeriod, GPCSCHEDULEDFUNC func);
	};
};

#endif //!_GPSOFTWARE_H
