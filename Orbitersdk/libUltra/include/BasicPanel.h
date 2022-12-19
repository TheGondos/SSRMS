/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic panel definitions



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
#ifndef ___BASIC_PANEL_H_INCLUDED___
#define ___BASIC_PANEL_H_INCLUDED___

#include "OrbiterAPI.h"
#include <set>
#include <string>
#include <vector>
#include "BasicVCComponent.h"


namespace vc 
{

using namespace std;

enum PanelState {
	PS_UNKNOWN = 0,
	PS_CREATED,
	PS_DEFINED,
	PS_REGISTERED,
	PS_REALIZED
};


template <class TVessel>
class BasicPanel
{
	TVessel* pv;
	string name;
	
	set<UINT> availableForMouse;
	vector< BasicVCComponent<TVessel>* > components;

	bool bCoordinateDisplayMode;

	PanelState pstate;
protected:
	// mesh handles for panels with own mesh
	MESHHANDLE hMesh;
	UINT mesh_idx;
	VECTOR3 panelofs;

	bool bHasOwnVCMesh;

	void AddAIDToMouseEventList(UINT aid);
	
	void DeleteAllComponents();
	bool DistributeMouseEvent( UINT aid, int _event, const VECTOR3& p);
	bool FindComponent( UINT aid, const VECTOR3& p, BasicVCComponent<TVessel>** foundElement) const;

	virtual void DefineMesh( const char* meshname ){};
	virtual void DefineMesh( const char* meshname, const VECTOR3& panelofs ){};

public:
	BasicPanel(TVessel* v, const string& _name);
	virtual ~BasicPanel();
	TVessel* V() const;
	virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC() = 0;
	virtual UINT GetVCMeshIndex() const;
	virtual MESHHANDLE GetVCMeshHandle( void ) const;
	virtual void ReplaceComponent(unsigned long ulIndex, BasicVCComponent<TVessel>* pReplacement);

	virtual BasicVCComponent<TVessel>* Add(BasicVCComponent<TVessel>* pNew);
	
	virtual void AddMeshes( const VECTOR3& ofs ){};
	virtual void SetMeshVisibility( bool visible ){};
	virtual BasicVCComponent<TVessel>* GetSwitch(const string& switchID) const;
	virtual void Realize();
	virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
	virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	virtual void OnPreStep (double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual bool OnParseLine (char *line);
	virtual bool OnReadState (FILEHANDLE scn);
	virtual void OnSaveState (FILEHANDLE scn);
	
	virtual const string& GetIdentifier() const;
	virtual const string& GetQualifiedIdentifier() const;

	virtual bool HasSwitch(const string& switchID);

	bool HasOwnVCMesh() const {return bHasOwnVCMesh;};


	bool EnableCoordinateDisplayMode() {bCoordinateDisplayMode = true; return true;};
	bool DisableCoordinateDisplayMode() {bCoordinateDisplayMode = false; return true;};
	bool ToggleCoordinateDisplayMode() {bCoordinateDisplayMode = !bCoordinateDisplayMode; return true;};

	virtual void VisualCreated();
	inline void SetPanelState(PanelState s)
	{
		pstate = s;
	}

	inline PanelState GetPanelState() const 
	{
		return pstate;
	}
};

template <class TVessel>
BasicPanel<TVessel>::BasicPanel(TVessel* _v, const string& _name)
	: bHasOwnVCMesh(false)
{
	name = _name;
	pv = _v;
	bCoordinateDisplayMode = false;
	pstate = PS_CREATED;
}

template <class TVessel>
BasicPanel<TVessel>::~BasicPanel()
{
	DeleteAllComponents();
}

template <class TVessel>
BasicVCComponent<TVessel>* BasicPanel<TVessel>::Add(BasicVCComponent<TVessel>* pNew)
{
	pNew->SetParentPanel(this);
	components.push_back(pNew);
	return pNew;
}

template <class TVessel>
void BasicPanel<TVessel>::DeleteAllComponents() 
{
	unsigned long i = 0;
	for(; i<components.size(); i++)
	{
		delete components.at(i);
	}
}

template <class TVessel>
TVessel* BasicPanel<TVessel>::V() const
{
	return pv;
}

template <class TVessel>
void BasicPanel<TVessel>::DefineVCAnimations(UINT vcidx)
{
	char pszBuffer[256];
	static char buf[100];
	if (pstate != PS_CREATED)
	{
		sprintf_s(buf, 100, "(libUltra) [DEBUG] Panel state violation in %s, not created at DefineVCAnimations", this->GetQualifiedIdentifier().c_str());
		oapiWriteLog(buf);
	}

	sprintf_s(pszBuffer, 255, "BasicPanel[%s]:\tDefine VC Animations. %d components", 
		GetQualifiedIdentifier().c_str(), components.size());
	oapiWriteLog(pszBuffer);

	UINT idx = vcidx;
	if (HasOwnVCMesh()) idx = mesh_idx;// use own mesh, if it has one

	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->DefineVCAnimations( idx );
		iter++;
	}
	pstate = PS_DEFINED;
}

template <class TVessel>
bool BasicPanel<TVessel>::DistributeMouseEvent( UINT aid, int _event, const VECTOR3& p)
{
	unsigned long i;
	float mx = 0, my = 0;
	for(i = 0; i<components.size(); i++)
	{
		BasicVCComponent<TVessel>* currentElement = components.at(i);
		if (currentElement->GetMouseRegionID() == aid)
		{
			if(currentElement->IsPointOver((float)p.x, (float)p.y))
			{
				currentElement->ProjectMouse((float)p.x, (float)p.y, mx, my);	
				if(currentElement->OnMouseEvent(_event, mx, my))
					return true;
			}
		}
	}
	return false;
}

template <class TVessel>
bool BasicPanel<TVessel>::FindComponent( UINT aid, const VECTOR3& p, BasicVCComponent<TVessel>** foundElement) const
{
	unsigned long i;
	for(i = 0; i<components.size(); i++)
	{
		BasicVCComponent<TVessel>* currentElement = components.at(i);
		if (currentElement->GetMouseRegionID() == aid)
		{
			if(currentElement->IsPointOver((float)p.x, (float)p.y))
			{
				*foundElement = currentElement;
				return true;
			}
		}
	}
	return false;
}

template <class TVessel>
BasicVCComponent<TVessel>* BasicPanel<TVessel>::GetSwitch(const string& switchID) const {
	unsigned long i;
	for(i = 0; i<components.size(); i++)
	{
		BasicVCComponent<TVessel>* currentElement = components.at(i);
		if(currentElement->GetIdentifier() == switchID)
		{
			return currentElement;
		}
	}
	return NULL;
}

template <class TVessel>
UINT BasicPanel<TVessel>::GetVCMeshIndex() const
{
	return mesh_idx;
}

template <class TVessel>
MESHHANDLE BasicPanel<TVessel>::GetVCMeshHandle( void ) const
{
	return hMesh;
}

template <class TVessel>
void BasicPanel<TVessel>::RegisterVC()
{
	static char buf[100];
	if ((pstate != PS_REALIZED) && (pstate != PS_REGISTERED))
	{
		sprintf_s(buf, 100, "(libUltra) [DEBUG] Panel state violation in %s, not realized at RegisterVC()", this->GetQualifiedIdentifier().c_str());
		oapiWriteLog(buf);
	}

	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->RegisterVC();
		iter++;
	}

	pstate = PS_REGISTERED;
}

template <class TVessel>
void BasicPanel<TVessel>::Realize()
{
	static char buf[100];
	if (pstate != PS_DEFINED)
	{
		sprintf_s(buf, 100, "(libUltra) [DEBUG] Panel state violation in %s, not defined at Realize()", this->GetQualifiedIdentifier().c_str());
		oapiWriteLog(buf);
	}
	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->Realize();
		iter++;
	}

	pstate = PS_REALIZED;
}

template <class TVessel>
bool BasicPanel<TVessel>::OnParseLine(char *line)
{
	return false;
}

template <class TVessel>
bool BasicPanel<TVessel>::OnReadState (FILEHANDLE scn) {
	char* line;
	char pszBuffer[256];
	char pszBuffer2[256];
	sprintf_s(pszBuffer, 255, "\t\tParse panel block for panel \"%s\"...",
		GetQualifiedIdentifier().c_str());
	oapiWriteLog(pszBuffer);
	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "@ENDPANEL", 9)) {
			oapiWriteLog("\t\tDone.");
			return true;
		} else if(!_strnicmp(line, "@OBJECT", 7)) {
			oapiWriteLog("\t\tEnter Multiline switch block...");
			//Multi line object block
			//Get identifier of object or switch
			line += 8;
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
			//Look up object
			sprintf_s(pszBuffer2, "\tLook up object \"%s\"...", 
				pszBuffer);
			oapiWriteLog(pszBuffer2);
			//
			if(HasSwitch(pszBuffer)) {
				BasicVCComponent<TVessel>* pT = this->GetSwitch(pszBuffer);
				pT->OnReadState(scn);
			} else {
				//skip block
				while(oapiReadScenario_nextline(scn, line)) {
					if(!_strnicmp(line, "@ENDOBJECT", 10))
						break;
				}
			}
			oapiWriteLog("\t\tLeave Multiline switch block...");
		} else {
			//single line object block
			//oapiWriteLog("\t\tEnter single line switch block...");
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
			line++;

			//Look up object
			sprintf_s(pszBuffer2, "\tLook up switch \"%s\"...\t\t(%s)", 
				pszBuffer, line);
			oapiWriteLog(pszBuffer2);
			

			if(HasSwitch(pszBuffer)) {
				oapiWriteLog("\tFound switch.");
				BasicVCComponent<TVessel>* pT = this->GetSwitch(pszBuffer);
				if(pT != NULL) {
					if(!pT->IsMultiLineSaveState()) {
						
						oapiWriteLog("\tRead switch state.");
						pT->OnParseLine(line);
					}
				}
			}

			//oapiWriteLog("\t\tLeave single line switch block...");
		}	
	}
	oapiWriteLog("\t\tParsing error.");
	return false;
}

template <class TVessel>
void BasicPanel<TVessel>::OnSaveState(FILEHANDLE scn)
{
	char pszBuffer[256];
	char pszBuffer2[256];

	if(components.empty()) {
		return;
	}

	strcpy_s(pszBuffer, 255, GetIdentifier().c_str());
	oapiWriteScenario_string(scn, "@PANEL", pszBuffer);
	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end()) {
		if((*iter)->IsMultiLineSaveState()) {
			sprintf_s(pszBuffer, 255, "\"%s\"", 
					(*iter)->GetIdentifier().c_str());
			oapiWriteScenario_string(scn, "@OBJECT", 
				pszBuffer);
			(*iter)->OnSaveState(scn);
			oapiWriteLine(scn, "  @ENDOBJECT");
		} else {
			if((*iter)->GetStateString(255, pszBuffer)) {
				sprintf_s(pszBuffer2, 255, "\"%s\"", 
					(*iter)->GetIdentifier().c_str());
				oapiWriteScenario_string(scn, 
					pszBuffer2,
					pszBuffer);
			}
		}
		iter++;
	}

	oapiWriteScenario_string(scn, "@ENDPANEL", "");
}

template <class TVessel>
bool BasicPanel<TVessel>::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
{
	if(availableForMouse.find(id) != availableForMouse.end()) {
		if(bCoordinateDisplayMode)
		{
			BasicVCComponent<TVessel>* foundElement;
			if(FindComponent( id, p, &foundElement))
			{
				sprintf_s(oapiDebugString(), 255, "PANEL %s (%d) MOUSEEVENT (%d, %f, %f, %f) FOR %s",
					name.c_str(), id, _event, p.x, p.y, p.z,
					foundElement->GetQualifiedIdentifier().c_str());
			}
			else
			{
				sprintf_s(oapiDebugString(), 255, "PANEL %s (%d) MOUSEEVENT (%d, %f, %f, %f)",
					name.c_str(), id, _event, p.x, p.y, p.z);
			}
			return DistributeMouseEvent( id, _event, p);
		} else {
			
			return DistributeMouseEvent( id, _event, p);
		}
	}
	else {
		return false;
	}
}

template <class TVessel>
bool BasicPanel<TVessel>::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
{
	bool bRedraw=false;

	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		BasicVCComponent<TVessel>* comp = (*iter);
		if(comp->IsOwnRegion(id)) {
			if(comp->OnVCRedrawEvent(id, _event, surf)) bRedraw=true;
		}

		iter++;
	}
	return bRedraw;
}

template <class TVessel>
void BasicPanel<TVessel>::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{
	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->OnPreStep(fSimT, fDeltaT, fMJD);
		iter++;
	}
}

template <class TVessel>
void BasicPanel<TVessel>::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{
	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->OnPostStep(fSimT, fDeltaT, fMJD);
		iter++;
	}
}

template <class TVessel>
void BasicPanel<TVessel>::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->OnPropagate(fSimT, fDeltaT, fMJD);
		iter++;
	}
}

template <class TVessel>
bool BasicPanel<TVessel>::HasSwitch(const string& id)
{
	unsigned long i;
	for(i = 0; i<components.size(); i++)
	{
		BasicVCComponent<TVessel>* currentElement = components.at(i);
		if(currentElement->GetIdentifier() == id)
		{
			return true;
		}
	}
	return false;
}

template <class TVessel>
const string& BasicPanel<TVessel>::GetIdentifier() const
{
	//static string null = "";
	return name;
}

template <class TVessel>
const string& BasicPanel<TVessel>::GetQualifiedIdentifier() const
{
	//static string null = "";
	return name;
}

template <class TVessel>
void BasicPanel<TVessel>::ReplaceComponent(unsigned long ulIndex, BasicVCComponent<TVessel>* pReplacement)
{
	if(ulIndex < components.size())
	{
		components.at(ulIndex) = pReplacement;
	}
}

template <class TVessel>
void BasicPanel<TVessel>::AddAIDToMouseEventList(UINT aid)
{
	if(availableForMouse.empty())
	{
		availableForMouse.insert(aid);
		return;
	}
	if(availableForMouse.find(aid) == availableForMouse.end())
	{
		availableForMouse.insert(aid);
	}
}

template <class TVessel>
void BasicPanel<TVessel>::VisualCreated()
{
	vector< BasicVCComponent<TVessel>* >::iterator iter = components.begin();
	while(iter != components.end())
	{
		(*iter)->VisualCreated();
		iter++;
	}
}

};

#endif