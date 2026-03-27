#ifndef PROCESS
#define PROCESS

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

namespace osl
{
	enum class State
	{
		NEW,
		READY,
		RUNNING,
		EXIT
	};
}

#endif
