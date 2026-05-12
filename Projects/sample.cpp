/*
 * sample.cpp — minimal usage patterns for the osl simulation headers.
 * The full assignment driver is main.cpp (compile: g++ -std=c++17 main.cpp -o alu).
 */

#include "Port.h"
#include "Socket.h"
#include "Switch.h"
#include "Word.h"

using namespace osl;

void sample_bind_and_socket_demo()
{
	Word bus("1010");
	Word sink("0000");
	Port p;
	p.bind(&bus, 1);
	Switch s;
	s.set(false);

	(void)p.eval();
	(void)s.eval();

	Socket sock(&p, &sink, 2);
	sock.evaluate();
}

/* Entry point for this course project is main.cpp (g++ -std=c++17 main.cpp -o alu). */
