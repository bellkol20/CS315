#ifndef GATE_H
#define GATE_H

#include "Dev.h"

namespace osl
{
	/** Combinational gate: output is a Boolean function of input Devs (no state). */
	class Gate : public Dev
	{
	public:
		~Gate() override = default;
	};
}

#endif
