/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel Group definition



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

#include "OrbiterAPI.h"
#include <vector>
#include "BasicPanel.h"
//#include "vc_defs.h"

namespace vc {
	using namespace std;

	template <class TVessel>
	class PanelGroup {
		vector< BasicPanel<TVessel>* > panels;
	public:
		PanelGroup();
		virtual ~PanelGroup();

		virtual bool AddPanel(BasicPanel<TVessel>* pPanel);
		void RegisterVC();
		void DefineVCAnimations (UINT vcidx);
		void DefineVC();
		bool HasPanel(const string& panelname) const;
		bool ParsePanelBlock(const string& panelname, FILEHANDLE scn);
		void Realize();
		bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
		void OnPreStep (double fSimT, double fDeltaT, double fMJD);
		void OnPostStep (double fSimT, double fDeltaT, double fMJD);
		void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		bool OnParseLine (char *line);
		void OnSaveState (FILEHANDLE scn);
		bool EnableCoordinateDisplayMode();
		bool DisableCoordinateDisplayMode();
		bool ToggleCoordinateDisplayMode();
		void HidePanels();
		void ShowPanels();
		void LogPanels(char *grp);
		void VisualCreated();
		void AddMeshes( const VECTOR3& ofs );
	};

	template <class TVessel>
	PanelGroup<TVessel>::PanelGroup()
	{
	}

	template <class TVessel>
	PanelGroup<TVessel>::~PanelGroup()
	{
		while(!panels.empty()) {
			BasicPanel<TVessel>* panel=panels.back();
			if(panel) delete panel;
			panels.pop_back();
		}
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::AddPanel(BasicPanel<TVessel>* pPanel)
	{
		panels.push_back(pPanel);
		return true;
	}

	template <class TVessel>
	void PanelGroup<TVessel>::DefineVCAnimations(UINT vcidx)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->DefineVCAnimations(vcidx);
	}

	template <class TVessel>
	void PanelGroup<TVessel>::DefineVC()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->DefineVC();
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::HasPanel(const string& panelname) const {
		for(unsigned int i = 0; i<panels.size(); i++) {
			if(panels.at(i)->GetIdentifier() == panelname) {
				return true;
			}
		}
		return false;
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::ParsePanelBlock(const string& panelname, FILEHANDLE scn) {
		
		for(unsigned int i = 0; i<panels.size(); i++) {
			if(panels.at(i)->GetIdentifier() == panelname) {
				return panels.at(i)->OnReadState(scn);
			}
		}
		oapiWriteLog("\tNo Panel found here!");
		return false;
	}

	template <class TVessel>
	void PanelGroup<TVessel>::RegisterVC()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->RegisterVC();
	}

	template <class TVessel>
	void PanelGroup<TVessel>::Realize()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->Realize();
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::OnParseLine(char *line)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
		{
			//Test syntax of line
			panels.at(i)->OnParseLine(line);
		}
		return false;
	}

	template <class TVessel>
	void PanelGroup<TVessel>::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->OnPostStep(fSimT, fDeltaT, fMJD);
	}

	template <class TVessel>
	void PanelGroup<TVessel>::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->OnPreStep(fSimT, fDeltaT, fMJD);
	}

	template <class TVessel>
	void PanelGroup<TVessel>::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->OnPropagate(fSimT, fDeltaT, fMJD);
	}

	template <class TVessel>
	void PanelGroup<TVessel>::OnSaveState(FILEHANDLE scn)
	{
		char pszBuffer[256];
		if(panels.empty()) {
			oapiWriteLog("\tNo panels to be saved here...");
			return;
		}
		for(unsigned int i = 0; i<panels.size(); i++) {
			sprintf_s(pszBuffer, 255, "\tSave panel \"%s\" ...",
				panels.at(i)->GetQualifiedIdentifier().c_str());
			oapiWriteLog(pszBuffer);
			panels.at(i)->OnSaveState(scn);
		}
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{
		bool r = false;
		for (unsigned int i = 0; i < panels.size(); i++) 
		{
			r |= panels.at(i)->OnVCMouseEvent(id, _event, p);
		}
		return r;
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		bool r = false;
		for(unsigned int i = 0; i<panels.size(); i++)
			r |= panels.at(i)->OnVCRedrawEvent(id, _event, surf);
		return r;
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::EnableCoordinateDisplayMode()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->EnableCoordinateDisplayMode();
		return true;
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::DisableCoordinateDisplayMode()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->DisableCoordinateDisplayMode();
		return true;
	}

	template <class TVessel>
	bool PanelGroup<TVessel>::ToggleCoordinateDisplayMode()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->ToggleCoordinateDisplayMode();
		return true;
	}

	template <class TVessel>
	void PanelGroup<TVessel>::HidePanels()
	{
		for(unsigned int i=0;i<panels.size();i++)
			panels.at(i)->SetMeshVisibility(false);
	}

	template <class TVessel>
	void PanelGroup<TVessel>::ShowPanels()
	{
		for(unsigned int i=0;i<panels.size();i++)
			panels.at(i)->SetMeshVisibility(true);
	}

	template <class TVessel> 
	void PanelGroup<TVessel>::LogPanels(char * panelgroup) {
		static char buf[100];

		char *state;

		sprintf_s(buf, 100, "Panel group dump for group \"%s\": ", panelgroup);
		oapiWriteLog(buf);

		for (unsigned int i = 0; i < panels.size(); i++)
		{
			switch (panels.at(i)->GetPanelState()) 
			{
			case PS_CREATED:
				state = "CREATED";
				break;
			case PS_DEFINED:
				state = "DEFINED";
				break;
			case PS_REGISTERED:
				state = "REGISTERED";
				break;
			case PS_REALIZED:
				state = "REALIZED";
				break;
			case PS_UNKNOWN:
			default:
				state = "UNKNOWN";
			}
			sprintf_s(buf, 100, "   %s  : %s ", panels.at(i)->GetQualifiedIdentifier().c_str(), state);
			oapiWriteLog(buf);
		}
	}

	template <class TVessel>
	void PanelGroup<TVessel>::VisualCreated()
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->VisualCreated();
	}

	template <class TVessel>
	void PanelGroup<TVessel>::AddMeshes( const VECTOR3& ofs )
	{
		for(unsigned int i = 0; i<panels.size(); i++)
			panels.at(i)->AddMeshes( ofs );
	}
};