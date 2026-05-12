#ifndef SOCKET_H
#define SOCKET_H

#include <sstream>
#include <string>

#include "Dev.h"
#include "Object.h"
#include "Word.h"

namespace osl
{
	/**
	 * Sink attached to the root of a sub-circuit. evaluate() pulls the Boolean
	 * result from the root and stores it into a destination Word at bitIndex.
	 */
	class Socket : public Object
	{
	public:
		Socket(const Dev* rootGate, Word* destination, size_t bitIndex)
			: root(rootGate), dest(destination), bit(bitIndex)
		{}

		void evaluate()
		{
			if (root != nullptr && dest != nullptr && bit < dest->size())
			{
				dest->set(bit, root->eval());
			}
		}

		bool probe() const { return root != nullptr && root->eval(); }

		std::string toString() const override
		{
			std::ostringstream out;
			out << (probe() ? '1' : '0');
			return out.str();
		}

	private:
		const Dev* root;
		Word* dest;
		size_t bit;
	};
}

#endif
