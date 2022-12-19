/****************************************************************************
  This file is part of Space Shuttle Ultra

  SSU Engineering constants



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

const double LBM2KG = 0.45359237;
const double MPS2FPS = 3.280839895;
const double MPS2KTS = 1.943844492;
const double MPS2MPH = 2.23693629; // meters per second to miles per hour
const double PA2PSF = 0.02088545;
const double PSI2PA = 6894.757;// pound per square inch to pascal
const double NMI2M = 1852.0; // nautical mile to meter
const double F2NM = 0.0001645784;// feet to nautical mile
const double LBF = 4.4482216152605; //lb(f) to Newton
const double INCH = 0.0254;

const double fps_to_ms=0.3048;
const double kg_to_pounds=2.205;

enum AXIS {PITCH, YAW, ROLL};

/**
 * Convert orbiter station coordinates (Xo, Yo, Zo) in inches to Orbiter local coordinates in meters
 *
 * \param Xo X-axis of the orbiter station coordinate system (pointing backwards)
 * \param Yo Y-axis of the orbiter station coordinate system (pointing right)
 * \param Zo Z-axis of the orbiter station coordinate system (pointing up)
 * \return Orbiter local coordinates, origin is still the origin of the station coordinate system.
 */
inline VECTOR3 _XYZO(double Xo, double Yo, double Zo) {
	return _V(Yo * INCH, Zo * INCH, -Xo * INCH);
}

/**
 * Convert a thrust vector defined in dynamic body coordinates (Xb, Yb, Zb) into a direction vector of for orbiter
 */
inline VECTOR3 _FXYZB_TO_DIRECTION(double FXb, double FYb, double FZb) {
	double total = sqrt(pow(FXb, 2) + pow(FYb, 2) + pow(FZb, 2));
	return _V(FYb / total, -FZb / total, FXb / total);
}