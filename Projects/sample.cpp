#include "Dev.h"
#include "Switch.h"
#include "Split.h"
#include "Word.h"
#include "Gate.h"
#include "Port.h"
#include "Socket.h"
#include <iostream>
#include <string>

//A XNOR Gate Class Definition
class XNORGate : public osp::Gate
{
	private:
	XNORGate(const XNORGate&) = delete;
	XNORGate& operator=(const XNORGate&) = delete;

	public:
	XNORGate() : osp::Gate(2,3) {}
	
	bool output() const override 
	{
		bool res = false;

		if(valid()) 
		{
			res = at(0).output();

			for(size_t i = 1;i < count();i += 1)
			{
				res = (res == at(i).output());
			}
		}
		return res;
	}

	std::string toString() const override 
	{
		std::stringstream out;

		if(valid()) 
		{
			for(size_t i = 0;i < count();i += 1)
			{
				out << at(i);

				if(i + 1 < count()) {out << " / ";}
			}
		}
		else {out << "X";}
		return out.str();
	}
};

class ORGate : public osp::Gate
{
	private:
	ORGate(const ORGate&) = delete;
	ORGate& operator=(const ORGate&) = delete;

	public:
	ORGate() : osp::Gate(2,3) {}
	
	bool output() const override 
	{
		if(valid()) 
		{
			for(size_t i = 0;i < count();i += 1)
			{
				if(at(i).output()) {return true;} 
			}
		}
		return false;
	}

	std::string toString() const override 
	{
		std::stringstream out;

		if(valid()) 
		{
			for(size_t i = 0;i < count();i += 1)
			{
				out << at(i);

				if(i + 1 < count()) {out << " | ";}
			}
		}
		else {out << "X";}
		return out.str();
	}
};
int main()
{
	//Input Word Declarations
	//Represents each column of a three input truth table 
	osp::Word I[3] = {osp::Word("11110000"), osp::Word("11001100"), osp::Word("10101010")};

	//Output Word
	osp::Word R(8);

	//Circuits Development
	XNORGate xg;
	ORGate rg;
	osp::Port first[3]; //inputs for XNOR gate
	osp::Port second[3]; //inputs for OR gate
	osp::Socket output[2]; 

	for(size_t i = 0;i < 3;i += 1)
	{
		first[i].input(I[i]);
		second[i].input(I[i]);
		xg.input(first[i]);
		rg.input(second[i]);
	}

	//Linking word and dev to the socket
	output[0].source(xg);
	output[1].source(rg);
	output[0].destination(R);
	output[1].destination(R);

	std::cout << "XNOR Gate\n";
	//Evaluate the XNOR circuit
	for(size_t i = 0;i < 8;i += 1)
	{
		for(size_t j = 0;j < 3;j += 1)
		{
			//sets input bit
			first[j].set(i);
		}
		//sets output bit
		output[0].set(i);
		//evaluate and store output at word bit
		output[0].evaluate();
		std::cout << xg << " = " << output[0] << "\n";
	}
	
	//Display Truth Table
	std::cout << "\nTruth Table of XNOR Gate\n"; 
	for(size_t i = 0;i < 8;i += 1)
	{
		for(size_t j = 0;j < 3;j += 1)
		{
			std::cout << I[j].get(i) << " | ";
		}
		std::cout << R.get(i) << "\n";
	}

	std::cout << "\nOr Gate\n";	
	//Evaluate the OR circuit
	for(size_t i = 0;i < 8;i += 1)
	{
		for(size_t j = 0;j < 3;j += 1)
		{
			second[j].set(i);
		}
		output[1].set(i);
		output[1].evaluate();
		std::cout << rg << " = " << output[1] << "\n";
	}
	std::cout << "\nTruth Table of OR Gate\n";
	
	//Display Truth Table 
	for(size_t i = 0;i < 8;i += 1)
	{
		for(size_t j = 0;j < 3;j += 1)
		{
			std::cout << I[j].get(i) << " | ";
		}
		std::cout << R.get(i) << "\n";
	}
	
	return 0;
}

