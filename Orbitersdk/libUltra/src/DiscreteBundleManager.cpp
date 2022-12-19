#include "DiscreteBundleManager.h"
#include <OrbiterAPI.h>

namespace discsignals {

DiscreteBundleManager::DiscreteBundleManager(void)
: bundles()
{
}

DiscreteBundleManager::~DiscreteBundleManager(void)
{
	for(unsigned int i=0;i<bundles.size();i++) {
		DiscreteBundle* pBundle=bundles[i];
		delete pBundle;
	}
	bundles.clear();
}

void DiscreteBundleManager::DumpToLog() const
{
	char pszBuffer[400];
	oapiWriteLog("(DiscreteBundleManager) Bundle dump:");
	//Dump all registered Bundles to the orbiter log
	for(unsigned int i=0;i<bundles.size();i++) {
		sprintf(pszBuffer, "%d\t:\t%s\t:\t%d", 
			i+1, bundles.at(i)->GetIdentity().c_str(),
			bundles.at(i)->CountLines());
		oapiWriteLog(pszBuffer);
	}
}

bool DiscreteBundleManager::AddBundle(discsignals::DiscreteBundle *pBundle)
{
	if(pBundle) {
		bundles.push_back(pBundle);
		return true;
	} else {
		oapiWriteLog("(DiscreteBundleManager::AddBundle) Attempt to add NULL pointer.");
		return false;
	}
	
}

DiscreteBundle* DiscreteBundleManager::CreateBundle(const std::string &_ident, unsigned short usLines)
{
	//char pszBuffer[400];
	//DumpToLog();
	//sprintf_s(pszBuffer, 400, "(DiscreteBundleManager::CreateBundle) Enter \"%s\"", _ident.c_str());
	//oapiWriteLog(pszBuffer);
	if(ExistsBundle(_ident))
	{
		//oapiWriteLog("(DiscreteBundleManager::CreateBundle) Found Bundle");
		return FindBundle(_ident);
	}
	DiscreteBundle* pR = new DiscreteBundle(_ident, usLines);
	if(AddBundle(pR))
	{
		//oapiWriteLog("(DiscreteBundleManager::CreateBundle) Added new bundle");
		return pR;
	}
	else {
		oapiWriteLog("(DiscreteBundleManager::CreateBundle) Failed to create new bundle");
		DumpToLog();
		if(pR)
			delete pR;
		return NULL;
	}
}

unsigned long DiscreteBundleManager::CountBundles() const 
{
	return bundles.size();
}

bool DiscreteBundleManager::ExistsBundle(const std::string &_ident) const {
	//char pszBuffer[400];
	//sprintf_s(pszBuffer, 400, "(DiscreteBundleManager::ExistsBundle) Enter \"%s\"", _ident.c_str());
	//oapiWriteLog(pszBuffer);
	try {
		for(unsigned int i = 0; i<bundles.size(); i++)
		{

			//oapiWriteLog("A1");
			DiscreteBundle* pB = bundles.at(i);
			//oapiWriteLog("B1");
			/*
			sprintf_s(pszBuffer, 400, "(DiscreteBundleManager::ExistsBundle) iteration %d.", i+1);
			oapiWriteLog(pszBuffer);
			*/
			if(!pB)
			{
				oapiWriteLog("(DiscreteBundleManager::ExistsBundle) encountered empty bundle.");
				continue;
			}

			if(pB->GetIdentity() == _ident)
			{
				//oapiWriteLog("(DiscreteBundleManager::ExistsBundle) Found.");
				return true;
			}
		}
	}
	catch(...)
	{
		oapiWriteLog("(DiscreteBundleManager::ExistsBundle) Uncaught exception.");
		return false;
	}
	//oapiWriteLog("(DiscreteBundleManager::ExistsBundle) No such bundle found.");
	return false;
};

DiscreteBundle* DiscreteBundleManager::FindBundle(const std::string &_ident) const {
	try {
		for(unsigned int i = 0; i<bundles.size(); i++)
		{
			if(bundles.at(i)->GetIdentity() == _ident)
				return bundles.at(i);
		}
	}
	catch(...)
	{
		oapiWriteLog("(DiscreteBundleManager::FindBundle) Uncaught exception.");
		return NULL;
	}
	return NULL;
};

DiscreteBundle* DiscreteBundleManager::GetBundleByIndex(unsigned long ulIndex) const
{
	try {
		return bundles.at(ulIndex);
	}
	catch(...)
	{
		oapiWriteLog("(DiscreteBundleManager::FindBundle) Uncaught exception.");
		return NULL;
	}
	return NULL;
}


};