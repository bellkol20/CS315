#ifndef BOOLEAN_GATES_H
#define BOOLEAN_GATES_H

#include "Gate.h"

namespace osl
{
	class NotGate : public Gate
	{
	public:
		explicit NotGate(const Dev* in) : a(in) {}

		bool eval() const override { return !a->eval(); }

	private:
		const Dev* a;
	};

	class AndGate : public Gate
	{
	public:
		AndGate(const Dev* x, const Dev* y) : a(x), b(y) {}

		bool eval() const override { return a->eval() && b->eval(); }

	private:
		const Dev* a;
		const Dev* b;
	};

	class OrGate : public Gate
	{
	public:
		OrGate(const Dev* x, const Dev* y) : a(x), b(y) {}

		bool eval() const override { return a->eval() || b->eval(); }

	private:
		const Dev* a;
		const Dev* b;
	};

	class XorGate : public Gate
	{
	public:
		XorGate(const Dev* x, const Dev* y) : a(x), b(y) {}

		bool eval() const override { return a->eval() != b->eval(); }

	private:
		const Dev* a;
		const Dev* b;
	};
}

#endif
