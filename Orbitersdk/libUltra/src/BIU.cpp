#include "dps_defs.h"
#include "BIU.h"
#include "ShuttleBus.h"

namespace dps {

	BIU::BIU() 
		: label(), pParent(NULL), m_owner(NULL)
	{
		usTerminalID = 0xFFFF;
	}

	BIU::BIU(unsigned short usTerminalID) 
		: label(), pParent(NULL), m_owner(NULL)
	{
		this->usTerminalID = usTerminalID;
		
	}

	BIU::~BIU() {
	}

	void BIU::Label(const string& _label) {
		label = _label;
	}

	const string& BIU::GetLabel() const {
		return label;
	}

	void BIU::SetParent(AtlantisSubsystem* pSubsys) {
		pParent = pSubsys;
	}

	AtlantisSubsystem* BIU::GetParent() const {
		return pParent;
	}

	bool BIU::IsController() const
	{
		return false;
	}

	bool BIU::IsTerminal() const
	{
		return false;
	}


	void BIU::SetTerminalID(unsigned short TermID) {
		usTerminalID = TermID;
	}

	void BIU::Init(AtlantisSubsystem* pSubsys,
		IConnectedToBus* owner,
		const string& _label, 
		unsigned short usTermID) {

			SetParent(pSubsys);
			Label(_label);
			SetTerminalID(usTermID);
	}
	
	void BIU::ConnectTo(ShuttleBus* _pBus) {
		//char pszBuffer[255];
		pBus = _pBus;
		
	}
};