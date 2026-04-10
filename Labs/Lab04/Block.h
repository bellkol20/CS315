#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <string>
#include "Pair.h"
#include "Word.h"

namespace osl 
{
	class Block : public Object 
	{
		private:
		Pair<size_t,Word> data;

		public:
		Block() : data(0,Word()) {}

		Block(const Block& obj) : data(obj.data) {}

		Block& operator=(const Block& rhs) 
		{
			if(this != &rhs)
			{
				data = rhs.data;
			}
			return *this;
		}

		~Block() {}

		size_t& tag() {return data.key();}

		const size_t& tag() const {return data.key();}
		
		Word& content() {return data.value();}

		const Word& content() const {return data.value();}

		std::string toString() const override 
		{
			std::stringstream out;
			out << data.key() << ": " << data.value();
			return out.str();
		}
	};
}

#endif
