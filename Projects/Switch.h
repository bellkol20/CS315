#ifndef SWITCH_H
#define SWITCH_H

#include "Dev.h"

namespace osp
{
	class Switch : public Dev 
	{
		private:
		bool bit;
		Switch(const Switch&) = delete;
		Switch& operator=(const Switch&) = delete;
		using Dev::bind;
		using Dev::unbind;

		public:
		Switch() : bit(false) {}
		
		Switch(bool value) : bit(value) {}
		
		virtual ~Switch() {}
		
		void invert() {bit = !bit;}
		
		bool valid() const final {return true;}
		
		bool output() const final {return bit;}
		
		std::string toString() const final 
		{
			return ((bit)?("T"):("F"));
		}
	};
}

#endif
