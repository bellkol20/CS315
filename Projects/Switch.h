#ifndef SWITCH_H
#define SWITCH_H

#include "Gate.h"

namespace osl
{
	/** Single control bit (op): 0 = addition, 1 = subtraction in this project. */
	class Switch : public Gate
	{
	public:
		Switch() : state(false) {}

		void set(bool bit) { state = bit; }
		bool getState() const { return state; }

		bool eval() const override { return state; }

	private:
		bool state;
	};
}

#endif
