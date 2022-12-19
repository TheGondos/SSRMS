// AtlantisSubsystem.cpp: Implementierung der Klasse AtlantisSubsystem.
//
//////////////////////////////////////////////////////////////////////

#include "AtlantisSubsystem.h"
#include "SubsystemDirector.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

AtlantisSubsystem::AtlantisSubsystem(SubsystemDirector* _director, const string& _ident)
: director(_director), ident(_ident)
{
	char pszBuffer[300];
	oapiWriteLog("(AtlantisSubsystem::AtlantisSubsystem)");
	sprintf_s(pszBuffer, 300, "(AtlantisSubsystem) Create subsystem %s.", ident.c_str());
	oapiWriteLog(pszBuffer);
}

AtlantisSubsystem::~AtlantisSubsystem()
{

}

void AtlantisSubsystem::AddMeshes(const VECTOR3& ofs)
{
}

bool AtlantisSubsystem::RegisterComponent(SubsystemComponent* pComponent)
{
	return false;
}

bool AtlantisSubsystem::DelComponent(SubsystemComponent* pComponent)
{
	return false;
}

unsigned long AtlantisSubsystem::CountComponents() const
{
	return 0;
}

void AtlantisSubsystem::Realize()
{
}


bool AtlantisSubsystem::OnParseLine(const char* line)
{
	return false;
}

bool AtlantisSubsystem::OnParseLine(const char* key, const char* line)
{
	return false;
}

void AtlantisSubsystem::OnSaveState(FILEHANDLE scn) const
{

}

void AtlantisSubsystem::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{

}

void AtlantisSubsystem::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{

}

void AtlantisSubsystem::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
}

void AtlantisSubsystem::OnSubPostStep(double fSimT, double fDeltaT, double fMJD)
{

}

void AtlantisSubsystem::OnSubPreStep(double fSimT, double fDeltaT, double fMJD)
{

}

void AtlantisSubsystem::OnSubPropagate(double fSimT, double fDeltaT, double fMJD)
{
}

void AtlantisSubsystem::OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event)
{
}

bool AtlantisSubsystem::OnFail(const string& strFailCode)
{
	return false;
}

void AtlantisSubsystem::OnDumpToLog() const
{
}

const string& AtlantisSubsystem::GetIdentifier() const
{
	return this->ident;
}


const string& AtlantisSubsystem::GetQualifiedIdentifier() const
{
	return GetIdentifier();
}

Atlantis* AtlantisSubsystem::STS() const
{
	return director->STS();
}


DiscreteBundleManager* AtlantisSubsystem::BundleManager() const
{
	return director->BundleManager();
}

ShuttleBusManager* AtlantisSubsystem::BusManager() const {
	return director->BusManager();
}

void AtlantisSubsystem::UnloadSubsystem() {
	
}

bool AtlantisSubsystem::OnReadState (FILEHANDLE scn) {
	char* line;
	char pszKey[256];
	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "@ENDSUBSYSTEM", 13)) {
			return true;
		} else {
			if(SingleParamParseLine())
				OnParseLine(line);
			else {
				unsigned long i = 0;
				while(*line != ' ' && *line != '\0') {
					pszKey[i++] = *line;
					line++;
				}
				pszKey[i++] = '\0';
				if(*line != '\0') {
					OnParseLine(pszKey, line);
				} else {
					OnParseLine(pszKey, NULL);
				}
			}
		}
	}
	return false;
}

void AtlantisSubsystem::OnSetClassCaps()
{
}

EmptySubsystem::EmptySubsystem(SubsystemDirector* _director)
: AtlantisSubsystem(_director, "[Empty]") {
}

EmptySubsystem::~EmptySubsystem() {
}