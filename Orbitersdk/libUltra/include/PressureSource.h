/****************************************************************************
  This file is part of Space Shuttle Ultra

  Pressure Source Definition



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
#ifndef _g_PressureSource_H_
#define _g_PressureSource_H_


class PressureSource
{
	public:
		PressureSource( void ){};
		virtual ~PressureSource( void ){};

		virtual double Use( double flow, int source = 0 ) = 0;
};


#endif// _g_PressureSource_H_
