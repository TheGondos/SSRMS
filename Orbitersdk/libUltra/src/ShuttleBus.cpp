// ShuttleBus.cpp: Implementierung der Klasse ShuttleBus.
//
//////////////////////////////////////////////////////////////////////

#include <OrbiterAPI.h>
#include <ShuttleBus.h>
#include <BIU.h>
#include <Subsystem.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

	ShuttleBus::ShuttleBus(const string& _ident)
		:ident( _ident)
	{
		
	}

	ShuttleBus::~ShuttleBus()
	{
	
	}

	void ShuttleBus::busPrePhase()
	{
		std::set<BusTerminal*>::iterator iter = terminals.begin();
		while(iter!= terminals.end())
		{
			(*iter)->Flush();
			iter++;
		}
	}

	void ShuttleBus::busCommandPhase()
	{
		std::set<BusController*>::iterator iter = controllers.begin();
		while(iter!= controllers.end())
		{
			(*iter)->busCommandPhase();
			iter++;
		}
	}

	void ShuttleBus::busReadPhase()
	{
		std::set<BusController*>::iterator iter = controllers.begin();
		while(iter!= controllers.end())
		{
			(*iter)->busReadPhase();
			iter++;
		}
	}

	void ShuttleBus::ConnectTo(BIU* pTarget) {
#if defined(___SSU_LOG_BUS_CONNECTION_EVENTS___)
		char pszBuffer[255];
		char pszBIABuffer[255];
#endif
		pTarget->ConnectTo(this);
		connections.insert(pTarget);
		if(pTarget->IsController())
			controllers.insert(static_cast<BusController*>(pTarget));
		if(pTarget->IsTerminal())
			terminals.insert(static_cast<BusTerminal*>(pTarget));

#if defined(___SSU_LOG_BUS_CONNECTION_EVENTS___)
		if(pTarget->GetParent() != NULL) {

			sprintf_s(pszBIABuffer, 255, "%s-%s", 
				pTarget->GetParent()->GetQualifiedIdentifier().c_str(),
				pTarget->GetLabel().c_str());
		} else {
			sprintf_s(pszBIABuffer, 255, "%s", 
				pTarget->GetLabel().c_str());
		}
		sprintf_s(pszBuffer, 255, "ShuttleBus::ConnectTo(): Connect bus \"%s\" to \"%s\"",
			GetIdent().c_str(),
			pszBIABuffer);
		oapiWriteLog(pszBuffer);		
#endif
	}


	const string& ShuttleBus::GetIdent() const {
		return ident;
	}

	BUS_COMMAND_WORD ShuttleBus::busCommand(BUS_COMMAND_WORD cw, unsigned long word_count, short *cdw)
	{
		if(m_lookup_table[cw.mdm_no] != NULL)
		{
			return m_lookup_table[cw.mdm_no]->busCommand(cw, word_count, cdw);
		} else {
			return _CW(0,0,0,0);
		}
	}

	/*
	long ShuttleBus::Transmit(unsigned short usTarget, unsigned short usNumData, short usData[]) {
		//Create transmit frame
		return 0;
	}*/
	/*
	unsigned short ShuttleBus::Transmit(bool toMDM, short sDataCount, word24 dataw[])
	{
		//Send xmit frame to target system
		if(toMDM)
		{
			std::set<BIU*>::iterator iter = connections.begin();
			while(iter != connections.end())
			{
				if(!(*iter)->IsBusMaster())
				{
					(*iter)->OnReceive(toMDM, sDataCount, dataw);
				}
				iter++;
			}
		} else 
		{
			std::set<BIU*>::iterator iter = connections.begin();
			while(iter != connections.end())
			{
				if((*iter)->IsBusMaster())
				{
					(*iter)->OnReceive(toMDM, sDataCount, dataw);
				}
				iter++;
			}
		}
		return 0;
	}

	*/

	ShuttleBusManager::ShuttleBusManager() {
	}

	ShuttleBusManager::~ShuttleBusManager() {
		vector<ShuttleBus*>::iterator iter = busses.begin();
		while(iter != busses.end()) {
			delete (*iter);
			iter++;
		}
	}

	void ShuttleBusManager::clbkPropagate(double fSimT, double fDeltaT)
	{
		vector<ShuttleBus*>::iterator iter = busses.begin();
		while(iter != busses.end()) {
			if(*iter != NULL)
				(*iter)->busPrePhase();
			iter++;
		}

		iter = busses.begin();
		while(iter != busses.end()) {
			if(*iter != NULL)
				(*iter)->busCommandPhase();
			iter++;
		}

		iter = busses.begin();
		while(iter != busses.end()) {
			if(*iter != NULL)
				(*iter)->busReadPhase();
			iter++;
		}
	}

	ShuttleBus* ShuttleBusManager::CreateBus(const string& _ident) {
		char pszBuffer[255];
		ShuttleBus* m_r = new ShuttleBus(_ident);
		busses.push_back(m_r);
		sprintf(pszBuffer, "(ShuttleBusManager::CreateBus) Create bus \"%s\"",
			_ident.c_str());
		oapiWriteLog(pszBuffer);
		return m_r;
	}

	ShuttleBus* ShuttleBusManager::FindBus(const string& _ident) const {
		vector<ShuttleBus*>::const_iterator iter = busses.begin();
		while(iter != busses.end()) {
			if((*iter)->GetIdent() == _ident) {
				return ( *iter );
			}
			iter++;
		}
		return NULL;
	}

	ShuttleBus* ShuttleBusManager::GetBus(const std::string &_ident) {
		ShuttleBus* m_r = FindBus(_ident);
		if(m_r != NULL) {
			return m_r;
		} else {
			return CreateBus(_ident);
		}
	}

};
