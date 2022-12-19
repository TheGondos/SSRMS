#pragma once
#include "dps_defs.h"
#include "../discsignals/DiscInPort.h"

namespace dps {

class IOModule
{
	MDM* m_parent;
public:
	IOModule(MDM* parent);
	virtual ~IOModule();
	virtual void Reset() = 0;
	virtual void PowerOff() = 0;
	virtual bool WriteToChannel(unsigned int channel_no, word16 data) = 0;
	virtual bool ReadFromChannel(unsigned int channel_no, word16& data) = 0;
	virtual bool BITE() = 0;
	virtual bool ConnectDiscrete(unsigned int port, discsignals::DiscreteBundle* bundle, unsigned int line) = 0;
};

class AIDModule: public IOModule
{
	word16 channel_data[16];
public:	
	AIDModule(MDM* parent);
	virtual ~AIDModule();
	virtual void Reset();
	virtual void PowerOff();
	virtual bool WriteToChannel(unsigned int channel_no, word16 data);
	virtual bool ReadFromChannel(unsigned int channel_no, word16& data);
	virtual bool BITE();
	virtual bool ConnectDiscrete(unsigned int port, discsignals::DiscreteBundle* bundle, unsigned int line);

	discsignals::DiscInPort channel_in[16];
};

class AODModule: public IOModule
{
	word16 channel_data[16];
public:	
	AODModule(MDM* parent);
	virtual ~AODModule();
	virtual void Reset();
	virtual void PowerOff();
	virtual bool WriteToChannel(unsigned int channel_no, word16 data);
	virtual bool ReadFromChannel(unsigned int channel_no, word16& data);
	virtual bool BITE();
	virtual bool ConnectDiscrete(unsigned int port, discsignals::DiscreteBundle* bundle, unsigned int line);

	discsignals::DiscOutPort channel_out[16];
};

};