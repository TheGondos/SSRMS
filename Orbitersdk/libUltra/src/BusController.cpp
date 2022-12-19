#include "BIU.h"

namespace dps {

	BusController::BusController()
	{
	}

	BusController::~BusController()
	{
	}


	BUS_COMMAND_WORD BusController::busCommand(BUS_COMMAND_WORD cw, unsigned long word_count, short* cdw)
	{
		return cw;
	}

	void BusController::busCommandPhase()
	{
		if(m_owner)
			m_owner->busCommandPhase(this);
	}

	void BusController::busReadPhase()
	{
		if(m_owner)
			m_owner->busReadPhase(this);
	}

	bool BusController::IsController() const
	{
		return true;
	}

};