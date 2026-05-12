#ifndef DEV_H
#define DEV_H

#include "Object.h"

namespace osl
{
	/** Leaf or internal node that exposes a single Boolean signal. */
	class Dev : public Object
	{
	public:
		~Dev() override = default;
		virtual bool eval() const = 0;

		std::string toString() const override
		{
			return eval() ? "1" : "0";
		}
	};
}

#endif
