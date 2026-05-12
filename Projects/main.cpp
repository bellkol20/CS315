/**
 * CS 315 — Combinational Circuit Project
 * Dual-operation (add / subtract) bit slice, 8-bit vector tests, 4-bit ripple ALU.
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "BooleanGates.h"
#include "Port.h"
#include "Socket.h"
#include "Split.h"
#include "Switch.h"
#include "Word.h"

using namespace osl;

namespace
{
	void destroyAll(std::vector<Dev*>& arena)
	{
		for (Dev* p : arena)
		{
			delete p;
		}
		arena.clear();
	}

	/**
	 * One dual-op stage: B_eff = B xor op; R = A xor B_eff xor Cin;
	 * C = A·B_eff + Cin·(A xor B_eff). Uses a Split on (A xor B_eff) for fan-out.
	 */
	class DualOpBitSlice
	{
	public:
		DualOpBitSlice(Port* pa, Port* pb, const Dev* pcin, Switch* op, std::vector<Dev*>& arena)
		{
			auto reg = [&](Dev* p) -> Dev*
			{
				arena.push_back(p);
				return p;
			};

			XorGate* bxor = static_cast<XorGate*>(reg(new XorGate(pb, op)));
			XorGate* axb = static_cast<XorGate*>(reg(new XorGate(pa, bxor)));
			Split* axbFan = static_cast<Split*>(reg(new Split(axb)));

			rRoot = static_cast<XorGate*>(reg(new XorGate(axbFan, pcin)));
			AndGate* t0 = static_cast<AndGate*>(reg(new AndGate(pa, bxor)));
			AndGate* t1 = static_cast<AndGate*>(reg(new AndGate(pcin, axbFan)));
			cRoot = static_cast<OrGate*>(reg(new OrGate(t0, t1)));
		}

		const Dev* resultWire() const { return rRoot; }
		const Dev* carryWire() const { return cRoot; }

	private:
		const Dev* rRoot;
		const Dev* cRoot;
	};

	void writeTableHeader(std::ostream& out)
	{
		out << std::left << std::setw(4) << "i" << std::setw(4) << "A" << std::setw(4) << "B" << std::setw(6) << "Cin"
		    << std::setw(4) << "op" << std::setw(4) << "R" << std::setw(4) << "C" << '\n';
	}

	void runEightBitTest(
		const std::string& label,
		bool opBit,
		Word& wordA,
		Word& wordB,
		Word& wordCin,
		Word& wordR,
		Word& wordC,
		const std::string& outPath)
	{
		std::ofstream file(outPath.c_str());
		auto writeBoth = [&](const std::string& s)
		{
			std::cout << s;
			if (file.is_open())
			{
				file << s;
			}
		};

		Port portA;
		Port portB;
		Port portCin;
		Switch opSw;
		opSw.set(opBit);

		writeBoth(label + "\n");
		writeTableHeader(std::cout);
		if (file.is_open())
		{
			writeTableHeader(file);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			portA.bind(&wordA, i);
			portB.bind(&wordB, i);
			portCin.bind(&wordCin, i);

			std::vector<Dev*> arena;
			DualOpBitSlice slice(&portA, &portB, &portCin, &opSw, arena);

			Socket sockR(slice.resultWire(), &wordR, i);
			Socket sockC(slice.carryWire(), &wordC, i);
			sockR.evaluate();
			sockC.evaluate();

			std::ostringstream row;
			row << std::setw(4) << i << std::setw(4) << (wordA.get(i) ? '1' : '0') << std::setw(4)
			    << (wordB.get(i) ? '1' : '0') << std::setw(6) << (wordCin.get(i) ? '1' : '0') << std::setw(4)
			    << (opBit ? '1' : '0') << std::setw(4) << (wordR.get(i) ? '1' : '0') << std::setw(4)
			    << (wordC.get(i) ? '1' : '0') << '\n';
			writeBoth(row.str());

			destroyAll(arena);
		}

		std::ostringstream tail;
		tail << "\nR bits (index 0 = MSB of string): " << wordR << "\n";
		tail << "C bits (index 0 = MSB of string): " << wordC << "\n";
		writeBoth(tail.str());

		if (file.is_open())
		{
			file.close();
		}
	}

	void runFourBitRipple(Word& wA, Word& wB, Word& wR, bool opBit, bool& finalCarryOut)
	{
		Switch op;
		op.set(opBit);

		Port pa[4];
		Port pb[4];
		for (int k = 0; k < 4; ++k)
		{
			const size_t idx = static_cast<size_t>(3 - k);
			pa[k].bind(&wA, idx);
			pb[k].bind(&wB, idx);
		}

		std::vector<Dev*> arena;
		const Dev* cinWire = &op;
		const Dev* rWire[4];

		for (int k = 0; k < 4; ++k)
		{
			DualOpBitSlice stage(&pa[k], &pb[k], cinWire, &op, arena);
			rWire[k] = stage.resultWire();
			cinWire = stage.carryWire();
		}

		finalCarryOut = cinWire->eval();

		for (int k = 0; k < 4; ++k)
		{
			const size_t idx = static_cast<size_t>(3 - k);
			Socket sock(rWire[k], &wR, idx);
			sock.evaluate();
		}

		destroyAll(arena);
	}
}

int main()
{
	const std::string dir;

	Word wordA("11110000");
	Word wordB("11001100");
	Word wordCin("10101010");

	Word rAdd("00000000");
	Word cAdd("00000000");
	Word rSub("00000000");
	Word cSub("00000000");

	std::cout << "=== Test 1: 8-bit addition (op = 0) ===\n";
	runEightBitTest("Test Case 1 - Addition (op = 0)", false, wordA, wordB, wordCin, rAdd, cAdd,
	                dir + "test1_add.txt");

	std::cout << "\n=== Test 2: 8-bit subtraction (op = 1) ===\n";
	runEightBitTest("Test Case 2 - Subtraction (op = 1)", true, wordA, wordB, wordCin, rSub, cSub,
	                 dir + "test2_sub.txt");

	Word aluA("0101");
	Word aluB("0011");
	Word aluR("0000");

	std::cout << "\n=== Test 3: 4-bit addition 0101 + 0011 ===\n";
	{
		std::ofstream f((dir + "test3_4bit_add.txt").c_str());
		bool fc = false;
		runFourBitRipple(aluA, aluB, aluR, false, fc);
		std::ostringstream line;
		line << aluA << " + " << aluB << " = " << aluR;
		std::cout << line.str() << "   (value(R)=" << value(aluR) << ", final carry=" << (fc ? '1' : '0')
		          << ")\n";
		if (f.is_open())
		{
			f << line.str() << "\nfinal carry out: " << (fc ? '1' : '0') << "\n";
			f.close();
		}
	}

	std::cout << "\n=== Test 4: 4-bit subtraction 0101 - 0011 ===\n";
	{
		Word aluR2("0000");
		std::ofstream f((dir + "test4_4bit_sub.txt").c_str());
		bool fc = false;
		runFourBitRipple(aluA, aluB, aluR2, true, fc);
		std::ostringstream line;
		line << aluA << " - " << aluB << " = " << aluR2;
		std::cout << line.str() << "   (value(R)=" << value(aluR2) << ", final carry=" << (fc ? '1' : '0')
		          << ")\n";
		if (f.is_open())
		{
			f << line.str() << "\nfinal carry out: " << (fc ? '1' : '0') << "\n";
			f.close();
		}
	}

	return 0;
}
