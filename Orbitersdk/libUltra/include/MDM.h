// MDM.h: Schnittstelle für die Klasse MDM.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_)
#define AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../AtlantisSubsystem.h"
#include "dps_defs.h"
#include "BIU.h"

namespace dps {

class MDMIOModule;

/**
 * Possible modules per MDM (maximal 16 modules):
 *    
 * Discrete output high (DOH), 28 volts, three l6-bit channels
 * Discrete output low (DOL), 5 volts, three l6-bit channels
 * Discrete input high (DIH), 28 volts, three l6-bit channels
 * Discrete input low (DIL), 5 volts, three l6-bit channels
 * Analog output differential (AOD), 16 channels
 * Analog input differential (AID), 16 channels
 * Analog input single-ended (AIS), 32 channels
 * Serial input/output (SIO), four channels
 * Tacan/radar altimeter (special-purpose I/O)
 * 
 * FF MDM:
 * Modules 5  and 13 connect to the RJDF - likely DOH/DIL modules
 *
 */
class MDM : public AtlantisSubsystem, public IConnectedToBus  
{
protected:
	word16 m_bite_status;
	word16 SCU_PROM[512];
	MDMIOModule* m_modules[16];
	void MasterReset(void);
public:
	MDM(SubsystemDirector* _director, const string& _ident,
		unsigned short MIA1_addr, unsigned short MIA2_addr);
	virtual ~MDM();

	void LoadPROM(const std::string& prom_file);
	void LoadPROMAndCreate(const std::string& prom_file);
	/**
	 * @param module_id Module address, 0 - 15
	 */
	void InstallDOHModule(unsigned int module_id);
	void InstallDIHModule(unsigned int module_id);
	void InstallDOLModule(unsigned int module_id);
	void InstallDILModule(unsigned int module_id);
	void InstallAODModule(unsigned int module_id);
	void InstallAIDModule(unsigned int module_id);
	void InstallAISModule(unsigned int module_id);
	void InstallSIOModule(unsigned int module_id);
	void InstallTacanModule(unsigned int module_id);

	void ExecuteProm(unsigned int start, unsigned int number_of_words);
	
	virtual void busCommandPhase(BusController* biu);
	virtual void busReadPhase(BusController* biu);
	virtual BUS_COMMAND_WORD busCommand(BusTerminal* biu, BUS_COMMAND_WORD cw, 
			unsigned long num_data, word16 *cdw);

	BusTerminal mia1, mia2;
};

};

#endif // !defined(AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_)
