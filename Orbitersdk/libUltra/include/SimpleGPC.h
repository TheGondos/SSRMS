#pragma once
#include "dps_defs.h"
#include "gpc.h"

namespace dps {

	class SimpleGPC: public GPC
	{
		PREFERRED_STORAGE* pPrefStorage;
		
		unsigned short usMajorCount;
		unsigned short usMinorCount;

		float fGPCSeconds;
		short usGPCMinutes;

		unsigned long ulRTC;

		unsigned long ulManualRTC;
		

		virtual void OnIOPPrestep(double fSimT, double fDeltaT, double fMJD);
		virtual void MinorCycle(double fSimT, double fDeltaT, double fMJD);
		virtual void MajorCycle(double fSimT, double fDeltaT, double fMJD);
	public:
		SimpleGPC(SubsystemDirector* _direct, const string& _ident, unsigned short usGPCID);
		virtual ~SimpleGPC();
		virtual void IPL();
		virtual void LoadMemoryConfiguration(unsigned short usMemConfig);

		//Functions for handling data blocks in GPC memory
		virtual void* AllocateGPCMemory(unsigned long usSize);
		virtual void FreeGPCMemory(void* pMemBlock);
		//clear the whole memory
		virtual void ClearGPCMemory();
		/**
		 * Clear the whole memory reserved for applications
		 */
		virtual void FreeApplicationMemory();

		virtual word24 fcosSchedule();
		virtual word24 fcosWait(long delay);
		virtual word24 fcosClose();
		virtual word24 SupervisorCall(char function);
	
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	};
};