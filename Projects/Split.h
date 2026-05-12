#ifndef SPLIT_H
#define SPLIT_H

#include "Gate.h"

namespace osl
{
	/**
	 * Fan-out node: repeats one upstream signal so several gates can share
	 * the same logical input without aliasing ownership in the object graph.
	 */
	class Split : public Gate
	{
	public:
		explicit Split(const Dev* source) : src(source) {}

		bool eval() const override { return src->eval(); }

	private:
		const Dev* src;
	};
}

#endif
