#include "BIU.h"

namespace dps {

	BusTerminal::BusTerminal()
	{
	}

	BusTerminal::~BusTerminal()
	{
	}
	

	BUS_COMMAND_WORD BusTerminal::busCommand(BUS_COMMAND_WORD cw, 
		unsigned long word_count, short* cdw)
	{
		if(m_owner != NULL)
			return m_owner->busCommand(this, cw, 
				word_count, cdw);
		else
			return cw;
	}

	void BusTerminal::busCommandPhase()
	{
	}

	void BusTerminal::busReadPhase()
	{
	}

	bool BusTerminal::Flush()
	{
		output_data.Purge();
		return true;
	}

	bool BusTerminal::IsTerminal() const
	{
		return true;
	}

	unsigned long BusTerminal::ReadData(unsigned long num_data, 
			word16* data)
	{
		unsigned long read_counter = 0;
		while(!output_data.EndOfData() && read_counter != num_data)
		{
			*data++ = output_data.front();
			output_data.Pop();
			read_counter++;
		}
		return read_counter;
	}

	bool BusTerminal::Rewind() 
	{
		output_data.Rewind();
		return true;
	}

	void BusTerminal::WriteData(unsigned long num_data, word16* data)
	{
		if(num_data > 512)
			return;

		while(num_data-- > 0)
			output_data.Push(*data++);
	}
};