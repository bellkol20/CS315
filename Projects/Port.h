#ifndef PORT_H
#define PORT_H

#include "Dev.h"
#include "Word.h"

namespace osp
{
	class Port : public Dev 
	{
		private:
		Word* data;
		size_t pin;
		Port(const Port&) = delete;
		Port& operator=(const Port&) = delete;
		using Dev::bind;
		using Dev::unbind;

		public:
		Port() : data(nullptr), pin(0) {}
		
		virtual ~Port() {data = nullptr;}
		
		bool input(Word& obj) 
		{
			if(data == nullptr)
			{
				data = &obj;
				return true;
			}
			return false;
		}

		void set(size_t idx)
		{
			if(data != nullptr && idx < data->size())
			{
				pin = idx;
			}
		}
		
		bool valid() const final {return data != nullptr;}
		
		bool output() const final 
		{
			if(data != nullptr) {return data->get(pin);}
			return false;
		}
		
		std::string toString() const final 
		{
			if(data != nullptr)
			{
				return ((data->get(pin))?("T"):("F"));
			}
			return "X";
		}
	};
}

#endif
