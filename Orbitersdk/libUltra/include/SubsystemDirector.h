/****************************************************************************
  This file is part of Space Shuttle Ultra

  Subsystem Director



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
#ifndef ___ULTRA_SUBSYSTEM_DIRECTOR_H___
#define ___ULTRA_SUBSYSTEM_DIRECTOR_H___

#include <vector>
#include "OrbiterAPI.h"
//#include "dps/dps_defs.h"
#include "DiscreteBundleManager.h"
//#include "Atlantis.h"

using namespace std;

//class Atlantis;
//class AtlantisSubsystem;
template <class TVessel>
class Subsystem;

using discsignals::DiscreteBundleManager;
//using class dps::ShuttleBusManager;
template <class TVessel>
class SubsystemDirector
{
protected:
	TVessel* pV;
	vector< Subsystem<TVessel>* > subsystems;
public:
	virtual TVessel* V() const;
	SubsystemDirector(TVessel* _v);
	virtual ~SubsystemDirector();
	bool AddSubsystem(Subsystem<TVessel>* pSubsys);
	Subsystem<TVessel>* ReplaceSubsystem(Subsystem<TVessel>* pCurrentSubsys, Subsystem<TVessel>* pBySubsys);
	bool ExistsSubsystem(const std::string& name) const;
	Subsystem<TVessel>* GetSubsystemByName(const std::string& name) const;

	double GetTotalSubsystemMass() const;

	void SetClassCaps(FILEHANDLE cfg);
	bool ParseScenarioLine(FILEHANDLE scn, char* line);
	bool PlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
	bool SaveState(FILEHANDLE scn);
	bool PostStep(double fSimT, double fDeltaT, double fMJD);
	bool PreStep(double fSimT, double fDeltaT, double fMJD);
	bool Report() const;
	bool WriteLog(const Subsystem<TVessel>* src, char* message);
	void Animate( void );

	
	/**
	 * 
	 */
	bool RealizeAll();
};

template <class TVessel>
SubsystemDirector<TVessel>::SubsystemDirector(TVessel* _v)
: pV(_v)
{
}

template <class TVessel>
SubsystemDirector<TVessel>::~SubsystemDirector()
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		delete subsystems[i];
	}
}

template <class TVessel>
bool SubsystemDirector<TVessel>::AddSubsystem(Subsystem<TVessel>* pSubsys)
{
	char pszBuffer[256];
	subsystems.push_back(pSubsys);
	sprintf_s(pszBuffer, 256, "Added subsystem %s.", pSubsys->GetQualifiedIdentifier().c_str());
	oapiWriteLog(pszBuffer);
	return true;
}

template <class TVessel>
Subsystem<TVessel>* SubsystemDirector<TVessel>::ReplaceSubsystem(Subsystem<TVessel>* pCurrentSubsys, Subsystem<TVessel>* pBySubsys)
{
	typename vector< Subsystem<TVessel>* >::iterator iter = std::find(subsystems.begin(),
		subsystems.end(), pCurrentSubsys);

	if(iter != subsystems.end()) {

		pCurrentSubsys->UnloadSubsystem();
		*iter = pBySubsys;

		oapiWriteLogV( "Replaced subsystem %s by subsystem %s.",
			pCurrentSubsys->GetQualifiedIdentifier().c_str(),
			pBySubsys->GetQualifiedIdentifier().c_str() );

		delete pCurrentSubsys;

		oapiWriteLog( "Finished clean up." );
		return pBySubsys;
	}
	return pCurrentSubsys;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::ExistsSubsystem(const std::string& name) const
{
	auto iter = subsystems.begin();
	while(iter != subsystems.end())
	{
		if((*iter)->GetIdentifier() == name) {
			return true;
		}
		++iter;
	}
	return false;
}

template <class TVessel>
Subsystem<TVessel>* SubsystemDirector<TVessel>::GetSubsystemByName(const std::string& name) const
{
	auto iter = subsystems.begin();
	while(iter != subsystems.end())
	{
		if((*iter)->GetIdentifier() == name) {
			return (*iter);
		}
		++iter;
	}
	return NULL;
}

template <class TVessel>
double SubsystemDirector<TVessel>::GetTotalSubsystemMass() const
{
	double mass = 0.0;
	auto iter = subsystems.begin();
	while(iter != subsystems.end())
	{
		mass += (*iter)->GetSubsystemEmptyMass();
		++iter;
	}
	return mass;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::RealizeAll() {
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->Realize();
	}
	return true;
}

template <class TVessel>
void SubsystemDirector<TVessel>::SetClassCaps(FILEHANDLE cfg)
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnSetClassCaps();
	}
}
#include <cstring>
template <class TVessel>
bool SubsystemDirector<TVessel>::ParseScenarioLine(FILEHANDLE scn, char* line)
{
	char pszBuffer[400];
	if(!strncasecmp(line, "@SUBSYSTEM", 10)) {
		oapiWriteLog(line);
		line += 11;
		unsigned long i = 0;
		bool bStringFlag = false;
		while((*line != '\0' && *line != ' ') || bStringFlag) {
			if(*line == '\"') {
				bStringFlag = !bStringFlag;
			} else {
				pszBuffer[i] = *line;
				i++;
			}
			line++;
		}

		pszBuffer[i] = '\0';

		if(ExistsSubsystem(pszBuffer)) {
			Subsystem<TVessel>* pT = GetSubsystemByName(pszBuffer);
			pT->OnReadState(scn);
		} else {
			//skip block
			//oapiWriteLog(pszBuffer);
			oapiWriteLog("Skip block");
			while(oapiReadScenario_nextline(scn, line)) {
				if(!strncasecmp(line, "@ENDSUBSYSTEM", 13))
					break;
			}
		}
	} else {
		for(unsigned long i = 0; i<subsystems.size(); i++)
		{
			//
			if(subsystems[i]->OnParseLine(line))
			{
				return true;
			}
		}
	}
	return false;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::PlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event)
{
	return false;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::SaveState(FILEHANDLE scn)
{
	unsigned long i;
	char pszBuffer[400];
	for(i = 0; i<subsystems.size(); i++)
	{
		sprintf_s(pszBuffer,"@SUBSYSTEM %s", subsystems[i]->GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);
		oapiWriteLine(scn, pszBuffer);

		subsystems[i]->OnSaveState(scn);

		sprintf_s(pszBuffer,"@ENDSUBSYSTEM\t\t;%s", subsystems[i]->GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);
		oapiWriteLine(scn, pszBuffer);
	}
	return true;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::PostStep(double fSimT, double fDeltaT, double fMJD)
{
	unsigned long i;
	//const double SUBSAMPLING_DELTAT = 0.0005;	//0.5 ms
	/*const double SUBSAMPLING_DELTAT = 0.04;	//40 ms
	

	//Subsampling pass
	double t0 = fSimT;
	long lSubCount = 0;

	while(t0 < fSimT + fDeltaT) {
		double dt = min(SUBSAMPLING_DELTAT, fSimT + fDeltaT - t0);
		for(i = 0; i<subsystems.size(); i++)
		{	
			subsystems[i]->OnSubPreStep(t0, dt, fMJD);
		}
		//if(BusManager() != NULL)
			//BusManager()->clbkPropagate(t0, dt);
		for(i = 0; i<subsystems.size(); i++)
		{
			//
			subsystems[i]->OnSubPropagate(t0, dt, fMJD);
		}


		for(i = 0; i<subsystems.size(); i++)
		{	
			subsystems[i]->OnSubPostStep(t0, dt, fMJD);
		}
		t0 += SUBSAMPLING_DELTAT;
		lSubCount ++;
		//measure average time for the subsampling during this time step
	}*/

	//sprintf_s(oapiDebugString(), 255, "%d SUBSAMPLING STEPS @ %5.2f us", lSubCount, tsf);

	//Propagate subsystem states to the end of the discrete timestep
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnPropagate(fSimT, fDeltaT, fMJD);
	}
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnPostStep(fSimT, fDeltaT, fMJD);
	}
	return true;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::PreStep(double fSimT, double fDeltaT, double fMJD)
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnPreStep(fSimT, fDeltaT, fMJD);
	}
	return true;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::Report() const
{
	/*
	unsigned int i;
	time_t t0;

	t0 = time(NULL);
	char *buffer = ctime(&t0);
	//Create report file
	std::ofstream report;
	report.open("ssu-subsys-report.html");
	report << "<!doctype html>" << std::endl;
	report << "<html>" << std::endl << "<head>" << std::endl;
	report << "<title>SSU subsystem report " << 
		STS()->GetName() << "</title>" << std::endl;
	report << "</head>" << std::endl;

	report << "<body>" << std::endl;
	report << "<header>" << buffer << "</header>" << std::endl;
	
	report << "<h1>SSU subsystem report</h1>" << std::endl;
	report << "<h2>List of existing subsystems</h2>" << std::endl;

	report << "<ol>" << std::endl;

	for(i = 0; i<subsystems.size(); i++)
	{
		report << "<li>" << subsystems[i]->GetIdentifier() << "</li>"
			<< std::endl;
	}

	report << "</ol>" << std::endl;

	report << "</body>" << std::endl << "</html>" << std::endl;
	report.close();*/
	return true;
}

template <class TVessel>
bool SubsystemDirector<TVessel>::WriteLog(const Subsystem<TVessel>* src, char* message)
{
	return true;
}

template <class TVessel>
void SubsystemDirector<TVessel>::Animate( void )
{
	for (unsigned int i = 0; i < subsystems.size(); i++) subsystems[i]->Animate();
	return;
}

template <class TVessel>
TVessel* SubsystemDirector<TVessel>::V() const
{
	return pV;
}

#endif
