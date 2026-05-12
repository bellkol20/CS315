#ifndef PORT_H
#define PORT_H

#include "Gate.h"
#include "Word.h"

namespace osl
{
	/** Reads one bit from an external Word (A, B, or Cin stream). */
	class Port : public Gate
	{
	public:
		Port() : word(nullptr), index(0) {}

		void bind(const Word* w, size_t idx)
		{
			word = w;
			index = idx;
		}

		bool eval() const override
		{
			if (word == nullptr || index >= word->size())
			{
				return false;
			}
			return word->get(index);
		}

	private:
		const Word* word;
		size_t index;
	};
}

#endif
